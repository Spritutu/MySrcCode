#pragma once
#include"CDDirTreeActiveX0.h"
#include "afxcmn.h"

// CDlgResLibTree �Ի���

class CDlgResLibTree : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResLibTree)

public:
	CDlgResLibTree(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgResLibTree();

// �Ի�������
	enum { IDD = IDD_JIANMO_COMPONENT_TREE };

	CDDirTreeActiveX0 m_ResLibTree;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//CTreeCtrl m_ResLibTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
