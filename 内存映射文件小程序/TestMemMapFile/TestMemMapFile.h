
// TestMemMapFile.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestMemMapFileApp:
// �йش����ʵ�֣������ TestMemMapFile.cpp
//

class CTestMemMapFileApp : public CWinAppEx
{
public:
	CTestMemMapFileApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestMemMapFileApp theApp;