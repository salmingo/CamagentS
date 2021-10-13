// DlgNetwork.cpp: 实现文件
//
#include "stdafx.h"
#include "CamagentS.h"
#include "DlgNetwork.h"
#include "afxdialogex.h"
#include <string>
#include <sstream>

using namespace std;

// CDlgNetwork 对话框

IMPLEMENT_DYNAMIC(CDlgNetwork, CDialogEx)

CDlgNetwork::CDlgNetwork(Parameter* param, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_NETWORK, pParent)
	, m_idGroup(_T(""))
	, m_idUnit(_T(""))
	, m_idCamera(_T(""))
{
	param_ = param;

	m_portCamera = param_->netGtoaes.portCamera;
	m_portAnnex = param_->netGtoaes.portAnnex;
	m_enableGtoaes = param_->netGtoaes.enabled;

	m_portFileServer = param_->netFileSrv.port;
	m_enableFileServer = param_->netFileSrv.enabled;

	m_enableNTP = param_->netNTP.enabled;
}

CDlgNetwork::~CDlgNetwork()
{
}

void CDlgNetwork::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDR_GTOAES, m_ipGtoaes);
	DDX_Text(pDX, IDC_ED_PORT_CAMERA, m_portCamera);
	DDX_Text(pDX, IDC_ED_PORT_ANNEX, m_portAnnex);
	DDX_Text(pDX, IDC_ED_GID, m_idGroup);
	DDX_Text(pDX, IDC_ED_UID, m_idUnit);
	DDX_Text(pDX, IDC_ED_CID, m_idCamera);
	DDX_Check(pDX, IDC_CHK_ENABLE_GTOAES, m_enableGtoaes);
	DDX_Control(pDX, IDC_IPADDR_FILE_SERVER, m_ipFileServer);
	DDX_Text(pDX, IDC_ED_PORT_FILE_SERVER, m_portFileServer);
	DDX_Check(pDX, IDC_CHK_ENABLE_FILE_SERVER, m_enableFileServer);
	DDX_Control(pDX, IDC_IPADDR_NTP, m_ipNTP);
	DDX_Check(pDX, IDC_CHK_ENABLE_NTP, m_enableNTP);
}

void CDlgNetwork::OnOK()
{
}

void CDlgNetwork::OnCancel()
{
}

BOOL CDlgNetwork::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 转换网络标志
	TCHAR id[100];
	int len = param_->netGtoaes.gid.size();
	MultiByteToWideChar(CP_ACP, 0, param_->netGtoaes.gid.c_str(), len, id, len);
	id[len] = 0;
	m_idGroup = id;

	len = param_->netGtoaes.uid.size();
	MultiByteToWideChar(CP_ACP, 0, param_->netGtoaes.uid.c_str(), len, id, len);
	id[len] = 0;
	m_idUnit = id;

	len = param_->netGtoaes.cid.size();
	MultiByteToWideChar(CP_ACP, 0, param_->netGtoaes.cid.c_str(), len, id, len);
	id[len] = 0;
	m_idCamera = id;
	// 转换IP地址
	union {
		BYTE field[4];
		DWORD addr;
	} addr;
	int i;
	string str;
	stringstream ss1(param_->netGtoaes.ip);
	i = 3;
	while (getline(ss1, str, '.'))
		addr.field[i--] = stoi(str);
	m_ipGtoaes.SetAddress(addr.addr);

	stringstream ss2(param_->netFileSrv.ip);
	i = 3;
	while (getline(ss2, str, '.'))
		addr.field[i--] = stoi(str);
	m_ipFileServer.SetAddress(addr.addr);

	stringstream ss3(param_->netNTP.ip);
	i = 3;
	while (getline(ss3, str, '.'))
		addr.field[i--] = stoi(str);
	m_ipNTP.SetAddress(addr.addr);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
BEGIN_MESSAGE_MAP(CDlgNetwork, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CDlgNetwork 消息处理程序

void CDlgNetwork::OnClose()
{
	UpdateData(TRUE);

	union {
		BYTE field[4];
		DWORD addr;
	} addr;
	char ip[20], id[40];
	int n1(0), n2(0), n3(0), n;
	// 调度服务
	ParamNetworkGtoaes& gto = param_->netGtoaes;
	if (gto.portCamera != m_portCamera) {
		++n1;
		gto.portCamera = m_portCamera;
	}
	if (gto.portAnnex != m_portAnnex) {
		++n1;
		gto.portAnnex = m_portAnnex;
	}
	if (gto.enabled != m_enableGtoaes) {
		++n1;
		gto.enabled = m_enableGtoaes;
	}

	/*IP*/
	m_ipGtoaes.GetAddress(addr.addr);
	sprintf_s(ip, "%d.%d.%d.%d", addr.field[3], addr.field[2], addr.field[1], addr.field[0]);
	if (gto.ip != ip) {
		++n1;
		gto.ip = ip;
	}
	/*ID*/
	n = m_idGroup.GetLength();
	WideCharToMultiByte(CP_ACP, 0, m_idGroup, n, id, n, 0, 0);
	id[n] = 0;
	if (gto.gid != id) {
		++n1;
		gto.gid = id;
	}

	n = m_idUnit.GetLength();
	WideCharToMultiByte(CP_ACP, 0, m_idUnit, n, id, n, 0, 0);
	id[n] = 0;
	if (gto.uid != id) {
		++n1;
		gto.uid = id;
	}

	n = m_idCamera.GetLength();
	WideCharToMultiByte(CP_ACP, 0, m_idCamera, n, id, n, 0, 0);
	id[n] = 0;
	if (gto.cid != id) {
		++n1;
		gto.cid = id;
	}

	gto.dirty = n1;
	// 文件服务
	ParamNetworkFileServer& fs = param_->netFileSrv;
	if (fs.enabled != m_enableFileServer) {
		++n2;
		fs.enabled = m_enableFileServer;
	}
	if (fs.port != m_portFileServer) {
		++n2;
		fs.port = m_portFileServer;
	}
	m_ipFileServer.GetAddress(addr.addr);
	sprintf_s(ip, "%d.%d.%d.%d", addr.field[3], addr.field[2], addr.field[1], addr.field[0]);
	if (fs.ip != ip) {
		++n1;
		fs.ip = ip;
	}

	fs.dirty = n2;

	// 时间服务
	ParamNetworkNTP& ntp = param_->netNTP;
	if (ntp.enabled != m_enableNTP) {
		++n3;
		ntp.enabled = m_enableNTP;
	}
	m_ipNTP.GetAddress(addr.addr);
	sprintf_s(ip, "%d.%d.%d.%d", addr.field[3], addr.field[2], addr.field[1], addr.field[0]);
	if (ntp.ip != ip) {
		++n1;
		ntp.ip = ip;
	}
	ntp.dirty = n3;

	CDialogEx::OnOK();
}
