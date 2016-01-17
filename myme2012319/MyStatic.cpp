// MyStatic.cpp : implementation file
//

#include "stdafx.h"
#include "MeYe.h"
#include "MyStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyStatic

CMyStatic::CMyStatic()
{
}

CMyStatic::~CMyStatic()
{
}


BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	//{{AFX_MSG_MAP(CMyStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyStatic message handlers

void CMyStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

   
	//�ڴ滭ͼ//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();
	szMemDC.FillSolidRect(rcClient,RGB(221,228,238));
	//szMemDC.Rectangle(rcClient);

	//д����
	szMemDC.SetBkMode(TRANSPARENT);

	CString str;
	GetWindowText(str);
	szMemDC.SetTextColor(RGB(0,0,0));
    szMemDC.TextOut(0,0,str);

	//szMemDC.DrawText(str,&rcClient,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	//������ 
	//m_memDC.BitBlt(r.left, r.top, r.Width(), r.Height(), &szMemDC, 0, 0, SRCCOPY);
    


	
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);

	szMemDC.DeleteDC();
}
