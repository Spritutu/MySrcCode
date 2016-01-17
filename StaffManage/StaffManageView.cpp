
// StaffManageView.cpp : CStaffManageView ���ʵ��
//

#include "stdafx.h"
#include "StaffManage.h"

#include "StaffManageSet.h"
#include "StaffManageDoc.h"
#include "StaffManageView.h"
#include "DialogQuery.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HBITMAP BufferToHBITMAP(CWnd *pWnd,char * pBMPBuffer)
{
	HBITMAP				hBmp;
	LPSTR				hDIB,lpBuffer = pBMPBuffer;
	LPVOID				lpDIBBits;
	BITMAPFILEHEADER	bmfHeader;
	DWORD				bmfHeaderLen;

	bmfHeaderLen = sizeof(bmfHeader);
	strncpy((LPSTR)&bmfHeader,(LPSTR)lpBuffer,bmfHeaderLen);

	if (bmfHeader.bfType != (*(WORD*)"BM"))
		return NULL;//��copy��Windows������ơ��ϵ�������

	hDIB = lpBuffer + bmfHeaderLen;
	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	lpDIBBits=(lpBuffer)+((BITMAPFILEHEADER *)lpBuffer)->bfOffBits;//���й��ܺ����汻ע�͵��Ĵ�����ͬ��������⡣
	CClientDC dc(pWnd);
	hBmp = CreateDIBitmap(dc.m_hDC,&bmiHeader,CBM_INIT,lpDIBBits,&bmInfo,DIB_RGB_COLORS);
	return hBmp;
}

// CStaffManageView

IMPLEMENT_DYNCREATE(CStaffManageView, CRecordView)

BEGIN_MESSAGE_MAP(CStaffManageView, CRecordView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRecordView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CStaffManageView::OnFilePrintPreview)
	ON_BN_CLICKED(IDC_ADDRECO, &CStaffManageView::OnBnClickedAddreco)
	ON_BN_CLICKED(IDC_RECODELE, &CStaffManageView::OnBnClickedRecodele)
	ON_BN_CLICKED(IDC_SORTRECO, &CStaffManageView::OnBnClickedSortreco)
	ON_BN_CLICKED(IDC_FILTERRECO, &CStaffManageView::OnBnClickedFilterreco)
	ON_BN_CLICKED(IDC_SET_PHOTO, &CStaffManageView::OnBnClickedSetPhoto)
	ON_BN_CLICKED(IDC_RECO_MODIFY, &CStaffManageView::OnBnClickedRecoModify)
END_MESSAGE_MAP()

// CStaffManageView ����/����

CStaffManageView::CStaffManageView()
: CRecordView(CStaffManageView::IDD)
{
	m_pSet = NULL;
	m_hPhotoBitmap = NULL;
	m_bClearPicture = false;
	// TODO: �ڴ˴���ӹ������

}

CStaffManageView::~CStaffManageView()
{
}

void CStaffManageView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	// �����ڴ˴����� DDX_Field* �����Խ��ؼ������ӡ������ݿ��ֶΣ�����
	DDX_FieldText(pDX, IDC_EDITZGBH, m_pSet->m_iStaffID, m_pSet); // ְ�����
	DDX_FieldText(pDX, IDC_EDITZGXM, m_pSet->m_strStaffName, m_pSet); // ְ������

	DDX_FieldText(pDX, IDC_EDITZGNL, m_pSet->m_iStaffAge, m_pSet); // ְ������
	DDX_FieldText(pDX, IDC_EDITSZBM, m_pSet->m_strDepartment, m_pSet); // ְ������


	DDX_FieldText(pDX, IDC_EDITZGXB, m_pSet->m_strStaffSex, m_pSet); //ְ���Ա�
	DDX_FieldText(pDX, IDC_EDITZC, m_pSet->m_strDuty, m_pSet); //ְ��


	//DDX_FieldText(pDX, IDC_EDITGZSJ, m_pSet->m_WorkTime, m_pSet); // ����ʱ��
	DDX_FieldText(pDX, IDC_EDITJBGZ, m_pSet->m_fStaffPrice, m_pSet); // ��������
	DDX_FieldText(pDX, IDC_EDIT_GRADUATA, m_pSet->m_strWhereGradute, m_pSet); // ��ҵԺУ

	// �йظ�����Ϣ������� MSDN �� ODBC ʾ��
	DDX_DateTimeCtrl(pDX, IDC_TIME_RZSJ, m_pSet->m_WorkTime); // ����ʱ��

	CWnd *pPictureWnd = GetDlgItem(IDC_STATIC_PHOTO);
	if (pPictureWnd)
	{
		if (m_pSet->m_PtrPhoto.m_hData && m_pSet->m_PtrPhoto.m_dwDataLength > 0)
		{
			LPSTR buffer = (LPSTR)GlobalLock(m_pSet->m_PtrPhoto.m_hData);           
			m_hPhotoBitmap = BufferToHBITMAP(pPictureWnd,buffer);
			GlobalUnlock(m_pSet->m_PtrPhoto.m_hData);

			m_bClearPicture = false;
			this->RedrawWindow();
		}
		else
		{
			m_bClearPicture = true;
			this->RedrawWindow();
		}
		
	}
}

BOOL CStaffManageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CRecordView::PreCreateWindow(cs);
}

void CStaffManageView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_StaffManageSet;
	CRecordView::OnInitialUpdate();

}


// CStaffManageView ��ӡ


void CStaffManageView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CStaffManageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CStaffManageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CStaffManageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CStaffManageView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CStaffManageView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CStaffManageView ���

#ifdef _DEBUG
void CStaffManageView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CStaffManageView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

CStaffManageDoc* CStaffManageView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStaffManageDoc)));
	return (CStaffManageDoc*)m_pDocument;
}
#endif //_DEBUG


// CStaffManageView ���ݿ�֧��
CRecordset* CStaffManageView::OnGetRecordset()
{
	return m_pSet;
}


void CStaffManageView::UpdateCtrlReadOnlyState(BOOL bReadOnly)
{
	CEdit * pEdit = (CEdit*)GetDlgItem(IDC_EDITZGXM);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITZGXB);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITZGNL);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITZC);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITJBGZ);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDITSZBM);
	pEdit->SetReadOnly(bReadOnly);

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_GRADUATA);
	pEdit->SetReadOnly(bReadOnly);
}
// CStaffManageView ��Ϣ�������

void CStaffManageView::OnBnClickedAddreco()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	static bool bAdd = true;
	if (bAdd)
	{
		UpdateCtrlReadOnlyState(FALSE);
		GetDlgItem(IDC_SET_PHOTO)->ShowWindow(SW_SHOW);

		m_pSet->m_strSort = "ְ�����";
		m_pSet->Requery();
		m_pSet->MoveLast();
		int iID = m_pSet->m_iStaffID + 1;
		m_pSet->AddNew();

		UpdateData(FALSE);

		CStringW strText = _T("");
		strText.Format(_T("%d"),iID);
		SetDlgItemText(IDC_EDITZGBH,strText);

		bAdd = false;
		SetDlgItemText(IDC_ADDRECO,_T("�����¼"));
	}
	else
	{
		SetDlgItemText(IDC_ADDRECO,_T("���Ӽ�¼"));
		GetDlgItem(IDC_SET_PHOTO)->ShowWindow(SW_HIDE);
		UpdateData(TRUE);
		m_pSet->Update();
		bAdd = true;
		AfxMessageBox(_T("���Ӽ�¼�ɹ�!"));
		UpdateCtrlReadOnlyState(TRUE);
		m_pSet->MoveLast();
	}

}

void CStaffManageView::OnBnClickedRecodele()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (MessageBox(_T("ȷ��Ҫɾ��������¼��"),_T("��ʾ"),MB_OKCANCEL|MB_ICONQUESTION) != IDOK)
		return;

	m_pSet->Delete();
	m_pSet->MoveNext();

	if (m_pSet->IsEOF())
		m_pSet->MoveLast();

	if (m_pSet->IsBOF())
		m_pSet->SetFieldNull(NULL);

	UpdateData(FALSE);
	AfxMessageBox(_T("ɾ���ɹ�"));
}

void CStaffManageView::OnBnClickedSortreco()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pSet->m_strSort = "��������";
	m_pSet->Requery();
	UpdateData(FALSE);
}

void CStaffManageView::OnBnClickedFilterreco()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogQuery dlg;
	CString value;
	if (dlg.DoModal()!=IDOK)
		return;


	value = _T("ְ������=") + dlg.m_query;
	m_pSet->m_strFilter = value;
	m_pSet->Requery();
	UpdateData(FALSE);
}

void CStaffManageView::OnBnClickedSetPhoto()
{
	CString strFilter = _T("BMPͼ���ļ�|*.bmp;*.dib||");

	CFileDialog     dlg(TRUE,NULL,NULL,0,strFilter,this);     
	if(dlg.DoModal()!=IDOK)
		return;

	//m_pSet->Edit();   //�����༭��ǰ��¼ 
	CFile     filePhoto;      
	CFileStatus     fileStatus;      
	CString     fileLocate = dlg.GetPathName();            
	filePhoto.Open(fileLocate,CFile::modeRead);           
	filePhoto.GetStatus(fileStatus);

	m_pSet->m_PtrPhoto.m_dwDataLength = (SQLUINTEGER)fileStatus.m_size;
	HGLOBAL     hGlobal     =     GlobalAlloc(GPTR,(SIZE_T)fileStatus.m_size);           
	m_pSet->m_PtrPhoto.m_hData     =     GlobalLock(hGlobal);
	filePhoto.Read(m_pSet->m_PtrPhoto.m_hData,(UINT)fileStatus.m_size);           

	//����ѡ����ļ�������д��m_pSet-m_PtrPhoto           
	m_pSet->SetFieldDirty(&m_pSet->m_PtrPhoto);           

	m_pSet->SetFieldNull(&m_pSet->m_PtrPhoto,FALSE);           
	//m_pSet->Update();     //     ���¼�¼           
	GlobalUnlock(hGlobal); 


	CWnd *pPictureWnd = GetDlgItem(IDC_STATIC_PHOTO);
	m_hPhotoBitmap = BufferToHBITMAP(pPictureWnd,(char*)m_pSet->m_PtrPhoto.m_hData);
	this->RedrawWindow();
}

void CStaffManageView::ClearPhotoView()
{
	CWnd *pPictureWnd = GetDlgItem(IDC_STATIC_PHOTO);
	if (!pPictureWnd)
		return;

	CRect rect;
	pPictureWnd->GetClientRect(rect);

	CDC  *pDC  = pPictureWnd->GetDC();
	CBrush brush(RGB(255,255,255));
	CBrush * pOldBrush = (CBrush*)pDC->SelectObject(&brush);
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);
}
void CStaffManageView::OnDraw(CDC *pDC)
{
	if(!m_hPhotoBitmap)
		return;

	CWnd *pPictureWnd = GetDlgItem(IDC_STATIC_PHOTO);
	if (!pPictureWnd)
		return;

	HBITMAP OldBitmap;
	CDC MemDC;
	MemDC.CreateCompatibleDC(pPictureWnd->GetDC());
	OldBitmap=(HBITMAP)MemDC.SelectObject(m_hPhotoBitmap);

	CRect rect;
	pPictureWnd->GetWindowRect(rect);
	pPictureWnd->GetDC()->BitBlt(0 ,0 , rect.Width() , rect.Height(), &MemDC , 0 , 0 ,SRCCOPY);

	MemDC.SelectObject(OldBitmap);

	if (m_bClearPicture)
		ClearPhotoView();
}

void CStaffManageView::OnBnClickedRecoModify()
{
	static bool bEdit = true;
	if (bEdit)
	{
		m_pSet->Edit();
		UpdateCtrlReadOnlyState(FALSE);
		GetDlgItem(IDC_SET_PHOTO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ADDRECO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RECODELE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SORTRECO)->EnableWindow(FALSE);
		GetDlgItem(IDC_FILTERRECO)->EnableWindow(FALSE);

	/*	GetDlgItem(ID_RECORD_PREV)->EnableWindow(FALSE);
		GetDlgItem(ID_RECORD_NEXT)->EnableWindow(FALSE);
		GetDlgItem(ID_RECORD_FIRST)->EnableWindow(FALSE);
		GetDlgItem(ID_RECORD_LAST)->EnableWindow(FALSE);*/


		SetDlgItemText(IDC_RECO_MODIFY,_T("�����޸�"));
		bEdit = false;
	}
	else
	{
		m_pSet->Update();  //���¼�¼ 
		MessageBox(_T("�޸ļ�¼�ɹ�!"),_T("��ʾ"),MB_OK|MB_ICONINFORMATION);

		GetDlgItem(IDC_SET_PHOTO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ADDRECO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RECODELE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SORTRECO)->EnableWindow(TRUE);
		GetDlgItem(IDC_FILTERRECO)->EnableWindow(TRUE);

		/*GetDlgItem(ID_RECORD_PREV)->EnableWindow(TRUE);
		GetDlgItem(ID_RECORD_NEXT)->EnableWindow(TRUE);
		GetDlgItem(ID_RECORD_FIRST)->EnableWindow(TRUE);
		GetDlgItem(ID_RECORD_LAST)->EnableWindow(TRUE);*/

		UpdateCtrlReadOnlyState(TRUE);
		SetDlgItemText(IDC_RECO_MODIFY,_T("��¼�޸�"));
		bEdit = true;
	}
}
