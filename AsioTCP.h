/**
 * @file AsioTCP �����ļ�, ����boost::asioʵ��TCPͨ�Žӿ�
 * @version 0.2
 * @date 2017-10-01
 * - ����v0.1�Ż�����ʵ��
 * - ��TCPClient��װ�ͻ��˽ӿ�
 * - ��TCPServer��װ�������ӿ�
 * @version 0.3
 * @date 2017-11-11
 * - ֧���޻��幤��ģʽ
 * - �ͻ��˽������Ӻ�����KEEP_ALIVE
 * - �Ż�����������
 * @version 1.0
 * @date 2020-10-02
 * @note
 * - �Ż�
 */

#ifndef SRC_ASIOTCP_H_
#define SRC_ASIOTCP_H_

#include <boost/asio/ip/tcp.hpp>
#include <boost/signals2.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/smart_ptr/shared_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string.h>
#include <string>
#include "AsioIOServiceKeep.h"

/////////////////////////////////////////////////////////////////////
/*--------------------- �ͻ��� ---------------------*/
#define TCP_PACK_SIZE		1500

class TcpClient : public boost::enable_shared_from_this<TcpClient> {
public:
	using Pointer = boost::shared_ptr<TcpClient>;
	/*!
	 * @brief �����ص����������
	 * @param 1 �ͻ��˶���
	 * @param 2 ʵ��ָ��
	 */
	using CallbackFunc = boost::signals2::signal<void (Pointer, const boost::system::error_code&)>;
	using CBSlot = CallbackFunc::slot_type;
	using TCP = boost::asio::ip::tcp;	// boost::ip::tcp����
	using CRCBuff = boost::circular_buffer<char>;	// �ַ���ѭ������
	using CBuff = boost::shared_array<char>;	//< char������
	using MtxLck = boost::unique_lock<boost::mutex>;	//< �źŵƻ�����

protected:
	bool mode_async_;		//< �첽��д/ģʽ
	AsioIOServiceKeep keep_;	//< �ṩboost::asio::io_service����, ����ʵ�������ڼ䱣��������
	TCP::socket sock_;			//< �׽ӿ�
	boost::mutex mtx_read_;		//< ������: ���׽ӿڶ�ȡ
	boost::mutex mtx_write_;	//< ������: ���׽ӿ�д��
	CBuff buf_read_;			//< ������: ���ν���
	int byte_read_;				//< ���ν������ݳ���
	CRCBuff crcbuf_read_;		//< ������: ���н���
	CRCBuff crcbuf_write_;		//< ������: ���д�д��
	CallbackFunc  cbconn_;	//< connect�ص�����
	CallbackFunc  cbread_;	//< read�ص�����
	CallbackFunc  cbwrite_;	//< write�ص�����

public:
	TcpClient(bool modeAsync = true);
	virtual ~TcpClient();

	/*!
	 * @brief ����TcpClient::Pointerʵ��
	 * @return
	 * shared_ptr<TcpClient>����ʵ��ָ��
	 */
	static Pointer Create(bool modeAsync = true) {
		return Pointer(new TcpClient(modeAsync));
	}
	/*!
	 * @brief �鿴�׽���
	 * @return
	 * TCP�׽���
	 */
	TCP::socket& Socket();
	/*!
	 * @brief ͬ����ʽ�������ӷ�����
	 * @param host  ��������ַ������
	 * @param port  ����˿�
	 * @return
	 * ���ӽ��
	 */
	bool Connect(const std::string& host, const uint16_t port);
	/*!
	 * @brief �ر��׽���
	 * @return
	 * �׽��ֹرս��.
	 * 0 -- �ɹ�
	 * ���� -- �����ʶ
	 */
	int Close();
	/*!
	 * @brief ����׽���״̬
	 * @return
	 * �׽���״̬
	 */
	bool IsOpen();
	/*!
	 * @brief ���ѽ�����Ϣ�ж�ȡָ�����ݳ���, ���ӻ��������������������
	 * @param data ����洢��
	 * @param n    ����ȡ���ݳ���
	 * @param from ��from��ʼ��ȡ
	 * @return
	 * ʵ�ʶ�ȡ���ݳ���
	 */
	int Read(char* data, const int n, const int from = 0);
	/*!
	 * @brief ����ָ������
	 * @param data ���������ݴ洢��ָ��
	 * @param n    ���������ݳ���
	 * @return
	 * ʵ�ʷ������ݳ���
	 */
	int Write(const char* data, const int n);
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
	 * @param n    ��ʶ�ַ�������
	 * @param from ��from��ʼ����flag
	 * @return
	 * ��ʶ����һ�γ���λ��. ��flag�������򷵻�-1
	 */
	int Lookup(const char* flag, const int n, const int from = 0);
	/*!
	 * @brief �������˽����������Ӻ����, ������������
	 */
	void Start();
	/*!
	 * @brief ע��connect�ص�����, ����������������ӽ��
	 * @param slot �������
	 */
	void RegisterConnect(const CBSlot& slot);
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
	/*!
	 * @brief ���Խ���������Ϣ
	 */
	void start_read();
	/*!
	 * @brief ���Է��ͻ���������
	 */
	void start_write();
	/* ��Ӧasync_�����Ļص����� */
	/*!
	 * @brief �����������ӽ��
	 * @param ec �������
	 */
	void handle_connect(const boost::system::error_code& ec);
	/*!
	 * @brief �����յ���������Ϣ
	 * @param ec �������
	 * @param n  �������ݳ���, ����: �ֽ�
	 */
	void handle_read(const boost::system::error_code& ec, int n);
	/*!
	 * @brief �����첽������Ϣ���ͽ��
	 * @param ec �������
	 * @param n  �������ݳ���, ����: �ֽ�
	 */
	void handle_write(const boost::system::error_code& ec, int n);
};
using TcpCPtr = TcpClient::Pointer;

/////////////////////////////////////////////////////////////////////
/*--------------------- ������ ---------------------*/
class TcpServer : public boost::enable_shared_from_this<TcpServer> {
protected:
	using TCP = boost::asio::ip::tcp;

public:
	using Pointer = boost::shared_ptr<TcpServer>;
	/*!
	 * @brief �����ص����������
	 * @param 1 �ͻ��˶���
	 * @param 2 ʵ��ָ��
	 */
	using CallbackFunc = boost::signals2::signal<void (const TcpCPtr, const Pointer)>;
	using CBSlot = CallbackFunc::slot_type;

protected:
	AsioIOServiceKeep keep_;	//< �ṩboost::asio::io_service����, ����ʵ�������ڼ䱣��������
	TCP::acceptor accept_;		//< �������
	CallbackFunc cbfunc_;		//< �ص�����

public:
	TcpServer();
	virtual ~TcpServer();
	/*!
	 * @brief ����TcpServer::Pointerʵ��
	 * @return
	 * shared_ptr<TcpServer>����ʵ��ָ��
	 */
	static Pointer Create() {
		return Pointer(new TcpServer);
	}
	/*!
	 * @brief ע��accept�ص�����, ����������յ���������������
	 * @param slot �������
	 */
	void RegisterAccept(const CBSlot &slot);
	/*!
	 * @brief ������portָ���Ķ˿��ϴ���TCP�������
	 * @param port ����˿�
	 * @param v6   ��������. true: V6, false: V4
	 * @return
	 * TCP������񴴽����
	 * 0 -- �ɹ�
	 * ���� -- �������
	 */
	int CreateServer(uint16_t port, bool v6 = false);

protected:
	/*!
	 * @brief �����������
	 */
	void start_accept();
	/*!
	 * @brief �����յ���������������
	 * @param client �����׽���
	 * @param ec     �������
	 */
	void handle_accept(const TcpCPtr client, const boost::system::error_code& ec);
};
using TcpSPtr = boost::shared_ptr<TcpServer>;

/////////////////////////////////////////////////////////////////////
#endif /* SRC_ASIOTCP_H_ */
