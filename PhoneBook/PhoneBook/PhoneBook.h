
// PhoneBook.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPhoneBookApp:
// �йش����ʵ�֣������ PhoneBook.cpp
//

class CPhoneBookApp : public CWinApp
{
public:
	CPhoneBookApp();

// ��д
public:
	virtual BOOL InitInstance();
	ULONG_PTR m_gdiplusToken;    // ULONG PTR Ϊint64 ����
// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CPhoneBookApp theApp;