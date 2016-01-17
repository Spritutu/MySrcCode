
// RePrintToWndTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "RePrintToWndTest.h"
#include "RePrintToWndTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRePrintToWndTestApp

BEGIN_MESSAGE_MAP(CRePrintToWndTestApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRePrintToWndTestApp ����

CRePrintToWndTestApp::CRePrintToWndTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRePrintToWndTestApp ����

CRePrintToWndTestApp theApp;


// CRePrintToWndTestApp ��ʼ��

BOOL CRePrintToWndTestApp::InitInstance()
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

	CRePrintToWndTestDlg dlg;
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

/*
*���ݽ���IDɱ������
*/
BOOL KillProcess(DWORD nProcessID)
{
	HANDLE hProcessHandle; 
	hProcessHandle =::OpenProcess( PROCESS_TERMINATE,FALSE, nProcessID );
	if (hProcessHandle == NULL)
		return FALSE;

	return ::TerminateProcess( hProcessHandle, 4 );
}

/*
*���ݽ�����ʾ�ı�������ɱ������
*/
BOOL KillProcess(LPCSTR pszWindowTitle)
{
	HWND TheWindow;

	TheWindow = ::FindWindow( NULL, pszWindowTitle );
	if (TheWindow == NULL)
		return FALSE;

	DWORD nProcessID;
	::GetWindowThreadProcessId( TheWindow, &nProcessID );

	return KillProcess(nProcessID);
}

int CRePrintToWndTestApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	KillProcess("���򸲸Ǹ߷�Ӱ���Ʒ��������");

	if (g_ExeProcessId!=0)
	{
		KillProcess(g_ExeProcessId); // �������˳�ʱɱ��exe����
	}
	
	return CWinAppEx::ExitInstance();
}
