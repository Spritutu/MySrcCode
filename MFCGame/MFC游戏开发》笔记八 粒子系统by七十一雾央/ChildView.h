
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
	//���ӽṹ��
	struct snow
	{
		int x;     //���ӵ�x����
		int y;	   //���ӵ�y����
		int number;//���ӱ�ţ�����������
	}Snow[SNOW_NUMBER];    
	//ѩ��ͼ��
	CImage m_snowMap[7];
	//Ӣ�۽ṹ��
	struct shero
	{
		CImage hero;     //����Ӣ�۵�ͼ��
		int x;             //����Ӣ�۵�λ��
		int y;
		int direct;        //Ӣ�۵ķ���
		int frame;         //�˶����ڼ���ͼƬ
	}MyHero;

	CRect m_client;    //����ͻ�����С
	CImage m_bg;      //����ͼƬ

	int m_xMapStart;     //x�����ϵ�ͼ����ʼ��
	int m_mapWidth;      //������ͼ�Ŀ��

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

