
// CamagentS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CCamagentSApp: 
// �йش����ʵ�֣������ CamagentS.cpp
//

class CCamagentSApp : public CWinApp
{
public:
	CCamagentSApp();

protected:
	/// �����ִ�г����ŵ�·��
	CString m_strAppPath;

// ��д
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()

public:
	/** �鿴��������Ŀ¼
	 * \return ��ִ�г����ŵ�Ŀ¼
	 **/
	LPCTSTR GetAppPath();
};

extern CCamagentSApp theApp;
