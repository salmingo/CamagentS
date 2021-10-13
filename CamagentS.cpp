
// CamagentS.cpp : 定义应用程序的类行为。
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


// CCamagentSApp 构造

CCamagentSApp::CCamagentSApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_strAppPath = _T("");
}

// 唯一的一个 CCamagentSApp 对象

CCamagentSApp theApp;

// CCamagentSApp 初始化

BOOL CCamagentSApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
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
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

/** 查看程序所在目录
 * \return 可执行程序存放的目录
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
