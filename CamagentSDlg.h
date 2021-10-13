
// CamagentSDlg.h : 头文件
//
#pragma once

#include "AsioTCP.h"
#include "afxwin.h"
#include "Parameter.h"
#include "PLCInner.h"
#include "PLCOuter.h"

// CCamagentSDlg 对话框
class CCamagentSDlg : public CDialogEx
{
// 构造
public:
	CCamagentSDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMAGENTS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	// Windows成员变量
	HICON m_hIcon;
	// 天窗
	CButton m_btnSlitOpenPeriod;
	CButton m_btnSlitOpen;
	CButton m_btnSlitClose;
	CButton m_btnSlitStop;
	int m_nSlitPeriod;
	// 调焦
	CButton m_btnFocusPlus;
	CButton m_btnFocusMinus;
	CButton m_btnFocusGoto;
	CButton m_btnFocusReset;

	/*===============================================================*/
	// 定制成员变量
	Parameter param_;		/// 配置参数
	CPLCInner plcInner_;	/// PLC接口: 天窗+室内降水
	CPLCOuter plcOuter_;	/// PLC接口: 调焦+室外降水
	TcpCPtr tcpcCamera_;	/// 网络接口: 相机=>调度
	TcpCPtr tcpcAnnex_;		/// 网络接口: 外设=>调度

	/*线程及同步*/
	typedef boost::shared_ptr<boost::thread> ThrdPtr;
	typedef boost::unique_lock<boost::mutex> MtxLck;

	ThrdPtr thrdClock_;			/// 显示本机本地时钟
	ThrdPtr thrdLiefCycle_;		/// 生命周期维护线程
	boost::condition_variable cvDevClosed_;	/// 设备已关闭, 触发重新连接
	/*===============================================================*/

protected:
	// 自定义功能
	void interrupt_thread(ThrdPtr& thrd);
	void thread_clock();
	void thread_lief_cycle();

	bool start_plc_inner();
	void stop_plc_inner();

	bool start_plc_outer();
	void stop_plc_outer();
	/*===============================================================*/

// MFC框架实现
protected:
	// 生成的消息映射函数
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
