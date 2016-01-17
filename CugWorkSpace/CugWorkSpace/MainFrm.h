// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include "MineToolBar.h"
#include"MineBCGPMenuBar.h"
class CMainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CBCGPStatusBar			m_wndStatusBar;
	CMineBCGPMenuBar		m_wndMenuBar;
	CMimeToolBar            m_MineToolbar;

	CBCGPToolBarImages	m_UserImages;

	void OpenJianMoHuanJing();
	void CreateMineToolBar();
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarReset(WPARAM,LPARAM);
	void OnToolsViewUserToolbar (UINT id);
	void OnUpdateToolsViewUserToolbar (CCmdUI* pCmdUI);
	afx_msg void OnWindowManager();
	afx_msg void OnAppLook(UINT id);
	afx_msg void OnUpdateAppLook(CCmdUI* pCmdUI);
	afx_msg void OnMdiMoveToNextGroup();
	afx_msg void OnMdiMoveToPrevGroup();
	afx_msg void OnMdiNewHorzTabGroup();
	afx_msg void OnMdiNewVertGroup();
	afx_msg void OnMdiCancel();
	afx_msg void OnClose();
	// �����Ƿ��򵼴�������Ϣ
public:
	afx_msg void OnNewShejiGuoCheng();  // �½���ƹ���
protected:
	afx_msg void OnOpenShejiGuoCheng(); // ����ƹ���
	afx_msg void OnMineToolbarClicked(UINT iID); // ��������ť�����Ӧ�¼�
	afx_msg void OnViewMineToolbar(); // �л��������Ƿ���ʾ
	afx_msg void OnUpdateViewMineToolbar(CCmdUI *pCmdUI); // �л���������ʾʱ���²˵�ǰ��ʾ�ĶԹ�
	afx_msg void OnToolHistoryMonitor(); // ����ҵ����ҳ��
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop);
	virtual CBCGPMDIChildWnd* CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/);
	UINT	m_nAppLook;
};


