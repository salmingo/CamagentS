#include "stdafx.h"
#include "PLCOuter.h"
#include "AstroDeviceDef.h"
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>

CPLCOuter::CPLCOuter() {

}

CPLCOuter::~CPLCOuter() {

}

bool CPLCOuter::Start(Parameter* param) {
	if (IsActive())
		return true;
	ParamSerialComm* p = paramSerialComm_ = &(param->serialCommOuter);
	paramFocuser_ = &(param->focuser);

	//state_
	if (!serialPort_->Open(p->portName, p->baudRate, p->dataBit, p->parity, p->stopBit, p->flowCtl))
		return false;
	return true;
}

void CPLCOuter::Stop() {

	serialPort_->Close();
}

bool CPLCOuter::Goto(int position) {

}

bool CPLCOuter::GotoRelative(int step) {

}

int CPLCOuter::GetPosition(int& real, int& target) {
	return state_;
}

bool CPLCOuter::IsRainy() {
	return true;
}

void CPLCOuter::serial_read(SerialPtr ptr, const boost::system::error_code& ec) {
	CPLC::serial_read(ptr, ec);

}

void CPLCOuter::interrupt_thread(ThrdPtr& thrd) {
	if (thrd.unique()) {
		thrd->interrupt();
		thrd->join();
		thrd.reset();
	}
}

void CPLCOuter::thread_command() {
	try {

	}
	catch (boost::thread_interrupted&) {
	}
}
