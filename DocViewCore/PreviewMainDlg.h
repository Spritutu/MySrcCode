#pragma once


// CPreviewMainDlg �Ի���

class CPreviewMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPreviewMainDlg)

public:
	CPreviewMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPreviewMainDlg();
	BOOL PreviewDocWithHandle(const CString & strDocFile,HWND hwnd,const CRect & rect);

// �Ի�������
	enum { IDD = IDD_DIALOG_PREVIEW_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	// ʹ��Preview Handle Ԥ���ĵ�
	IPreviewHandler *m_pIP ;
	IInitializeWithFile *m_pIFile ;
	IInitializeWithStream *m_pIStream ;
	HGLOBAL m_hGlobal;
	IStream * m_pStream;	
	CString GetClsidFromExtn( char * szExtn );
};
