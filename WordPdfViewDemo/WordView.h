
// WordPdfViewDemoView.h : CWordPdfViewDemoView ��Ľӿ�
//

#pragma once
/************************************************************************/
/* Word �ĵ���ͼ��                                                                     */
/************************************************************************/
#include "CWebBrowser2.h"
class CWordView : public CView
{
protected: // �������л�����
	CWordView();
	DECLARE_DYNCREATE(CWordView)

// ����
public:
	CWordDoc* GetDocument() const;
	void OpenDocFile(CString strFile);
	BOOL IsWordFile(const CString &strFile);
	BOOL ConvertWordToHtml(CString strWordFile,CString strHtml);
	void RemoveTempHtmlFile();

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
	virtual ~CWordView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CWebBrowser2 m_web;
	CString m_strLastTmpFile;
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOpenWordDoc();
};

#ifndef _DEBUG  // WordPdfViewDemoView.cpp �еĵ��԰汾
inline CWordDoc* CWordView::GetDocument() const
   { return reinterpret_cast<CWordDoc*>(m_pDocument); }
#endif

