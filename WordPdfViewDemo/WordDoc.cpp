
// WordPdfViewDemoDoc.cpp : CWordPdfViewDemoDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "WordPdfViewDemo.h"
#endif

#include "WordDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWordPdfViewDemoDoc

IMPLEMENT_DYNCREATE(CWordDoc, CDocument)

BEGIN_MESSAGE_MAP(CWordDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CWordDoc, CDocument)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IWordPdfViewDemo ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {EAC5604D-2E09-41AB-B632-DCD3D12194C1}
static const IID IID_IWordPdfViewDemo =
{ 0xEAC5604D, 0x2E09, 0x41AB, { 0xB6, 0x32, 0xDC, 0xD3, 0xD1, 0x21, 0x94, 0xC1 } };

BEGIN_INTERFACE_MAP(CWordDoc, CDocument)
	INTERFACE_PART(CWordDoc, IID_IWordPdfViewDemo, Dispatch)
END_INTERFACE_MAP()


// CWordPdfViewDemoDoc ����/����

CWordDoc::CWordDoc()
{
	// TODO: �ڴ����һ���Թ������

	EnableAutomation();

	AfxOleLockApp();
}

CWordDoc::~CWordDoc()
{
	AfxOleUnlockApp();
}

BOOL CWordDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CWordPdfViewDemoDoc ���л�

void CWordDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CWordDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CWordDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CWordDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWordPdfViewDemoDoc ���

#ifdef _DEBUG
void CWordDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWordDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWordPdfViewDemoDoc ����
