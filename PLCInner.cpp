#include "stdafx.h"
#include "PLCInner.h"
#include "AstroDeviceDef.h"
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

using namespace boost::system;
using namespace boost::placeholders;
using namespace boost::posix_time;

#define PLCINNER_LEFT_OPEN		0x0008
#define PLCINNER_LEFT_CLOSE		0x0208
#define PLCINNER_RIGHT_OPEN		0x0108
#define PLCINNER_RIGHT_CLOSE	0x0308
#define PLCINNER_INQUIRY		0x0101

#define PLCINNER_NDX_OPEN		0
#define PLCINNER_NDX_CLOSE		1

static int PLCINNER_ADDR[][2] = {// 注意顺序必须一致, 包括: LEFT/RIGHT; OPEN/CLOSE
	{ PLCINNER_RIGHT_OPEN, PLCINNER_RIGHT_CLOSE },
	{ PLCINNER_LEFT_OPEN, PLCINNER_LEFT_CLOSE }
};

CPLCInner::CPLCInner(){
	cntError_ = 0;
}

CPLCInner::~CPLCInner() {
}

bool CPLCInner::Start() {
	if (!IsActive())
		return false;

	for (int i = 0; i < SLIT_MAX; ++i) slitState_[i] = StateSlit::SLIT_ERROR;
	for (int i = 0; i < SLIT_STAT; ++i) {
		slitCmd_[i] = CommandSlit::SLITC_MIN;
		queCmd_[i].clear();
	}
	cntError_ = 0;
	rainy_ = false;
	thrdCommand_.reset(new boost::thread(boost::bind(&CPLCInner::thread_command, this)));

	return true;
}

void CPLCInner::Stop() {
	interrupt_thread(thrdCommand_);
	serialPort_->Close();
}

bool CPLCInner::OpenSlit(int period) {
	if (!IsActive())
		return false;

	int state;
	openPeriod_ = period;
	for (int i = 0; i < SLIT_STAT; ++i) {
		state = slitState_[i];
		if (state != StateSlit::SLIT_FULLY_OPEN && state != StateSlit::SLIT_OPENING) {
			MtxLck lck(mtxQue_[i]);

			if (state == StateSlit::SLIT_CLOSING) queCmd_[i].push_back(CommandSlit::SLITC_STOP);
			queCmd_[i].push_back(CommandSlit::SLITC_OPEN);
		}
	}

	return true;
}

bool CPLCInner::CloseSlit() {
	if (!IsActive())
		return false;

	int state;
	for (int i = 0; i < SLIT_STAT; ++i) {
		state = slitState_[i];
		if (state != StateSlit::SLIT_CLOSED && state != StateSlit::SLIT_CLOSING) {
			MtxLck lck(mtxQue_[i]);

			if (state == StateSlit::SLIT_OPENING) queCmd_[i].push_back(CommandSlit::SLITC_STOP);
			queCmd_[i].push_back(CommandSlit::SLITC_CLOSE);
		}
	}

	return true;
}

bool CPLCInner::StopSlit() {
	if (!IsActive())
		return false;

	for (int i = 0; i < SLIT_STAT; ++i) {
		MtxLck lck(mtxQue_[i]);
		queCmd_[i].push_back(CommandSlit::SLITC_STOP);
	}

	return true;
}

void CPLCInner::GetSlitState(int& state, int* stateLeft, int* stateRight) {
	state = slitState_[SLIT_STAT];
	if (stateLeft) *stateLeft = slitState_[SLIT_LEFT];
	if (stateRight) *stateRight = slitState_[SLIT_RIGHT];
}

bool CPLCInner::IsRainy() {
	return rainy_;
}

void CPLCInner::serial_read(SerialPtr ptr, const boost::system::error_code& ec) {
	CPLC::serial_read(ptr, ec); // 基类, 接收、解析数据

	if (retCode_ == PLC_FAIL) {
		if (++cntError_ >= 3 && slitState_[0] != StateSlit::SLIT_ERROR) {// 连续失败
			int n = sizeof(slitState_) / sizeof(int);
			for (int i = 0; i < n; ++i) slitState_[i] = StateSlit::SLIT_ERROR;
		}
	}
	else {
		if (cntError_) cntError_ = 0;
		if (retCode_ == PLC_DATA) {
			int x;
			int defCmd = CommandSlit::SLITC_MIN;

			for (int i = 0; i < SLIT_STAT; ++i) {
				int& cmd = slitCmd_[i];
				int& state = slitState_[i];
				// bufRcv_: 4字节
				//   0    1    2    3
				//  右   左   --   雨
				x = bufRcv_[i];

				if (x != 0x30) {
					if (cmd != defCmd) cmd = defCmd;
					state = x == 0x31 ? StateSlit::SLIT_FULLY_OPEN : StateSlit::SLIT_CLOSED;
				}
				else if (cmd == defCmd) state = StateSlit::SLIT_OPEN;
				else if (cmd == CommandSlit::SLITC_OPEN) state = StateSlit::SLIT_OPENING;
				else if (cmd == CommandSlit::SLITC_CLOSE) state = StateSlit::SLIT_CLOSING;
				else cmd = defCmd;
				if (x != 0x30 && cmd != defCmd) cmd = defCmd;
			}
			slitState_[SLIT_STAT] = (slitState_[SLIT_LEFT] == slitState_[SLIT_RIGHT] || slitCmd_[SLIT_LEFT] != defCmd) ? slitState_[SLIT_LEFT] : slitState_[SLIT_RIGHT];

			rainy_ = bufRcv_[3] == 0x31;
		}

		cv_read_.notify_one();
	}
}

void CPLCInner::interrupt_thread(ThrdPtr& thrd) {
	if (thrd.unique()) {
		thrd->interrupt();
		thrd->join();
		thrd.reset();
	}
}

void CPLCInner::thread_command() {
	try {
		boost::mutex mtx;
		MtxLck lck(mtx);
		boost::chrono::seconds tWait(1); // 周期: 1秒
		ptime now, tmLast[SLIT_STAT];
		int defCmd = CommandSlit::SLITC_MIN;
		int openCmd = CommandSlit::SLITC_OPEN;
		int closeCmd = CommandSlit::SLITC_CLOSE;
		int stopCmd = CommandSlit::SLITC_STOP;
		int i, lastcmd, newcmd, cmdaddr;
		bool needWrite, onoff;

		for (int i = 0; i < SLIT_STAT; ++i) {
			tmLast[i] = ptime(not_a_date_time);
		}
		while (true) {
			// 定时查询状态
			boost::this_thread::sleep_for(tWait);
			Read(PLCINNER_INQUIRY, 2);
			cv_read_.wait_for(lck, tWait);

			// 检查是否需要发送控制指令
			for (i = 0, needWrite = false; i < SLIT_STAT && !needWrite; ++i) {
				needWrite = queCmd_[i].size() > 0 || (slitCmd_[i] == openCmd && openPeriod_ > 0);
			}
			if (!needWrite) continue;
			// 发送指令
			now = second_clock::universal_time();
			for (i = 0; i < SLIT_STAT; ++i) {
				newcmd = defCmd;
				lastcmd = slitCmd_[i];

				if (lastcmd == openCmd && openPeriod_ > 0 && (now - tmLast[i]).total_seconds() >= openPeriod_) {// 特殊处理: 定时开指令
					newcmd = stopCmd;
				}
				else if (queCmd_[i].size()) {// 处理: 队列中指令
					MtxLck lck(mtxQue_[i]);
					newcmd = queCmd_[i].front();
					queCmd_[i].pop_front();
				}

				if (newcmd == stopCmd && (lastcmd == defCmd || lastcmd == stopCmd))
					newcmd = defCmd;
				if (newcmd != defCmd) {
					onoff = newcmd == openCmd || newcmd == closeCmd;
					if (onoff) 
						cmdaddr = PLCINNER_ADDR[i][newcmd == openCmd ? PLCINNER_NDX_OPEN : PLCINNER_NDX_CLOSE];
					else
						cmdaddr = PLCINNER_ADDR[i][lastcmd == openCmd ? PLCINNER_NDX_OPEN : PLCINNER_NDX_CLOSE];

					tmLast[i] = now;
					slitCmd_[i] = newcmd;
					SwitchOnOff(cmdaddr, onoff);
					cv_read_.wait_for(lck, tWait);
				}
			}
		}
	}
	catch (boost::thread_interrupted&) {
	}
}
