// MyEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GridViewTest.h"
#include "MyEdit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit()
{

}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CMyEdit ��Ϣ�������



void CMyEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);
	// �򶥼������ڼ��Ի��򷢳����뽹��ı��֪ͨ
	(GetParent()->GetParent())->SendMessage(GRID_DATA_CHANGED,0);

	// TODO: �ڴ˴������Ϣ����������
}
