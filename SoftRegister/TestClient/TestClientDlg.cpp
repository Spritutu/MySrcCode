
// TestClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestClient.h"
#include "TestClientDlg.h"
#include "afxdialogex.h"
#include "Core.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestClientDlg �Ի���




CTestClientDlg::CTestClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CHECK_LICENSE, &CTestClientDlg::OnBnClickedButtonCheckLicense)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_KEY, &CTestClientDlg::OnBnClickedButtonSaveKey)
	ON_BN_CLICKED(IDC_BUTTON_GET_PC_CODE, &CTestClientDlg::OnBnClickedButtonGetPcCode)
	ON_BN_CLICKED(IDC_BUTTON_Get_LICENSE, &CTestClientDlg::OnBnClickedButtonGetLicense)
END_MESSAGE_MAP()


// CTestClientDlg ��Ϣ�������

BOOL CTestClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	char * pszKeyVal = GetRegsiterKeyFromReg("TestClient.exe");
	string strKeyVal  = pszKeyVal;
	FreeStrMen(pszKeyVal);
	if (strKeyVal.empty())
	{
		SetWindowText("δע��汾��");
	}
	else
	{
		char *pszId = GetPcIdStr();
		string strId = pszId;
		FreeStrMen(pszId);
		string strErrMsg;
		if (!IsValidCode(strId.c_str(),"TestClient.exe",strKeyVal.c_str(),strErrMsg))   // У�����
		{
			SetWindowText(strErrMsg.c_str());
		}
		else
			SetWindowText("��ʽ�汾��");

	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestClientDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestClientDlg::OnBnClickedButtonCheckLicense()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char *pszKey = GetRegsiterKeyFromReg("TestClient.exe");
	string strKey = pszKey;
	FreeStrMen(pszKey);

	if (strKey.empty())
	{
		MessageBox("��ɲ����ڣ�","��ʾ",MB_ICONERROR|MB_OK);
	}
	else
	{
		string strErrMsg;
		char *pszId = GetPcIdStr();
		string strId = pszId;
		FreeStrMen(pszId);
		if (!IsValidCode(strId.c_str(),"TestClient.exe",strKey.c_str(),strErrMsg))   // У�����
		{
			MessageBox(strErrMsg.c_str(),"��ʾ",MB_ICONINFORMATION|MB_OK);
		}
		else
			MessageBox("�����Ч��","��ʾ",MB_ICONINFORMATION|MB_OK);

	}
}


void CTestClientDlg::OnBnClickedButtonSaveKey()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_strLicenseData.IsEmpty())
	{
		MessageBox("��ɲ����ڣ�","��ʾ",MB_ICONERROR|MB_OK);
		return;
	}
	char *pszId = GetPcIdStr();
	string strId = pszId;
	FreeStrMen(pszId);
	string strErrMsg;
	if (!IsValidCode(strId.c_str(),"TestClient.exe",m_strLicenseData,strErrMsg))
	{
		MessageBox(strErrMsg.c_str(),"��ʾ",MB_ICONERROR|MB_OK);
		return;
	}

	if (!WriteLicenseToReg(m_strLicenseData,"TestClient.exe"))
	{
		MessageBox("��ɵ���ʧ�ܣ�","��ʾ",MB_ICONERROR|MB_OK);
		return;
	}

	MessageBox("����ɹ���","��ʾ",MB_ICONINFORMATION|MB_OK);
	SetWindowText("��ʽ�汾��");
}


void CTestClientDlg::OnBnClickedButtonGetPcCode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char *pszId = GetPcIdStr();
	string strId = pszId;
	FreeStrMen(pszId);
	CString str;
	str.Format("ע����Ϊ:%s",strId.c_str());
	MessageBox(str,"��ʾ",MB_ICONINFORMATION|MB_OK);
}


void CTestClientDlg::OnBnClickedButtonGetLicense() // �������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char *pszId = GetPcIdStr();
	string strId = pszId;
	FreeStrMen(pszId);

	
	char * pszKey = GetRegsiterKey(strId.c_str(),"TestClient.exe",1);
	string strVal = pszKey;
	FreeStrMen(pszKey);

	m_strLicenseData = strVal.c_str();
	CString strMsg;
	strMsg.Format("��ɣ�%s",m_strLicenseData);
	MessageBox(strMsg,"��ʾ",MB_ICONINFORMATION|MB_OK);
}
