
//-----------------------------------������˵����----------------------------------------------
// ��MFC��Ϸ�������ʼǾ� ��ײ�����˶���AI ����Դ����
// VS2010����
// �����������������CSDN���� http://blog.csdn.net/u011371356/article/category/1497651
// ���������΢���� @��ʮһ����
//------------------------------------------------------------------------------------------------


// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "GameMFC.h"
#include "ChildView.h"

#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")//��������ͷ�ļ���

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
//���ڴ�С
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	mciSendString("stop bgMusic ",NULL,0,NULL);
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
	MyHero.character.Load("heroMove.png");
	TransparentPNG(&MyHero.character);
	
	MyHero.width=80;
	MyHero.height=80;
	//��ʼ��Ӣ��״̬
	MyHero.direct=UP;
	MyHero.frame=0;
	//����Ӣ�۳�ʼλ��
	MyHero.x=80;    
	MyHero.y=400;

	//���ع���
	Monster.character.Load("monster.png");
	TransparentPNG(&Monster.character);
	Monster.width=96;
	Monster.height=96;
	Monster.direct=LEFT;
	Monster.frame=0;
	Monster.x=700;
	Monster.y=100;

	//�������ļ�
	mciSendString("open background.mp3 alias bgMusic ", NULL, 0, NULL);
	mciSendString("play bgMusic repeat", NULL, 0, NULL);

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
	m_bg.Draw(m_cacheDC,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	//��Ӣ��
	MyHero.character.Draw(m_cacheDC,MyHero.x,MyHero.y,80,80,
								MyHero.frame*80,MyHero.direct*80,80,80);
	//������
	Monster.character.Draw(m_cacheDC,Monster.x,Monster.y,96,96,
								Monster.frame*96,Monster.direct*96,96,96);
	//����״̬����
	//ˮƽ�����Ͽ���
	if(Monster.x<MyHero.x)
	{
		Monster.x++;
		Monster.direct=RIGHT;
	}
	else if(Monster.x>MyHero.x)
	{
		Monster.x--;
		Monster.direct=LEFT;
	}
	//��ֱ�����Ͽ���
	if(Monster.y<MyHero.y)
		Monster.y++;
	else 
		Monster.y--;

	//�ж��Ƿ���ײ
	MyHero.Xcenter=MyHero.x+MyHero.width/2;
	MyHero.Ycenter=MyHero.y+MyHero.height/2;
	Monster.Xcenter=Monster.x+Monster.width/2;
	Monster.Ycenter=Monster.y+Monster.height/2;
	//�������ֱ���͸��
	m_cacheDC.SetBkMode(TRANSPARENT);
	//��������Ϊ��ɫ
	m_cacheDC.SetTextColor(RGB(255,0,0));

	//�ٶ����ǽ�Ӣ����Ϊͼ�еĺ�ɫ����
	if(Monster.Xcenter< MyHero.Xcenter+(MyHero.width/2+Monster.width/2) &&
       Monster.Xcenter> MyHero.Xcenter-(MyHero.width/2+Monster.width/2) &&
	   Monster.Ycenter< MyHero.Ycenter+(MyHero.height/2+Monster.height/2) &&
       Monster.Ycenter> MyHero.Ycenter-(MyHero.height/2+Monster.height/2) )
	   m_cacheDC.TextOut(0,0,"������ײ");   //�ڴ������Ͻ���ʾ��ײ��Ϣ
	else
	   m_cacheDC.TextOut(0,0,"û����ײ");

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
			Monster.frame++;
			if(Monster.frame==4)         //�Ժ������Դ�����з�װ���Ͳ�����������ô�ظ��ˡ�
				Monster.frame=0;
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
