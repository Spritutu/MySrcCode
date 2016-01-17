//{{AFX_INCLUDES()
#include "webbrowser2.h"
//}}AFX_INCLUDES
#if !defined(AFX_VIDEOCTRLDLG_H__2A1B0C10_4F4E_4C80_BE57_A389FC4CBD29__INCLUDED_)
#define AFX_VIDEOCTRLDLG_H__2A1B0C10_4F4E_4C80_BE57_A389FC4CBD29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoCtrlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVideoCtrlDlg dialog
#include "SkinBtn.h"
class CMeYeDlg;
class CVideoCtrlDlg : public CDialog
{
// Construction
public:
	CVideoCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	void SetMainCall(CMeYeDlg *pWnd)
	{
		m_pMainDlg = pWnd;
	}


	void SetIEshow(BOOL bShow)
	{
		m_WebCtrl.ShowWindow(bShow);
	}


	void SetUrl(CString strurl)
	{

	//?	CString* msg = new CString(strurl);

		SendMessage(OPEN_MSG_ID_URL,(LPARAM)strurl.GetBuffer(0),0);
//		AfxMessageBox("SetUrl1");
	//	m_WebCtrl.Navigate( strurl, NULL, NULL, NULL, NULL );
		//m_WebCtrl->stop();
      //  AfxMessageBox("SetUrl2");
		//m_WebCtrl.PutVisible(TRUE);
		//m_WebCtrl.Refresh();
	}

	CString  GetSataFlag()
	{

		if(m_iestat==0)
			return "";

        
        return m_strIeGet;
	}


	void SetConWebtext(CString strall)
	{

		/*IHTMLDocument2 *pDoc = (IHTMLDocument2 *)m_WebCtrl.GetDocument(); 
	    if ( !pDoc ) return;

		IHTMLElement *pElement, *pElement1;
		HRESULT hr = pDoc->get_body( &pElement );
		if ( hr != S_OK ) return;



		 if(pElement!=NULL)   
		  {      
			   BSTR pbBody = strall.AllocSysString();
			   hr=pElement->put_innerHTML(pbBody);   //���ƵĻ���put_innerTEXT
			   pElement->Release();   
		  }      

		 pDoc->Release(); 
	//	m_WebCtrl.Navigate( strurl, NULL, NULL, NULL, NULL );

		//m_WebCtrl.PutVisible(TRUE);
		//m_WebCtrl.Refresh();*/
	}
	



	
	

private:
	CMeYeDlg * m_pMainDlg;
// Dialog Data
	//{{AFX_DATA(CVideoCtrlDlg)
	enum { IDD = IDD_DIALOG_VIDEO_CTRL };
	CStatic	m_VideoShowCtrl;
	CSkinBtn	m_BtnStop;
	CSkinBtn	m_BtnPlay;
	CSkinBtn	m_BtnBack;
	CSkinBtn	m_BtnNext;
	CWebBrowser2	m_WebCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVideoCtrlDlg)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	
//   	virtual   BOOL PreTranslateMessage(MSG *pMsg) ;


private:
	void SetBackImage(int index,CString strName);
    void DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	void SetImageSize(CBitmap *pBitmap, CSize &sz);
	void SetImage(CBitmap &bitmap, CString strName);


	afx_msg LRESULT OnOpenUrl(WPARAM wParam, LPARAM lParam);

	void SetSteep(long nStep);//������Ƶ���
private:

	CBitmap m_BitmapBack[8];  //1��Ƶ����,2��SCRoll�Ѿ����صĽ���,3���Ѿ����Ľ���,4�ǰ�椱���,5��ʼ��6��ͣ,7�ǿ����8�ǿ���
	CSize   m_szBitmapBack[8];






//SRCOLL

	CRect  m_ScrollRect;
	CRect  m_ScrollBtnRect;
	int    m_btnStatScroll;
	int    m_btnHeightScroll;//������椸߶�,�൱��ʱ�����,��ʱ�����������m_vecStrContent���Ƶ�
	int    m_IndexDisplay;   //��ǰ��ʾ�ڼ���
	int    m_rowDataShow;  // һ�п�����ʾ������ͼ�����������,Ĭ��Ϊ1���˲����ڱ�������Ҫ�޸�

	POINT   m_lastPoint;
	long    m_MouSelItem;
	long    m_LBtnDownItem;
	long    m_LDClickItem;

	CPoint m_ptCurPoint;
	CPoint m_ptCurPointLast;
	CRect  m_rcCurRect,m_ScrollRectCal;

	CString m_strStatusText;

	int m_iestat;
	CString m_strIeGet;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOCTRLDLG_H__2A1B0C10_4F4E_4C80_BE57_A389FC4CBD29__INCLUDED_)
