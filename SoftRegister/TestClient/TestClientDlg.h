
// TestClientDlg.h : ͷ�ļ�
//

#pragma once


// CTestClientDlg �Ի���
class CTestClientDlg : public CDialogEx
{
// ����
public:
	CTestClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	CString m_strLicenseData;

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
	afx_msg void OnBnClickedButtonCheckLicense();
	afx_msg void OnBnClickedButtonSaveKey();
	afx_msg void OnBnClickedButtonGetPcCode();
	afx_msg void OnBnClickedButtonGetLicense();
};
