#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CDocViewDlg �Ի���
#include "pdf\CAcroAXDocShim.h"
#include "pdf\CRadaeePDFDocument.h"
#include "pdf\CDPdfview.h"

class CDocViewDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDocViewDlg)
public:
	typedef enum
	{
		NONE_PDF = 0,
		Adobe_PDF = 1,
		RadaeePDFDll_PDF = 2,
		PdfviewOcx_PDF = 3,
		CajOcx_PDF = 4
	}PDFActiveXType; // pdf �������

public:
	CDocViewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDocViewDlg();
	BOOL OpenDocFile(CString strUrl);
// ��д
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

	BOOL IsWordFile(const CString &strFile);
	BOOL ConvertWordToHtml(CString strWordFile,CString strHtml);
	void RemoveTempHtmlFile();
	BOOL UpdateHtmlToActivexSafe(CString strHtml);
	// ȡ���Ҽ��˵�
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

// �Ի�������
	enum { IDD = IDD_DOCVIEWDLG};
protected:
	CAcroAXDocShim m_drvPDF;
	CRadaeePDFDocument *m_pRadaeePdfDoc;
	CDPdfview m_drvViewPDF;
	CWnd           m_wndPDF;
	CString m_strLastTmpFile;
	PDFActiveXType m_ActiveXType;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL IsExternalDispatchSafe();
	BOOL TestCreatePdfViewCtrl();
	BOOL TestCreateAdobeCtrl();
	BOOL TestCreateRadaeePDFDllCtrl();
	BOOL TestCreatePdfviewOcxCtrl();
	BOOL TestCreateCAjCtrl();
	BOOL TestOpenPdf(CString strPdfFile);

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
