// DlgSerialPort.cpp : 实现文件
//

#include "stdafx.h"
#include "CamagentS.h"
#include "DlgSerialPort.h"
#include "afxdialogex.h"

// CDlgSerialPort 对话框

IMPLEMENT_DYNAMIC(CDlgSerialPort, CDialogEx)

CDlgSerialPort::CDlgSerialPort(Parameter *param, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SERIAL_PORT, pParent)
{
	param_ = param;
}

CDlgSerialPort::~CDlgSerialPort()
{
}

void CDlgSerialPort::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IN_PORTNAME, m_cmbPortNameIn);
	DDX_Control(pDX, IDC_COMBO_IN_BAUDRATE, m_cmbBaudrateIn);
	DDX_Control(pDX, IDC_COMBO_IN_DATABIT, m_cmbDatabitIn);
	DDX_Control(pDX, IDC_COMBO_IN_PARITY, m_cmbParityIn);
	DDX_Control(pDX, IDC_COMBO_IN_STOPBIT, m_cmbStopbitIn);
	DDX_Control(pDX, IDC_COMBO_IN_FLOWCTL, m_cmbFlowctlIn);

	DDX_Control(pDX, IDC_COMBO_OUT_PORTNAME, m_cmbPortNameOut);
	DDX_Control(pDX, IDC_COMBO_OUT_BAUDRATE, m_cmbBaudrateOut);
	DDX_Control(pDX, IDC_COMBO_OUT_DATABIT, m_cmbDatabitOut);
	DDX_Control(pDX, IDC_COMBO_OUT_PARITY, m_cmbParityOut);
	DDX_Control(pDX, IDC_COMBO_OUT_STOPBIT, m_cmbStopbitOut);
	DDX_Control(pDX, IDC_COMBO_OUT_FLOWCTL, m_cmbFlowctlOut);
	DDX_Control(pDX, IDC_CHK_ENABLE_OUT_RAINFALL, m_chkEnableOutRainfall);
}

void CDlgSerialPort::OnOK()
{
}

void CDlgSerialPort::OnCancel()
{
}

BOOL CDlgSerialPort::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	FillPortname();
	TCHAR name[50];
	int n = param_->serialCommInner.portName.size();
	MultiByteToWideChar(CP_ACP, 0, param_->serialCommInner.portName.c_str(), n, name, n);
	name[n] = 0;
	m_cmbPortNameIn.SetWindowText(name);

	n = param_->serialCommOuter.portName.size();
	MultiByteToWideChar(CP_ACP, 0, param_->serialCommOuter.portName.c_str(), n, name, n);
	name[n] = 0;
	m_cmbPortNameOut.SetWindowText(name);

	// 初始化控件
	CString txt;
	txt.Format(_T("%d"), param_->serialCommInner.baudRate);
	m_cmbBaudrateIn.SetWindowText(txt);
	m_cmbDatabitIn.SetCurSel(param_->serialCommInner.dataBit == 7 ? 0 : 1);
	m_cmbParityIn.SetCurSel(param_->serialCommInner.parity);
	m_cmbStopbitIn.SetCurSel(param_->serialCommInner.stopBit);
	m_cmbFlowctlIn.SetCurSel(param_->serialCommInner.flowCtl);

	txt.Format(_T("%d"), param_->serialCommOuter.baudRate);
	m_cmbBaudrateOut.SetWindowText(txt);
	m_cmbDatabitOut.SetCurSel(param_->serialCommOuter.dataBit == 7 ? 0 : 1);
	m_cmbParityOut.SetCurSel(param_->serialCommOuter.parity);
	m_cmbStopbitOut.SetCurSel(param_->serialCommOuter.stopBit);
	m_cmbFlowctlOut.SetCurSel(param_->serialCommOuter.flowCtl);

	if (param_->serialCommOuter.enabled)
		m_chkEnableOutRainfall.SetCheck(BST_CHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgSerialPort::FillPortname() {
	HKEY hKey;

	if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), NULL, KEY_READ, &hKey)) {
		int i(-1);
		DWORD dwLong, dwSize;
		TCHAR portName[256], commName[256];
		BOOL bValid;

		m_cmbPortNameIn.ResetContent();
		m_cmbPortNameOut.ResetContent();
		do {
			dwLong = dwSize = sizeof(portName);
			bValid = !RegEnumValue(hKey, ++i, portName, &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize);
			if (bValid) {
				m_cmbPortNameIn.AddString(commName);
				m_cmbPortNameOut.AddString(commName);
			}
		} while (bValid);

		RegCloseKey(hKey);
	}
}

BEGIN_MESSAGE_MAP(CDlgSerialPort, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CDlgSerialPort 消息处理程序
void CDlgSerialPort::OnClose()
{
	// 读取参数并判定是否有更新
	CString txt;
	int n1(0), n2(0), idx, val;
	BOOL enableRainfall = m_chkEnableOutRainfall.GetCheck() == BST_CHECKED;

	m_cmbPortNameIn.GetWindowText(txt);
	if (!txt.IsEmpty()) {
		int n = txt.GetLength();
		char* name = new char[n + 1];
		WideCharToMultiByte(CP_ACP, 0, txt, n, name, n, 0, 0);
		name[n] = 0;
		if (param_->serialCommInner.portName != name) { ++n1; param_->serialCommInner.portName = name; }
		delete[]name;
	}
	m_cmbBaudrateIn.GetWindowText(txt);
	if (!txt.IsEmpty()) {
		val = _ttoi(txt);
		if (val != param_->serialCommInner.baudRate) { ++n1; param_->serialCommInner.baudRate = val; }
	}
	m_cmbDatabitIn.GetWindowText(txt);
	if (!txt.IsEmpty()) {
		val = _ttoi(txt);
		if (val != param_->serialCommInner.dataBit) { ++n1; param_->serialCommInner.dataBit = val; }
	}
	if ((idx = m_cmbParityIn.GetCurSel()) != param_->serialCommInner.parity)   { ++n1; param_->serialCommInner.parity  = idx; }
	if ((idx = m_cmbStopbitIn.GetCurSel()) != param_->serialCommInner.stopBit) { ++n1; param_->serialCommInner.stopBit = idx; }
	if ((idx = m_cmbFlowctlIn.GetCurSel()) != param_->serialCommInner.flowCtl) { ++n1; param_->serialCommInner.flowCtl = idx; }
	param_->serialCommInner.dirty = n1;

	m_cmbPortNameOut.GetWindowText(txt);
	if (!txt.IsEmpty()) {
		int n = txt.GetLength();
		char* name = new char[n + 1];
		WideCharToMultiByte(CP_ACP, 0, txt, n, name, n, 0, 0);
		name[n] = 0;
		if (param_->serialCommOuter.portName != name) { ++n1; param_->serialCommOuter.portName = name; }
		delete[]name;
	}
	m_cmbBaudrateOut.GetWindowText(txt);
	if (!txt.IsEmpty()) {
		val = _ttoi(txt);
		if (val != param_->serialCommOuter.baudRate) { ++n1; param_->serialCommOuter.baudRate = val; }
	}
	m_cmbDatabitOut.GetWindowText(txt);
	if (!txt.IsEmpty()) {
		val = _ttoi(txt);
		if (val != param_->serialCommOuter.dataBit) { ++n1; param_->serialCommOuter.dataBit = val; }
	}
	if ((idx = m_cmbParityOut.GetCurSel()) != param_->serialCommOuter.parity)   { ++n2; param_->serialCommOuter.parity = idx; }
	if ((idx = m_cmbStopbitOut.GetCurSel()) != param_->serialCommOuter.stopBit) { ++n2; param_->serialCommOuter.stopBit = idx; }
	if ((idx = m_cmbFlowctlOut.GetCurSel()) != param_->serialCommOuter.flowCtl) { ++n2; param_->serialCommOuter.flowCtl = idx; }
	if (enableRainfall != param_->serialCommOuter.enabled) { ++n2; param_->serialCommOuter.enabled = enableRainfall; }
	param_->serialCommOuter.dirty = n2;

	if (param_->serialCommInner.portName.size() && param_->serialCommInner.portName == param_->serialCommOuter.portName) 
		MessageBox(_T("使用同一串口将带来访问冲突"));
	else
		CDialogEx::OnOK();
}
