/*!
 * @struct Parameter 使用XML文件管理配置参数
 */
#pragma once

#include <string>

/*!
 * @struct ParamSerialComm 串口配置参数 
 */
struct ParamSerialComm {
	// 串口参数, 对象设备: 圆顶和雨水
	std::string portName;
	int baudRate;
	int dataBit;
	int parity;
	int stopBit;
	int flowCtl;
	BOOL enabled; /// 是否启用降水感知
	BOOL dirty;	/// 修改标记: 串口参数

public:
	ParamSerialComm() {
		baudRate = 9600;
		dataBit = 8;
		parity = 0;
		stopBit = 0;
		flowCtl = 0;
		enabled = TRUE;
		dirty = FALSE;
	}
};

/*!
 * @struct ParamNetworkGtoaes 调度服务参数
 */
struct ParamNetworkGtoaes {
	std::string gid;
	std::string uid;
	std::string cid;
	std::string ip;
	int portCamera;
	int portAnnex;
	BOOL enabled;
	BOOL dirty;

public:
	ParamNetworkGtoaes() {
		dirty = FALSE;
	}
};

/*!
 * @struct ParamNetworkFileServer 文件服务参数
 */
struct ParamNetworkFileServer {
	std::string ip;
	int port;
	BOOL enabled;
	BOOL dirty;

public:
	ParamNetworkFileServer() {
		dirty = FALSE;
	}
};

/*!
 * @struct ParamNetworkNTP 时间服务参数
 */
struct ParamNetworkNTP {
	std::string ip;
	BOOL enabled;
	BOOL dirty;

public:
	ParamNetworkNTP() {
		dirty = FALSE;
	}
};

/*!
 * @struct ParamFocuser 调焦参数 
 */
struct ParamFocuser {
	int lastPos;	/// 上次软件退出时位置
	int offset;		/// 零点
	int limitMin;	/// 最小软限位
	int limitMax;	/// 最大软限位
	int factor;		/// 调制因子
	BOOL dirty;

public:
	ParamFocuser() {
		dirty = FALSE;
	}
};

struct Parameter
{
	ParamSerialComm serialCommInner;	/// 室内传感器串口 => 圆顶+室内雨水
	ParamSerialComm serialCommOuter;	/// 室外传感器串口 => 调焦+室外雨水
	ParamNetworkGtoaes netGtoaes;		/// 调度服务器
	ParamNetworkFileServer netFileSrv;	/// 文件服务器
	ParamNetworkNTP netNTP;				/// 时间服务器
	ParamFocuser focuser;				/// 调焦

	int slitOpenPeriod;		/// 尝试打开天窗的周期, 量纲: 秒
	int slitOpenPeriodOld;	/// 尝试打开天窗的周期, 量纲: 秒

protected:
	std::string filePath;	/// 配置文件路径

protected:
	void Init(); /// 生成默认参数的XML文件

public:
	Parameter();
	BOOL Load(const char* filePath);
	void Save();
};
