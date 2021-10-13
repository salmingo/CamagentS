/*!
 * @class CPLCInner ��װ����PLC��Բ�����ƺͽ�ˮ��֪
 * @note
 * ����˳��:
 * 1/ SetSerialPort
 * 2/ Start
 * 3/ ָ��
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
	// ��������
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
	// ��Ա����
	int cntError_;	/// ����: ͨ�Ŵ���
	int openPeriod_;/// �촰������
	int slitState_[SLIT_MAX];	/// �촰״̬. 0: ��; 1: ��; 2: �ۺ�
	int slitCmd_[SLIT_STAT];	/// �촰ָ��. 0: ��; 1: ��
	bool rainy_;		/// ��ˮ��־

	QueCommand queCmd_[SLIT_STAT];		/// �촰ָ�����
	boost::mutex mtxQue_[SLIT_STAT];	/// ������

	ThrdPtr thrdCommand_;	/// �߳̽ӿ�: �����򴮿ڷ���ָ��
	boost::condition_variable cv_read_;	/// �յ����ڷ���

public:
	// �ӿ�
	/*!
	 * @brief ��������
	 */
	bool Start();
	/*!
	 * @brief ֹͣ����
	 */
	void Stop();
	/*!
	 * @brief ���촰
	 * @param period  ��ָ�����ʱ��, ����: ��
	 * @note
	 * - ��period<=0ʱ, ��ֱ��������λ
	 * - ��period>0ʱ, ����ָ�����ʱ�����ָֹͣ��
	 */
	bool OpenSlit(int period = 0);
	/*!
	 * @brief �ر��촰
	 * @note
	 * - ����
	 */
	bool CloseSlit();
	/*!
	 * @brief �ж��촰�Ĵ򿪻�رչ���
	 */
	bool StopSlit();
	/*!
	 * @brief �鿴�촰����״̬ 
	 */
	void GetSlitState(int& state, int* stateLeft, int* stateRight);
	/*!
	 * @brief �鿴�Ƿ�ˮ
	 */
	bool IsRainy();

protected:
	// ����: ����ͨ��
	void serial_read(SerialPtr ptr, const boost::system::error_code& ec);
	void interrupt_thread(ThrdPtr& thrd);
	/*!
	 * @brief �߳�: �򴮿ڷ���ָ��
	 */
	void thread_command();
};
