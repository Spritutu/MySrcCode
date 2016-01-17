
// TreeCtrlDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TreeCtrlDemo.h"
#include "TreeCtrlDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTreeCtrlDemoDlg �Ի���




CTreeCtrlDemoDlg::CTreeCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeCtrlDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTreeCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_TreeCtrl);
	DDX_Control(pDX, IDC_LIST, m_ListBox);
}

BEGIN_MESSAGE_MAP(CTreeCtrlDemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_DEL_SEl, &CTreeCtrlDemoDlg::OnBnClickedButtonDelSel)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ALL, &CTreeCtrlDemoDlg::OnBnClickedButtonDelAll)
	ON_BN_CLICKED(IDC_BUTTON_RESET_TREE, &CTreeCtrlDemoDlg::OnBnClickedButtonResetTree)
	ON_BN_CLICKED(IDC_BUTTON_FOREACH, &CTreeCtrlDemoDlg::OnBnClickedButtonForeach)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CTreeCtrlDemoDlg::OnTvnSelchangedTree)
END_MESSAGE_MAP()


// CTreeCtrlDemoDlg ��Ϣ�������

BOOL CTreeCtrlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitTree();
	//m_TreeCtrl.Expand(m_TreeCtrl.GetRootItem(),TVE_EXPAND);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTreeCtrlDemoDlg::OnPaint()
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
HCURSOR CTreeCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTreeCtrlDemoDlg::OnBnClickedButtonDelSel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HTREEITEM hSelItem = m_TreeCtrl.GetSelectedItem();
	if (m_TreeCtrl.ItemHasChildren(hSelItem))
	{
		if(MessageBox(_T("ѡ�нڵ����ӽڵ�,ȷʵҪɾ���ýڵ�?"),_T("��ʾ"),MB_YESNO) == IDNO)
			return;
	}
	m_TreeCtrl.DeleteItem(hSelItem);
}

void CTreeCtrlDemoDlg::OnBnClickedButtonDelAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TreeCtrl.DeleteAllItems();
}

void CTreeCtrlDemoDlg::OnBnClickedButtonResetTree()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_TreeCtrl.DeleteAllItems();
	InitTree();
}

void CTreeCtrlDemoDlg::OnBnClickedButtonForeach()
{
	// TODO: 
	m_ListBox.ResetContent();
	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	CString strText = m_TreeCtrl.GetItemText(hRoot);
	m_ListBox.AddString(strText);
	SearchTree(hRoot,1);
}
void CTreeCtrlDemoDlg::InitTree()
{
	m_TreeCtrl.ModifyStyle(0,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS);//Ϊ���������
	CString strText = _T("Root");
	HTREEITEM hRoot = m_TreeCtrl.InsertItem(strText,TVI_ROOT);
	for (int i=0;i<4;i++)
	{
		strText.Format(_T("Item %d"),i);
		HTREEITEM hParent = m_TreeCtrl.InsertItem(strText,hRoot,TVI_LAST);
		for (int j=0;j<4;j++)
		{
			strText.Format(_T("SubItem %d %d"),i,j);
			HTREEITEM hParent2 = m_TreeCtrl.InsertItem(strText,hParent,TVI_LAST);
		}
		m_TreeCtrl.Expand(hParent,TVE_EXPAND); //չ���ýڵ�
	}
	m_TreeCtrl.Expand(hRoot,TVE_EXPAND); //չ�����ڵ�
}

void CTreeCtrlDemoDlg::SearchTree(HTREEITEM hParent,int iStrOffset)
{
	CString strText = _T("");
	HTREEITEM hChild = NULL;
	if (!hParent)
		return;

	hChild = m_TreeCtrl.GetChildItem(hParent);
	while (hChild)
	{
		strText = m_TreeCtrl.GetItemText(hChild);
		for (int i=0;i< iStrOffset;i++)
			strText.Insert(0,_T(" "));

		m_ListBox.AddString(strText);
		if (m_TreeCtrl.ItemHasChildren(hChild))
			SearchTree(hChild,iStrOffset+1); // �����¼��ӽڵ�

		hChild = m_TreeCtrl.GetNextItem(hChild,TVGN_NEXT);
	}
}
void CTreeCtrlDemoDlg::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
