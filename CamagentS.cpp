
// CamagentS.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "CamagentS.h"
#include "CamagentSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCamagentSApp

BEGIN_MESSAGE_MAP(CCamagentSApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCamagentSApp ����

CCamagentSApp::CCamagentSApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_strAppPath = _T("");
}

// Ψһ��һ�� CCamagentSApp ����

CCamagentSApp theApp;

// CCamagentSApp ��ʼ��

BOOL CCamagentSApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	CCamagentSDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

/** �鿴��������Ŀ¼
 * \return ��ִ�г����ŵ�Ŀ¼
 **/
LPCTSTR CCamagentSApp::GetAppPath()
{
	if (!m_strAppPath.IsEmpty()) return m_strAppPath;

	TCHAR pszFileName[1024];
	DWORD dwLen;
	DWORD i;

	dwLen = ::GetModuleFileName(NULL, pszFileName, 1024);
	for (i = dwLen; i > 0; i--) {
		if (pszFileName[i - 1] == '\\') {
			pszFileName[i] = 0;
			break;
		}
	}

	m_strAppPath = pszFileName;
	return m_strAppPath;
}
