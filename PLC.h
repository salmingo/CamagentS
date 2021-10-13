/*!
 * @class CPLC 封装PLC通信接口, 实现与圆顶+雨水的控制指令和状态检测
 * @version 0.1
 * @author 卢晓猛
 * @date 2021-10-03
 * @note
 * 功能列表:
 * 1. 维护串口开关和参数配置
 * 2. 响应上层指令, 控制圆顶天窗开、关、停
 * 3. 定时查询天窗状态和降水状态
 * 4. 设置回调函数接口, 通知上层工作状态的变化
 */
#pragma once

#include "SerialComm.h"

#define PLC_START	0x02
#define PLC_END		0x03
#define PLC_READ	0x30
#define PLC_WRITE	0x31
#define PLC_ON		0x37
#define PLC_OFF		0x38
#define PLC_SUCCESS	0x06
#define PLC_FAIL	0x15
#define PLC_DATA	0x02
#define PLC_DATA_0	0x10	// 收到起始符, 但数据不完全

class CPLC
{
public:
	CPLC();
	virtual ~CPLC();

protected:
	// 数据类型
	typedef boost::shared_array<char> carray;		//< 字符型数组

protected:
	// 成员变量
	SerialPtr serialPort_;	/// 串口接口
	unsigned char retCode_;	/// PLC串口写入反馈
	carray bufRcv_;		/// 串口接收数据存储区, 含结束符和校验码
	int dataLen_;		/// 串口接收数据存储区长度

public:
	// 接口
	/*!
	 * @brief 设置串口参数
	 * @param portName  串口名称
	 * @param baudRate  波特率
	 * @param dataBit   数据位长度. 7或8
	 * @param parity    检验模式. 0: 无校验; 1: 奇; 2: 偶
	 * @param stopBit   停止位长度. 0: 1; 1: 1.5; 2: 2
	 * @param flowCtl   流量控制模式. 0: 无控制; 1: 软; 2: 硬
	 */
	virtual bool SetSerialPort(const char* portName, int baudRate = 9600, int dataBit = 8, int parity = 0, int stopBit = 1, int flowCtl = 0);
	/*!
	 *@brief 活跃标志
	 */
	bool IsActive();
	/*!
	 * @brief 强制接点ON
	 */
	bool SwitchOnOff(WORD addr, bool onoff = true);
	/*!
	 * @brief 尝试从指定地址读取指定长度的数据
	 */
	bool Read(WORD addr, unsigned char len);
	/*!
	 * @brief 尝试将指定类型的数据写入指定地址 
	 */
	template<typename T> bool Write(WORD addr, T data) {
		if (!IsActive())
			return false;

		int lenType = int(sizeof(T));
		int i;
		unsigned char* dataPtr = ((unsigned char*)&data) + lenType - 1;
		carray buff;
		char * toWrite, * ptr;

		n = 11 + lenType * 2;
		buff.reset(new char[n]);
		toWrite = buff.get();

		toWrite[0] = PLC_START;
		toWrite[1] = PLC_WRITE;
		char2code(unsigned char(HIBYTE(addr)), toWrite + 2);
		char2code(unsigned char(LOBYTE(addr)), toWrite + 4);
		char2code(lenType, toWrite + 6);
		for (i = 0, ptr = toWrite + 8; i < lenType; ++i, ptr += 2, --dataPtr) {
			char2code(*dataPtr, ptr);
		}
		toWrite[lenType * 2 + 8] = PLC_END;
		check_sum(toWrite, 1, lenType * 2 + 8, toWrite + lenType * 2 + 9);
		serialPort_->Write(toWrite, n);

		return true;
	}

protected:
	/*!
	 * @brief 将16进制数值转换为PLC编码
	 */
	void char2code(unsigned char value, char* code);
	unsigned char code2char(char* code);
	/*!
	 * @brief 计算校验和
	 */
	void check_sum(const char* data, const int first, const int last, char* crc);
	// 功能: 串口通信
	virtual void serial_read(SerialPtr ptr, const boost::system::error_code& ec);
};
