#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "mapcorectrl1.h"

// CWsMainDlg �Ի���

class CWsMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWsMainDlg)

public:
	CWsMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWsMainDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WS_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStatic m_StaticTitle;
	CStatic m_StaticZoomNum;
	CStatic m_WsContainer;
	CSliderCtrl m_SliderZoom;
	virtual BOOL OnInitDialog();
	CMapcorectrl1 m_MapCore;
};
