
// 7zClientUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy7zClientUIApp:
// �йش����ʵ�֣������ 7zClientUI.cpp
//

class CMy7zClientUIApp : public CWinApp
{
public:
	CMy7zClientUIApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy7zClientUIApp theApp;