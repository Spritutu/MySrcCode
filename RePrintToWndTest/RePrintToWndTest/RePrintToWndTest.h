
// RePrintToWndTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRePrintToWndTestApp:
// �йش����ʵ�֣������ RePrintToWndTest.cpp
//

class CRePrintToWndTestApp : public CWinAppEx
{
public:
	CRePrintToWndTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CRePrintToWndTestApp theApp;