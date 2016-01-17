
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "GameMFC2.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
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

	//�������ݵ�DC
	m_bgcDC.CreateCompatibleDC(NULL);
	//����Դ�м���λͼ
	m_bgBitmap.LoadBitmap(IDB_BITMAP1);
	//ѡ��λͼ����
	m_bgcDC.SelectObject(&m_bgBitmap);
	//����Ӣ��ͼƬ
	m_hero.Load("hero.png");
	TransparentPNG(&m_hero);


	return TRUE;
}

void CChildView::OnPaint() 
{
	//��ȡ����DCָ��
	CDC *cDC=this->GetDC();
	//��ȡ���ڴ�С
	GetClientRect(&m_client);
	//��ͼ
	cDC->BitBlt(0,0,m_client.Width(),m_client.Height(),&m_bgcDC,0,0,SRCCOPY);
	//�ͷ�DC
	m_hero.Draw(*cDC,100,400,60,60);
	ReleaseDC(cDC);
}

