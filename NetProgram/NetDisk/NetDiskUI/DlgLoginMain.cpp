// DlgLoginMain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "DlgLoginMain.h"
#include "afxdialogex.h"


// CDlgLoginMain �Ի���

IMPLEMENT_DYNAMIC(CDlgLoginMain, CDialogEx)

CDlgLoginMain::CDlgLoginMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLoginMain::IDD, pParent)
{
	m_bCurPageIsLogin = TRUE;
}

CDlgLoginMain::~CDlgLoginMain()
{
}

void CDlgLoginMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LOGIN_PAGE_POS, m_wndPosPage);
	DDX_Control(pDX, IDC_STATIC_LOGIN_PIC, m_wndPosImg);
	DDX_Control(pDX, IDC_BTN_LOGIN_CHANGE, m_btnChangePage);
}


BEGIN_MESSAGE_MAP(CDlgLoginMain, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_LOGIN_CHANGE, &CDlgLoginMain::OnBnClickedBtnLoginChange)
	ON_MESSAGE(WM_LOGIN_SUCCESS,&CDlgLoginMain::OnMsgLoginSuccess)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgLoginMain ��Ϣ�������


BOOL CDlgLoginMain::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pageLogin.Create(CPageLogin::IDD,this);
	m_pageRegister.Create(CPageRegister::IDD,this);

	CRect rectPos;
	m_wndPosPage.GetClientRect(rectPos);
	m_wndPosPage.MapWindowPoints(this,&rectPos);

	rectPos.InflateRect(-1,-1,-1,-1);
	m_pageLogin.MoveWindow(rectPos);
	m_pageRegister.MoveWindow(rectPos);

	m_pageLogin.ShowWindow(SW_SHOW);
	m_pageRegister.ShowWindow(SW_HIDE);

	char szFileDir[MAX_PATH] = {0};
	GetModuleFileName(NULL,szFileDir,MAX_PATH);
	CString strDir = szFileDir;
	strDir = strDir.Left(strDir.ReverseFind('\\')+1);

	CString strImgBk = strDir + "Images\\LoginBk.bmp";

	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)::LoadImage(NULL, strImgBk, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);

	if(hBitmap == NULL)
	{
		CString strErr;
		strErr.Format("�޷�����λͼ��Դ��%s��",strImgBk);
		MessageBox(strErr,"����",MB_OK|MB_ICONERROR);
		PostQuitMessage(0);
	}

	if (m_BitMapBkImg.m_hObject)
		m_BitMapBkImg.Detach();

	m_BitMapBkImg.Attach(hBitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgLoginMain::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect RectImg;
	m_wndPosImg.GetClientRect(RectImg);
	m_wndPosImg.MapWindowPoints(this,&RectImg);
	RectImg.InflateRect(-1,-1,-1,-1);

	//�ڴ滭ͼ//////////////////////////
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmapMain;
	bitmapMain.CreateCompatibleBitmap(&dc, RectImg.Width(), RectImg.Height());
	MemDC.SelectObject(&bitmapMain);
	MemDC.Rectangle(RectImg);

	DrawBkImg(&MemDC,RectImg);
	dc.BitBlt(RectImg.left, RectImg.top, RectImg.Width(), RectImg.Height(), &MemDC, 0, 0, SRCCOPY);

	MemDC.DeleteDC();
}
void CDlgLoginMain::DrawBkImg(CDC *pDC,CRect rect)
{
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = MemDC.SelectObject(&m_BitMapBkImg);

	BITMAP bm;
	m_BitMapBkImg.GetBitmap(&bm);
	pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

void CDlgLoginMain::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_pageLogin.m_hWnd)
	{
		CRect rectPos;
		m_wndPosPage.GetWindowRect(rectPos);
		m_pageLogin.MoveWindow(rectPos);
	}

	if (m_pageRegister.m_hWnd)
	{
		CRect rectPos;
		m_wndPosPage.GetWindowRect(rectPos);
		m_pageRegister.MoveWindow(rectPos);
	}
}


void CDlgLoginMain::OnBnClickedBtnLoginChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_bCurPageIsLogin)
	{
		m_btnChangePage.SetWindowText("�����ʺţ�������¼>>");
		m_pageRegister.ShowWindow(SW_SHOW);
		m_pageLogin.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnChangePage.SetWindowText("��û���ʺţ����ע��>>");
		m_pageLogin.ShowWindow(SW_SHOW);
		m_pageRegister.ShowWindow(SW_HIDE);
	}

	m_bCurPageIsLogin = !m_bCurPageIsLogin;
}
 LRESULT CDlgLoginMain::OnMsgLoginSuccess(WPARAM wp,LPARAM lp)
 {
	 m_strUsrID   = m_pageLogin.m_LoginRes.m_szUsrId;
	 m_strUsrName = m_pageLogin.m_LoginRes.m_szUserName;
	 m_iAllSpace = m_pageLogin.m_LoginRes.m_iAllSpace;
	 m_iUsedSpace = m_pageLogin.m_LoginRes.m_iUsedSpace;
	 m_strPasswd  = m_pageLogin.GetPassWd();
	 EndDialog(IDOK);
	 return S_OK;
 }

 int CDlgLoginMain::OnCreate(LPCREATESTRUCT lpCreateStruct)
 {
	 if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		 return -1;

	 // TODO:  �ڴ������ר�õĴ�������
	 SetProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName,(HANDLE)1); // ���ô����־
	 return 0;
 }


 void CDlgLoginMain::OnDestroy()
 {
	 CDialogEx::OnDestroy();

	 // TODO: �ڴ˴������Ϣ����������
	 RemoveProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName); //�Ƴ������־
 }
