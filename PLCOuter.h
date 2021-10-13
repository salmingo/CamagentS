/*!
 * @class CPLCOuter ��װ����PLC�ĵ������ƺͽ�ˮ��֪
 * @note
 * ����˳��:
 * 1/ Start
 * 2/ ָ��
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
	// ��������
	typedef boost::shared_ptr<boost::thread> ThrdPtr;
	typedef boost::unique_lock<boost::mutex> MtxLck;

protected:
	// ��Ա����
	ParamSerialComm* paramSerialComm_;
	ParamFocuser* paramFocuser_;

	int cntError_;	/// ����: ͨ�Ŵ���

	int state_;		/// ������״̬
	bool rainy_;	/// ��ˮ��־
	int posReal_;	/// ʵ��λ��
	int posTarget_;	/// Ŀ��λ��

	ThrdPtr thrdCommand_;	/// �߳̽ӿ�: �����򴮿ڷ���ָ��
	boost::condition_variable cv_read_;	/// �յ����ڷ���

public:
	// �ӿ�
	/*!
	 * @brief ��������
	 */
	bool Start(Parameter* param);
	/*!
	 * @brief ֹͣ����
	 */
	void Stop();
	/*!
	 * @brief ��λ������λ��
	 */
	bool Goto(int position);
	/*!
	 * @brief ���ڵ�ǰλ����Զ�λ
	 */
	bool GotoRelative(int step);
	/*!
	 * @breif �鿴����λ��
	 * @param real   ʵ��λ��
	 * @param target Ŀ��λ��
	 * @return
	 * ����״̬
	 */
	int GetPosition(int& real, int& target);
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

