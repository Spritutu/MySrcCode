
// StaffManageDoc.h : CStaffManageDoc ��Ľӿ�
//


#pragma once
#include "StaffManageSet.h"


class CStaffManageDoc : public CDocument
{
protected: // �������л�����
	CStaffManageDoc();
	DECLARE_DYNCREATE(CStaffManageDoc)

// ����
public:
	CStaffManageSet m_StaffManageSet;

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();

// ʵ��
public:
	virtual ~CStaffManageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


