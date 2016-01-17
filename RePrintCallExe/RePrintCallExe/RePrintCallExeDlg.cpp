
// RePrintCallExeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RePrintCallExe.h"
#include "RePrintCallExeDlg.h"
#include<vector>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD  g_ExeProcessID = 0;

static const int g_iTimerElpase = 20; //  ��ʱ��ʱ����
static UINT_PTR  g_TimerID = 0;
HANDLE g_hRead = NULL;

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

// CRePrintCallExeDlg �Ի���
void CRePrintCallExeDlg::CallExe(const char *pszCmd,bool bShowWnd)
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
	si.wShowWindow = SW_HIDE; //���ش���  
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  


	char cmdline[200];  
	sprintf(cmdline,"%s",pszCmd);  

	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) //�����ӽ���  
	{  
		MessageBox("CreateProcess Failed!","��ʾ",MB_OK | MB_ICONWARNING);  
		return;  
	}  
	CloseHandle(hWrite); //�رչܵ����  

	g_hRead = hRead;
	g_ExeProcessID = pi.dwProcessId;

	SetTimer(g_TimerID,g_iTimerElpase,NULL); // ���ö�ʱ��
}

CRePrintCallExeDlg::CRePrintCallExeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRePrintCallExeDlg::IDD, pParent),m_strLast("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRePrintCallExeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MSG, m_ListBox);
}

BEGIN_MESSAGE_MAP(CRePrintCallExeDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CALL_EXE, &CRePrintCallExeDlg::OnBnClickedBtnCallExe)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CRePrintCallExeDlg ��Ϣ�������

BOOL CRePrintCallExeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRePrintCallExeDlg::OnPaint()
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
HCURSOR CRePrintCallExeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRePrintCallExeDlg::OnBnClickedBtnCallExe()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_strLast = "";
	m_ListBox.ResetContent(); // �������
	g_ExeProcessID  = 0;
	const char * pszCmd = "\"D:\\Program Files (x86)\\NSIS\\makensis.exe\"  F:\\RsSrcDir\\Release\\��������������װ�ű�.nsi";
	CallExe(pszCmd,false);
}

void CRePrintCallExeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	char buffer[1024] = {0};  
	DWORD bytesRead;  
	if (ReadFile(g_hRead,buffer,1023,&bytesRead,NULL) == NULL) //��ȡ�ܵ�
	{
		CloseHandle(g_hRead);  
		g_hRead = NULL;
		SetTimer(g_TimerID,-1,NULL);
		g_ExeProcessID = 0;
	}

	string strThisTime  = m_strLast;
	strThisTime += buffer;

	strThisTime = StrTrim(strThisTime.c_str());
	if (strThisTime.empty())
		return CDialog::OnTimer(nIDEvent); // δ��׽����Ϣ

	SetTimer(g_TimerID,-1,NULL);

	vector<string> vStrLine;
	StrSplit(strThisTime.c_str(),vStrLine,"\n");

	bool bBadEnd = false;
	int iLen = strThisTime.length();

	int iCount = vStrLine.size();
	if (strThisTime[iLen-1]!='\n')
	{
		m_strLast = vStrLine[iCount-1]; // ���һ����Ϣ����������¼�������´���� 
		bBadEnd = true;
	}

	if (bBadEnd)
		iCount --; // ���һ����Ϣ������,���һ�б��β����

	for(int i=0;i< iCount;i++)
	{
		m_ListBox.AddString(vStrLine[i].c_str());
		int iListSize = m_ListBox.GetCount();
		m_ListBox.SetCaretIndex(iListSize-1,TRUE);
		m_ListBox.SetCurSel(iListSize-1);
	}

	SetTimer(g_TimerID,g_iTimerElpase,NULL);

	CDialog::OnTimer(nIDEvent);
}
