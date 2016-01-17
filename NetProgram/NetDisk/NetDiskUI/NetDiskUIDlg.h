
// NetDiskUIDlg.h : ͷ�ļ�
//

#pragma once
#include "skinbtn.h"
#include "PageNetDisk.h"
#include "PageXiangCe.h"

// CNetDiskUIDlg �Ի���
class CNetDiskUIDlg : public CDialogEx
{
// ����
public:
	CNetDiskUIDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CNetDiskUIDlg();

// �Ի�������
	enum { IDD = IDD_NETDISKUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonNetdisk();
	afx_msg void OnBnClickedButtonXiangce();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnMin();
	afx_msg void OnBnClickedBtbSetting();
	// ��������Ҫ�ػ�İ�ť
	CSkinBtn m_btnNetDisk;
	CSkinBtn m_btnXiangCe;
	CSkinBtn m_btnBaoXianXiang;
	CSkinBtn m_btnPhone;
	CSkinBtn m_btnFuncAll;
	CSkinBtn m_btnSetting;
	CSkinBtn m_btnMin;
	CSkinBtn m_btnClose;
	CPageNetDisk *m_pPageNetDisk;
	CPageXiangCe *m_pPageXiangce;
	BOOL m_bLogOut;
public:
	// ���ñ���ͼƬ
	void SetBackImage();
	void SetImage(CBitmap &bitmap, CString strName);
	void DrawImage(CBitmap &bitmap,CDC *pDC,CRect rect);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	void AdjustLocation();
	void DrawUsrInfo(CDC *pDC);
	void DrawStatusBar(CDC *pDC);
	void RefreshStatusBar();
private:
	CBitmap m_BitmapHeader;
	CBitmap m_BitmapStatus;
	HBRUSH m_brushBkrectMid;
	HBRUSH m_brushBkrect;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMenuSettingLogout();
	afx_msg void OnMenuSettingQuit();
	afx_msg LRESULT OnStatusChange(WPARAM,LPARAM);
};
