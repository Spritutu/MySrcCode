// SvgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CugWorkSpace.h"
#include "SvgDlg.h"


// CSvgDlg �Ի���

IMPLEMENT_DYNCREATE(CSvgDlg, CDHtmlDialog)

CSvgDlg::CSvgDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CSvgDlg::IDD, 0, pParent)
{

}

CSvgDlg::~CSvgDlg()
{
}

void CSvgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CSvgDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	OpenDefaultPage();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CSvgDlg::OpenJianMoHuanJing()
{
	Navigate("F:/������̫/WorkPlatForm/Debug/PeraWorkspace/modelEditor/html/modelEditor.html");
}
void CSvgDlg::OpenDefaultPage()
{
	Navigate("F:/������̫/WorkPlatForm/Debug/PeraWorkSpace/Res/SvgDefaultPage/code.html");
}
void CSvgDlg::OpenWorkHistPage()
{
	Navigate("J:/program/Programing Tools/Adobe_photoshop_cs5_sdk_win/connectionsdk/samples/ios/firetutorial/resources/1.html");
}
BEGIN_MESSAGE_MAP(CSvgDlg, CDHtmlDialog)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CSvgDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CSvgDlg ��Ϣ�������

HRESULT CSvgDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	//OnOK();
	return S_OK;
}

HRESULT CSvgDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	//OnCancel();
	return S_OK;
}


void CSvgDlg::OnNcDestroy()
{
	CDHtmlDialog::OnNcDestroy();

	// TODO: �ڴ˴������Ϣ����������
	delete this;
}
