
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "MyGame1.h"
#include "ChildView.h"
#include <atlimage.h>

#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")

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
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
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

	//�������ļ�
	mciSendString("open K:\\����.mp3 alias bgMusic ", NULL, 0, NULL);
	mciSendString("play bgMusic repeat", NULL, 0, NULL);

	return TRUE;
}

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
void CChildView::OnPaint() 
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	// TODO: �ڴ˴������Ϣ����������
	CBitmap bitmapBG;
	bitmapBG.LoadBitmap(IDB_BITMAP1_BG);

	CDC bgDC;
	bgDC.CreateCompatibleDC(NULL);
	bgDC.SelectObject(&bitmapBG);

	CImage hero;
	hero.Load("hero.png");
	TransparentPNG(&hero);
	hero.Draw(bgDC.m_hDC,100,400,60,60);

	CRect clientRect;
	GetClientRect(&clientRect);

	dc.BitBlt(0,0,clientRect.Width(),clientRect.Height(),&bgDC,0,0,SRCCOPY);
	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
}

