
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once

#define SNOW_NUMBER 100  //ѩ�����ӵ�����
// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:
	//����ṹ��
	struct charcter
	{
		CImage character;     //���������ͼ��
		int x;             //���������λ��
		int y;
		int direct;        //����ķ���
		int frame;         //�˶����ڼ���ͼƬ
		int width;          //ͼƬ�Ŀ�Ⱥ͸߶ȣ�������ײ�ж�
		int height;
		int Xcenter;
		int Ycenter;
	}MyHero,Monster;
	
	CRect m_client;    //����ͻ�����С
	CImage m_bg;      //����ͼƬ


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
	void GetMapStartX();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

