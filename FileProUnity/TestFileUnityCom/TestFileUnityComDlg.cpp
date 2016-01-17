
// TestFileUnityComDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestFileUnityCom.h"
#include "TestFileUnityComDlg.h"
#include "afxdialogex.h"


//JSON ͷ�ļ�
#include "value.h" 
#include "reader.h"
#include "writer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>

#import "../Debug/FileProUnity.dll" no_namespace

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


// CTestFileUnityComDlg �Ի���




CTestFileUnityComDlg::CTestFileUnityComDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestFileUnityComDlg::IDD, pParent)
	, m_StrCopyInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestFileUnityComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StrCopyInfo);
}

BEGIN_MESSAGE_MAP(CTestFileUnityComDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestFileUnityComDlg::OnBnClickedOpenFileDlg)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestFileUnityComDlg::OnBtnSelectDir)
	ON_BN_CLICKED(IDC_BTN_FILE_INFO, &CTestFileUnityComDlg::OnBnClickedBtnFileInfo)
	ON_BN_CLICKED(IDC_BTN_DeleteFile, &CTestFileUnityComDlg::OnBnClickedBtnDeletefile)
	ON_BN_CLICKED(IDC_BTN_Get_MD5, &CTestFileUnityComDlg::OnBnClickedBtnGetMd5)
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CTestFileUnityComDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(IDC_BTN_CopyFile, &CTestFileUnityComDlg::OnBnClickedBtnCopyfile)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTestFileUnityComDlg ��Ϣ�������

BOOL CTestFileUnityComDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestFileUnityComDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestFileUnityComDlg::OnPaint()
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
HCURSOR CTestFileUnityComDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestFileUnityComDlg::OnBnClickedOpenFileDlg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("��ȡcom����ʧ��!","��ʾ");
		return;
	}

	Json::Value obj;
	CString str =  "Word File(*doc;*.docx)|*.doc;*.docx|�ı��ļ�(*.txt)|*.txt|��ִ�г���(*exe)|*exe|�����ļ�(*.*)|*.*||";
	obj["filter"] = Json::Value( (LPCTSTR)str );
	obj["selectmode"] = Json::Value("single"); //"multi"��ѡ,"single"��ѡ����������Ĭ�ϵ�ѡ
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();
	BSTR bsName;
	bsName = FileProObj->OpenFileDlg(bst);
	CString strOutMsg = (LPCTSTR)_bstr_t(bsName);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBtnSelectDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("��ȡcom����ʧ��!","��ʾ");
		return;
	}
	BSTR bsName;
	bsName = FileProObj->SelectDirDlg("");
	CString strOutMsg =  (LPCTSTR)_bstr_t(bsName);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnFileInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("��ȡcom����ʧ��!","��ʾ");
		return;
	}

	Json::Value obj;
	CString str =  "E:\\WorkDir\\FileProUnity\\libd\\jsond.lib";
	obj["file"] = Json::Value( (LPCTSTR)str );
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();
	BSTR bsName;
	bsName = FileProObj->GetFileInfo(bst);
	CString strOutMsg =  (LPCTSTR)_bstr_t(bsName);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnDeletefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("��ȡcom����ʧ��!","��ʾ");
		return;
	}

	Json::Value obj;
	CString str =  "E:\\�½��ı��ĵ�.txt";
	obj["file"] = Json::Value( (LPCTSTR)str );
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();
	BSTR bstOut;
	bstOut = FileProObj->DeleteLocalFile(bst);

	CString strOutMsg =  (LPCTSTR)_bstr_t(bstOut);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnGetMd5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("��ȡcom����ʧ��!","��ʾ");
		return;
	}

	Json::Value obj;
	CString str =  "E:\\�½��ı��ĵ�.txt";
	obj["file"] = Json::Value( (LPCTSTR)str );
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();

	BSTR bstOut;
	bstOut = FileProObj->GetFileMd5(bst);
	CString strOutMsg = (LPCTSTR)_bstr_t(bstOut);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnOpenFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("��ȡcom����ʧ��!","��ʾ");
		return;
	}

	Json::Value obj;
	CString str =  "E:\\pera13.1��֧�������б�˵���ĵ����Էɣ�.docx";
	obj["file"] = Json::Value( (LPCTSTR)str );
	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();

	BSTR bstOut;
	bstOut = FileProObj->OpenFile(bst);
	CString strOutMsg =  (LPCTSTR)_bstr_t(bstOut);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnBnClickedBtnCopyfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CoInitialize(NULL);
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("��ȡcom����ʧ��!","��ʾ");
		return;
	}

	Json::Value obj;
	CString str =  "Z:\\oarcle 10G_win32.zip";
	CString strDest = "E:\\oarcle.zip";
	obj["Source"] = Json::Value( (LPCTSTR)str );
	obj["Dest"]   = Json::Value( (LPCTSTR)strDest);

	str = obj.toStyledString().c_str();

	BSTR bst = _bstr_t(str).GetBSTR();

	BSTR bstOut;
	bstOut = FileProObj->CopyFileCom(bst);

	CString strOutMsg =  (LPCTSTR)_bstr_t(bstOut);
	SetTimer(1000,1500,NULL);
	MessageBox(strOutMsg);
}


void CTestFileUnityComDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IFileProComPtr FileProObj = NULL;   
	HRESULT hr = S_OK;   
	hr = FileProObj.CreateInstance(__uuidof(FileProCom)); 
	if (!FileProObj)
	{
		MessageBox("��ȡcom����ʧ��!","��ʾ");
		return;
	}

	Json::Value obj;
	CString strDest = "E:\\oarcle.zip";
	obj["Dest"]   = Json::Value( (LPCTSTR)strDest);

	CString str = obj.toStyledString().c_str();
	BSTR bst = _bstr_t(str).GetBSTR();

	BSTR bstOut;
	bstOut = FileProObj->GetCopyProgress(bst);

	m_StrCopyInfo = (LPCTSTR)_bstr_t(bstOut);
	UpdateData(FALSE);

	CDialogEx::OnTimer(nIDEvent);
}
