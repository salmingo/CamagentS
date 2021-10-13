#pragma once

#include "Parameter.h"
// CDlgFocus 对话框

class CDlgFocus : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFocus)

public:
	CDlgFocus(ParamFocuser* param, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFocus();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FOCUS };
#endif

protected:
	ParamFocuser* param_;
	int m_min;
	int m_max;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};
