
// AnimateLineDrawView.h : CAnimateLineDrawView ��Ľӿ�
//


#pragma once


class CAnimateLineDrawView : public CView
{
protected: // �������л�����
	CAnimateLineDrawView();
	DECLARE_DYNCREATE(CAnimateLineDrawView)

// ����
public:
	CAnimateLineDrawDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CAnimateLineDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	int m_offsetx;
	CDC m_dc;
	CBitmap m_bitmap;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // AnimateLineDrawView.cpp �еĵ��԰汾
inline CAnimateLineDrawDoc* CAnimateLineDrawView::GetDocument() const
   { return reinterpret_cast<CAnimateLineDrawDoc*>(m_pDocument); }
#endif

