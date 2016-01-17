// DocViewCoreCtrl.cpp : CDocViewCoreCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "DocViewCore.h"
#include "DocViewCoreCtrl.h"
#include "DocViewCorePropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CString GetDocTitle(CString strDoc);

IMPLEMENT_DYNCREATE(CDocViewCoreCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CDocViewCoreCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_DOC_OPENED,OnDocOpened)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CDocViewCoreCtrl, COleControl)
	DISP_FUNCTION_ID(CDocViewCoreCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CDocViewCoreCtrl, "OpenDocFile", dispidOpenDocFile, OpenDocFile, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION_ID(CDocViewCoreCtrl, "SelectFile", dispidSelectFile, SelectFile, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CDocViewCoreCtrl, "DocumentOpened", dispidDocumentOpened, DocumentOpened, VT_EMPTY, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CDocViewCoreCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CDocViewCoreCtrl, 1)
	PROPPAGEID(CDocViewCorePropPage::guid)
END_PROPPAGEIDS(CDocViewCoreCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CDocViewCoreCtrl, "DOCVIEWCORE.DocViewCoreCtrl.1",
	0x2f519fba, 0x91e5, 0x439e, 0x8c, 0x46, 0x93, 0x18, 0xec, 0x43, 0xa7, 0xed)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CDocViewCoreCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DDocViewCore = { 0xB890C406, 0xEAD1, 0x44FF, { 0xAF, 0x3B, 0x4F, 0xD9, 0x69, 0xC7, 0x67, 0xF0 } };
const IID IID_DDocViewCoreEvents = { 0x13A00996, 0xF84A, 0x4DD7, { 0x9E, 0x74, 0x2, 0xF4, 0x2E, 0x17, 0x69, 0xC5 } };


// �ؼ�������Ϣ

static const DWORD _dwDocViewCoreOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CDocViewCoreCtrl, IDS_DOCVIEWCORE, _dwDocViewCoreOleMisc)



// CDocViewCoreCtrl::CDocViewCoreCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CDocViewCoreCtrl ��ϵͳע�����

BOOL CDocViewCoreCtrl::CDocViewCoreCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_DOCVIEWCORE,
			IDB_DOCVIEWCORE,
			afxRegApartmentThreading,
			_dwDocViewCoreOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CDocViewCoreCtrl::CDocViewCoreCtrl - ���캯��

CDocViewCoreCtrl::CDocViewCoreCtrl()
{
	InitializeIIDs(&IID_DDocViewCore, &IID_DDocViewCoreEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
}



// CDocViewCoreCtrl::~CDocViewCoreCtrl - ��������

CDocViewCoreCtrl::~CDocViewCoreCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CDocViewCoreCtrl::OnDraw - ��ͼ����

void CDocViewCoreCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(LTGRAY_BRUSH)));
	//pdc->Ellipse(rcBounds);
}



// CDocViewCoreCtrl::DoPropExchange - �־���֧��

void CDocViewCoreCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CDocViewCoreCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CDocViewCoreCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CDocViewCoreCtrl::AboutBox - ���û���ʾ�����ڡ���

void CDocViewCoreCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_DOCVIEWCORE);
	dlgAbout.DoModal();
}



// CDocViewCoreCtrl ��Ϣ�������


void CDocViewCoreCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_ViewDlg.m_hWnd)
	{
		m_ViewDlg.MoveWindow(1,1,cx-1,cy-1);
	}
	if (m_PreviewDlg.m_hWnd)
	{
		m_PreviewDlg.MoveWindow(1,1,cx-1,cy-1);
	}
	DragAcceptFiles(TRUE); //������ק�ļ�����ͼ
}


int CDocViewCoreCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	OnActivateInPlace (TRUE, NULL); // == UI-Activate the control
	// TODO:  �ڴ������ר�õĴ�������
	if (m_ViewDlg.m_hWnd == NULL)
	{
		m_ViewDlg.Create(m_ViewDlg.IDD,this);
		m_ViewDlg.ShowWindow(SW_SHOW);
	}
	if (m_PreviewDlg.m_hWnd == NULL)
	{
		m_PreviewDlg.Create(m_PreviewDlg.IDD,this);
		m_PreviewDlg.ShowWindow(SW_SHOW);
	}
	return 0;
}


VARIANT_BOOL CDocViewCoreCtrl::OpenDocFile(LPCTSTR strDocFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ���ӵ��ȴ���������
	//BOOL
	CRect rc;
	m_PreviewDlg.GetClientRect(&rc);
	rc.DeflateRect(1,1,1,1);
	BOOL bOK = m_PreviewDlg.PreviewDocWithHandle(strDocFile,m_PreviewDlg.GetSafeHwnd(),rc);
	if (bOK)
	{
		m_ViewDlg.ShowWindow(SW_HIDE);
		m_PreviewDlg.ShowWindow(SW_SHOW);
		Invalidate(TRUE);
		DocDesc *pdoc = new DocDesc;
		pdoc->strPath = strDocFile;
		pdoc->strTitle = GetDocTitle(strDocFile);
		PostMessage(WM_DOC_OPENED,0,(LPARAM)pdoc);
		return VARIANT_TRUE;
	}

	 bOK = m_ViewDlg.OpenDocFile(strDocFile);
	 if (bOK)
	 {
		 m_PreviewDlg.ShowWindow(SW_HIDE);
		 m_ViewDlg.ShowWindow(SW_SHOW);
		 Invalidate(TRUE);
		 return VARIANT_TRUE;
	 }

	CString strErrMsg;
	strErrMsg.Format("���ļ�[%s]ʧ��!",strDocFile);
	MessageBox(strErrMsg,"����",MB_OK|MB_ICONERROR);
	return VARIANT_FALSE;
}


BSTR CDocViewCoreCtrl::SelectFile(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	CFileDialog dlg(TRUE,".docx",NULL,6UL,"���и�ʽ(*.*)|*.*|\
										  ֧�ֵĸ�ʽ|*.doc;*.docx;*.pdf;*.kdh;*.caj;*.htm;*.html;*.xml;*.txt;*.bmp;*.jpg;*.png|\
										  Word �ĵ�(*.doc;*.docx)|*.doc;*.docx|\
										  PDF�ĵ�(*.pdf)|*.pdf|\
										  ��ҳ�ļ�(*.html;*.html)|*.html;*.html|\
										  ͼƬ�ļ�(*.jpg;*.bmp)|*.jpg;*.bmp|\
										  �ı��ļ�(*.txt)|*.txt|\
										  XML�ļ�(*.xml)|*.xml|\
										  ���ĸ�ʽ(*.kdh;*.caj)|*.kdh;*.caj||",this);

	if (dlg.DoModal()==IDOK)
	   strResult = dlg.GetPathName();

	return strResult.AllocSysString();
}


void CDocViewCoreCtrl::DocumentOpened(LPCTSTR strDocPath, LPCTSTR strDocTitle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	FireEvent(dispidDocumentOpened,EVENT_PARAM(VTS_BSTR VTS_BSTR),strDocPath,strDocTitle);
}
HRESULT CDocViewCoreCtrl::OnDocOpened(WPARAM,LPARAM lData)
{
	DocDesc *pDesc = (DocDesc*)lData;
	DocumentOpened(pDesc->strPath,pDesc->strTitle);
	delete pDesc;
	return S_OK;
}

int CDocViewCoreCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!m_bUIActive)
		OnActivateInPlace (TRUE, NULL); // == UI-Activate the control
	return COleControl::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


BOOL CDocViewCoreCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_ViewDlg.m_hWnd)
	{
		::SendMessage(m_ViewDlg.m_hWnd,pMsg->message,pMsg->wParam,pMsg->lParam);
		return TRUE;
	}
	
	//switch (pMsg->message)
	//{
	//case WM_KEYDOWN:
	//case WM_KEYUP:
	//	switch (pMsg->wParam)
	//	{
	//	case VK_UP:
	//	case VK_DOWN:
	//	case VK_LEFT:
	//	case VK_RIGHT:
	//	case VK_HOME:
	//	case VK_END:
	//		SendMessage (pMsg->message, pMsg->wParam, pMsg->lParam);
	//		// Windowless controls won't be able to call SendMessage.
	//		// Instead, just respond to the message here.
	//		return TRUE;
	//	}
	//	break;
	//}
	return COleControl::PreTranslateMessage(pMsg);
}


void CDocViewCoreCtrl::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int num = DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);
	if (num <=0)
		return;
	char szPath[MAX_PATH];
	bool bOK = false;
	for (int i=0;i<num;i++)
	{
		DragQueryFile(hDropInfo,i,szPath,MAX_PATH);
		CString strFile = szPath;
		if (m_ViewDlg.OpenDocFile(strFile))
		{
			bOK = true;
			break;
		}
	}
	if (!bOK)
		MessageBox(_T("ָ�����ļ����ܴ򿪣�"),_T("��ʾ"),MB_OK|MB_ICONINFORMATION);
	DragFinish(hDropInfo);
	COleControl::OnDropFiles(hDropInfo);
}
