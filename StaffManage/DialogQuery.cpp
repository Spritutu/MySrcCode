// DialogQuery.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StaffManage.h"
#include "DialogQuery.h"


// CDialogQuery �Ի���

IMPLEMENT_DYNAMIC(CDialogQuery, CDialog)

CDialogQuery::CDialogQuery(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogQuery::IDD, pParent)
	, m_query(_T(""))
{

}

CDialogQuery::~CDialogQuery()
{
}

void CDialogQuery::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_QUERY, m_query);
}


BEGIN_MESSAGE_MAP(CDialogQuery, CDialog)
END_MESSAGE_MAP()


// CDialogQuery ��Ϣ�������
