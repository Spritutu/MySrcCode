// DlgResLibTree.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CugWorkSpace.h"
#include "DlgResLibTree.h"
#include "afxdialogex.h"


// CDlgResLibTree �Ի���

IMPLEMENT_DYNAMIC(CDlgResLibTree, CDialogEx)

CDlgResLibTree::CDlgResLibTree(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgResLibTree::IDD, pParent)
{

}

CDlgResLibTree::~CDlgResLibTree()
{
}

void CDlgResLibTree::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgResLibTree, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDlgResLibTree ��Ϣ�������


BOOL CDlgResLibTree::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ResLibTree.ModifyStyleEx(WS_EX_CLIENTEDGE | NM_CUSTOMDRAW, 0);

	ModifyStyle(DS_3DLOOK, 0);
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgResLibTree::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rcClient(3,5,cx,cy);
	if ( IsWindow(m_ResLibTree.GetSafeHwnd()))
	{
		m_ResLibTree.MoveWindow(&rcClient);
	}
}


int CDlgResLibTree::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	m_ResLibTree.Create("Ŀ¼��",WS_CHILD|WS_VISIBLE,CRect(0,0,500,1000),this,IDC_RESLIB_TREE);
	return 0;
}
