
// NetDiskUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "PageNetDisk.h"

// CNetDiskUIApp:
// �йش����ʵ�֣������ NetDiskUI.cpp
//

class CNetDiskUIApp : public CWinApp
{
public:
	CNetDiskUIApp();

// ��д
public:
	virtual BOOL InitInstance();

	CString m_strUsrId;
	CString m_strUsrName;
	CString m_strPasswd;
	long long m_iAllSpace;
	long long m_iUsedSpace;
	CString m_strServerIp;
	CString m_strCacheDir;
	CPageNetDisk * m_pPageNetDisk;
private:
	HANDLE m_hMutex;
	BOOL   m_bRestart;

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern CNetDiskUIApp theApp;