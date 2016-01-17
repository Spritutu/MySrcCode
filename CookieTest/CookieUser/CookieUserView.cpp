
// CookieUserView.cpp : CCookieUserView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "CookieUser.h"
#endif

#include "CookieUserDoc.h"
#include "CookieUserView.h"

#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCookieUserView

IMPLEMENT_DYNCREATE(CCookieUserView, CHtmlView)

BEGIN_MESSAGE_MAP(CCookieUserView, CHtmlView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_COMMAND(ID_REFRESH, &CCookieUserView::OnRefresh)
END_MESSAGE_MAP()

// CCookieUserView ����/����

CCookieUserView::CCookieUserView()
{
	// TODO: �ڴ˴���ӹ������

}

CCookieUserView::~CCookieUserView()
{
}

BOOL CCookieUserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CHtmlView::PreCreateWindow(cs);
}

void CCookieUserView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	SetSilent(TRUE);
	Navigate2(_T("http://www.jd.com/"),NULL,NULL);
}


// CCookieUserView ��ӡ



// CCookieUserView ���

#ifdef _DEBUG
void CCookieUserView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CCookieUserView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CCookieUserDoc* CCookieUserView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCookieUserDoc)));
	return (CCookieUserDoc*)m_pDocument;
}
#endif //_DEBUG


// CCookieUserView ��Ϣ�������


void CCookieUserView::OnRefresh()
{
	// TODO: �ڴ���������������
	Refresh();
}


void CCookieUserView::OnNavigateComplete2(LPCTSTR strURL)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (strcmp(strURL, "about:blank"))
	{
		CMainFrame *pFrameWnd = dynamic_cast<CMainFrame*>(GetParentFrame());
		if (pFrameWnd)
			pFrameWnd->m_editUrlAddress.SetWindowText(strURL);
	}

	CHtmlView::OnNavigateComplete2(strURL);
}
