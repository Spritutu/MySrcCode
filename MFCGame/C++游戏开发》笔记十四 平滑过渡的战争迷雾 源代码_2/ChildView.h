
// ChildView.h : CChildView ��Ľӿ�
//

#pragma once
#include "particle.h"
#include "scene.h"

// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:
	//����ͻ�����С
	CRect m_client;   
	//ѩ��
	CParticle *m_snow;
	//����
	CScene *m_scene;
	//����DC
	CDC m_cacheDC;  
	//����λͼ
	CBitmap m_cacheCBitmap;
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

