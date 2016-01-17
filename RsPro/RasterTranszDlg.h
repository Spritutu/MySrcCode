#pragma once
#include "afxwin.h"


// CRasterTranszDlg �Ի���

class CRasterTranszDlg : public CDialog
{
	DECLARE_DYNAMIC(CRasterTranszDlg)

public:
	CRasterTranszDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRasterTranszDlg();

// �Ի�������
	enum { IDD = IDD_DLG_TRANSRASTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedOk();
	CString m_strInFile;
	CString m_strOutFile;
	CString m_strFormat;
	CEdit m_CtrEdit_Input;
	CEdit m_CtrEditOutPut;
};
