#pragma once
#include "DocViewDlg.h"
#include "PreviewMainDlg.h"
// DocViewCoreCtrl.h : CDocViewCoreCtrl ActiveX �ؼ����������


// CDocViewCoreCtrl : �й�ʵ�ֵ���Ϣ������� DocViewCoreCtrl.cpp��

class CDocViewCoreCtrl : public COleControl
{
	DECLARE_DYNCREATE(CDocViewCoreCtrl)
// ���캯��
public:
	CDocViewCoreCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CDocViewCoreCtrl();

	DECLARE_OLECREATE_EX(CDocViewCoreCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CDocViewCoreCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CDocViewCoreCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CDocViewCoreCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
		dispidDocumentOpened = 1L,
		dispidSelectFile = 2L,
		dispidOpenDocFile = 1L
	};
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HRESULT OnDocOpened(WPARAM,LPARAM);

	CDocViewDlg m_ViewDlg;
	CPreviewMainDlg m_PreviewDlg;
protected: // ���Ƚӿ�
	VARIANT_BOOL OpenDocFile(LPCTSTR strDocFile);
	BSTR SelectFile(void);
protected: // ֪ͨ�¼�
	void DocumentOpened(LPCTSTR strDocPath, LPCTSTR strDocTitle);
public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

