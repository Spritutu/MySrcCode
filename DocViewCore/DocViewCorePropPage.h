#pragma once

// DocViewCorePropPage.h : CDocViewCorePropPage ����ҳ���������


// CDocViewCorePropPage : �й�ʵ�ֵ���Ϣ������� DocViewCorePropPage.cpp��

class CDocViewCorePropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CDocViewCorePropPage)
	DECLARE_OLECREATE_EX(CDocViewCorePropPage)

// ���캯��
public:
	CDocViewCorePropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_DOCVIEWCORE };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

