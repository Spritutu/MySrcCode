
// RePrintCallExe.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RePrintCallExe.h"
#include "RePrintCallExeDlg.h"
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRePrintCallExeApp

BEGIN_MESSAGE_MAP(CRePrintCallExeApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CRePrintCallExeApp ����

CRePrintCallExeApp::CRePrintCallExeApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRePrintCallExeApp ����

CRePrintCallExeApp theApp;


// CRePrintCallExeApp ��ʼ��

BOOL CRePrintCallExeApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CRePrintCallExeDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

inline BOOL KillProcess(WORD nProcessID)
{
	HANDLE hProcessHandle =::OpenProcess( PROCESS_TERMINATE,FALSE, nProcessID );
	return ::TerminateProcess( hProcessHandle, 4 );
}
int CRePrintCallExeApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���

	if (g_ExeProcessID)
	{
		KillProcess(g_ExeProcessID);
	}

	return CWinAppEx::ExitInstance();
}
