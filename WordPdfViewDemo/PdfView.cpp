
// WordPdfViewDemoView.cpp : CWordPdfViewDemoView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "WordPdfViewDemo.h"
#endif

#include "PdfDoc.h"
#include "PdfView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWordPdfViewDemoView

IMPLEMENT_DYNCREATE(CPdfView, CView)

BEGIN_MESSAGE_MAP(CPdfView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPdfView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPEN_PDF, &CPdfView::OnOpenPdfDoc)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CWordPdfViewDemoView ����/����

CPdfView::CPdfView()
{
	// TODO: �ڴ˴���ӹ������

}

CPdfView::~CPdfView()
{
}

BOOL CPdfView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CWordPdfViewDemoView ����

void CPdfView::OnDraw(CDC* /*pDC*/)
{
	CPdfDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (m_wndPDF.m_hWnd)
		m_wndPDF.RedrawWindow();
}


// CWordPdfViewDemoView ��ӡ


void CPdfView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPdfView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPdfView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPdfView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CPdfView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPdfView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWordPdfViewDemoView ���

#ifdef _DEBUG
void CPdfView::AssertValid() const
{
	CView::AssertValid();
}

void CPdfView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPdfDoc* CPdfView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPdfDoc)));
	return (CPdfDoc*)m_pDocument;
}
#endif //_DEBUG


// CWordPdfViewDemoView ��Ϣ�������


void CPdfView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	if (m_wndPDF.m_hWnd==NULL)
	{
		CRect rt;
		GetClientRect(&rt);
		// ����CWnd::CreateControl����PDF�ؼ�����
		// Acrobat (Reader) 5.x/6.x�Ŀؼ���IDΪ"PDF.PdfCtrl.5"��7.xΪ"AcroPDF.PDF.1"
		m_wndPDF.CreateControl("AcroPDF.PDF.1", NULL, WS_CHILD | WS_VISIBLE, rt, this, 0);

		// ��ȡIDispatch�ӿ�ָ��
		LPUNKNOWN lpUnknown = m_wndPDF.GetControlUnknown();
		LPDISPATCH lpDispatch;
		lpUnknown->QueryInterface(IID_IDispatch, (void**)&lpDispatch);
		// �ѻ�õĽӿ�ָ�봫�ݸ������������m_drvPDF��ͨ�������ÿؼ������Ժͷ���
		// ��m_drvPDF����ʱ�����Զ��ͷŽӿ�ָ��
		m_drvPDF.AttachDispatch(lpDispatch,TRUE);
	}
	CPdfDoc* pDoc = GetDocument();
	CString strPdfFile = pDoc->GetPathName();
	if (!strPdfFile.IsEmpty())
	{
		m_drvPDF.LoadFile(strPdfFile);
		//m_drvPDF.setZoom(0.5);
	}
}


void CPdfView::OnOpenPdfDoc()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(TRUE,".pdf",NULL,6UL,"PDF(*.pdf)|*.pdf||",this);
	if (dlg.DoModal()!=IDOK)
		return;

	CString strFile = dlg.GetPathName();
	m_drvPDF.LoadFile(strFile);
}


void CPdfView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_wndPDF.m_hWnd)
	{
		CRect rect;
		GetClientRect(&rect);
		m_wndPDF.SetWindowPos(&wndTop,rect.left,rect.top,rect.Width(),rect.Height(),NULL);
		m_wndPDF.RedrawWindow();
	}
}
