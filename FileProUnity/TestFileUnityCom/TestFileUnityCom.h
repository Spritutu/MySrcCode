
// TestFileUnityCom.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestFileUnityComApp:
// �йش����ʵ�֣������ TestFileUnityCom.cpp
//

class CTestFileUnityComApp : public CWinApp
{
public:
	CTestFileUnityComApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestFileUnityComApp theApp;