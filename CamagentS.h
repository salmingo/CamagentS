
// CamagentS.h : PROJECT_NAME 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CCamagentSApp: 
// 有关此类的实现，请参阅 CamagentS.cpp
//

class CCamagentSApp : public CWinApp
{
public:
	CCamagentSApp();

protected:
	/// 缓存可执行程序存放的路径
	CString m_strAppPath;

// 重写
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()

public:
	/** 查看程序所在目录
	 * \return 可执行程序存放的目录
	 **/
	LPCTSTR GetAppPath();
};

extern CCamagentSApp theApp;
