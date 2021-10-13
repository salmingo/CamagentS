/*
 * @file SerialComm.h �����ļ�, ����boost::asioʵ�ִ����첽����
 * @version 0.1
 * @date 2017-10-10
 * - �򿪴���
 * - ���ô��ڲ���
 * - �رմ���
 * - ע��ص�����
 * - ��������
 * - д������
 * - �쳣����
 */

#ifndef SERIALCOMM_H_
#define SERIALCOMM_H_

#include <string>
#include <boost/signals2.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "AsioIOServiceKeep.h"

using std::string;
using boost::system::error_code;

class SerialComm : public boost::enable_shared_from_this<SerialComm> {
public:
	SerialComm();
	virtual ~SerialComm();

public:
	/* �������� */
	typedef boost::shared_ptr<SerialComm> Pointer;
	/*!
	 * @brief ����SerialComm�ص���������
	 * @param _1 ����ָ��
	 * @param _2 �������. 0: ��ȷ
	 */
	typedef boost::signals2::signal<void (Pointer, const boost::system::error_code&)> CallbackFunc;
	// ����boost::signals2�����������
	typedef CallbackFunc::slot_type CBSlot;

protected:
	/* �������� */
	typedef boost::unique_lock<boost::mutex> mutex_lock;	//< ������
	typedef boost::circular_buffer<char> crcbuff;	//< ѭ��������
	typedef boost::shared_array<char> carray;		//< �ַ�������

protected:
	/* ��Ա���� */
	AsioIOServiceKeep keep_;		//< �ṩio_service����
	boost::asio::serial_port port_;	//< ����
	CallbackFunc  cbrcv_;	//< receive�ص�����
	CallbackFunc  cbsnd_;	//< send�ص�����

	carray bufrcv_;		//< �������ջ�����
	crcbuff crcrcv_;		//< ѭ�����ջ�����
	crcbuff crcsnd_;		//< ѭ�����ͻ�����
	boost::mutex mtxrcv_;	//< ���ջ�����
	boost::mutex mtxsnd_;	//< ���ͻ�����

public:
	/* �ӿ� */
	static Pointer Create();
	/*!
	 * @brief ���Դ򿪴���
	 * @param portname  ��������
	 * @param baud_rate ������
	 * @param dataBit   ����λ����. 7��8
	 * @param parity    ����ģʽ. 0: ��У��; 1: ��; 2: ż
	 * @param stopBit   ֹͣλ����. 0: 1; 1: 1.5; 2: 2
	 * @param flowCtl   ��������ģʽ. 0: �޿���; 1: ��; 2: Ӳ
	 * @return
	 * ���ڴ򿪽��
	 */
	bool Open(const string& portname, int baud_rate = 9600, int dataBit = 8, int parity = 0, int stopBit = 1, int flowCtl = 0);
	/*!
	 * @brief ���Թرմ���
	 */
	void Close();
	/*!
	 * @brief ���ڴ򿪱�־
	 * @return
	 * �����Ƿ��Ѿ���
	 */
	bool IsOpen();
	/*!
	 * @brief �����ѽ�����Ϣ�е�һ���ַ�
	 * @param flag ��ʶ��
	 * @return
	 * �ѽ�����Ϣ����
	 */
	int Lookup(char* first = NULL);
	/*!
	 * @brief �����ѽ�����Ϣ�е�һ�γ���flag��λ��
	 * @param flag ��ʶ�ַ���
	 * @param len  ��ʶ�ַ�������
	 * @param from ��from��ʼ���ұ�ʶ��
	 * @return
	 * ��ʶ����һ�γ���λ��. ��flag�������򷵻�-1
	 */
	int Lookup(const char* flag, const int len, const int from = 0);
	/*!
	 * @brief �򴮿ڷ�����Ϣ
	 * @param buff ��������Ϣ
	 * @param len  ��������Ϣ����
	 * @return
	 * �ѷ�����Ϣ����
	 */
	int Write(const char* buff, const int len);
	/*!
	 * @brief �Ӵ��ڶ�����Ϣ
	 * @param buff ������Ϣ������
	 * @param len  ����������Ϣ����
	 * @param from ��from��ʼ������Ϣ
	 * @return
	 * ʵ�ʶ�����Ϣ����
	 */
	int Read(char* buff, const int len, const int from = 0);
	/*!
	 * @brief ע��read_some�ص�����, �����յ���������Ϣ
	 * @param slot �������
	 */
	void RegisterRead(const CBSlot& slot);
	/*!
	 * @brief ע��write_some�ص�����, ����������Ϣ���ͽ��
	 * @param slot �������
	 */
	void RegisterWrite(const CBSlot& slot);

protected:
	/* ���� */
	/*!
	 * @brief �첽ͨ����, ���������ݶ���
	 * @param ec �������
	 * @param n  �Ѷ������ݳ���, ����: �ֽ�
	 */
	void handle_read(const error_code& ec, int n);
	/*!
	 * @brief �첽ͨ����, ����������д��
	 * @param ec �������
	 * @param n  ��д�����ݳ���, ����: �ֽ�
	 */
	void handle_write(const error_code& ec, int n);
	/*!
	 * @brief ���Խ��մ�����Ϣ
	 */
	void start_read();
	/*!
	 * @brief ���Է��ͻ���������
	 */
	void start_write();
};
typedef SerialComm::Pointer SerialPtr;

#endif /* SERIALCOMM_H_ */
