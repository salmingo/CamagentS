/**
 * @class CDlgSerialPort 配置串口参数
 */
#pragma once
#include "afxwin.h"
#include "Parameter.h"

// CDlgSerialPort 对话框

class CDlgSerialPort : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSerialPort)

public:
	CDlgSerialPort(Parameter *param, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSerialPort();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERIAL_PORT };
#endif

protected:
	CComboBox m_cmbPortNameIn;
	CComboBox m_cmbBaudrateIn;
	CComboBox m_cmbDatabitIn;
	CComboBox m_cmbParityIn;
	CComboBox m_cmbStopbitIn;
	CComboBox m_cmbFlowctlIn;

	CComboBox m_cmbPortNameOut;
	CComboBox m_cmbBaudrateOut;
	CComboBox m_cmbDatabitOut;
	CComboBox m_cmbParityOut;
	CComboBox m_cmbStopbitOut;
	CComboBox m_cmbFlowctlOut;
	CButton m_chkEnableOutRainfall;

	Parameter *param_;	/// 配置参数

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	/**
	 * @brief 从注册表查找串口并填充控件
	 */
	void FillPortname();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};
