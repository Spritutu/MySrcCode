
// GridViewTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GridViewTest.h"
#include "GridViewTestDlg.h"
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGridViewTestDlg �Ի���




CGridViewTestDlg::CGridViewTestDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGridViewTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iSelRow = 0;
	m_iSelCol = 0;
}

void CGridViewTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_flexGrid);
}

BEGIN_MESSAGE_MAP(CGridViewTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(GRID_DATA_CHANGED,&CGridViewTestDlg::OnGridDataChanged)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ExportGRid, &CGridViewTestDlg::OnBnClickedButtonExportgrid)
	ON_BN_CLICKED(IDC_BUTTON_ImportCSV, &CGridViewTestDlg::OnBnClickedButtonImportcsv)
END_MESSAGE_MAP()


// CGridViewTestDlg ��Ϣ�������

BOOL CGridViewTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	long int iRows = m_flexGrid.get_Rows();
	long int iCols = m_flexGrid.get_Cols();

	m_flexGrid.put_Rows(13);
	m_flexGrid.put_Cols(7);

	/*iRows = m_flexGrid.get_Rows();
	iCols = m_flexGrid.get_Cols();

	int iSelRow = m_flexGrid.get_Row();
	int iSelCol = m_flexGrid.get_Col();

	m_flexGrid.put_Row(10);
	m_flexGrid.put_Col(6);

	iSelRow = m_flexGrid.get_Row();
	iSelCol = m_flexGrid.get_Col();
*/

	char szBuf[10] ={0};
	for (int i=0;i<13;i++)
	{
		for (int j=0;j<7;j++)
		{
			CString strVal;
			strVal.Format("[%d��%d]",i,j);

			sprintf(szBuf,"[%d��%d]",i,j);
			//m_flexGrid.put_TextMatrix(i,j,strVal);
			m_flexGrid.put_TextMatrix(i,j,szBuf);
		}
	}


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGridViewTestDlg::OnPaint()
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
HCURSOR CGridViewTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CGridViewTestDlg, CDialog)
	ON_EVENT(CGridViewTestDlg, IDC_MSFLEXGRID1, 71, CGridViewTestDlg::EnterCellMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CGridViewTestDlg::EnterCellMsflexgrid1()
{
	// TODO: �ڴ˴������Ϣ����������
	if (m_Edit.GetSafeHwnd()!=0 )
		m_Edit.DestroyWindow();

	//��ȡѡ������
	int iSelCol = m_flexGrid.get_Col(); 
	int iSelRow = m_flexGrid.get_Row();
	if(iSelRow ==0 || iSelCol==0)
		return ; // ѡ�б������򷵻�

	m_iSelRow = iSelRow;
	m_iSelCol = iSelCol;

	CreateGridEditBox(); //�����༭�ؼ�

	m_iOldSelRow = m_iSelRow;
	m_iOldSelCol = m_iSelCol;
}

#define  MYEDIE_CTRL 11111 //����༭�ؼ���ԴID
void CGridViewTestDlg::CreateGridEditBox()
{
	// ����༭�ؼ�����λ��
	CClientDC dc(this);
	int x_pixels = dc.GetDeviceCaps(LOGPIXELSX); //x����ÿӢ���������
	int y_pixels = dc.GetDeviceCaps(LOGPIXELSY); //x����ÿӢ���������

	// ��ȡѡ������Ŀ��
	int iRowHeight = m_flexGrid.get_RowHeight(m_iSelRow); 
	int iColWidth  = m_flexGrid.get_ColWidth(m_iSelCol);

	//��ȡѡ���������ʼλ��
	int iPosX = m_flexGrid.get_ColPos(m_iSelCol);
	int iPosy = m_flexGrid.get_RowPos(m_iSelRow);

	//ѡ������ľ��ο������ر�ʾ
	CRect editRect;
	editRect.left = int((iPosX /1440.0)*x_pixels);
	editRect.top  = int((iPosy /1440.0)*y_pixels);


	editRect.right = int(((iPosX  + iColWidth)/1440.0)*x_pixels);
	editRect.bottom = int(((iPosy + iRowHeight)/1440.0)*y_pixels);


	//�����༭�ؼ�����
	m_Edit.Create(ES_AUTOHSCROLL|WS_VISIBLE|WS_BORDER|LVS_NOLABELWRAP,
		editRect,m_flexGrid.FromHandle(m_flexGrid.GetSafeHwnd()),MYEDIE_CTRL);

	m_Edit.SetFont(GetFont());

	//�������ƶ����༭�ؼ���
	m_Edit.SetFocus();

	CString str = m_flexGrid.get_TextMatrix(m_iSelRow,m_iSelCol);
	//����ǰ������ַ��������༭�ؼ�����
	m_Edit.SetWindowText(str);
	m_Edit.SetSel(0,str.GetLength());
}
 LONG CGridViewTestDlg::OnGridDataChanged(UINT wParam,LONG lParam)
{
	CString str;
	m_Edit.GetWindowText(str);
	m_flexGrid.put_TextMatrix(m_iOldSelRow,m_iOldSelCol,str);
	return 1L;
 }
 void CGridViewTestDlg::OnBnClickedButtonExportgrid()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������

	 CString strFilter = "CSV�ļ�(*.csv)|*.csv||";
	 CFileDialog fdlg(FALSE,"","MFC�����������",4|2,strFilter,this);
	 if (fdlg.DoModal()!=IDOK)
		 return;
	 
	 CString strFileName = fdlg.GetPathName();
	 int iRows = m_flexGrid.get_Rows();
	 int iCols = m_flexGrid.get_Cols();

	 char szBuf[500]={0};
	 int iStrLen = strFileName.GetLength();
	 for (int i=0;i< iStrLen;i++)
	 {
		 szBuf[i] = strFileName[i];
	 }

	 FILE *pFile = NULL;
	 pFile = fopen(szBuf,"w+");
	 if (pFile==NULL)
		 return;

	 for (int i=0;i<iRows;i++)
	 {
		 for (int j=0;j<iCols;j++)
		 {
			 CString strVal = m_flexGrid.get_TextMatrix(i,j);
			 fprintf(pFile,"%s",strVal);
			 if( j!=iCols-1 )
				 fprintf(pFile,",");
		 }
		 fprintf(pFile,"\n");
	 }

	 fclose(pFile);

	 MessageBox("����������ݳɹ�!","��ʾ",MB_OK|MB_ICONINFORMATION);
 }

 void StrSplit(const char *pszStr,vector<string> &vStr,const char* strDelim)
 {
	 int iLen = strlen(pszStr) + 1;
	 char * pszCopy = new char[iLen];
	 memset(pszCopy,0,iLen);
	 strcpy_s(pszCopy,iLen,pszStr);
	 char *toKen;
	 char *Out = NULL;
	 toKen = strtok_s(pszCopy,strDelim,&Out);
	 string str = toKen;
	 vStr.push_back(str);
	 while(toKen!=NULL)
	 {
		 toKen = strtok_s(NULL,strDelim,&Out);
		 if (toKen)
		 {	str = toKen;
			vStr.push_back(str);
		 }
	 }
	 delete pszCopy;
 }

 void CGridViewTestDlg::OnBnClickedButtonImportcsv()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������

	 CString strFilter = "CSV�ļ�(*.csv)|*.csv||";
	 CFileDialog fdlg(TRUE,NULL,NULL,4|2,strFilter,this);
	 if (fdlg.DoModal()!=IDOK)
		 return;

	 CString strFileName = fdlg.GetPathName();

	 char szBuf[500]={0};
	 int iStrLen = strFileName.GetLength();
	 for (int i=0;i< iStrLen;i++)
	 {
		 szBuf[i] = strFileName[i];
	 }

 	std::locale::global(std::locale(""));
 	setlocale(LC_ALL, "Chinese-simplified");

	 ifstream ifs(szBuf,ios::in);
	 if (!ifs.is_open())
	 {
		 CString strMsg;
		 strMsg.Format("���ļ���%s��ʧ��!",strFileName);
		 MessageBox(strMsg,"��ʾ",MB_OK|MB_ICONERROR);
		 return;
	 }

	 string strLine;
	 vector<string> vLineStr;
	 while(!ifs.eof())
	 {
		 getline(ifs,strLine,'\n');
		 if (!strLine.empty())
			 vLineStr.push_back(strLine);
	 }

	 std::locale::global(std::locale("C"));
	 ifs.close();


	 typedef vector<string> OneLine;
	 vector<OneLine> vAllLine;
	 int  iMaxColum = 0;
	 int  iCurrentSize = 0;
	 for (vector<string>::iterator iter = vLineStr.begin();iter!=vLineStr.end();iter++)
	 {
		 OneLine Line;
		 StrSplit(iter->c_str(),Line,",");
		 iCurrentSize = Line.size();
		 if (iCurrentSize > iMaxColum)
			 iMaxColum = iCurrentSize;
		 if (iCurrentSize >0)
			 vAllLine.push_back(Line);
	 }

	 vLineStr.clear();

	 int iRowCount = vAllLine.size();
	 int iColCount = iMaxColum;

	 m_flexGrid.put_Rows(iRowCount);
	 m_flexGrid.put_Cols(iColCount);

	 for (int i=0;i< iRowCount ;i++)
	 {
		 for (int j=0;j< iColCount;j++ )
		 {
			 m_flexGrid.put_TextMatrix(i,j,vAllLine[i][j].c_str());
		 }
	 }
 }
