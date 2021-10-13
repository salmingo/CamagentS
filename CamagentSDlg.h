
// CamagentSDlg.h : ͷ�ļ�
//
#pragma once

#include "AsioTCP.h"
#include "afxwin.h"
#include "Parameter.h"
#include "PLCInner.h"
#include "PLCOuter.h"

// CCamagentSDlg �Ի���
class CCamagentSDlg : public CDialogEx
{
// ����
public:
	CCamagentSDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMAGENTS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	// Windows��Ա����
	HICON m_hIcon;
	// �촰
	CButton m_btnSlitOpenPeriod;
	CButton m_btnSlitOpen;
	CButton m_btnSlitClose;
	CButton m_btnSlitStop;
	int m_nSlitPeriod;
	// ����
	CButton m_btnFocusPlus;
	CButton m_btnFocusMinus;
	CButton m_btnFocusGoto;
	CButton m_btnFocusReset;

	/*===============================================================*/
	// ���Ƴ�Ա����
	Parameter param_;		/// ���ò���
	CPLCInner plcInner_;	/// PLC�ӿ�: �촰+���ڽ�ˮ
	CPLCOuter plcOuter_;	/// PLC�ӿ�: ����+���⽵ˮ
	TcpCPtr tcpcCamera_;	/// ����ӿ�: ���=>����
	TcpCPtr tcpcAnnex_;		/// ����ӿ�: ����=>����

	/*�̼߳�ͬ��*/
	typedef boost::shared_ptr<boost::thread> ThrdPtr;
	typedef boost::unique_lock<boost::mutex> MtxLck;

	ThrdPtr thrdClock_;			/// ��ʾ��������ʱ��
	ThrdPtr thrdLiefCycle_;		/// ��������ά���߳�
	boost::condition_variable cvDevClosed_;	/// �豸�ѹر�, ������������
	/*===============================================================*/

protected:
	// �Զ��幦��
	void interrupt_thread(ThrdPtr& thrd);
	void thread_clock();
	void thread_lief_cycle();

	bool start_plc_inner();
	void stop_plc_inner();

	bool start_plc_outer();
	void stop_plc_outer();
	/*===============================================================*/

// MFC���ʵ��
protected:
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();

	afx_msg void OnBnClickedBtnConfigSerial();
	afx_msg void OnBnClickedBtnConfigNetwork();
	afx_msg void OnBnClickedBtnConfigFocuser();

	afx_msg void OnBnClickedBtnSlitOpenPeriod();
	afx_msg void OnBnClickedBtnSlitOpen();
	afx_msg void OnBnClickedBtnSlitClose();
	afx_msg void OnBnClickedBtnSlitStop();

	afx_msg void OnBnClickedBtnFocusPlus();
	afx_msg void OnBnClickedBtnFocusMinus();
	afx_msg void OnBnClickedBtnFocusGoto();
	afx_msg void OnBnDoubleclickedBtnFocusReset();
};
