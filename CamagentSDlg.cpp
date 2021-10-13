
// CamagentSDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "CamagentS.h"
#include "CamagentSDlg.h"
#include "DlgSerialPort.h"
#include "DlgNetwork.h"
#include "DlgFocus.h"
#include "AstroDeviceDef.h"
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

using namespace boost::placeholders;
using namespace boost::posix_time;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CCamagentSDlg �Ի���

CCamagentSDlg::CCamagentSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAMAGENTS_DIALOG, pParent)
	, m_nSlitPeriod(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCamagentSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SLIT_OPEN_PERIOD, m_btnSlitOpenPeriod);
	DDX_Control(pDX, IDC_BTN_SLIT_OPEN, m_btnSlitOpen);
	DDX_Control(pDX, IDC_BTN_SLIT_CLOSE, m_btnSlitClose);
	DDX_Control(pDX, IDC_BTN_SLIT_STOP, m_btnSlitStop);
	DDX_Text(pDX, IDC_ED_SLIT_PERIOD, m_nSlitPeriod);
	DDX_Control(pDX, IDC_BTN_FOCUS_PLUS, m_btnFocusPlus);
	DDX_Control(pDX, IDC_BTN_FOCUS_MINUS, m_btnFocusMinus);
	DDX_Control(pDX, IDC_BTN_FOCUS_RESET, m_btnFocusReset);
	DDX_Control(pDX, IDC_BTN_FOCUS_GOTO, m_btnFocusGoto);
}

BEGIN_MESSAGE_MAP(CCamagentSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CONFIG_SERIAL, &CCamagentSDlg::OnBnClickedBtnConfigSerial)
	ON_BN_CLICKED(IDC_BTN_CONFIG_NETWORK, &CCamagentSDlg::OnBnClickedBtnConfigNetwork)
	ON_BN_CLICKED(IDC_BTN_CONFIG_FOCUSER, &CCamagentSDlg::OnBnClickedBtnConfigFocuser)
	ON_BN_CLICKED(IDC_BTN_SLIT_OPEN_PERIOD, &CCamagentSDlg::OnBnClickedBtnSlitOpenPeriod)
	ON_BN_CLICKED(IDC_BTN_SLIT_OPEN, &CCamagentSDlg::OnBnClickedBtnSlitOpen)
	ON_BN_CLICKED(IDC_BTN_SLIT_CLOSE, &CCamagentSDlg::OnBnClickedBtnSlitClose)
	ON_BN_CLICKED(IDC_BTN_SLIT_STOP, &CCamagentSDlg::OnBnClickedBtnSlitStop)
	ON_BN_CLICKED(IDC_BTN_FOCUS_PLUS, &CCamagentSDlg::OnBnClickedBtnFocusPlus)
	ON_BN_CLICKED(IDC_BTN_FOCUS_MINUS, &CCamagentSDlg::OnBnClickedBtnFocusMinus)
	ON_BN_DOUBLECLICKED(IDC_BTN_FOCUS_RESET, &CCamagentSDlg::OnBnDoubleclickedBtnFocusReset)
	ON_BN_CLICKED(IDC_BTN_FOCUS_GOTO, &CCamagentSDlg::OnBnClickedBtnFocusGoto)
END_MESSAGE_MAP()

// CCamagentSDlg ��Ϣ�������

BOOL CCamagentSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/*===============================================================*/
	/* �������ò��� */
	CString filePathT;
	filePathT.Format(_T("%s%s"),
		((CCamagentSApp*) AfxGetApp())->GetAppPath(),
		_T("CamagentS.xml"));

	char filePath[256];
	memset(filePath, 0, 256);
	WideCharToMultiByte(CP_ACP, 0, filePathT, filePathT.GetLength(), filePath, filePathT.GetLength(), NULL, NULL);
	param_.Load(filePath);

	m_nSlitPeriod = param_.slitOpenPeriod;
	SetDlgItemInt(IDC_ED_FOCUS_STEP, 10);

	stop_plc_inner();
	/* ������Ч�Լ�鼰ά������ */
	thrdClock_.reset(new boost::thread(boost::bind(&CCamagentSDlg::thread_clock, this)));
	thrdLiefCycle_.reset(new boost::thread(boost::bind(&CCamagentSDlg::thread_lief_cycle, this)));

	/*===============================================================*/
	/* MFC��ܴ��ڳ�ʼ�� */
	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCamagentSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCamagentSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCamagentSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCamagentSDlg::OnOK()
{
}

void CCamagentSDlg::OnCancel()
{
}

void CCamagentSDlg::OnClose()
{
	param_.Save();	// ���ֲ���

	interrupt_thread(thrdClock_);
	interrupt_thread(thrdLiefCycle_);

	stop_plc_inner();
	stop_plc_outer();

	CDialogEx::OnOK();
}

void CCamagentSDlg::OnBnClickedBtnSlitOpenPeriod()
{
	m_nSlitPeriod = GetDlgItemInt(IDC_ED_SLIT_PERIOD);
	param_.slitOpenPeriod = m_nSlitPeriod;
	plcInner_.OpenSlit(m_nSlitPeriod);
}

void CCamagentSDlg::OnBnClickedBtnSlitOpen()
{
	plcInner_.OpenSlit();
}

void CCamagentSDlg::OnBnClickedBtnSlitClose()
{
	plcInner_.CloseSlit();
}

void CCamagentSDlg::OnBnClickedBtnSlitStop()
{
	plcInner_.StopSlit();
}

void CCamagentSDlg::OnBnClickedBtnConfigNetwork()
{
	CDlgNetwork dlg(&param_);
	dlg.DoModal();

	if (param_.netGtoaes.dirty || param_.netFileSrv.dirty || param_.netNTP.dirty) {
		if (param_.netGtoaes.dirty) {

		}

		if (param_.netFileSrv.dirty) {

		}

		if (param_.netNTP.dirty) {

		}

		// �������
		param_.Save();
	}
}

void CCamagentSDlg::OnBnClickedBtnConfigSerial()
{
	CDlgSerialPort dlg(&param_);
	dlg.DoModal();

	if (param_.serialCommInner.dirty || param_.serialCommOuter.dirty) {// �رմ��ڲ������¼�
		if (param_.serialCommInner.dirty) plcInner_.Stop();
		//if (param_.serialCommOuter.dirty)
		//	plcOuter_.Stop();

		cvDevClosed_.notify_one();
		// �������
		param_.Save();
	}
}

void CCamagentSDlg::interrupt_thread(ThrdPtr& thrd) {
	if (thrd.unique()) {
		thrd->interrupt();
		thrd->join();
		thrd.reset();
	}
}

void CCamagentSDlg::thread_clock() {
	try {
		boost::chrono::seconds tWait(1);
		boost::this_thread::sleep_for(boost::chrono::seconds(5));

		while (1) {
			boost::this_thread::sleep_for(tWait);
			// ��ʾʱ��
			ptime now = second_clock::local_time();
			ptime::date_type ymd = now.date();
			ptime::time_duration_type hms = now.time_of_day();
			CString txt;
			txt.Format(_T("%d-%02d-%02dT%02d:%02d:%02d"),
				ymd.year(), ymd.month(), ymd.day(),
				hms.hours(), hms.minutes(), hms.seconds());
			SetDlgItemText(IDC_CTIME, txt);

			if (plcInner_.IsActive()) {
				// ��ʾ�촰״̬
				int s, l, r;
				plcInner_.GetSlitState(s, &l, &r);
				SetDlgItemText(IDC_STATE_SLIT, StateSlit::ToString(s));
				SetDlgItemText(IDC_STATE_SLIT_LEFT, StateSlit::ToString(l));
				SetDlgItemText(IDC_STATE_SLIT_RIGHT, StateSlit::ToString(r));
				// ��ʾ��ˮ��־
				SetDlgItemText(IDC_STATE_RAINFALL, plcInner_.IsRainy() ? _T("��    ��") : _T("��    ��"));
			}
		}
	}
	catch (boost::thread_interrupted&) {
	}
}

void CCamagentSDlg::thread_lief_cycle() {
	try {
		int n;
		MtxLck mtxClosed;
		boost::chrono::minutes tWait(5); // �������: 5����

		boost::this_thread::sleep_for(boost::chrono::seconds(5));
		while (TRUE) {
			do {
				n = 0;
				// �������

				// ���Ӵ���
				if (!(plcInner_.IsActive() || start_plc_inner()))
					++n;
				//if (!(plcOuter_.IsActive() || plcOuter_.Start()))
				//	++n;

				// �����������

				boost::this_thread::sleep_for(tWait);
			} while (n);

			cvDevClosed_.wait(mtxClosed);
		}
	}
	catch (boost::thread_interrupted& ) {
	}
}

bool CCamagentSDlg::start_plc_inner() {
	ParamSerialComm& p = param_.serialCommInner;
	bool rslt = (plcInner_.SetSerialPort(p.portName.c_str(), p.baudRate, p.dataBit, p.parity, p.stopBit, p.flowCtl)
		&& plcInner_.Start());
	if (rslt) {
		SetDlgItemText(IDC_STATE_PLC_INNER, _T("��  ��"));

		m_btnSlitOpenPeriod.EnableWindow(TRUE);
		m_btnSlitOpen.EnableWindow(TRUE);
		m_btnSlitClose.EnableWindow(TRUE);
		m_btnSlitStop.EnableWindow(TRUE);
	}
	return rslt;
}

void CCamagentSDlg::stop_plc_inner() {
	SetDlgItemText(IDC_STATE_PLC_INNER, _T("��  ��"));
	plcInner_.Stop();

	m_btnSlitOpenPeriod.EnableWindow(FALSE);
	m_btnSlitOpen.EnableWindow(FALSE);
	m_btnSlitClose.EnableWindow(FALSE);
	m_btnSlitStop.EnableWindow(FALSE);
}

void CCamagentSDlg::OnBnClickedBtnConfigFocuser()
{
	CDlgFocus dlg(&param_.focuser);
	dlg.DoModal();
}

void CCamagentSDlg::OnBnClickedBtnFocusPlus()
{
	int step = GetDlgItemInt(IDC_ED_FOCUS_STEP);

	if (step) {

	}
}

void CCamagentSDlg::OnBnClickedBtnFocusMinus()
{
}

void CCamagentSDlg::OnBnClickedBtnFocusGoto()
{
	int target = GetDlgItemInt(IDC_ED_FOCUS_TARGET);
	//plcOuter_.Goto
}

void CCamagentSDlg::OnBnDoubleclickedBtnFocusReset()
{
}

bool CCamagentSDlg::start_plc_outer() {
	m_btnFocusMinus.EnableWindow(TRUE);
	m_btnFocusPlus.EnableWindow(TRUE);
	m_btnFocusReset.EnableWindow(TRUE);

	return true;
}

void CCamagentSDlg::stop_plc_outer() {
	m_btnFocusMinus.EnableWindow(FALSE);
	m_btnFocusPlus.EnableWindow(FALSE);
	m_btnFocusReset.EnableWindow(FALSE);
}
