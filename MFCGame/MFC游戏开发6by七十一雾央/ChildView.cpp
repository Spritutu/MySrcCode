
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "GameMFC.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//��ʱ���������ú�Ƚ����
#define TIMER_PAINT 1
#define TIMER_HEROMOVE 2
//�ĸ�����
#define DOWN 0
#define LEFT 1
#define RIGHT 2
#define UP 3

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()


//��png��ͼ͸��
void TransparentPNG(CImage *png)
{
	for(int i = 0; i <png->GetWidth(); i++)
	{
		for(int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char *>(png->GetPixelAddress(i , j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}

// CChildView ��Ϣ�������

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	
	//-----------------------------------��Ϸ���ݳ�ʼ������-------------------------
	
	//���ر���
	m_bg.Load("bg.png");
	//����Ӣ��ͼƬ
	MyHero.hero.Load("heroMove.png");
	TransparentPNG(&MyHero.hero);
	//��ʼ��Ӣ��״̬
	MyHero.direct=UP;
	MyHero.frame=0;
	//����Ӣ�۳�ʼλ��
	MyHero.x=100;    
	MyHero.y=400;
	
	return TRUE;
}

void CChildView::OnPaint() 
{
	//��ȡ����DCָ��
	CDC *cDC=this->GetDC();
	//��ȡ���ڴ�С
	GetClientRect(&m_client);
	//��������DC
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC,m_client.Width(),m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);
	
	//������������������������������������������ʼ���ơ�������������������������������������������
	//������,������ͼ�������ڻ���DC��m_cache����
	m_bg.Draw(m_cacheDC,m_client);
	//��Ӣ��
	MyHero.hero.Draw(m_cacheDC,MyHero.x,MyHero.y,80,80,MyHero.frame*80,MyHero.direct*80,80,80);
	//��󽫻���DC�������������DC��
	cDC->BitBlt(0,0,m_client.Width(),m_client.Height(),&m_cacheDC,0,0,SRCCOPY);

	//�������������������������������������������ƽ���������������������������������������������
	
	//�ڻ�����ͼ��,ʹ��������Ч
	ValidateRect(&m_client);
	//�ͷŻ���DC
	m_cacheDC.DeleteDC();
	//�ͷŶ���
	m_cacheCBitmap.DeleteObject();
	//�ͷŴ���DC
	ReleaseDC(cDC);
}

//������Ӧ����
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//nChar��ʾ���µļ�ֵ
	switch(nChar)
	{
	case 'd':         //��Ϸ�а��µļ���ȻӦ�ò����ִ�Сд��
	case 'D':
		MyHero.direct=RIGHT;
		MyHero.x+=5;
		break;
	case 'a':
	case 'A':
		MyHero.direct=LEFT;
		MyHero.x-=5;
		break;
	case 'w':
	case 'W':
		MyHero.direct=UP;
		MyHero.y-=5;
		break;
	case 's':
	case 'S':
		MyHero.direct=DOWN;
		MyHero.y+=5;
		break;
	}
}

//������������Ӧ����
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	char bufPos[50];
	sprintf(bufPos,"�㵥���˵�X:%d,Y:%d",point.x,point.y);
	AfxMessageBox(bufPos);
}

//��ʱ����Ӧ����
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	
	switch(nIDEvent)
	{
	case TIMER_PAINT:OnPaint();break;  //�����ػ涨ʱ������ִ��OnPaint����
	case TIMER_HEROMOVE:               //���������ƶ��Ķ�ʱ��
		{
			MyHero.frame++;              //ÿ�ε��˼��ʱ��ͽ�ͼƬ��Ϊ��һ֡
			if(MyHero.frame==4)          //�����������ͷ��ʼ
				MyHero.frame=0;
		}
		break;
	}
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//����һ��10�������һ����Ϣ�Ķ�ʱ��
	SetTimer(TIMER_PAINT,10,NULL);
	//�����������߶�����ʱ��
	SetTimer(TIMER_HEROMOVE,100,NULL);
	return 0;
}
