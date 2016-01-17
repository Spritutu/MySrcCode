// InfoWnd.cpp: implementation of the CInfoWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfoWnd.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define TITLE_HEIGHT        55

#define PIC_HEIGHT         25
#define PIC_WIDTH          100
#define ITEM_X_SPACE         10

#define ITEM_Y_SPACE         10

#define ITEM_OTHER_HEIGHT         20

CInfoWnd::CInfoWnd()
{
   m_strTitle = "վ����Ϣ";
   m_strContent = "������Ϣ";
   m_TimeWnd = 10;
   m_btnStatClose = 0;
   m_btnStatPos = 0;
   m_bBtnTimer = FALSE;
   m_IndexDisplay = 0;
   m_btnStatScroll = 0;
   m_btnHeightScroll = 20;
   m_vecStrContent.clear();
   m_rowDataShow = 1;
   m_ItemWidth = -1;
   m_ItemSpace = ITEM_X_SPACE;
   memset(&m_lastPoint,0x00,sizeof(m_lastPoint));

   m_MouSelItem = -1;
   m_LDClickItem = m_LBtnDownItem  = -1;
}

CInfoWnd::~CInfoWnd()
{
	//����ַ���
	vector<ListItemProductors* >::iterator iter;
	for(iter = m_vecStrContent.begin(); iter!=m_vecStrContent.end(); iter++){
		delete *iter;
	}
	m_vecStrContent.clear();

}

BEGIN_MESSAGE_MAP(CInfoWnd, CWnd)
	//{{AFX_MSG_MAP(CMapRTWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInfoWnd::SetWndCaption (CString str)
{
	m_strTitle = str;
	return;
}

void CInfoWnd::AdditemData(Productors* pData)
{
	ListItemProductors *pTemp = new ListItemProductors;
	pTemp->productor = *pData;

	// ���ͼ��
	pTemp->pImage = new CBitmap();
	HBITMAP bm = (HBITMAP)::LoadImage(NULL,pData->picpath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	pTemp->pImage->Attach(bm);


	m_vecStrContent.push_back(pTemp);

	m_ItemWidth = PIC_WIDTH;
	return;
}

void CInfoWnd::SetCloseTime(int nTime)
{
	m_TimeWnd = nTime;
	return;
}

BOOL CInfoWnd::Create(HWND parhnd)
{
	return(CWnd::Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),	CWnd::FromHandle(parhnd),2250,NULL));

	CString szClassName = AfxRegisterWndClass(CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW,
		AfxGetApp()->LoadStandardCursor(IDC_ARROW),(HBRUSH) (COLOR_WINDOW), 0);
	BOOL bRet = CreateEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW,
		szClassName,NULL,
		WS_THICKFRAME | WS_BORDER | WS_CHILD  |
		WS_CLIPSIBLINGS | WS_OVERLAPPED,
		CRect(0,0,0,0),
		CWnd::FromHandle(parhnd), 0, NULL) ;
	if(!bRet) return FALSE;

	//ȥ�߿�
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);
	style &= ~WS_CAPTION; 
	::SetWindowLong (m_hWnd,GWL_STYLE,style); 

	//͸����
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,   
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);   
	HINSTANCE   hInst   =   LoadLibrary("User32.DLL");     
	if(hInst)     
	{     
		typedef   BOOL   (WINAPI   *MYFUNC)(HWND,COLORREF,BYTE,DWORD);     
		MYFUNC   fun   =   NULL;   
		//ȡ��SetLayeredWindowAttributes����ָ��     
		fun=(MYFUNC)GetProcAddress(hInst,   "SetLayeredWindowAttributes");   
		if(fun)fun(this->GetSafeHwnd(),0,255,2);    //͸����128, 255��͸�� 
		FreeLibrary(hInst);     
	}   
	SetTimer(0,1000,NULL);

	//��ʼ��ͼƬ�ļ�
//	m_Scrollback.LoadBitmap(IDB_BITMAP_SCROLL);
//	m_Btnback.LoadBitmap(IDB_BITMAP_INFO_WND);
	return TRUE;
}

void CInfoWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient,rcBtn,rcScroll,rcPic;
	GetClientRect(&rcClient);

	//�ڴ滭ͼ//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();
	szMemDC.Rectangle(rcClient);
//	szMemDC.Draw3dRect(&rcClient,RGB(0,0,0),RGB(221,228,238));

	szMemDC.FillSolidRect( &rcClient ,RGB(221,228,238));//RGB( 150,150,150));

	m_BtnRectClose.left = rcClient.right - 19;
	m_BtnRectClose.right = rcClient.right - 3;
	m_BtnRectClose.top = rcClient.top ;
	m_BtnRectClose.bottom = rcClient.top + 16;
	{
		rcBtn.left = 32;
		rcBtn.top = 0;
		rcBtn.bottom = TITLE_HEIGHT + 1;
		rcBtn.right = 48;
//		DrawRangeImage(&m_Btnback , &szMemDC,m_BtnRectClose,rcBtn);
	}

	m_BtnRectPos.left = rcClient.right - 38;
	m_BtnRectPos.right = rcClient.right - 22;
	m_BtnRectPos.top = rcClient.top;
	m_BtnRectPos.bottom = rcClient.top + 16;
	{
		if(!m_bBtnTimer){
			rcBtn.left = 0;
			rcBtn.right = TITLE_HEIGHT+ 1;
		}
		else{
			rcBtn.left = 16;
			rcBtn.right = 32;
		}
		rcBtn.top = 0;
		rcBtn.bottom = TITLE_HEIGHT+ 1;
//		DrawRangeImage(&m_Btnback , &szMemDC,m_BtnRectPos,rcBtn);
	}

	//д������
	CRect rect = rcClient;
	rect.bottom = rect.top + TITLE_HEIGHT;
	CPen pen, *oldpen;
	pen.CreatePen( PS_SOLID, 1, RGB(174,175,176) );
    oldpen = szMemDC.SelectObject( &pen );
	szMemDC.SetTextColor( RGB(174,175,176) ); 
	szMemDC.SetBkMode(TRANSPARENT);
	LOGFONT ft; 
	memset( &ft, 0, sizeof( ft ) ); 
	ft.lfHeight = 0x0F; ft.lfWeight = FW_NORMAL;
	ft.lfClipPrecision = CLIP_LH_ANGLES;
	strcpy( ft.lfFaceName,_T( "Arial" ) );
	CFont font, *oldfont; font.CreateFontIndirect( &ft );  // ��������ṹ�����������
	oldfont = szMemDC.SelectObject( &font );
	// ������ʾ
	CSize size = szMemDC.GetTextExtent( m_strTitle );
    szMemDC.TextOut( rect.left + ( rect.Width() - size.cx ) / 2, rect.top, m_strTitle );	
	szMemDC.SelectObject( &oldpen ); 
	szMemDC.SelectObject( &oldfont );

	CRect rect_Main;//�ı���ʾ����
	rect_Main = rcClient;
	rect_Main.top = TITLE_HEIGHT + 5;
	rect_Main.bottom = rcClient.bottom - 5;
	rect_Main.left = rcClient.left + 6;
	rect_Main.right = rcClient.right - 6;

	szMemDC.Draw3dRect(&rect_Main,RGB(0,0,0),RGB(221,228,238));

	//��ʾͼ����
	szMemDC.SetTextColor( RGB( 0, 255, 0 ) ); 
    vector<ListItemProductors* >::iterator iter;
	int nBegin = 0;
	long nDrawStarIndex(0);
	int nTotal = m_vecStrContent.size();
	long nRowIndex(0);
//	RECT rcPic = {0};
	for(iter = m_vecStrContent.begin(); iter!=m_vecStrContent.end(); iter++){
		if(nBegin < m_IndexDisplay){
			nBegin++;
			continue;
		}

		ListItemProductors* pItemData = *iter;
		nBegin++;

		if(nRowIndex == 0)
		{ // һ��ȫ��ʾ����Ű�����������
			BITMAP   bmp; 
            pItemData->pImage->GetBitmap(&bmp); 
            long nWidth = bmp.bmWidth;
			long nHeight=bmp.bmHeight;

			rect.top = rect.top + TITLE_HEIGHT  + ITEM_Y_SPACE; // �Ǳ߾�
    		rect.bottom = rect.top + nHeight + ITEM_OTHER_HEIGHT;   // PIC_HEIGHT��ͼ��ĸ߶�,ITEM_OTHER_HEIGHT������������ռ�Ĵ�С
		}

		if(rect.bottom  <=  rect_Main.bottom)
		{
			    

			    nDrawStarIndex++;
				rect.left = ITEM_X_SPACE*(nRowIndex + 1) + nRowIndex*m_ItemWidth ;
				rect.right = rect.left + m_ItemWidth ;//? ��ͼҪ������ , ����ֻ������������,pItemData->pImage->GetWidth();
			
				rcPic = rect;
				rcPic.top += 14;
				rcPic.bottom += 14;
				rcPic.left += 5;
				rcPic.right += 5;

				// �����
				pItemData->rcPosMain = rcPic;
				pItemData->rcPosMain.left -=2 ;
				pItemData->rcPosMain.right += 2;
				pItemData->rcPosMain.top -= 2;
				pItemData->rcPosMain.bottom += 2;
				
			//    szMemDC.FillSolidRect( &pItemData->rcPosMain ,RGB(248,248,248));


				DrawRangeImage(pItemData->pImage,&szMemDC,rcPic,rcPic);
			//	pItemData->pImage->Stretch(szMemDC,rcPic);

				//�������
				szMemDC.TextOut( rect.left + 12, rect.top, pItemData->productor.itemName );

				pItemData->rcPos = rcPic;

				rcPic.left -=1 ;
				rcPic.right += 2;
				rcPic.top -= 1;
				rcPic.bottom += 2;

				

				COLORREF picColFram;
				if(m_MouSelItem == nBegin)
				{// ��ͼ��������ɫ
					picColFram = RGB(43,225,32);
				}
				else
				{
					picColFram = RGB(0,255,0);
				}

				CPen Pen(PS_SOLID,1,picColFram);
		        HGDIOBJ oldPen  = szMemDC.SelectObject(&Pen);

				szMemDC.MoveTo(rcPic.left ,rcPic.top );
				szMemDC.LineTo(rcPic.right ,rcPic.top );
				szMemDC.LineTo(rcPic.right,rcPic.bottom);
				szMemDC.LineTo(rcPic.left,rcPic.bottom);
				szMemDC.LineTo(rcPic.left,rcPic.top);

			/*	szMemDC.MoveTo(rcPic.left ,rcPic.top );
				szMemDC.LineTo(rcPic.right ,rcPic.top );
				szMemDC.LineTo(rcPic.right,rcPic.bottom);
				szMemDC.LineTo(rcPic.left,rcPic.bottom);
				szMemDC.LineTo(rcPic.left,rcPic.top);
*/
				szMemDC.SelectObject(&oldPen);

				if(nRowIndex < m_rowDataShow-1)
				{
					nRowIndex++;
				}
				else
				{
					nRowIndex = 0;
				}

	
				if(nRowIndex == 0)
				{
					BITMAP   bmp; 
					pItemData->pImage->GetBitmap(&bmp); 
					long nWidth = bmp.bmWidth;
					long nHeight=bmp.bmHeight;

			    	rect.top+=nHeight + ITEM_OTHER_HEIGHT;
				}

//?			size = szMemDC.GetTextExtent( (*iter) );
//?			szMemDC.TextOut( rect.left + 12, rect.top, *iter );
		}
		else break;
	}
    int nDisplayItem = nBegin - m_IndexDisplay;//��ǰ������ʾ������

	//������SCROLL
	rcScroll.left = rcClient.right - 22;
	rcScroll.top = rcClient.top + TITLE_HEIGHT + 8;
	rcScroll.bottom = rcClient.bottom - 5;
	rcScroll.right = rcScroll.left + 16;
	szMemDC.Draw3dRect(&rcScroll,RGB(99,115,132),RGB(132,148,165));//?RGB(0,0,0),RGB(221,228,238));//����������ɫ

	int nScrollHeight = 0;
	int nBtnScrollSpan = 0;
	if(nTotal > 1){
		m_btnHeightScroll = rcScroll.Height()/nTotal;
		nScrollHeight = rcScroll.Height() - m_btnHeightScroll - 4;
		nBtnScrollSpan =  nScrollHeight/(nTotal - 1);
	}
	m_ScrollRect = rcScroll;
	m_ScrollRect.left = m_ScrollRect.left + 2;
	m_ScrollRect.right = m_ScrollRect.right - 2;
	m_ScrollRect.top = m_ScrollRect.top + 2 + m_IndexDisplay*nBtnScrollSpan;
	m_ScrollRect.bottom = m_ScrollRect.top + m_btnHeightScroll;
	if(m_btnStatScroll == 0)
	  szMemDC.Draw3dRect(&m_ScrollRect,RGB(99,115,132),RGB(99,115,132));//?RGB(0,0,0),RGB(0,0,0));//���������
    else
	  szMemDC.Draw3dRect(&m_ScrollRect,RGB(221,228,238),RGB(0,0,0));//?RGB(0,0,0),RGB(0,0,0));//���������
    dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
    
	szMemDC.DeleteDC();
}


void CInfoWnd::DrawRangeImage(CBitmap* pBmp , CDC *pDC,CRect rcPic,CRect rcBmp)
{
	CDC MemDC;
	CBitmap *pBitmap;
    pBitmap = pBmp;

	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = MemDC.SelectObject(pBitmap);

	pDC->StretchBlt(rcPic.left, rcPic.top,rcPic.Width(),rcPic.Height() , &MemDC, rcBmp.left, rcBmp.top,rcBmp.Width(), rcBmp.Height(), SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC(); 
}

BOOL CInfoWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    if(zDelta>0)
	{
		if(m_IndexDisplay >= m_rowDataShow)
		{
		   m_IndexDisplay -= m_rowDataShow;
		}
		else
		{
			m_IndexDisplay = 0;
		}
	}
	else
	{
		if(m_IndexDisplay <= m_vecStrContent.size() - m_rowDataShow)
		{
		    m_IndexDisplay+=m_rowDataShow;	
		}
	}
	if(m_IndexDisplay < 0)
		m_IndexDisplay = 0;
	if(m_IndexDisplay > m_vecStrContent.size() - 1)
		m_IndexDisplay = m_vecStrContent.size() - 1;
	this->Invalidate(FALSE);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CInfoWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();		   //�������ȫ����׽
    
	CPoint szPoint;
	::GetCursorPos(&szPoint);	
    m_ptCurPointLast=szPoint;	   //���°���ʱ���������
	m_ptCurPoint=point;	   //���°���ʱ���������
	CRect rc,rcClient,rcScroll;
	GetWindowRect(rc);
	GetClientRect(&rcClient);

	m_rcCurRect = rc;//
	if(m_BtnRectClose.PtInRect(point))
	{
		m_btnStatClose = 1;
		this->Invalidate(FALSE);
		return;
	}
	if(m_BtnRectPos.PtInRect(point))
	{
		m_btnStatPos = 1;
		this->Invalidate(FALSE);
		return;
	}

	m_lPARAM.rcPos = rc;

	rcScroll.left = rcClient.right - 22;
	rcScroll.top = rcClient.top + 20;
	rcScroll.bottom = rcClient.bottom - 5;
	rcScroll.right = rcScroll.left + 16;
	if(rcScroll.PtInRect(point))
	{
		m_btnStatScroll = 1;
	    SetTimer(2,100,NULL);
		return;
	}

	if(MouseLDown(point))
	{
		return;
	}

/*	CRect rcW(0,0,rc.Width(),rc.Height());
	if(rcW.PtInRect(point))
	{
	    SetTimer(1,20,NULL);
		return;
	}
*/
	

	CWnd::OnLButtonDown(nFlags, point);
}

void CInfoWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

	long lastSelItem = m_LDClickItem;
	vector<ListItemProductors* >::iterator iter;
	int nBegin = 0;
	long nDrawStarIndex(0);
	int nTotal = m_vecStrContent.size();
	BOOL bFine = FALSE;

	for(iter = m_vecStrContent.begin(); iter!=m_vecStrContent.end(); iter++)
	{
		if(nBegin < m_IndexDisplay)
		{
			nBegin++;
			continue;
		}

		nBegin++;
		ListItemProductors* pItemData = *iter;
		if(pItemData->rcPos.PtInRect(point))
		{
			 m_LDClickItem = nBegin;
			 bFine = TRUE;

			 char sztemp[64] = {0};
			 sprintf(sztemp,"��˫��ѡȡ�˵�%d��",m_LDClickItem);
			 AfxMessageBox(sztemp);
			 break;
		}
		
	}

	if(lastSelItem!=m_LDClickItem)
	{
		return;
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CInfoWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	ReleaseCapture();
	KillTimer(1);		//�ر��϶�ʱ������
	KillTimer(2);		//�ر��϶�ʱ������
	if(m_btnStatScroll == 1){
	   m_btnStatScroll = 0;
	   this->Invalidate(FALSE);
	}
	if(m_BtnRectClose.PtInRect(point)&&m_btnStatClose == 1)
	{
		m_btnStatClose = 0;
		this->Invalidate(FALSE);
		CloseDrawWnd();
		return;//�رմ��ں󷵻أ�����ִ��������Ϣ
	}
	if(m_BtnRectPos.PtInRect(point)&&m_btnStatPos == 1)
	{
		m_btnStatPos = 0;
		 m_bBtnTimer ^= TRUE;
		if(m_bBtnTimer)
		   KillTimer(0);
		else 
		   SetTimer(0,1000,NULL);
		this->Invalidate(FALSE);
		return;
	}
	CWnd::OnLButtonUp(nFlags, point);
}
void CInfoWnd::KillTimerButton()
{
	ReleaseCapture();
	KillTimer(1);
}

void CInfoWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	int nLastStat = m_btnStatClose;
	if(m_BtnRectClose.PtInRect(point))
	{
		if(m_btnStatClose != 1)
		   m_btnStatClose = 2;
		if(nLastStat != m_btnStatClose)
			this->Invalidate(FALSE);
	}
	else
	{
		m_btnStatClose = 0;
		if(nLastStat != m_btnStatClose)
			this->Invalidate(FALSE);
	}

	nLastStat = m_btnStatPos;
	if(m_BtnRectPos.PtInRect(point))
	{
		if(m_btnStatPos != 1)
		   m_btnStatPos = 2;
		if(nLastStat != m_btnStatPos)
			this->Invalidate(FALSE);
	}
	else
	{
		m_btnStatPos = 0;
		if(nLastStat != m_btnStatPos)
			this->Invalidate(FALSE);
	}

	MouseSelect( point );

	CWnd::OnMouseMove(nFlags, point);
}


void CInfoWnd::MouseSelect( const CPoint & point)
{

	long lastSelItem = m_MouSelItem;
	vector<ListItemProductors* >::iterator iter;
	int nBegin = 0;
	long nDrawStarIndex(0);
	int nTotal = m_vecStrContent.size();
	BOOL bFine = FALSE;

	for(iter = m_vecStrContent.begin(); iter!=m_vecStrContent.end(); iter++)
	{
		if(nBegin < m_IndexDisplay)
		{
			nBegin++;
			continue;
		}

		nBegin++;
		ListItemProductors* pItemData = *iter;
		if(pItemData->rcPos.PtInRect(point))
		{
			 m_MouSelItem = nBegin;
			 bFine = TRUE;
			 break;
		}
		
	}

  
	if(bFine == FALSE )
	{
		m_MouSelItem = -1;
	}

	if(m_MouSelItem != lastSelItem)
	{
		this->Invalidate(FALSE);
	}

}


BOOL CInfoWnd::MouseLDown( const CPoint & point)
{

	long lastSelItem = m_LBtnDownItem;
	vector<ListItemProductors* >::iterator iter;
	int nBegin = 0;
	long nDrawStarIndex(0);
	int nTotal = m_vecStrContent.size();
	BOOL bFine = FALSE;

	for(iter = m_vecStrContent.begin(); iter!=m_vecStrContent.end(); iter++)
	{
		if(nBegin < m_IndexDisplay)
		{
			nBegin++;
			continue;
		}

		nBegin++;
		ListItemProductors* pItemData = *iter;
		if(pItemData->rcPos.PtInRect(point))
		{
			 m_LBtnDownItem = nBegin;
			 bFine = TRUE;

			 char sztemp[64] = {0};
			 sprintf(sztemp,"��ѡȡ�˵�%d��",m_LBtnDownItem);
			 OutputDebugString(sztemp);
			 break;
		}
		
	}

  
/*	if(bFine == FALSE )
	{
		m_LBtnDownItem = -1;
	}

	if(m_LBtnDownItem != lastSelItem)
	{
		this->Invalidate(FALSE);
	}*/

	return bFine;

}

void CInfoWnd::OnSize(UINT nType, int cx, int cy) 
{
	long nNumItem = m_vecStrContent.size();
	if(nNumItem > 0 && m_ItemWidth > 0)
	{// Ҫע��ľ��ǲ������ͻ�����һ�㳤�ȶ�û�еĵز�
		long nOldIndexStar = m_IndexDisplay;
		long nOldShowNum = m_rowDataShow;

		RECT rtClient ; 
	    this->GetClientRect(&rtClient);
		long nW = rtClient.right - rtClient.left - ITEM_X_SPACE*2 - 22 ;// �ȿ۳�����, 22��ScrollBar���
		if(nW > PIC_HEIGHT)
		{// ����ʾһ��ʱ�Ž������´���
			long nNummaxShow = nW / m_ItemWidth;
			if(nNummaxShow > 1)
			{
				m_ItemSpace = (nW - nNummaxShow*m_ItemWidth)/(nNummaxShow-1);
				m_rowDataShow = nNummaxShow;
				if(m_ItemSpace < ITEM_X_SPACE )
				{// �߾�̫С��Ҫ���¼���
					m_ItemSpace = (nW - (nNummaxShow-1)*m_ItemWidth)/(nNummaxShow-1);
					m_rowDataShow = nNummaxShow-1;
				}
			}
			else
			{
				m_rowDataShow = 1;
				m_ItemSpace = ITEM_X_SPACE;
			}
		}

		if(nOldShowNum!= m_rowDataShow)
		{ // Ҫ���¼��㿪ʼλ��
			if(m_IndexDisplay + nOldShowNum == nNumItem)
			{
			    m_IndexDisplay = m_IndexDisplay + nOldShowNum -  m_rowDataShow;
			}
			else if(m_IndexDisplay + nOldShowNum > nNumItem)
			{
				m_IndexDisplay = nNumItem -  m_rowDataShow;
			}
			else
			{
				m_IndexDisplay = m_IndexDisplay + nOldShowNum -  m_rowDataShow;
			}
		}
	}
	CWnd::OnSize(nType, cx, cy);
}

void CInfoWnd::OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized )
{
	CWnd::OnActivate(nState,pWndOther,bMinimized );
}

void CInfoWnd::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	if(bActive)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CInfoWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent ==0)
	   m_TimeWnd -= 1;
	switch(nIDEvent)
	{
	case 0:
		if(m_TimeWnd < 0){
			CloseDrawWnd();
			return;
		}
		break;
	case 1:	
		{
			CRect rcW;
			POINT point;
			GetWindowRect(&rcW);
			//ʵ���϶�ʱ��������ƶ�
			::GetCursorPos(&point);	
			if(point.x != m_ptCurPointLast.x||point.y != m_ptCurPointLast.y){
			   //?m_flist(2,m_lPARAM);//��ʼ�����ڱ�־
			   MoveWindow(point.x-m_ptCurPoint.x ,point.y-m_ptCurPoint.y ,rcW.right - rcW.left,rcW.bottom - rcW.top ,true); 
			   //?m_flist(1,m_lPARAM);
			}
		}
		break;
	case 2:	
		{
			CRect rcW;
			POINT point;
			GetWindowRect(&rcW);
    
			//ʵ���϶�ʱ��������ƶ�
			::GetCursorPos(&point);	
			{
				/*if(abs(m_lastPoint.x - point.x) < 2 && abs(m_lastPoint.y - point.y) < 3)
				{
					break;   // ������ 
				}*/

				if(point.y-rcW.top-m_ScrollRect.top < -1)
				{
					if(m_IndexDisplay >= m_rowDataShow)
					   m_IndexDisplay -= m_rowDataShow;
					else
						m_IndexDisplay = 0;
				}
				else if(point.y-rcW.top-m_ScrollRect.bottom > 1)
				{
					if(m_IndexDisplay <= m_vecStrContent.size() -1)
					{
						m_IndexDisplay+=m_rowDataShow;	
					}
				}
				else 
					return;

				if(m_IndexDisplay < 0)
					m_IndexDisplay = 0;
				if(m_IndexDisplay > m_vecStrContent.size() - 1)
					m_IndexDisplay = m_vecStrContent.size() - 1;

			/*	if(point.y-rcW.top-m_ScrollRect.top < -1)
					m_IndexDisplay-=m_rowDataShow;
				else if(point.y-rcW.top-m_ScrollRect.bottom > 1)
					m_IndexDisplay+=m_rowDataShow;
				else 
					return;
				if(m_IndexDisplay < 0)
					m_IndexDisplay = 0;
				if(m_IndexDisplay > m_vecStrContent.size() - m_rowDataShow)
					m_IndexDisplay = m_vecStrContent.size() - m_rowDataShow;*/

				m_lastPoint = point;
				this->Invalidate(FALSE);
			}
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CInfoWnd::CloseDrawWnd()
{
//?	m_flist(0,m_lPARAM);
	return;
	
}
