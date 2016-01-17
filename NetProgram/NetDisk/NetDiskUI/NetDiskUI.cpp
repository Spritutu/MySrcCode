
// NetDiskUI.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "NetDiskUIDlg.h"
#include "DlgLoginMain.h"
#include "CrashHandler.h"
#include "TaskSerialization.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetDiskUIApp

BEGIN_MESSAGE_MAP(CNetDiskUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNetDiskUIApp ����

CNetDiskUIApp::CNetDiskUIApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_strServerIp = "";
	m_hMutex = NULL;
	m_pPageNetDisk = NULL;
	m_bRestart = FALSE;
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNetDiskUIApp ����

CNetDiskUIApp theApp;


// CNetDiskUIApp ��ʼ��

BOOL CNetDiskUIApp::InitInstance()
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

	CWinApp::InitInstance();
	CCrashHandler ch;
	ch.SetProcessExceptionHandlers();
	ch.SetThreadExceptionHandlers();
	m_hMutex = CreateMutex(NULL,TRUE,"NetDiskUI");
	if (m_hMutex)
	{
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
			CWnd * pDeskTopWnd = CWnd::GetDesktopWindow();
			CWnd *pFind = pDeskTopWnd->GetWindow(GW_CHILD);
			while(pFind)
			{
				if (::GetProp(pFind->m_hWnd,m_pszExeName))
				{
					if (::IsIconic(pFind->m_hWnd))
						pFind->ShowWindow(SW_RESTORE); // ����������Ѿ���С������ָ����С
					pFind->ShowWindow(SW_SHOW);
					pFind->SetForegroundWindow();
					::SetForegroundWindow(::GetLastActivePopup(pFind->m_hWnd));
					return FALSE; // ǰһ������ʵ���Ѿ����ڣ��˳���ʵ��
				}
				pFind = pFind->GetWindow(GW_HWNDNEXT);
			}
		}
	}

	char szDir[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(),szDir,MAX_PATH);
	CString strConfigINI = szDir;
	strConfigINI.MakeLower();
	strConfigINI = strConfigINI.Left(strConfigINI.ReverseFind('.') + 1) + "ini";
	GetPrivateProfileString("setting","ServerIp","",m_strServerIp.GetBuffer(100),100,strConfigINI);
	m_strServerIp.ReleaseBuffer();
	if (m_strServerIp.IsEmpty())
	{
		m_strServerIp = "127.0.0.1";
		WritePrivateProfileString("setting","ServerIp",m_strServerIp,strConfigINI);
	}

	GetPrivateProfileString("setting","CacheDir","",m_strCacheDir.GetBuffer(500),500,strConfigINI);
	m_strCacheDir.ReleaseBuffer();
	if (m_strCacheDir.IsEmpty())
	{
		m_strCacheDir = strConfigINI.Mid(0,strConfigINI.ReverseFind('\\')+1) + "CacheDir";
		WritePrivateProfileString("setting","CacheDir",m_strCacheDir,strConfigINI);
	}

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	//CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CDlgLoginMain DlgLogin;
	if (DlgLogin.DoModal()!=IDOK)
		return FALSE;

	m_strUsrId = DlgLogin.m_strUsrID;
	m_strUsrName = DlgLogin.m_strUsrName;
	m_iAllSpace = DlgLogin.m_iAllSpace;
	m_iUsedSpace = DlgLogin.m_iUsedSpace;
	m_strPasswd = DlgLogin.m_strPasswd;

	CNetDiskUIDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (dlg.m_bLogOut)
		m_bRestart = TRUE;
	else
		m_bRestart = FALSE;

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

	// ɾ�����洴���� shell ��������
	/*if (pShellManager != NULL)
	{
		delete pShellManager;
	}*/

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CNetDiskUIApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_hMutex)
	{
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

	SerializationManager.SaveXmlFile();

	if (m_bRestart)
	{
		char szExe[MAX_PATH] = {0};
		GetModuleFileName(AfxGetInstanceHandle(),szExe,MAX_PATH);
		ShellExecute(NULL,"open",szExe,NULL,NULL,SW_SHOWNORMAL);
	}
	
	return CWinApp::ExitInstance();
}
