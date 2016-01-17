// DocViewCorePropPage.cpp : CDocViewCorePropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "DocViewCore.h"
#include "DocViewCorePropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CDocViewCorePropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CDocViewCorePropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CDocViewCorePropPage, "DOCVIEWCORE.DocViewCorePropPage.1",
	0xcf260641, 0x2390, 0x4615, 0xae, 0x53, 0x60, 0x45, 0x76, 0xe7, 0xc4, 0x41)



// CDocViewCorePropPage::CDocViewCorePropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CDocViewCorePropPage ��ϵͳע�����

BOOL CDocViewCorePropPage::CDocViewCorePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_DOCVIEWCORE_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CDocViewCorePropPage::CDocViewCorePropPage - ���캯��

CDocViewCorePropPage::CDocViewCorePropPage() :
	COlePropertyPage(IDD, IDS_DOCVIEWCORE_PPG_CAPTION)
{
}



// CDocViewCorePropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CDocViewCorePropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CDocViewCorePropPage ��Ϣ�������
