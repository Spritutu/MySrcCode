// LineStyleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "��ͼ����.h"
#include "LineStyleDlg.h"
#include "MainFrm.h"
#include "��ͼ����Doc.h"
#include "��ͼ����View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineStyleDlg dialog


CLineStyleDlg::CLineStyleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineStyleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineStyleDlg)
	m_nLineStyle = -1;
	//}}AFX_DATA_INIT
}


void CLineStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineStyleDlg)
	DDX_Control(pDX, IDC_COMBO_LINESTYLE, m_ComboBox);
	DDX_CBIndex(pDX, IDC_COMBO_LINESTYLE, m_nLineStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineStyleDlg, CDialog)
	//{{AFX_MSG_MAP(CLineStyleDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LINESTYLE, OnSelchangeComboLinestyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineStyleDlg message handlers

BOOL CLineStyleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ComboBox.AddString("�ջ���");  //�����µ��б���
	m_ComboBox.SetCurSel(0); //���õ�ǰѡ����Ϊ��һ��

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLineStyleDlg::OnSelchangeComboLinestyle() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);   //��ʹ���ݽ���
	int linestyle;   //��ʱ����
	switch (m_nLineStyle)      //�����m_nLineStyle��combobox�ı���
	{
	case 1 :
		linestyle=PS_DOT;     //����
		break;
     case 2 :
		 linestyle=PS_DASH; //����
		 break;
	 case 3 :
		 linestyle=PS_DASHDOT;  //�㻮��
		 break;
	 case 4 :
		 linestyle=PS_DASHDOTDOT;  //˫�㻮��
		 break;
	 case 5:
		 linestyle=PS_NULL;      //�ջ���
		 break;
	 default:
		 linestyle=PS_SOLID;      //���������Ϊʵ��
		 break;
	}
     CMainFrame *pmainframe=(CMainFrame *)GetParent();  //��ȡ��ܴ���ָ��
	 CMyView *pview=(CMyView*)pmainframe->GetActiveView();  //��ȡ��ͼ����ָ��
	 pview->m_nLineStyle=linestyle;    //������ͼ�ĳ�Ա��������ֵ
}
