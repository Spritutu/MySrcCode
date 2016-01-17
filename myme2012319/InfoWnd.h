// InfoWnd.h: interface for the CInfoWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOWND_H__298486A5_CB7D_48A1_BA94_3354A78A86F5__INCLUDED_)
#define AFX_INFOWND_H__298486A5_CB7D_48A1_BA94_3354A78A86F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000






typedef BOOL (WINAPI *fList_FireStar)(int,_tagLpWnd);//�ص��ӿڴ��ر���Ϣ��������
class CInfoWnd : public CWnd  
{
public:
	CInfoWnd();
	virtual ~CInfoWnd();
public:
	BOOL   Create(HWND parhnd);
public:
	void SetWndCaption (CString str);//���ô��ڱ���

    void AdditemData(Productors* pData);//���ô�����������

    void SetCloseTime(int nTime);//���ô���ʧȥ�����Ĺر�ʱ�䣻-1Ϊһֱ��ʾ

	BOOL SetEventUserList(fList_FireStar flist,_tagLpWnd lPARAM){
//?		m_flist = flist;
		m_lPARAM = lPARAM;
		return TRUE;
	}

	void KillTimerButton();
private:
	void DrawRangeImage(CBitmap* pBmp , CDC *pDC,CRect rcPic,CRect rcBmp);
protected:
//?	fList_FireStar m_flist;
public:
	_tagLpWnd        m_lPARAM;
private:
	CString m_strTitle;
	CString m_strContent;
	int     m_TimeWnd;
	
	CPoint m_ptCurPoint;
	CPoint m_ptCurPointLast;
	CRect  m_rcCurRect;

	CRect  m_BtnRectClose;
	int    m_btnStatClose;

	CRect  m_BtnRectPos;
	int    m_btnStatPos;
	BOOL   m_bBtnTimer;

	CRect  m_ScrollRect;
	int    m_btnStatScroll;
	int    m_btnHeightScroll;//������椸߶�

	int    m_IndexDisplay;
	int    m_rowDataShow;  // һ�п�����ʾ������ͼ�����������
	int    m_ItemWidth;    // �����ʾ���
	int    m_ItemSpace;    // �����ʾ���

	CBitmap m_Scrollback;
	CBitmap m_Btnback;

	POINT   m_lastPoint;
	long    m_MouSelItem;
	long    m_LBtnDownItem;
	long    m_LDClickItem;
//	CBitmap m_BitmapUpNormal,m_BitmapCenterNormal,m_BitmapDownNormal,m_Scrollback;
//	CBitmap m_BitmapUpOver,m_BitmapCenterOver,m_BitmapDownOver;
private:
	void CloseDrawWnd();
protected:
	//{{AFX_MSG(CInfoWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnActivate(UINT nState,CWnd* pWndOther,BOOL bMinimized );
	afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
	afx_msg void OnTimer(UINT nIDEvent) ;
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    void MouseSelect( const CPoint & point);
	BOOL MouseLDown( const CPoint & point );
};

#endif // !defined(AFX_INFOWND_H__298486A5_CB7D_48A1_BA94_3354A78A86F5__INCLUDED_)
