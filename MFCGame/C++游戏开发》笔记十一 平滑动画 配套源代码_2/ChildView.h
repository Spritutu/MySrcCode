
// ChildView.h : CChildView ��Ľӿ�
//

#pragma once
#include "particle.h"

// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:

	CRect m_client;    //����ͻ�����С
	CImage m_bg;      //����ͼƬ

	CParticle *m_snow;
	CDC m_cacheDC;   //����DC
	CBitmap m_cacheCBitmap;//����λͼ
// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

