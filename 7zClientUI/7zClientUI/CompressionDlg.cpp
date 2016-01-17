// Compression.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "7zClientUI.h"
#include "CompressionDlg.h"
#include "afxdialogex.h"
#include "FileSelectDlg.h"
#include "7zInterface.h"

static bool m_bCancel = false;
#define RE_USER_CANCEL  9999
#define WM_COMPRESS_FINISH_MSG WM_USER+1

class CMyProgress: public CArchiveUpdateCallback//����ѹ����������
{
	friend CCompressionDlgPage;
public:
	CMyProgress(CProgressCtrl *bar):CArchiveUpdateCallback()
	{
		m_ProgressBar = bar;
	}
	virtual ~CMyProgress()
	{
	}
	CProgressCtrl * GetProgresCtrl()
	{
		return m_ProgressBar;
	}
	STDMETHOD(SetTotal)(UInt64 size)
	{
		m_iTotal64 = size;
		m_iTotal32 = m_iTotal64/1000;
		m_ProgressBar->SetRange32(0,m_iTotal32);
		m_bCancel = false; // ��ȫ�ֱ���ȡ������Ϊfalse
		return S_OK;
	}
	STDMETHOD(SetCompleted)(const UInt64 *completeValue)
	{
		if (m_bCancel)
			return RE_USER_CANCEL; // ��׽���û�ȡ������Ϣ�������û�ȡ��

		int iPos = *completeValue / 1000;
		if (*completeValue == m_iTotal64)
			iPos = m_iTotal32 ;

		m_ProgressBar->SetPos(iPos);
		return S_OK;
	}
private:
	CProgressCtrl *m_ProgressBar;
	UInt64 m_iTotal64;
	int    m_iTotal32;
};

typedef struct _TagThreadDataCompress
{
	_TagThreadDataCompress()
	{
		m_OutPath.Empty();
		m_PassWd.Empty();
		m_vFileList.Clear();
		m_iCompressLevel = 0;
		m_pProgress  = NULL;
		m_hWndRecvMsg = NULL;
	}
	UString m_OutPath;
	UStringVector m_vFileList;
	UString m_PassWd;
	int    m_iCompressLevel;
	CMyProgress *m_pProgress;
	HWND   m_hWndRecvMsg;
}ThreadDataCompress;

DWORD WINAPI ThreadCompressFun(LPVOID pData)
{
	ThreadDataCompress *pComressData = (ThreadDataCompress*)(pData);
	CProgressCtrl *pProgressCtrl = pComressData->m_pProgress->GetProgresCtrl();

	int iRev = compress(pComressData->m_OutPath,
		pComressData->m_vFileList,
		pComressData->m_PassWd,
		pComressData->m_iCompressLevel,
		pComressData->m_pProgress);

	if (iRev == 0)
	{
		int iRange[2]={0};
		pProgressCtrl->GetRange(iRange[0],iRange[1]);
		pProgressCtrl->SetPos(iRange[1]);
	}
	if (iRev == RE_USER_CANCEL)
		;
		//remove(pComressData->m_OutPath);

	if (pComressData->m_hWndRecvMsg)
		::PostMessage(pComressData->m_hWndRecvMsg,WM_COMPRESS_FINISH_MSG,iRev,NULL);

	delete pComressData;
	return 0;
}
// CCompression �Ի���

IMPLEMENT_DYNAMIC(CCompressionDlgPage, CDialogEx)

CCompressionDlgPage::CCompressionDlgPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompressionDlgPage::IDD, pParent)
{

}

CCompressionDlgPage::~CCompressionDlgPage()
{
}

void CCompressionDlgPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_FileList);
	DDX_Control(pDX, IDC_PROGRESS_COM, m_bProgressBar);
}


BEGIN_MESSAGE_MAP(CCompressionDlgPage, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ADD_FILE, &CCompressionDlgPage::OnBnClickedBtnAddFile)
	ON_BN_CLICKED(IDC_BTN_EXCUTE_COMPRESS, &CCompressionDlgPage::OnBnClickedBtnExcuteCompress)
	ON_MESSAGE(WM_COMPRESS_FINISH_MSG,&CCompressionDlgPage::OnMsgCompressFinish)
END_MESSAGE_MAP()


// CCompression ��Ϣ�������


void CCompressionDlgPage::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}


void CCompressionDlgPage::OnBnClickedBtnAddFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD flags = OFN_ALLOWMULTISELECT|OFN_PATHMUSTEXIST;

	CFileSelectDlg dlg(TRUE,NULL,NULL,flags,"*.*||",NULL,0UL,FALSE);
	dlg.m_ofn.lpstrTitle   =   "ѡ��Ҫ��ӵ�ѹ�������ļ�";                                    //��������ѡ��100���ļ�
	dlg.m_ofn.lpstrFile = new TCHAR[MAX_PATH * SELECT_MAX_FILE_NUMBER]; //���¶��建������С          
	memset(dlg.m_ofn.lpstrFile, 0, MAX_PATH * SELECT_MAX_FILE_NUMBER);  //��ʼ������Ļ�����
	dlg.m_ofn.nMaxFile = MAX_PATH*SELECT_MAX_FILE_NUMBER; // ָ����������С

	char szDir[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(),szDir,MAX_PATH);
	CString strConfigINI = szDir;
	strConfigINI.MakeLower();
	strConfigINI = strConfigINI.Left(strConfigINI.ReverseFind('.') + 1) + "ini";
	CHAR szDefaultpath[MAX_PATH];
	SHGetSpecialFolderPath(NULL,szDefaultpath,CSIDL_DESKTOPDIRECTORY,FALSE); // �Ҳ�����Ĭ��Ϊ����·��
	CString strLastDir = "";
	GetPrivateProfileString("setting","LastSelectFolder",szDefaultpath,strLastDir.GetBuffer(512),512,strConfigINI);
	if (!PathFileExists(strLastDir))
		strLastDir = szDefaultpath; // �ϴε�·��������ʱ��ʹ������·��
	dlg.m_ofn.lpstrInitialDir = strLastDir;
	strLastDir.ReleaseBuffer();

	if (dlg.DoModal() != IDOK)
	{
		delete[] dlg.m_ofn.lpstrFile;     //���������ͷ��ڴ�
		return;
	}

	strLastDir = CFileSelectDlg::m_strCurrendDirectory;
	WritePrivateProfileString("setting","LastSelectFolder",strLastDir,strConfigINI);
	int iSize = CFileSelectDlg::m_SelectedItemList.GetCount();

	for (int i=0;i<iSize;i++)
	{
		m_FileList.AddString(CFileSelectDlg::m_SelectedItemList[i]);
	}

	delete[] dlg.m_ofn.lpstrFile;     //���������ͷ��ڴ�
}


void CCompressionDlgPage::OnBnClickedBtnExcuteCompress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strText;
	GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->GetWindowText(strText);
	if (strText =="ȡ��")
	{
		int iRev = MessageBox("�Ƿ�Ҫȡ��ѹ����","��ʾ",MB_YESNO|MB_ICONQUESTION);
		if (iRev==IDYES)
		{
			m_bCancel = true;
			GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->EnableWindow(FALSE);
		}
		return;
	}
	CFileDialog dlg(FALSE,"7z",NULL,6UL,"*.7z||",this,0UL,FALSE);
	if (dlg.DoModal()!=IDOK)
		return;

	CString strFileOut = dlg.GetPathName();

	UString OutFilePath,InFilePath,PassWordCompress;
	UStringVector SwitchStrings;
	Char2UString(OutFilePath ,strFileOut);
	Char2UString(PassWordCompress,"");

	for (int i=0;i<m_FileList.GetCount();i++)
	{
		CString strFile;
		m_FileList.GetText(i,strFile);
		Char2UString(InFilePath ,strFile);
		SwitchStrings.Add(InFilePath);
	}

	CMyProgress* pProgress = new CMyProgress(&m_bProgressBar);
	ThreadDataCompress *pThreadData = new ThreadDataCompress;
	pThreadData->m_OutPath = OutFilePath;
	pThreadData->m_vFileList = SwitchStrings;
	pThreadData->m_pProgress = pProgress;
	pThreadData->m_iCompressLevel = 1;
	pThreadData->m_PassWd = UString(L"");
	pThreadData->m_hWndRecvMsg = GetSafeHwnd();
	GetDlgItem(IDC_BTN_ADD_FILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->SetWindowText("ȡ��");
	CreateThread(NULL,0,ThreadCompressFun,pThreadData,0,NULL);
}


BOOL CCompressionDlgPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_bProgressBar.SendMessage (PBM_SETBARCOLOR, 0, RGB(0,200,0));
	m_bProgressBar.SendMessage (PBM_SETBKCOLOR, 0, RGB(200,200,200));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
LRESULT CCompressionDlgPage::OnMsgCompressFinish(WPARAM wp,LPARAM)
{
	if (wp == 0)
	{
		MessageBox("ѹ����ɣ�","��ʾ",MB_OK|MB_ICONINFORMATION);
	}
	else if (wp == RE_USER_CANCEL)
	{
		m_bProgressBar.SetPos(0);
		MessageBox("�û�ѹ��ȡ����","��ʾ",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox("�ļ�ѹ��ʧ�ܣ�","��ʾ",MB_OK|MB_ICONINFORMATION);
	}
	GetDlgItem(IDC_BTN_ADD_FILE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->SetWindowText("��ʼѹ��");
	GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->EnableWindow(TRUE);
	return S_OK;
}

BOOL CCompressionDlgPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			CString strText;
			GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->GetWindowText(strText);
			if (strText =="ȡ��")
			{
				int iRev = MessageBox("�Ƿ�Ҫȡ��ѹ����","��ʾ",MB_YESNO|MB_ICONQUESTION);
				if (iRev==IDYES)
				{
					m_bCancel = true;
					GetDlgItem(IDC_BTN_EXCUTE_COMPRESS)->EnableWindow(FALSE);
				}
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
			return TRUE;
		else if (pMsg->wParam == VK_SPACE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
