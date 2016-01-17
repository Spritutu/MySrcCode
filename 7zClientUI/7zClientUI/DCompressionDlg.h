#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDCompression �Ի���

class CDCompressionDlgPage : public CDialogEx
{
	DECLARE_DYNAMIC(CDCompressionDlgPage)

public:
	CDCompressionDlgPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDCompressionDlgPage();

// �Ի�������
	enum { IDD = IDD_DLG_DCOMPRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen7zfile();
	afx_msg void OnBnClickedBtnExtractTo();
	LRESULT OnMsgDCompressFinish(WPARAM wp,LPARAM);
	CListBox m_FileList;
	CProgressCtrl m_bProgressBar;
	CString m_strIn7zFile;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
