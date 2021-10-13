#pragma once

#include "afxwin.h"
#include "Parameter.h"

// CDlgNetwork 对话框

class CDlgNetwork : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNetwork)

public:
	CDlgNetwork(Parameter* param, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgNetwork();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NETWORK };
#endif

protected:
	CIPAddressCtrl m_ipGtoaes;
	int m_portCamera;
	int m_portAnnex;
	CString m_idGroup;
	CString m_idUnit;
	CString m_idCamera;
	BOOL m_enableGtoaes;

	CIPAddressCtrl m_ipFileServer;
	int m_portFileServer;
	BOOL m_enableFileServer;

	CIPAddressCtrl m_ipNTP;
	BOOL m_enableNTP;
	
	Parameter* param_;	/// 配置参数

protected:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};
