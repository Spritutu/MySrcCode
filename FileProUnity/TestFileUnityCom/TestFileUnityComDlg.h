
// TestFileUnityComDlg.h : ͷ�ļ�
//

#pragma once


// CTestFileUnityComDlg �Ի���
class CTestFileUnityComDlg : public CDialogEx
{
// ����
public:
	CTestFileUnityComDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTFILEUNITYCOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenFileDlg();
	afx_msg void OnBtnSelectDir();
	afx_msg void OnBnClickedBtnFileInfo();
	afx_msg void OnBnClickedBtnDeletefile();
	afx_msg void OnBnClickedBtnGetMd5();
	afx_msg void OnBnClickedBtnOpenFile();
	afx_msg void OnBnClickedBtnCopyfile();
	CString m_StrCopyInfo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
