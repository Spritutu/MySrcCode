
// CookieUserView.h : CCookieUserView ��Ľӿ�
//

#pragma once


class CCookieUserView : public CHtmlView
{
protected: // �������л�����
	CCookieUserView();
	DECLARE_DYNCREATE(CCookieUserView)

// ����
public:
	CCookieUserDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CCookieUserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRefresh();
	virtual void OnNavigateComplete2(LPCTSTR strURL);
};

#ifndef _DEBUG  // CookieUserView.cpp �еĵ��԰汾
inline CCookieUserDoc* CCookieUserView::GetDocument() const
   { return reinterpret_cast<CCookieUserDoc*>(m_pDocument); }
#endif

