/*!
* @class CPLC 封装PLC通信接口, 实现与圆顶+雨水的控制指令和状态检测
* @version 0.1
* @date 2021-10-03
*/
#include "stdafx.h"
#include "PLC.h"
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>

using namespace boost::placeholders;
using namespace boost::system;

CPLC::CPLC()
{
	serialPort_ = SerialComm::Create();
	retCode_ = 0;
	dataLen_ = 0;
}

CPLC::~CPLC()
{
}

bool CPLC::SetSerialPort(const char* portName, int baudRate, int dataBit, int parity, int stopBit, int flowCtl) {
	if (serialPort_->IsOpen())
		return true;

	const SerialComm::CBSlot& slot = boost::bind(&CPLC::serial_read, this, _1, _2);
	serialPort_->RegisterRead(slot);
	return (serialPort_->Open(portName, baudRate, dataBit, parity, stopBit, flowCtl));
}

bool CPLC::IsActive() {
	return serialPort_->IsOpen();
}

bool CPLC::SwitchOnOff(WORD addr, bool onoff) {
	if (!IsActive())
		return false;

	int n(9);
	carray buff;
	char* toWrite;
	buff.reset(new char[n]);
	toWrite = buff.get();

	toWrite[0] = PLC_START;
	toWrite[1] = onoff ? PLC_ON : PLC_OFF;
	char2code(unsigned char(HIBYTE(addr)), toWrite + 2);
	char2code(unsigned char(LOBYTE(addr)), toWrite + 4);
	toWrite[6] = PLC_END;
	check_sum(toWrite, 1, 6, toWrite + 7);
	serialPort_->Write(toWrite, n);

	return true;
}

bool CPLC::Read(WORD addr, unsigned char len) {
	if (!IsActive())
		return false;

	int n(11);
	carray buff;
	char* toWrite;

	buff.reset(new char[n]);
	toWrite = buff.get();

	toWrite[0] = PLC_START;
	toWrite[1] = PLC_READ;
	char2code(unsigned char(HIBYTE(addr)), toWrite + 2);
	char2code(unsigned char(LOBYTE(addr)), toWrite + 4);
	char2code(len, toWrite + 6);
	toWrite[8] = PLC_END;
	check_sum(toWrite, 1, 8, toWrite + 9);
	serialPort_->Write(toWrite, n);

	return true;
}

void CPLC::char2code(unsigned char value, char* code)
{
	unsigned char hi = (value & 0xF0) >> 4;
	unsigned char lo = value & 0x0F;
	code[0] = hi < 0x0A ? hi + 0x30 : hi + 0x37;
	code[1] = lo < 0x0A ? lo + 0x30 : lo + 0x37;
}

unsigned char CPLC::code2char(char* code) {
	unsigned char value(0);
	char x = code[0];
	x -= (x < 0x40 ? 0x30 : 0x37);
	value = x << 4;
	x = code[1];
	x -= (x < 0x40 ? 0x30 : 0x37);
	value += x;

	return value;
}

void CPLC::check_sum(const char* data, const int first, const int last, char* crc) {
	DWORD sum(0);
	for (int i = first; i <= last; ++i)
		sum += data[i];
	char2code(unsigned char(sum & 0xFF), crc);
}

void CPLC::serial_read(SerialPtr ptr, const boost::system::error_code& ec) {
	if (ec) retCode_ = PLC_FAIL;
	else {
		unsigned char first;
		int n = serialPort_->Lookup((char*) &first);
		retCode_ = PLC_FAIL; // 错误
		if (first == PLC_SUCCESS || first == PLC_FAIL) {
			retCode_ = first;
			serialPort_->Read((char*)&first, 1, 0);
		}
		else if (first == PLC_DATA) {
			char flag = PLC_END;
			int pos = serialPort_->Lookup(&flag, 1, 1);
			if (pos < 0) // 不完整接收
				retCode_ = PLC_DATA_0;
			else {
				int toread = pos + 2;
				char crc[2];

				if (pos > (dataLen_ + 1)) {
					bufRcv_.reset(new char[toread]);
					dataLen_ = pos - 1; // ==> 有效数据长度
				}
				serialPort_->Read(bufRcv_.get(), toread, 1);

				check_sum(bufRcv_.get(), 0, pos - 1, crc);
				if (bufRcv_[pos] == crc[0] && bufRcv_[pos + 1] == crc[1])  // 校验码正确
					retCode_ = PLC_DATA;
			}
		}
	}
}
