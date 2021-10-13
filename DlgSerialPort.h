/**
 * @class CDlgSerialPort ���ô��ڲ���
 */
#pragma once
#include "afxwin.h"
#include "Parameter.h"

// CDlgSerialPort �Ի���

class CDlgSerialPort : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSerialPort)

public:
	CDlgSerialPort(Parameter *param, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSerialPort();

// �Ի�������
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

	Parameter *param_;	/// ���ò���

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();

	/**
	 * @brief ��ע�����Ҵ��ڲ����ؼ�
	 */
	void FillPortname();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
};
