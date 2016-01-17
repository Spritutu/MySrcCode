
// TreeCtrlDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTreeCtrlDemoDlg �Ի���
class CTreeCtrlDemoDlg : public CDialog
{
// ����
public:
	CTreeCtrlDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TREECTRLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void InitTree();
	void SearchTree(HTREEITEM hParent,int iStrOffset);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDelSel();
	afx_msg void OnBnClickedButtonDelAll();
	afx_msg void OnBnClickedButtonResetTree();
	afx_msg void OnBnClickedButtonForeach();
	CTreeCtrl m_TreeCtrl;
	CListBox m_ListBox;
};
