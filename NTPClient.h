#pragma once
/*
 * @file NTPClient.h ��NTPClient�����ļ�
 * @author       ¬����
 * @description  ��鱾����NTP��������ʱ��ƫ��, ����������ʱ��
 * @version      1.0
 * @date         2016��10��29��
 * @note
 * (1) ÿ���Ӽ��һ�α�����NTP��ʱ��ƫ��. ��ʱ��ƫ��ϴ�ʱ, ����־�ļ��м�¼����ʾ
 * (2) ����Ҫ��������ʱ��ʱ, ֱ�Ӳ������һ�ε�ʱ��ƫ��
 * (3) ��������ʱ��
 */

#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
#include <winsock.h>
#include <string>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

class NTPClient {
public:
	typedef struct _ntp_time {
		unsigned int coarse;
		unsigned int fine;
	}ntp_time;

	/**
	 * offset = ((T2 - T1) + (T3 - T4)) / 2
	 * delay  = (T4 - T1) - (T3 - T2)
	 *
	 * local_time_corrected = local_time_pc + offset
	 */

	struct ntp_packet {
		unsigned char leap_ver_mode;
		unsigned char stratum;
		char poll;
		char precision;
		int root_delay;
		int root_dispersion;
		unsigned char reference_identifier[4];		// in the case of stratum 0 or 1, this is a four-octet, left-justified
										// zero-padded ASCII string, referring to clock resource.
										// in the case of stratum 2 and greater, this is the four-octet internet
										// address of the primary reference host
		ntp_time reference_timestamp;	// ����ʱ��������û�������ʱ��
		ntp_time originate_timestamp;	// T1: �ɱ�����NTP��������������ı���ʱ��
		ntp_time receive_timestamp;		// T2: ����ִ������ʱ�ı���ʱ��
		ntp_time transmit_timestamp;	// T3: �����뿪������ʱ�ı���ʵ��
										// T4: �����ִﱾ��ʱ��ʱ��
	};
	/* ������������ */
	typedef boost::unique_lock<boost::mutex> mutex_lock; //< ����boost::mutex�Ļ�����
	typedef boost::shared_ptr<boost::thread> threadptr;  //< �߳�ָ������
	typedef boost::shared_array<char> carray;		//< �ַ�������

public:
	/*!
	 * @brief ���캯��
	 * @param hostIP  NTP����IPv4��ַ
	 * @param port    NTP����˿�, Ĭ��123
	 * @param tSyn    ����ʱ�ӵ����ʱ��ƫ��, ����: ����
	 */
	NTPClient(const char* hostIP, const uint16_t port = 123, const int tSync = 5);
	virtual ~NTPClient();

protected:
	/* ������Ա���� */
	boost::mutex mtx_;		//< ������
	threadptr    thrd_;		//< �߳�ָ��
	std::string  host_;		//< NTP��������IPv4��ַ
	uint16_t     port_;		//< NTP�������Ķ˿�
	int          sock_;		//< SOCKET�׽���
	carray       pack_;		//< ���罻����Ϣ
	double       offset_;	//< ʱ��ƫ��, ����: ��
	bool         valid_;		//< ������Ч��
	int          nfail_;		//< ʱ��ƫ����ʧ�ܴ���
	double       tSync_;		//< ��������ʱ�ӵ����ʱ��ƫ��
	bool         autoSync_;	//< �Ƿ��Զ�����ʱ��ƫ��

protected:
	/*!
	 * @brief ����������������Ϣ
	 */
	void construct_packet();
	/*!
	 * @brief �ɼ�NTPʱ��
	 * @param addr      ��ַ
	 * @param ret_time  NTP���ݰ�
	 * @return
	 */
	int get_time(struct addrinfo* addr, struct ntp_packet* ret_time);
	/*!
	 * @brief �߳�����
	 */
	void thread_body();

public:
	/*!
	 * @brief ����NTP������
	 * @param ip   IPv4��ַ
	 * @param port ����˿�
	 */
	void SetHost(const char* ip, const uint16_t port = 123);
	/*!
	 * @brief ����ʱ��������ֵ
	 * @param tSync ʱ��������ֵ, ����: ����
	 */
	void SetSyncLimit(const int tSync = 5);
	/*!
	 * @brief ͬ������ʱ��
	 */
	void SynchClock();
	/*!
	 * @brief ���û��ֹ�Զ�ʱ��ͬ��
	 */
	void EnableAutoSynch(bool bEnabled = true);
};
typedef boost::shared_ptr<NTPClient> NTPPtr; //< NTPclientָ��
/*!
 * @brief ��������, �����µ�NTPClientָ��
 * @return
 * ָ�봴�����
 */
extern NTPPtr make_ntp(const char* hostIP, const uint16_t port, const int tSync);
