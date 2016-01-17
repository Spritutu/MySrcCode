// RePrintToWndTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RePrintToWndTest.h"
#include "RePrintToWndTestDlg.h"
#include <vector>
// using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD g_ExeProcessId = 0;

HANDLE g_hRead = NULL;

static const int g_TimerElapse = 20;

static int g_TimerId = 0;

#define  WM_SUB_THREAD_MSG WM_USER + 200

static string g_strSubThread = "";

CRITICAL_SECTION g_CS;
static DWORD WINAPI ThreadProc(LPVOID pParamater)
{
	static bool bProcessOK = true;
	char buffer[1024] = {0};  
	DWORD bytesRead; 

	while(ReadFile(g_hRead,buffer,1023,&bytesRead,NULL)) //��ȡ�ܵ�
	{
		EnterCriticalSection(&g_CS);
		 g_strSubThread = buffer;
		::SendMessage((HWND)pParamater,WM_SUB_THREAD_MSG,0,0);
	}
	return 0;
}

string StrTrim(const char *pszInStr)
{
	int iStart = 0;
	char * strPtr = (char*)pszInStr;
	while(*strPtr==32 || *strPtr==10 || *strPtr==9)
	{
		iStart ++;
		strPtr++;
	}

	int iEnd = strlen(pszInStr) -1;
	strPtr = (char*)&pszInStr[iEnd];
	while(*strPtr==32 || *strPtr==10 || *strPtr==9 )
	{
		iEnd--;
		strPtr--;
	}

	int iCount = iEnd - iStart +1;
	string strRev = string(pszInStr + iStart).substr(0,iCount);
	return strRev;
}

void StrSplit(const char *pszStr,vector<string> &vStr,const char* strDelim)
{
	int iLen = strlen(pszStr) + 1;
	char * pszCopy = new char[iLen];
	memset(pszCopy,0,iLen);
	strcpy_s(pszCopy,iLen,pszStr);
	char *toKen;
	char *Out = NULL;
	toKen = strtok_s(pszCopy,strDelim,&Out);
	string str = toKen;
	str = StrTrim(str.c_str());
	if (!str.empty())
		vStr.push_back(str);
	while(toKen!=NULL)
	{
		toKen = strtok_s(NULL,strDelim,&Out);
		if (toKen)
		{	str = toKen;
		str = StrTrim(str.c_str());
		if (!str.empty())
			vStr.push_back(str);
		}
	}
	delete pszCopy;
}

// �˺���ֻ�����ڽ����߳�
static DWORD WaitForObjectEx( HANDLE hHandle, DWORD dwMilliseconds )
{
	BOOL bRet;
	MSG msg;
	INT iWaitRet;
	int nTimeOut = 0;

	
	while( (bRet = ::GetMessage( &msg, NULL, 0, 0 )) != 0)
	{ 
		if (dwMilliseconds != INFINITE)
		{
			if(nTimeOut++ * 20 >= dwMilliseconds)
				break;
		}

		iWaitRet = WaitForSingleObject(hHandle, 20);
		if(iWaitRet != WAIT_TIMEOUT)
		{
			break;
		}
		if (bRet == -1)
		{
			break;
		}
		else
		{
			::TranslateMessage(&msg); 
			::DispatchMessage(&msg); 
		}
	}

	return iWaitRet;
}
void CRePrintToWndTestDlg::CallExe(const char * pszCmd,bool bShowWnd)
{
	SECURITY_ATTRIBUTES sa;  
	HANDLE hRead,hWrite;  
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
	sa.lpSecurityDescriptor = NULL; //ʹ��ϵͳĬ�ϵİ�ȫ������  
	sa.bInheritHandle = TRUE; //�����Ľ��̼̳о��  

	if (!CreatePipe(&hRead,&hWrite,&sa,0)) //���������ܵ�  
	{  
		MessageBox("CreatePipe Failed!","��ʾ",MB_OK | MB_ICONWARNING);  
		return;  
	}  

	STARTUPINFO si;  
	PROCESS_INFORMATION pi;  

	ZeroMemory(&si,sizeof(STARTUPINFO));  

	si.cb = sizeof(STARTUPINFO);  
	GetStartupInfo(&si);  
	si.hStdError = hWrite;  
	si.hStdOutput = hWrite; //�´������̵ı�׼�������д�ܵ�һ��  
	if (bShowWnd)
		si.wShowWindow = SW_SHOW; //��ʾ����  
	else
		si.wShowWindow = SW_HIDE; //���ش��� 

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  


	char cmdline[200];  
	sprintf(cmdline,"%s",pszCmd);  

	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) //�����ӽ���  
	{  
		MessageBox("CreateProcess Failed!","��ʾ",MB_OK | MB_ICONWARNING);  
		return;  
	}
	g_ExeProcessId = pi.dwProcessId;
	CloseHandle(hWrite); //�رչܵ����  
	g_hRead = hRead;

	InitializeCriticalSection(&g_CS);
	HANDLE hThread = CreateThread(NULL,0,ThreadProc,GetSafeHwnd(),0,NULL);
	WaitForObjectEx(hThread,INFINITE);
	//MsgWaitForMultipleObjects(1,&hThread,TRUE,INFINITE,-1);
	//SetTimer(g_TimerId,g_TimerElapse,NULL);
}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRePrintToWndTestDlg �Ի���




CRePrintToWndTestDlg::CRePrintToWndTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRePrintToWndTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRePrintToWndTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListBox);
}

BEGIN_MESSAGE_MAP(CRePrintToWndTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START, &CRePrintToWndTestDlg::OnBnClickedBtnStart)
	ON_MESSAGE(WM_SUB_THREAD_MSG,&CRePrintToWndTestDlg::OnSubThreadMsg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_START2, &CRePrintToWndTestDlg::OnBnClickedBtnStart2)
	ON_BN_CLICKED(IDC_BTN_START3, &CRePrintToWndTestDlg::OnBnClickedBtnStart3)
END_MESSAGE_MAP()


// CRePrintToWndTestDlg ��Ϣ�������

BOOL CRePrintToWndTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRePrintToWndTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRePrintToWndTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRePrintToWndTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRePrintToWndTestDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strCallMsg.clear();
	m_ListBox.ResetContent();
	g_hRead = NULL;
	const char *pszCmd1 = "D:\\RsSrcDir\\Bin\\AtToolBox.exe -sj9_dp";
	const char *pszCmd2 = "\"C:\\Program Files (x86)\\NSIS\\makensis.exe\" D:\\RsSrcDir\\Release\\��������������װ�ű�.nsi";
	const char *pszCmd3 = "doxygen.exe D:\\doxyWorkdir\\Doxyfile";
	CallExe(pszCmd1,true);
}
LRESULT CRePrintToWndTestDlg::OnSubThreadMsg(WPARAM wparam,LPARAM lparam)
{
	if(g_strSubThread.empty())
	{
		LeaveCriticalSection(&g_CS);
		return 0; // ���յ���ϢΪ��ʱ��������
	}

	static string strLast ="";
	m_strCallMsg.c_str();
	m_strCallMsg += strLast; //���һ���ַ�������������Ҫ���´εĺϲ���������Ĵ�
	m_strCallMsg += g_strSubThread;


	g_strSubThread.clear(); //�����Ϣ�ַ���
	LeaveCriticalSection(&g_CS);


	vector<string> vStrMsg;
	StrSplit(m_strCallMsg.c_str(),vStrMsg,"\n");

	int iCurCount = vStrMsg.size();

	int iLen = m_strCallMsg.length();
	bool bBadEnd = false;
	if(m_strCallMsg[iLen-1] != '\n')
	{
		strLast = vStrMsg[iCurCount-1]; // ���һ�в�����ʱ����¼�������Ĳ��֣������´����
		bBadEnd = true;
	}

	if (bBadEnd)
		iCurCount--; // ���һ�в�����ʱ����������ȴ��´����

	for (int i= 0;i< iCurCount;i++)
	{
		m_ListBox.AddString(vStrMsg[i].c_str()); // ֻ�����µ�
		int iSize = m_ListBox.GetCount();
		m_ListBox.SetCaretIndex(iSize-1,TRUE);
		m_ListBox.SetCurSel(iSize-1);
	}

	//MessageBox(g_strSubThread.c_str(),"��ʾ",MB_OK|MB_ICONINFORMATION);  
	return 0;
}
void CRePrintToWndTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	static bool bProcessOK = true;
	char buffer[1024] = {0};  
	DWORD bytesRead; 
	static string strLast ="";

	if (ReadFile(g_hRead,buffer,1023,&bytesRead,NULL) == NULL) //��ȡ�ܵ�  
	{
		g_ExeProcessId = 0;
		if (g_hRead)
			CloseHandle(g_hRead); 

		bProcessOK  = true;
		SetTimer(g_TimerId,-1,NULL); // ������ʱ��
		return CDialog::OnTimer(nIDEvent);
	}

	if (strcmp(buffer,"")==0)
		return CDialog::OnTimer(nIDEvent);

	SetTimer(g_TimerId,-1,NULL); // ������ʱ��

	while(!bProcessOK); // ��ֹ�޸��ַ���

	m_strCallMsg.clear();
	m_strCallMsg += strLast; //���һ���ַ�������������Ҫ���´εĺϲ���������Ĵ�
	m_strCallMsg += buffer;

	bProcessOK = false; // ��ʼ�����ַ���
	vector<string> vStrMsg;
	StrSplit(m_strCallMsg.c_str(),vStrMsg,"\n");

	int iCurCount = vStrMsg.size();

	int iLen = m_strCallMsg.length();
	bool bBadEnd = false;
	if(m_strCallMsg[iLen-1] != '\n')
	{
		strLast = vStrMsg[iCurCount-1]; // ���һ�в�����ʱ����¼�������Ĳ��֣������´����
		bBadEnd = true;
	}

	if (bBadEnd)
		iCurCount--; // ���һ�в�����ʱ����������ȴ��´����

	for (int i= 0;i< iCurCount;i++)
	{
		m_ListBox.AddString(vStrMsg[i].c_str()); // ֻ�����µ�
		int iSize = m_ListBox.GetCount();
		m_ListBox.SetCaretIndex(iSize-1,TRUE);
		m_ListBox.SetCurSel(iSize-1);
	}
	SetTimer(g_TimerId,g_TimerElapse,NULL); // ����������ʱ��

	bProcessOK = true;

	CDialog::OnTimer(nIDEvent);
}

void CRePrintToWndTestDlg::OnBnClickedBtnStart2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strCallMsg.clear();
	m_ListBox.ResetContent();
	g_hRead = NULL;
	const char *pszCmd = "doxygen.exe D:\\doxyWorkdir\\Doxyfile";
	CallExe(pszCmd,false);
}

void CRePrintToWndTestDlg::OnBnClickedBtnStart3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strCallMsg.clear();
	m_ListBox.ResetContent();
	g_hRead = NULL;
	const char *pszCmd = "\"C:\\Program Files (x86)\\NSIS\\makensis.exe\" /V4 D:\\RsSrcDir\\Release\\��������������װ�ű�.nsi";
	CallExe(pszCmd,false);
}
