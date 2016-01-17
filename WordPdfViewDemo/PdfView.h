
// PdfView.h : CWordPdfViewDemoView ��Ľӿ�
//

#pragma once
/************************************************************************/
/* Word �ĵ���ͼ��                                                                     */
/************************************************************************/
#include "CAcroAXDocShim.h"
class CPdfView : public CView
{
protected: // �������л�����
	CPdfView();
	DECLARE_DYNCREATE(CPdfView)

// ����
public:
	CPdfDoc* GetDocument() const;

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
	virtual ~CPdfView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CAcroAXDocShim m_drvPDF;
	CWnd           m_wndPDF;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnOpenPdfDoc();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // WordPdfViewDemoView.cpp �еĵ��԰汾
inline CPdfDoc* CPdfView::GetDocument() const
   { return reinterpret_cast<CPdfDoc*>(m_pDocument); }
#endif

