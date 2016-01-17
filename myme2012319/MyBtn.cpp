// MyBtn.cpp : implementation file
//

#include "stdafx.h"
#include "MyBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyBtn

CMyBtn::CMyBtn()
{
	bTrackLeave = FALSE;
	m_btnMove = FALSE;
	m_btnText = "";
}

CMyBtn::~CMyBtn()
{
}


BEGIN_MESSAGE_MAP(CMyBtn, CWnd)
	//{{AFX_MSG_MAP(CMyBtn)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyBtn message handlers

void CMyBtn::OnPaint() 
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


	szMemDC.Rectangle(rcClient);


    //��椱���
	if(bTrackLeave)  //�Ƿ��뿪�ı�־
	{
		DrawRangeImage(&m_BitmapBack[0],&szMemDC,rcClient);
	}
	else
	{
		DrawRangeImage(&m_BitmapBack[1],&szMemDC,rcClient);
	}
	

	if(m_btnText.GetLength() > 0)
	{
		CSize sz = szMemDC.GetTextExtent(m_btnText);

		long nLeft = (rcClient.Width() - sz.cx)/2;
		long nTop = (rcClient.Height() - sz.cy)/2;
		szMemDC.SetBkMode(TRANSPARENT);
		szMemDC.SetTextColor(RGB(255,255,255));
		szMemDC.TextOut(nLeft,nTop,m_btnText,m_btnText.GetLength());
	}
	//������������
	//ChangeWindowRgn(&szMemDC);

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    
	szMemDC.DeleteDC();
}

/*
void CMyBtn::CreateMsgWindow(HWND hWnd,RECT rc,long nDlgID)   
{   


	CWnd*pWndParent=CWnd::FromHandle(hWnd);

	//_rect = rc;
	long wndWidth = (rc.right - rc.left) ;
	long wndHeight = (rc.bottom - rc.top) ;

    Create(NULL , NULL,WS_CHILD|WS_VISIBLE,rc,pWndParent,nDlgID,NULL);

}   */





//ƽ�̻�λͼ
void CMyBtn::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
{
	CDC MemDC;
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	int li_Width = bm.bmWidth;
	int li_Height = bm.bmHeight;


	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = MemDC.SelectObject(pBitmap);

	int x=rc.left;
	int y=rc.top;
	while (y < (rc.Height()+rc.top)) 
 	{
 		while(x < (rc.Width()+rc.left)) 
 		{
			pDC->BitBlt(x, y, li_Width, li_Height, &MemDC, 0, 0, SRCCOPY);
 			x += li_Width;
 		}
		x = rc.left;
 		y += li_Height;
 	}

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

//������Դλͼ
void CMyBtn::SetBackImage(int index,CString strName)
{
	SetImage(m_BitmapBack[index], strName);
	SetImageSize(&m_BitmapBack[index], m_szBitmapBack[index]);
}

//����Ӳ���ļ�λͼ
void CMyBtn::SetImage(CBitmap &bitmap, CString strName)
{
	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)::LoadImage(NULL, strName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	//ASSERT(hBitmap);

	if(hBitmap == NULL)
	{
		CString str;
		AfxMessageBox(_T("Not Open SetImage Bitmap"));
		PostQuitMessage(0);
	}

	if (bitmap.m_hObject)
		bitmap.Detach();

	bitmap.Attach(hBitmap);
}

//����λͼ�ߴ�
void CMyBtn::SetImageSize(CBitmap *pBitmap, CSize &sz)
{
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	sz = CSize(bm.bmWidth, bm.bmHeight);
}

void CMyBtn::SetLayeredWindowTrans(int nTrans)
{
   SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,   
   GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);   
   HINSTANCE   hInst   =   LoadLibrary("User32.DLL");     
   if(hInst)     
   {     
      typedef   BOOL   (WINAPI   *MYFUNC)(HWND,COLORREF,BYTE,DWORD);     
      MYFUNC   fun   =   NULL;   
      //ȡ��SetLayeredWindowAttributes����ָ��     
      fun=(MYFUNC)GetProcAddress(hInst,   "SetLayeredWindowAttributes");   
      if(fun)fun(this->GetSafeHwnd(),0,nTrans,2);     
      FreeLibrary(hInst);     
   }   
}

void CMyBtn::ChangeWindowRgn(CDC *pDC)
{
	COLORREF col = RGB(255,0,255);
	CRect rcClient;
	GetClientRect (rcClient);
	CRgn rgn;
	rgn.CreateRoundRectRgn (0, 0, rcClient.Width(), rcClient.Height(),6,6);

	SetWindowRgn (rgn, TRUE);
}

void CMyBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
    BOOL OldFlag = bTrackLeave;
	if (!bTrackLeave)
	{

       // ����һ�����봰��ʱ�� ����һ��WM_MOUSELEAVE ��Ϣ
      
        TRACKMOUSEEVENT tme;

        tme.cbSize = sizeof(tme);

        tme.hwndTrack = m_hWnd;

        tme.dwFlags = TME_LEAVE;

        _TrackMouseEvent(&tme);

        bTrackLeave = TRUE;
        // ��������Ӵ���������Ĵ��� ��

    }

    if(OldFlag != bTrackLeave)
	{
		this->Invalidate(FALSE);
	}
	CWnd::OnMouseMove(nFlags, point);
}


LPARAM CMyBtn::OnMouseLeave(WPARAM wp, LPARAM lp)
{

  BOOL OldFlag = bTrackLeave;
  bTrackLeave = FALSE;

 
  if(OldFlag != bTrackLeave)
  {
    	this->Invalidate(FALSE);
  }

// ��������Ӵ�������뿪�Ĵ��� ��
    return 0;

}

void CMyBtn::SetBtnFlag(BOOL bFlag)
{
	BOOL bOldFlag = m_btnMove;
	m_btnMove = bFlag;
	if(bOldFlag != m_btnMove)
	{
		this->Invalidate(FALSE);
	}
}

void CMyBtn::SetBtnText(CString str)
{
	m_btnText = str;
}
