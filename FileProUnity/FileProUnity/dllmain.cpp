// dllmain.cpp : DllMain ��ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "FileProUnity_i.h"
#include "dllmain.h"
#include "FileCopy.h"
CFileProUnityModule _AtlModule;

class CFileProUnityApp : public CWinApp
{
public:

// ��д
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CFileProUnityApp, CWinApp)
END_MESSAGE_MAP()

CFileProUnityApp theApp;

BOOL CFileProUnityApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CFileProUnityApp::ExitInstance()
{
	CCopyManager::Instance()->DestoyrCopyProgressInfo();
	return CWinApp::ExitInstance();
}
