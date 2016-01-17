
// RePrintCallExeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <string>
#include "MyListBox.h"
using namespace std;

extern DWORD  g_ExeProcessID;
// CRePrintCallExeDlg �Ի���
class CRePrintCallExeDlg : public CDialog
{
// ����
public:
	CRePrintCallExeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REPRINTCALLEXE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	void CallExe(const char *pszCmd,bool bShowWnd = false);
// ʵ��
protected:
	HICON m_hIcon;
	string m_strLast; // ������¼������Ϣ���һ������������Ϣ�����棬�������´δ���


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ��ʾ����exe�����Ϣ�Ŀؼ�����
	CMyListBox m_ListBox;
	afx_msg void OnBnClickedBtnCallExe();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
