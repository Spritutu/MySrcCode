
//-----------------------------------������˵����----------------------------------------------
// ��MFC��Ϸ�������ʼ�ʮһ ƽ������ ����Դ����
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


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	mciSendString("stop bgMusic ",NULL,0,NULL);
	delete m_snow;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()



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

	//�������ļ�
	mciSendString("open background.mp3 alias bgMusic ", NULL, 0, NULL);
	mciSendString("play bgMusic repeat", NULL, 0, NULL);

	m_snow=new CParticle(100);
	//��ʼ��
	m_snow->Init();

	return TRUE;
}

void CChildView::OnPaint() 
{
	static float lastTime=timeGetTime();    
	static float currentTime=timeGetTime();
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
	//��ѩ��
	m_snow->Draw(m_cacheDC);
	//����ѩ��
	currentTime=timeGetTime();
	m_snow->Update(currentTime-lastTime);
	lastTime=currentTime;
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


//��ʱ����Ӧ����
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	
	OnPaint();
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//����һ��10�������һ����Ϣ�Ķ�ʱ��
	SetTimer(TIMER_PAINT,10,NULL);

	return 0;
}

