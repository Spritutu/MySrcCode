// MainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "7zClientUI.h"
#include "CLientUIMainDlg.h"
#include "afxdialogex.h"


// CMainDlg �Ի���

IMPLEMENT_DYNAMIC(CCLientUIMainDlg, CDialogEx)

CCLientUIMainDlg::CCLientUIMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCLientUIMainDlg::IDD, pParent)
{

}

CCLientUIMainDlg::~CCLientUIMainDlg()
{
}

void CCLientUIMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_PAGE, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CCLientUIMainDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PAGE, &CCLientUIMainDlg::OnTcnSelchangeTabPage)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������


BOOL CCLientUIMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_TabCtrl.InsertItem(0,"ѹ��"); 
	m_TabCtrl.InsertItem(1,"��ѹ"); 

	m_TabCtrl.SetMinTabWidth(77);
	m_TabCtrl.SetItemSize(CSize(77,22));
	//������ҳ 
	m_ComDlg.Create(m_ComDlg.IDD,&m_TabCtrl); 
	m_DComDlg.Create(m_DComDlg.IDD,&m_TabCtrl); 

	//����ҳ���λ����m_tab�ؼ���Χ�� 
	CRect rect; 
	GetClientRect(rect);
	m_TabCtrl.MoveWindow(rect);
	m_TabCtrl.GetClientRect(&rect); 
	rect.top+=23; 

	rect.InflateRect(-1,-5,-1,-1);
	m_ComDlg.MoveWindow(&rect);
	m_DComDlg.MoveWindow(&rect); 
	m_ComDlg.ShowWindow(SW_SHOW); 
	m_DComDlg.ShowWindow(SW_HIDE);
	m_TabCtrl.SetCurSel(0);
	//m_ComDlg.SetFocus();
	return TRUE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCLientUIMainDlg::OnTcnSelchangeTabPage(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int CurSel; 
	CurSel=m_TabCtrl.GetCurSel(); 
	switch(CurSel) 
	{ 
	case 0: 
		m_ComDlg.ShowWindow(SW_SHOW); 
		m_DComDlg.ShowWindow(SW_HIDE); 
		break; 
	case 1: 
		m_ComDlg.ShowWindow(SW_HIDE); 
		m_DComDlg.ShowWindow(SW_SHOW); 
		break; 
	default: ; 
	} 

	*pResult = 0; 
}


void CCLientUIMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_TabCtrl.m_hWnd)
	{
		m_TabCtrl.MoveWindow(0,0,cx,cy);
	}
}


BOOL CCLientUIMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
		else if (pMsg->wParam == VK_SPACE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
