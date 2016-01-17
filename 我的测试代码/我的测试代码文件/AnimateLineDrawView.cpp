
// AnimateLineDrawView.cpp : CAnimateLineDrawView ���ʵ��
//

#include "stdafx.h"
#include "AnimateLineDraw.h"

#include "AnimateLineDrawDoc.h"
#include "AnimateLineDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimateLineDrawView

IMPLEMENT_DYNCREATE(CAnimateLineDrawView, CView)

BEGIN_MESSAGE_MAP(CAnimateLineDrawView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAnimateLineDrawView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CAnimateLineDrawView ����/����

CAnimateLineDrawView::CAnimateLineDrawView()
{
	// TODO: �ڴ˴���ӹ������

}

CAnimateLineDrawView::~CAnimateLineDrawView()
{
}

BOOL CAnimateLineDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CAnimateLineDrawView ����

void CAnimateLineDrawView::OnDraw(CDC* pDC)
{
	CAnimateLineDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

	// �ڿͻ���Ļ�Ͻ���һ�����߻������������СΪ200 x 100,����Ϊ�����ɫ����
	CRect rect(200,100,400,200);
	CBrush bkBrush(HS_CROSS,RGB(0,128,0));
	pDC->SetBkColor(RGB(0,0,0));
	pDC->FillRect(rect,&bkBrush);

	//����һ�����豸��������ݵ�λͼ���ߴ��СΪ195 x 100������������Ҫ�ƶ�������ͼ
	if(m_dc.GetSafeHdc()==NULL)
	{

		CRect rect1(0,0,195,100);
		m_dc.CreateCompatibleDC(pDC);
		m_bitmap.CreateCompatibleBitmap(pDC,195,100);
		m_dc.SelectObject(m_bitmap);
		m_dc.SetBkColor(RGB(0,0,0));
		m_dc.FillRect(rect1,&bkBrush);
	}
}


// CAnimateLineDrawView ��ӡ


void CAnimateLineDrawView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CAnimateLineDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CAnimateLineDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CAnimateLineDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CAnimateLineDrawView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CAnimateLineDrawView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CAnimateLineDrawView ���

#ifdef _DEBUG
void CAnimateLineDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimateLineDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimateLineDrawDoc* CAnimateLineDrawView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimateLineDrawDoc)));
	return (CAnimateLineDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimateLineDrawView ��Ϣ�������

void CAnimateLineDrawView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	GetParentFrame()->RecalcLayout();
	m_offsetx = 5;
	SetTimer(1,100,NULL);
}

void CAnimateLineDrawView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CClientDC dc(this);
	static int x = 200;
	static int y = 200;
	CPen pen1(PS_SOLID,0,RGB(255,0,8));
	CPen *pOldPen = dc.SelectObject(&pen1);

	//���´���ʵ�ֶ�̬����
	x = x+ m_offsetx;

	// �������û�г�����ĻͼƬ���ұ߽��ߣ�ֱ������ĻͼƬ���ڻ������ߣ�����ƽ��
	if( x <= 400)
	{
		dc.MoveTo(x-m_offsetx,y);
		y = 200 - rand()%90; //ģ�����ݲ���ֵ
		dc.LineTo(x,y);
	}
	else // ���������ĻͼƬ���ұ߽��ߣ�ֱ��Ҫƽ������ͼ
	{
		CRect rect(200,100,400,200);
		CBrush bkBrush(HS_CROSS,RGB(0,128,0));

		// ������Ҫ�ƶ�������ͼ��
		m_dc.BitBlt(0,0,195,100,&dc,205,100,SRCCOPY);

		// �ñ���ͼ�ػ���ĻͼƬ���������е���������
		dc.SetBkColor(RGB(0,0,0));
		dc.FillRect(rect,&bkBrush);

		//�����������ͼ����ƽ��5����λ
		dc.BitBlt(200,100,195,100,&m_dc,0,0,SRCCOPY);

		// �������ݼ�����������
		dc.MoveTo(395,y);
		y = 200 - rand()%90; //ģ�����ݲ���ֵ
		dc.LineTo(400,y);
	}

	dc.SelectObject(pOldPen);
	CView::OnTimer(nIDEvent);
}
