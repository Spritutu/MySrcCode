
// StaffManageDoc.cpp : CStaffManageDoc ���ʵ��
//

#include "stdafx.h"
#include "StaffManage.h"

#include "StaffManageSet.h"
#include "StaffManageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStaffManageDoc

IMPLEMENT_DYNCREATE(CStaffManageDoc, CDocument)

BEGIN_MESSAGE_MAP(CStaffManageDoc, CDocument)
END_MESSAGE_MAP()


// CStaffManageDoc ����/����

CStaffManageDoc::CStaffManageDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CStaffManageDoc::~CStaffManageDoc()
{
}

BOOL CStaffManageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CStaffManageDoc ���

#ifdef _DEBUG
void CStaffManageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStaffManageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStaffManageDoc ����
