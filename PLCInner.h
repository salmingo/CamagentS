/*!
 * @class CPLCInner 封装基于PLC的圆顶控制和降水感知
 * @note
 * 调用顺序:
 * 1/ SetSerialPort
 * 2/ Start
 * 3/ 指令
 * 4/ Stop
 */
#pragma once

#include "PLC.h"
#include <boost/container/deque.hpp>

 class CPLCInner : public CPLC
{
public:
	CPLCInner();
	~CPLCInner();

protected:
	// 数据类型
	typedef boost::shared_ptr<boost::thread> ThrdPtr;
	typedef boost::unique_lock<boost::mutex> MtxLck;
	typedef boost::container::deque<int> QueCommand;

	enum {
		SLIT_RIGHT,
		SLIT_LEFT,
		SLIT_STAT,
		SLIT_MAX
	};

protected:
	// 成员变量
	int cntError_;	/// 计数: 通信错误
	int openPeriod_;/// 天窗打开周期
	int slitState_[SLIT_MAX];	/// 天窗状态. 0: 右; 1: 左; 2: 综合
	int slitCmd_[SLIT_STAT];	/// 天窗指令. 0: 右; 1: 左
	bool rainy_;		/// 降水标志

	QueCommand queCmd_[SLIT_STAT];		/// 天窗指令队列
	boost::mutex mtxQue_[SLIT_STAT];	/// 互斥锁

	ThrdPtr thrdCommand_;	/// 线程接口: 集中向串口发送指令
	boost::condition_variable cv_read_;	/// 收到串口反馈

public:
	// 接口
	/*!
	 * @brief 启动服务
	 */
	bool Start();
	/*!
	 * @brief 停止服务
	 */
	void Stop();
	/*!
	 * @brief 打开天窗
	 * @param period  打开指令持续时间, 量纲: 秒
	 * @note
	 * - 当period<=0时, 打开直至触碰限位
	 * - 当period>0时, 到达指令持续时间后发送停止指令
	 */
	bool OpenSlit(int period = 0);
	/*!
	 * @brief 关闭天窗
	 * @note
	 * - 持续
	 */
	bool CloseSlit();
	/*!
	 * @brief 中断天窗的打开或关闭过程
	 */
	bool StopSlit();
	/*!
	 * @brief 查看天窗工作状态 
	 */
	void GetSlitState(int& state, int* stateLeft, int* stateRight);
	/*!
	 * @brief 查看是否降水
	 */
	bool IsRainy();

protected:
	// 功能: 串口通信
	void serial_read(SerialPtr ptr, const boost::system::error_code& ec);
	void interrupt_thread(ThrdPtr& thrd);
	/*!
	 * @brief 线程: 向串口发送指令
	 */
	void thread_command();
};
