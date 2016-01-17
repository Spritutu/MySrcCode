#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CSvgDlg �Ի���

class CSvgDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CSvgDlg)

public:
	CSvgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSvgDlg();
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	void OpenJianMoHuanJing();
	void OpenDefaultPage();
	void OpenWorkHistPage();

// �Ի�������
	enum { IDD = IDD_SVG_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnNcDestroy();
};
