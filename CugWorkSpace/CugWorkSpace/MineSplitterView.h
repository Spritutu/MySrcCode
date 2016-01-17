#pragma once


// CCxSplitterView ��ͼ

class CMineSplitterView : public CView
{
	DECLARE_DYNCREATE(CMineSplitterView)

protected:
	CMineSplitterView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CMineSplitterView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	int m_nSpliterViewFlag;
	HWND m_hwndInner;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


