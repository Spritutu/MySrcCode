
// TestMemMapFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestMemMapFile.h"
#include "TestMemMapFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestMemMapFileDlg �Ի���




CTestMemMapFileDlg::CTestMemMapFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestMemMapFileDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestMemMapFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestMemMapFileDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestMemMapFileDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestMemMapFileDlg ��Ϣ�������

BOOL CTestMemMapFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestMemMapFileDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestMemMapFileDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTestMemMapFileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


const char* getline(const char* buf, int* len)   
{   
	const char* tmp = buf;   
	while(*tmp && (*tmp != 0x0d || *tmp != 0x0a)) ++tmp;    
	*len = tmp - buf;   
	if(*tmp == 0)   
		return NULL;   
	++tmp;   
	if(*tmp == 0x0d)   
	{   
		tmp += 2;   
		assert(*(tmp-1) == 0x0a);   
	}   
	else   
		++tmp;   
	return tmp;      
}      

void CTestMemMapFileDlg::OnBnClickedButton1()
{
	 CFileDialog fileDlg(TRUE, "*.txt", "*.txt", NULL, "�ı��ļ� (*.txt)|*.txt|ͼ���ļ���*.tif *.jpg *.img��|*.tif|*.jpg|*.img||");   
    fileDlg.m_ofn.Flags |= OFN_FILEMUSTEXIST;   
    fileDlg.m_ofn.lpstrTitle = "ͨ���ڴ�ӳ���ļ���ȡ����";   
    if (fileDlg.DoModal() == IDOK)   
    {   
        HANDLE hFile=CreateFile(fileDlg.GetPathName(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);   
           
        SYSTEM_INFO sinf;   
        GetSystemInfo(&sinf);   
        DWORD dwAllocationGranularity = sinf.dwAllocationGranularity;   
           
        HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);   
        DWORD dwFileSizeHigh;   
        __int64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);   
        qwFileSize += (((__int64) dwFileSizeHigh) << 32);   
           
        CloseHandle(hFile);//We nolonger need access to the file object shandle.   
               
           
        __int64 qwFileOffset = 0;//A�ļ���ͼ��ƫ����   
           
           
           
           
        while(qwFileSize>0)   
        {   
            //Determinethenumberofbytestobemappedinthisview   
           
            DWORD dwBytesInBlock = sinf.dwAllocationGranularity;   
               
            if(qwFileSize < sinf.dwAllocationGranularity)//�ļ�С��ϵͳ��������   
                dwBytesInBlock = (DWORD)qwFileSize;//ƫ����Ϊ�ļ���С   
               
            PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping,FILE_MAP_READ,(DWORD)(qwFileOffset>>32),(DWORD)(qwFileOffset&0xFFFFFFFF),dwBytesInBlock);   
            CString   DataStr(_T(""));    
               
            DWORD   i   =   0;      
            while( *(pbFile + i)  && (*(pbFile + i) != 0x0d) && (*(pbFile + i) != 0x0a))      
            {      
                DataStr += *(pbFile + i);      
                ++i;                
            }   
               
    /*  
            const char* buf = (const char*)pbFile;     
            const char* start= (const char*)pbFile;  
            int len;  
            while(start != NULL)  
            {  
                start = getline(buf, &len);  
                CString   str(buf, len);  
                  
                buf = start;  
            }     
    */   
   
            UnmapViewOfFile(pbFile);   
                       
            //Skiptothenextsetofbytesinthefile.   
            qwFileOffset += dwBytesInBlock;   
            qwFileSize -= dwBytesInBlock;   
        }   
           
        CloseHandle(hFileMapping);   
    }   
       
}
