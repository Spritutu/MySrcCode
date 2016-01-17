#pragma once

#include "SkinBtn.h"
#include "afxwin.h"

// CPageRegister �Ի���

class CPageRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CPageRegister)

public:
	CPageRegister(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageRegister();

// �Ի�������
	enum { IDD = IDD_DLG_PAGE_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CSkinBtn m_btnRegister;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRegister();

	CEdit m_EditUsrID;
	CEdit m_EditUsrName;
	CEdit m_EditPassWd;
	CEdit m_EditPassWd2;

	CString m_strValUsrID;
	CString m_strValUsrName;
	CString m_strValPassWd;
	CString m_strValPassWd2;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
