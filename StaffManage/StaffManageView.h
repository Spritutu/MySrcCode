
// StaffManageView.h : CStaffManageView ��Ľӿ�
//


#pragma once

class CStaffManageSet;

class CStaffManageView : public CRecordView
{
protected: // �������л�����
	CStaffManageView();
	DECLARE_DYNCREATE(CStaffManageView)

public:
	enum{ IDD = IDD_StaffManage_FORM };
	CStaffManageSet* m_pSet;

// ����
public:
	CStaffManageDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual CRecordset* OnGetRecordset();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void UpdateCtrlReadOnlyState(BOOL bEnable);

// ʵ��
public:
	virtual ~CStaffManageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddreco();
	afx_msg void OnBnClickedRecodele();
	afx_msg void OnBnClickedSortreco();
	afx_msg void OnBnClickedFilterreco();
	afx_msg void OnBnClickedSetPhoto();
protected:
	virtual void OnDraw(CDC* /*pDC*/);
	void ClearPhotoView();
private:
	HBITMAP			m_hPhotoBitmap;
	bool			m_bClearPicture;
public:
	afx_msg void OnBnClickedRecoModify();
};

#ifndef _DEBUG  // StaffManageView.cpp �еĵ��԰汾
inline CStaffManageDoc* CStaffManageView::GetDocument() const
   { return reinterpret_cast<CStaffManageDoc*>(m_pDocument); }
#endif

