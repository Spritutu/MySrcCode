
// RePrintToWndTestDlg.h : ͷ�ļ�
//

#pragma once

#include <string>
#include "afxwin.h"
#include "MyListBox.h"
using namespace std;
// CRePrintToWndTestDlg �Ի���

extern DWORD g_ExeProcessId;
class CRePrintToWndTestDlg : public CDialog
{
// ����
public:
	CRePrintToWndTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REPRINTTOWNDTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
private:
	void CallExe(const char * pszCmd,bool bShowWnd = false);
private:
	string  m_strCallMsg;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSubThreadMsg(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	CMyListBox m_ListBox;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStart2();
	afx_msg void OnBnClickedBtnStart3();
};
