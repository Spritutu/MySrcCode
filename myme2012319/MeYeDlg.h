// MeYeDlg.h : header file
//

#if !defined(AFX_MEYEDLG_H__B2C678B2_7F75_46DD_86C3_3173B2AF2277__INCLUDED_)
#define AFX_MEYEDLG_H__B2C678B2_7F75_46DD_86C3_3173B2AF2277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMeYeDlg dialog
#include"MyBtn.h"
#include"SkinBtn.h"
#include "RealVideoDlg.h"
#include "RecoderDlg.h"
#include "SConfigDlg.h"
#include "VideoCtrlDlg.h"
#include "ClientConfigDlg.h"
#include "LeftClientDlg.h"
#include "PriceUpDlg.h"


class CMeYeDlg : public CDialog
{
// Construction
public:

	void DCickVideoGridListCtrl(CString temp);
public:




	CString m_strStatus;   // ¼�񻺳���ʾ�ַ�
	CString m_strloginStatus;   // ��½״̬��ʾ
	CMeYeDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL    b_get_mi;


	CString	m_strTimeInfo;   //��ǰ��Ƶ���ŵ�״̬����
	BOOL b_seeking;          //�Ƿ����϶���Ƶ���ŵı�־

    char login_user[256];
	char login_pwd[256];
	char m_spid[256];   //��DLL���ڴ�







	BOOL  m_nCMNET; //���ֻ������������õ��Խ���
	long    m_cam_id; //��ǰ���ڲ��ŵ���ƵID

	CVideoCtrlDlg*  m_pVideoCtrlDlg;//IE����1
	CVideoCtrlDlg*  m_pBjCtrlDlg;//IE����2
// Dialog Data
	//{{AFX_DATA(CMeYeDlg)
	enum { IDD = IDD_MEYE_DIALOG };
	CSkinBtn	m_BtnSearch;
	CSkinBtn	m_ExitBtn;
	CSkinBtn	m_MainSConfigBtn;
	CSkinBtn	m_MainRealBtn;
	CSkinBtn	m_MainHistoryBtn;
	CSkinBtn	m_MainCConfogBtn;
	CMyBtn	m_ServerConf;
	CMyBtn	m_HistoryVideo;
	CMyBtn	m_Real;
	CMyBtn	m_ClientConf;
	CSkinBtn	m_CloseBtn;
	CSkinBtn	m_MinBtn;
	CSkinBtn	m_ConfBtn;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeYeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CString TimeToString(__int64 llTime);

	void SetBackImage(int index,CString strName);
	void DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	void SetImageSize(CBitmap *pBitmap, CSize &sz);
    //����Ӳ���ļ�λͼ
    void SetImage(CBitmap &bitmap, CString strName);

	// ����͸����
	void SetLayeredWindowTrans(int nTrans);

	void ChangeWindowRgn(CDC *pDC);	//������������

	void OnChangeSelDlg(void);

   
	afx_msg LRESULT OnItemChange(WPARAM wParam, LPARAM lParam); //��������б�����ı�ʱ

	//��½ʧ��
	void LoginExit(void);
	//��½�ɹ�,�Զ�������Ƶ
	void LoginViewVideo(void);
	
	//�鵽��Ƶ
	void SetVideoInput(int sel,int count);

	//��½ʧ�ܺ����µ�½
	void loginagain(void);
private:
	CBitmap m_BitmapBack[3];
	CSize   m_szBitmapBack[3];

	HBRUSH m_brushBkrectMid;
    HBRUSH m_brushBkrect;

	long     _icur;        // ��ǰMOUSE״̬

//	CRealVideoDlg* m_pRealRightDlg;  //ʵʱ�Ҵ���
//	CRecoderDlg*    m_pRecoderDlg;  //��Ƶ�ط��Ҵ���
	CSConfigDlg*    m_pSConfigDlg;  //���������ô���
	CClientConfigDlg* m_pCConfigDlg;//�ն����ô���

	CLeftClientDlg*   m_pLeftDlg;//΢���Զ�������ߴ���

    CPriceUpDlg*      m_pLeftPriceDlg;//�۸����󴰿�
	

	long      m_CurSel;
	char      m_FilePath[255]; 

	CString m_strVersion;
	CString m_strscroll;
	long    m_scrollLen;
	long    m_scrollpos;


	int m_ucSel,m_ucNum,m_ucUser,m_ucChan;
	


	long    m_bloging_index;
	BOOL    m_bloging_falg;

	

protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMeYeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonMin();
	afx_msg void OnButtonMainRal();
	afx_msg void OnButtonMainHistory();
	afx_msg void OnButtonMainCConfig();
	afx_msg void OnButtonMainSConfig();
	afx_msg void OnButtonMainSExit();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnOK() ;
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonConf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	CString GetGbkToUrlEncode(CString keyWord);
//    virtual   BOOL PreTranslateMessage(MSG *pMsg) ;

    BOOL DownXmlVer(void );
	CString  GetCurrentApPath();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEYEDLG_H__B2C678B2_7F75_46DD_86C3_3173B2AF2277__INCLUDED_)
