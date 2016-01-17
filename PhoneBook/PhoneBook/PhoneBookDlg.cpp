
// PhoneBookDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PhoneBook.h"
#include "PhoneBookDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhoneBookDlg �Ի���
#include <fstream>

class DataBuf : public std::streambuf
{
public:
	DataBuf(char* d, size_t s)
	{
		setg(d, d, d+s);
	}
};

CPhoneBookDlg::CPhoneBookDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPhoneBookDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_iAge(0)
	, m_strPhoneNumber(_T(""))
	, m_strLocation(_T(""))
	, m_strRelation(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pdriver = NULL;
	m_pCon = NULL;
	m_pres = NULL;
	m_bIsUpdateEditing = FALSE;
	m_bIsAddEditing = FALSE;
	m_bDrawSelectPhoto = FALSE;
	m_bDrawBlank = FALSE;
}

void CPhoneBookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SEX, m_strSex);
	DDX_Text(pDX, IDC_EDIT_AGE, m_iAge);
	DDV_MinMaxInt(pDX, m_iAge, 0, 200);
	DDX_Text(pDX, IDC_EDIT_PHONE_NUM, m_strPhoneNumber);
	DDX_Text(pDX, IDC_EDIT_LOCATION, m_strLocation);
	DDX_Text(pDX, IDC_EDIT_RELATION, m_strRelation);
}

BEGIN_MESSAGE_MAP(CPhoneBookDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CPhoneBookDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CPhoneBookDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CPhoneBookDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CPhoneBookDlg::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CPhoneBookDlg::OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CPhoneBookDlg::OnBnClickedButtonNext)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPhoneBookDlg ��Ϣ�������

BOOL CPhoneBookDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_pdriver = get_driver_instance();
	m_pCon = m_pdriver->connect("tcp://127.0.0.1:3309", "root", "1005150782");
	/* Connect to the MySQL test database */
	m_pCon->setSchema("Cug");
	sql::Statement *stmt = m_pCon->createStatement();
	stmt->execute("SET NAMES GBK");
	stmt->execute("create table if not exists PhotoBook(\
				  ID INT(9) NOT NULL AUTO_INCREMENT,\
				  NAME VARCHAR(255),\
				  SEX CHAR(4) DEFAULT '��',\
				  AGE INT(3) DEFAULT 0,\
				  PHONE_NUM CHAR(11) NOT NULL,\
				  LOCATION  VARCHAR(255) DEFAULT NULL,\
				  RELATION  VARCHAR(20) DEFAULT NULL,\
				  PHOTO   MEDIUMBLOB,\
				  PRIMARY KEY(ID)\
				  )");
	delete stmt;

	ReloadData();
	GetDlgItem(IDC_BUTTON_NEXT)->SetFocus();
	return FALSE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPhoneBookDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		if (m_bDrawSelectPhoto)
			DrawSelectPhoto(dc);
		else if (m_bDrawBlank)
			DrawBlank();
		else
			DrawPhoto(dc);
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPhoneBookDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPhoneBookDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CPhoneBookDlg::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strText;
	GetDlgItemText(IDC_BUTTON_ADD,strText);
	if (strText.Compare("����")==0)
	{
		SetDlgItemText(IDC_BUTTON_ADD,"����");
		m_strName.Empty();
		m_strSex = "��";
		m_iAge = 0;
		m_strPhoneNumber.Empty();
		m_strLocation.Empty();
		m_strRelation.Empty();
		UpdateData(FALSE);
		EnableEdit(TRUE);
		GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
		m_bIsAddEditing = TRUE;
		m_bDrawBlank = TRUE;
		ReDrawPhotoArea();
	}
	else if (strText.Compare("����")==0)
	{
		m_bDrawBlank = FALSE;
		m_bIsAddEditing  = FALSE;
		FILE *pFile = NULL;
		if (!m_strSelectPhoto.IsEmpty() && m_bDrawSelectPhoto)
			fopen_s(&pFile,(LPCTSTR)m_strSelectPhoto,"rb");

		unsigned long iLen = 0;
		char *pData = NULL;
		if (pFile)
		{
			fseek(pFile,0L,SEEK_END);
			//��ȡ�ļ�����;
			iLen = ftell(pFile);
			pData = new char[iLen];
			fseek(pFile,0L,SEEK_SET);
			fread_s(pData,iLen,1,iLen,pFile);
			fclose(pFile);
		}

		UpdateData(TRUE);
		sql::PreparedStatement *pstmt = NULL;
		pstmt = m_pCon->prepareStatement("INSERT INTO PhotoBook(NAME,SEX,AGE,PHONE_NUM,LOCATION,RELATION,PHOTO) VALUES (?,?,?,?,?,?,?)");
		pstmt->setString(1, (LPCTSTR)m_strName);
		pstmt->setString(2, (LPCTSTR)m_strSex);
		pstmt->setInt(3, m_iAge);
		pstmt->setString(4, (LPCTSTR)m_strPhoneNumber);
		pstmt->setString(5, (LPCTSTR)m_strLocation);
		pstmt->setString(6, (LPCTSTR)m_strRelation);
		if (pData!=NULL && iLen >0)
		{
			DataBuf buffer(pData,iLen);
			std::istream s(&buffer);
			pstmt->setBlob(7, &s);
			int iRev = pstmt->executeUpdate();
		}
		else
		{
			pstmt->setBlob(7, NULL);
			int iRev = pstmt->executeUpdate();
		}
		
		SetDlgItemText(IDC_BUTTON_ADD,"����");
		delete pstmt;
		delete pData;
		pData = NULL;

		EnableEdit(FALSE);
		ClearAddState();
		ReloadData();
		ReDrawPhotoArea();
		GetDlgItem(IDC_BUTTON_UPDATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(TRUE);
		UpdatePreNextBtnState();
	}
}


void CPhoneBookDlg::OnBnClickedButtonUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pres==NULL)
		return;

	if (m_bIsUpdateEditing==FALSE)
	{
		m_bIsUpdateEditing = TRUE;
		EnableEdit(TRUE);
		SetDlgItemText(IDC_BUTTON_UPDATE,"�������");
		GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
		return;
	}

	if (!m_bIsUpdateEditing)
		return;

	UpdateData(TRUE);
	int iId = m_pres->getInt("ID");
	sql::Statement *stmt = NULL;
	stmt = m_pCon->createStatement();
	CString strSQl;
	strSQl.Format("UPDATE PhotoBook SET NAME=\"%s\",\
		SEX=\"%s\",AGE=%d,PHONE_NUM=\"%s\",LOCATION=\"%s\",RELATION=\"%s\" \
		WHERE ID=%d"
		,(LPCTSTR)m_strName
		,(LPCTSTR)m_strSex
		,m_iAge
		,(LPCTSTR)m_strPhoneNumber
		,(LPCTSTR)m_strLocation
		,(LPCTSTR)m_strRelation
		,iId);
	bool bFaild = stmt->execute((LPCTSTR)strSQl);
	delete stmt;


	FILE *pFile = NULL;
	if (!m_strSelectPhoto.IsEmpty() && m_bDrawSelectPhoto)
		fopen_s(&pFile,(LPCTSTR)m_strSelectPhoto,"rb");

	unsigned long iLen = 0;
	char *pData = NULL;
	if (pFile)
	{
		fseek(pFile,0L,SEEK_END);
		//��ȡ�ļ�����;
		iLen = ftell(pFile);
		pData = new char[iLen];
		fseek(pFile,0L,SEEK_SET);
		fread_s(pData,iLen,1,iLen,pFile);
		fclose(pFile);

		sql::PreparedStatement *pstmt = NULL;
		strSQl.Format("UPDATE PhotoBook SET PHOTO=? WHERE ID=%d",iId);
		pstmt = m_pCon->prepareStatement((LPCTSTR)strSQl);
		DataBuf buffer(pData,iLen);
		std::istream s(&buffer);
		pstmt->setBlob(1, &s);
		int iRev = pstmt->executeUpdate();
		m_strSelectPhoto.Empty();
		m_bDrawSelectPhoto = FALSE;
	}
	m_bIsUpdateEditing = FALSE;
	EnableEdit(FALSE);
	SetDlgItemText(IDC_BUTTON_UPDATE,"����");
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(TRUE);
	UpdatePreNextBtnState();
	ReloadData();
	ReDrawPhotoArea();
	if (!bFaild)
	{
		MessageBox("���³ɹ���","��ʾ",MB_OK|MB_ICONINFORMATION);
	}
	else
		MessageBox("����ʧ�ܣ�","��ʾ",MB_OK|MB_ICONERROR);
}


void CPhoneBookDlg::OnBnClickedButtonRemove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ɾ��
	if (m_pres!=NULL)
	{
		if (IDYES !=MessageBox("ɾ����ǰ��¼��","��ʾ",MB_YESNO|MB_ICONQUESTION))
			return;
	}
	int iId = m_pres->getInt("ID");
	sql::Statement *stmt = NULL;
	stmt = m_pCon->createStatement();
	CString strSQl;
	strSQl.Format("DELETE from PhotoBook WHERE ID=%d",iId);
	bool bFaild = stmt->execute((LPCTSTR)strSQl);
	delete stmt;
	if (!bFaild)
	{
		MessageBox("ɾ���ɹ���","��ʾ",MB_OK|MB_ICONINFORMATION);
		ReloadData();
	}
	else
		MessageBox("ɾ��ʧ�ܣ�","��ʾ",MB_OK|MB_ICONERROR);
}


void CPhoneBookDlg::OnBnClickedButtonPre()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pres!=NULL && m_pres->previous())
	{
		m_strName = m_pres->getString("NAME").c_str();
		m_strSex = m_pres->getString("SEX").c_str();
		m_iAge = m_pres->getInt("AGE");
		m_strPhoneNumber = m_pres->getString("PHONE_NUM").c_str();
		m_strLocation = m_pres->getString("LOCATION").c_str();
		m_strRelation = m_pres->getString("RELATION").c_str();
		UpdateData(FALSE);
		UpdatePreNextBtnState();

		ReDrawPhotoArea();
	}
}


void CPhoneBookDlg::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pres!=NULL && m_pres->next())
	{
		m_strName = m_pres->getString("NAME").c_str();
		m_strSex = m_pres->getString("SEX").c_str();
		m_iAge = m_pres->getInt("AGE");
		m_strPhoneNumber = m_pres->getString("PHONE_NUM").c_str();
		m_strLocation = m_pres->getString("LOCATION").c_str();
		m_strRelation = m_pres->getString("RELATION").c_str();
		UpdateData(FALSE);
		UpdatePreNextBtnState();
		
		ReDrawPhotoArea();
	}
}


void CPhoneBookDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	if (m_pres)
	{
		delete m_pres;
		m_pres = NULL;
	}
	if(m_pCon)
	{
		delete m_pCon;
		m_pCon = NULL;
	}
}
void CPhoneBookDlg::ReloadData()
{
	if (m_pres)
	{
		delete m_pres;
		m_pres = NULL;
	}

	sql::PreparedStatement *pstmt = NULL;
	pstmt = m_pCon->prepareStatement("SELECT * FROM PhotoBook ORDER BY ID ASC");
	m_pres = pstmt->executeQuery();
	m_pres->beforeFirst();
	if (m_pres->next())
	{
		m_strName = m_pres->getString("NAME").c_str();
		m_strSex = m_pres->getString("SEX").c_str();
		m_iAge = m_pres->getInt("AGE");
		m_strPhoneNumber = m_pres->getString("PHONE_NUM").c_str();
		m_strLocation = m_pres->getString("LOCATION").c_str();
		m_strRelation = m_pres->getString("RELATION").c_str();
	}
	delete pstmt;
	UpdateData(FALSE);
	UpdatePreNextBtnState();
}
void CPhoneBookDlg::UpdatePreNextBtnState()
{
	if (m_pres->isFirst())
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);

	if (m_pres->isLast())
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
}
void CPhoneBookDlg::EnableEdit(BOOL bEnable)
{
	bEnable =!bEnable;
	((CEdit*)(GetDlgItem(IDC_EDIT_NAME)))->SetReadOnly(bEnable);
	((CEdit*)(GetDlgItem(IDC_EDIT_SEX)))->SetReadOnly(bEnable);
	((CEdit*)(GetDlgItem(IDC_EDIT_AGE)))->SetReadOnly(bEnable);
	((CEdit*)(GetDlgItem(IDC_EDIT_PHONE_NUM)))->SetReadOnly(bEnable);
	((CEdit*)(GetDlgItem(IDC_EDIT_LOCATION)))->SetReadOnly(bEnable);
	((CEdit*)(GetDlgItem(IDC_EDIT_RELATION)))->SetReadOnly(bEnable);
}
void CPhoneBookDlg::DrawPhoto(CDC & dc)
{
	if (m_pres==NULL || m_pres->isBeforeFirst() || m_pres->isAfterLast())
		return;

	if (m_pres->isNull("PHOTO"))
		return;

	std::istream  * pstream = m_pres->getBlob("PHOTO");
	if (pstream==NULL) return;

	istreambuf_iterator<char> beg(*pstream),eos;                                                 
	string  data(beg,eos);
	int iFileSize = data.length();
	HGLOBAL hMemPic = GlobalAlloc(GMEM_FIXED, iFileSize);
	IStream* pStmBmp = NULL;
	CreateStreamOnHGlobal(hMemPic, TRUE, &pStmBmp);
	BYTE* pbyBmp = (BYTE *)GlobalLock(hMemPic);
	copy(data.begin(),data.end(),pbyBmp);
	delete pstream;
	Image *pImage = Image::FromStream(pStmBmp);

	Graphics g(dc);
	CRect rect;
	GetDlgItem(IDC_STATIC_PHOTO)->GetWindowRect(rect);
	ScreenToClient(rect);
	Gdiplus::Rect recGid(rect.left,rect.top,rect.Width(),rect.Height());
	g.DrawImage(pImage,recGid,0,0,pImage->GetWidth(),pImage->GetHeight(),Gdiplus::UnitPixel);
}

BOOL CPhoneBookDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		if (m_bIsUpdateEditing || m_bIsAddEditing)
		{
			SelectPhoto();
		}

	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
void CPhoneBookDlg::SelectPhoto()
{

	CFileDialog dlg(TRUE,NULL,NULL,6UL,"ͼƬ(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;||",this,0,FALSE);
	if (dlg.DoModal()!=IDOK)
		return;

	m_strSelectPhoto = dlg.GetPathName();
	m_bDrawSelectPhoto = TRUE;
	Invalidate(TRUE);
}
void CPhoneBookDlg::DrawSelectPhoto(CDC & dc)
{
	CStringW Wstr = (wchar_t*)_bstr_t((LPCTSTR)m_strSelectPhoto);
	Graphics g(dc);
	Gdiplus::Image * pImage = Image::FromFile(Wstr);
	if (pImage==NULL) return;

	CRect rect;
	GetDlgItem(IDC_STATIC_PHOTO)->GetWindowRect(rect);
	ScreenToClient(rect);
	Gdiplus::Rect recGid(rect.left,rect.top,rect.Width(),rect.Height());
	g.DrawImage(pImage,recGid,0,0,pImage->GetWidth(),pImage->GetHeight(),Gdiplus::UnitPixel);
}
void CPhoneBookDlg::ClearAddState()
{
	m_strSelectPhoto.Empty();
	m_bDrawSelectPhoto = FALSE;
}
void CPhoneBookDlg::DrawBlank()
{

}
void  CPhoneBookDlg::ReDrawPhotoArea()
{
	CRect rect;
	GetDlgItem(IDC_STATIC_PHOTO)->GetWindowRect(rect);
	ScreenToClient(rect);
	InvalidateRect(rect,TRUE);
}