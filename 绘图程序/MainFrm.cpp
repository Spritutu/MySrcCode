// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "��ͼ����.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_GRAPHTOOL, OnViewGraphtool)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GRAPHTOOL, OnUpdateViewGraphtool)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MYOPENDLG, OnMyopendlg)
	ON_COMMAND(ID_SET_LINESTYLE, OnSetLinestyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,    //�����ӵ�״̬��ʾ����
	ID_INDICATOR_TIME,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
     //////////////////////////
	 //�Լ������Ĺ�����
	if (!m_newtoolbar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_newtoolbar.LoadToolBar(IDR_GRAPHTOOL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//��������
	//////////////////////////////////
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetWindowText("��׼");
	m_newtoolbar.SetWindowText("��ͼ������");    //���ù���������
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
   ////////////////////////////
	//�Լ������Ĺ���������ʾ
	m_newtoolbar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_newtoolbar);
	///////////////
	//�趨��ʱ��
	SetTimer(1,1000,NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewGraphtool() 
{
	// TODO: Add your command handler code here
	
	if (m_newtoolbar.IsWindowVisible())
	{
		m_newtoolbar.ShowWindow(SW_HIDE);  //����ɼ���������
	}
	else 
	{
		m_newtoolbar.ShowWindow(SW_SHOW);
	}
	DockControlBar(&m_newtoolbar);    //ʹ��������ͣ����ʽ��ʾ
	RecalcLayout(TRUE);  //Ĭ��Ϊtrue

//	ShowControlBar(&m_newtoolbar,!m_newtoolbar.IsWindowVisible(),FALSE);
}

void CMainFrame::OnUpdateViewGraphtool(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_newtoolbar.IsWindowVisible());
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	/////////////////////��ȡ����ϵͳ��ǰʱ��
	/*
	CTime time;
	time=CTime::GetCurrentTime();   //����CTime��̬��Ա������ȡϵͳʱ��
	CString str;
	str=time.Format("%H:%M:%S");
	m_wndStatusBar.SetPaneText( //�ڴ���������ַ���
		m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME),str);
   */
	/////////////////////
	//��ȡ��������ʱ��
	SYSTEMTIME systemtime;
	::GetSystemTime(&systemtime); //��ȡ��������ʱ��
    CTime time(systemtime);
	CString str=time.Format("��������ʱ�� %H:%M:%S");
	m_wndStatusBar.SetPaneText(
		m_wndStatusBar.CommandToIndex(ID_INDICATOR_TIME),str);
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	if (MessageBox("Ҫ�˳�������?","��ʾ",MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		CFrameWnd::OnClose();
	}
	else return;
}

void CMainFrame::OnMyopendlg() 
{
	// TODO: Add your command handler code here
	char szFilter[]="�ı��ļ�(*.txt)|*.txt|word�ļ�(*.doc,*.docx)|*.doc;*.docx|��ִ���ļ�(*.exe)|*.exe||";
	CFileDialog filedlg(TRUE,"exe","����.txt",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,szFilter,this);
	filedlg.m_ofn.lpstrTitle="��ͼ���ļ�";        //m_ofnΪһ���ṹ�壬�ṩ��CFileDialog�����������������
	filedlg.DoModal();
}

void CMainFrame::OnSetLinestyle() 
{
	// TODO: Add your command handler code here
	m_LineStyleDlg.Create(IDD_LINESTYLE,this);
	m_LineStyleDlg.ShowWindow(SW_SHOW);  //��ʾ�Ի���
}
