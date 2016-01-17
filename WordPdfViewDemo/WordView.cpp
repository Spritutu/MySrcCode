
// WordPdfViewDemoView.cpp : CWordPdfViewDemoView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "WordPdfViewDemo.h"
#endif

#include "WordDoc.h"
#include "WordView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "./word/CWordApplication.h"
#include "./word/CDocuments.h"
#include "./word/CDocument0.h"

BOOL DeleteDir(CString strDir)
{
	CString strWildCard  = strDir;
	strWildCard +="\\*.*";
	CFileFind finder;
	BOOL bFind = FALSE;
	// �����ļ�
	bFind = finder.FindFile(strWildCard);
	while(bFind)
	{
		// ������һ���ļ�
		bFind = finder.FindNextFileA();
		if (finder.IsDots())
			continue;

		//�ҵ��ļ���·��
		CString strPathName = finder.GetFilePath();
		if (finder.IsDirectory())
		{
			DeleteDir(strPathName);
		}
		else
		{
			if (!::DeleteFile(strPathName))
			{
				return FALSE;
			}
		}
	}
	finder.Close();
	if (!::RemoveDirectory(strDir))
		return FALSE;

	return TRUE;
}
// CWordPdfViewDemoView

IMPLEMENT_DYNCREATE(CWordView, CView)

BEGIN_MESSAGE_MAP(CWordView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWordView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_COMMAND(ID_OPEN_WORD_DOC, &CWordView::OnOpenWordDoc)
END_MESSAGE_MAP()

// CWordPdfViewDemoView ����/����

CWordView::CWordView()
{
	// TODO: �ڴ˴���ӹ������

}

CWordView::~CWordView()
{
	RemoveTempHtmlFile();
}

BOOL CWordView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CWordPdfViewDemoView ����

void CWordView::OnDraw(CDC* /*pDC*/)
{
	CWordDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CWordPdfViewDemoView ��ӡ


void CWordView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWordView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CWordView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CWordView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CWordView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWordView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWordPdfViewDemoView ���

#ifdef _DEBUG
void CWordView::AssertValid() const
{
	CView::AssertValid();
}

void CWordView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWordDoc* CWordView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWordDoc)));
	return (CWordDoc*)m_pDocument;
}
#endif //_DEBUG


// CWordPdfViewDemoView ��Ϣ�������


void CWordView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	if (m_web.m_hWnd == NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		m_web.Create(NULL,WS_CHILD|WS_VISIBLE,rect,this,ID_WEBBROWSER_DOC);
	}
	
	CString strWord = GetDocument()->GetPathName();
	if (!strWord.IsEmpty())
	{
		OpenDocFile(strWord);
	}
}

void CWordView::OpenDocFile(CString strFile)
{
	if (m_web.m_hWnd)
	{
		RemoveTempHtmlFile();
		CString strHtml = strFile;
		if (IsWordFile(strFile))
		{
			strHtml = strFile.Left(strFile.ReverseFind('.'))+ ".html";
			char szTmpBuf[256] = {0};
			char szFileName[256] = {0};
			::GetTempPath(256,szTmpBuf);
			CString strTmp = szTmpBuf;
			::GetTempFileName(szTmpBuf,"~htm",TRUE,szFileName);
			strHtml = szFileName;
			BOOL bOk = ConvertWordToHtml(strFile,strHtml);
			if (!bOk)
			{
				MessageBox("�޷��򿪸��ĵ�","��ʾ",MB_OK|MB_ICONERROR);
				return;
			}
			m_strLastTmpFile = strHtml;
		}
		m_web.Navigate(strHtml,NULL,NULL,NULL,NULL);
		GetDocument()->SetTitle(strFile);
	}
}
BOOL CWordView::IsWordFile(const CString &strFile)
{
	CString strTmp = strFile;
	strTmp.MakeLower();
	if (strFile.Right(3)=="doc"|| strFile.Right(4)=="docx")
		return TRUE;

	return FALSE;
}
BOOL CWordView::ConvertWordToHtml(CString strWordFile,CString strHtml)
{
	CWordApplication app;
	if (!app.CreateDispatch("Word.Application",NULL))
		return FALSE;

	CDocuments docxs = app.get_Documents();
	CComVariant FileName =  strWordFile;
	CComVariant passWd = "";
	CComVariant templateDoc = "";
	CComVariant wrietPassWdDoc = "";
	CComVariant wriePassWdtemplateDoc = "";
	CComVariant xmltrans = "";
	CComVariant vBoolTrue,vBoolFalse;
	vBoolTrue  = TRUE;
	vBoolFalse = FALSE;
	CComVariant vFormat = 0;
	CComVariant vEcode  = 0;
	CComVariant vDirection  = 0;
	app.put_Visible(FALSE);
	// ��Word �ĵ�
	CDocument0 docx = docxs.Open(&FileName,&vBoolFalse,&vBoolTrue,&vBoolTrue,
		&passWd,&templateDoc,&vBoolFalse,&wrietPassWdDoc,&wriePassWdtemplateDoc,&vFormat,&vEcode,&vBoolFalse,&vBoolTrue,&vDirection,&vBoolTrue,&xmltrans);

	// ���Ϊhtml ҳ��
	FileName = strHtml;
	vFormat = 8;
	docx.SaveAs(&FileName,&vFormat,&vBoolFalse,&passWd,&vBoolFalse,&wrietPassWdDoc,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse,&vEcode,
		&vBoolFalse,&vBoolFalse,&vBoolFalse,&vBoolFalse);

	// �˳�Word
	CComVariant vRotuneDoc = 0;
	vFormat = 0;
	app.Quit(&vBoolFalse,&vFormat,&vRotuneDoc);
	return TRUE;
}
void CWordView::RemoveTempHtmlFile()
{
	if (m_strLastTmpFile.IsEmpty())
		return;

	::DeleteFile(m_strLastTmpFile);
	CString strFileDir = m_strLastTmpFile.Left(m_strLastTmpFile.ReverseFind('.')) + ".files";
	DeleteDir(strFileDir);
}

void CWordView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_web.m_hWnd)
	{
		CRect rect;
		GetClientRect(&rect);
		m_web.SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),NULL);
		m_web.RedrawWindow();
	}
}


void CWordView::OnOpenWordDoc()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(TRUE,".docx",NULL,6UL,"֧�ֵĸ�ʽ|*.doc;*.docx;*.pdf;*.htm;*.html;*.xml;*.txt;*.bmp;*.jpg;*.png|\
		Word �ĵ�(*.doc;*.docx)|*.doc;*.docx|\
		PDF�ĵ�(*.pdf)|*.pdf|\
		��ҳ�ļ�(*.html;*.html)|*.html;*.html|\
		ͼƬ�ļ�(*.jpg;*.bmp)|*.jpg;*.bmp|\
		�ı��ļ�(*.txt)|*.txt|\
		XML�ļ�(*.xml)|*.xml||",this);

	if (dlg.DoModal()!=IDOK)
		return;

	CString strFile = dlg.GetPathName();
	OpenDocFile(strFile);
}
