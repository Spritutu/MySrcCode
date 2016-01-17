
// 7zClientUI.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "7zClientUI.h"
#include "CLientUIMainDlg.h"
#include "SkinMagicLib.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  USE_SKIN_MAGIC 1
// CMy7zClientUIApp

BEGIN_MESSAGE_MAP(CMy7zClientUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMy7zClientUIApp ����

CMy7zClientUIApp::CMy7zClientUIApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}



// Ψһ��һ�� CMy7zClientUIApp ����

CMy7zClientUIApp theApp;


// CMy7zClientUIApp ��ʼ��

BOOL CMy7zClientUIApp::InitInstance()
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

#if USE_SKIN_MAGIC
	int gInt = InitSkinMagicLib( AfxGetInstanceHandle(), "hello" ,NULL,NULL );
	gInt = LoadSkinFromResource(NULL, (LPCTSTR)IDR_SKINMAGIC1, "SKINMAGIC");
	SetDialogSkin( "Dialog" );
#endif

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CCLientUIMainDlg dlg;
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

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CMy7zClientUIApp::ExitInstance()
{
#if USE_SKIN_MAGIC
	ExitSkinMagicLib();
#endif
	return CWinApp::ExitInstance();
}