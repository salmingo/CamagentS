/**
 * @file AsioIOServiceKeep.h ��װboost::asio::io_service, ά��run()�����������ڵ���Ч��
 * @date 2017-01-27
 * @version 0.1
 * @author Xiaomeng Lu
 * @note
 * @li boost::asio::io_service::run()����Ӧ��ע����첽���ú��Զ��˳�. Ϊ�˱����˳�run()����,
 * ����ioservice_keepά���䳤����Ч��
 * @li ʹ��shared_ptr����ָ��
 */

#ifndef SRC_ASIOIOSERVICEKEEP_H_
#define SRC_ASIOIOSERVICEKEEP_H_

#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

class AsioIOServiceKeep {
public:
	using IOService = boost::asio::io_service;

protected:
	using Work = IOService::work;
	using WorkPtr = boost::shared_ptr<Work>;
	using ThreadPtr = boost::shared_ptr<boost::thread>;
	/* ��Ա���� */
	IOService io_service_;
	WorkPtr work_;
	ThreadPtr thrd_keep_;

public:
	AsioIOServiceKeep();
	virtual ~AsioIOServiceKeep();
	IOService& GetIOService();

protected:
	void thread_keep();
};

#endif /* SRC_ASIOIOSERVICEKEEP_H_ */
