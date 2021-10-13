/*!
 * @struct Parameter ʹ��XML�ļ��������ò���
 */
#pragma once

#include <string>

/*!
 * @struct ParamSerialComm �������ò��� 
 */
struct ParamSerialComm {
	// ���ڲ���, �����豸: Բ������ˮ
	std::string portName;
	int baudRate;
	int dataBit;
	int parity;
	int stopBit;
	int flowCtl;
	BOOL enabled; /// �Ƿ����ý�ˮ��֪
	BOOL dirty;	/// �޸ı��: ���ڲ���

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
 * @struct ParamNetworkGtoaes ���ȷ������
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
 * @struct ParamNetworkFileServer �ļ��������
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
 * @struct ParamNetworkNTP ʱ��������
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
 * @struct ParamFocuser �������� 
 */
struct ParamFocuser {
	int lastPos;	/// �ϴ�����˳�ʱλ��
	int offset;		/// ���
	int limitMin;	/// ��С����λ
	int limitMax;	/// �������λ
	int factor;		/// ��������
	BOOL dirty;

public:
	ParamFocuser() {
		dirty = FALSE;
	}
};

struct Parameter
{
	ParamSerialComm serialCommInner;	/// ���ڴ��������� => Բ��+������ˮ
	ParamSerialComm serialCommOuter;	/// ���⴫�������� => ����+������ˮ
	ParamNetworkGtoaes netGtoaes;		/// ���ȷ�����
	ParamNetworkFileServer netFileSrv;	/// �ļ�������
	ParamNetworkNTP netNTP;				/// ʱ�������
	ParamFocuser focuser;				/// ����

	int slitOpenPeriod;		/// ���Դ��촰������, ����: ��
	int slitOpenPeriodOld;	/// ���Դ��촰������, ����: ��

protected:
	std::string filePath;	/// �����ļ�·��

protected:
	void Init(); /// ����Ĭ�ϲ�����XML�ļ�

public:
	Parameter();
	BOOL Load(const char* filePath);
	void Save();
};
