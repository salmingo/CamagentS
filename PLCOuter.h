/*!
 * @class CPLCOuter 封装基于PLC的调焦控制和降水感知
 * @note
 * 调用顺序:
 * 1/ Start
 * 2/ 指令
 * 3/ Stop
 */
#pragma once

#include "PLC.h"
#include "Parameter.h"

class CPLCOuter : public CPLC
{
public:
	CPLCOuter();
	~CPLCOuter();

protected:
	// 数据类型
	typedef boost::shared_ptr<boost::thread> ThrdPtr;
	typedef boost::unique_lock<boost::mutex> MtxLck;

protected:
	// 成员变量
	ParamSerialComm* paramSerialComm_;
	ParamFocuser* paramFocuser_;

	int cntError_;	/// 计数: 通信错误

	int state_;		/// 调焦器状态
	bool rainy_;	/// 降水标志
	int posReal_;	/// 实际位置
	int posTarget_;	/// 目标位置

	ThrdPtr thrdCommand_;	/// 线程接口: 集中向串口发送指令
	boost::condition_variable cv_read_;	/// 收到串口反馈

public:
	// 接口
	/*!
	 * @brief 启动服务
	 */
	bool Start(Parameter* param);
	/*!
	 * @brief 停止服务
	 */
	void Stop();
	/*!
	 * @brief 定位到绝对位置
	 */
	bool Goto(int position);
	/*!
	 * @brief 基于当前位置相对定位
	 */
	bool GotoRelative(int step);
	/*!
	 * @breif 查看焦点位置
	 * @param real   实际位置
	 * @param target 目标位置
	 * @return
	 * 工作状态
	 */
	int GetPosition(int& real, int& target);
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

