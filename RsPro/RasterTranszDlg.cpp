// RasterTranszDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RsProc.h"
#include "RasterTranszDlg.h"

// CRasterTranszDlg �Ի���

IMPLEMENT_DYNAMIC(CRasterTranszDlg, CDialog)

CRasterTranszDlg::CRasterTranszDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRasterTranszDlg::IDD, pParent)
	, m_strInFile(_T(""))
	, m_strOutFile(_T(""))
{

}

CRasterTranszDlg::~CRasterTranszDlg()
{
}

void CRasterTranszDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Input, m_strInFile);
	DDX_Text(pDX, IDC_EDIT_Output, m_strOutFile);
	DDX_Control(pDX, IDC_EDIT_Input, m_CtrEdit_Input);
	DDX_Control(pDX, IDC_EDIT_Output, m_CtrEditOutPut);
}


BEGIN_MESSAGE_MAP(CRasterTranszDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CRasterTranszDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CRasterTranszDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDOK, &CRasterTranszDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRasterTranszDlg ��Ϣ�������

void CRasterTranszDlg::OnBnClickedBtnOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	const char* szFilter="BMP Files(*.BMP)|*.bmp|GeoTif Files(*.tif)|*.tif|IMG File(*.img)|*.img|Raster Matrix Format(*.rsw)|*.rsw|Raster Matrix Format(*.mtw)|*mtw||";
	CFileDialog dlg(true,"bmp",NULL,OFN_OVERWRITEPROMPT,szFilter);

	if (dlg.DoModal()!=IDOK)
		return;
	m_strInFile=dlg.GetPathName();
	m_CtrEdit_Input.SetWindowText(m_strInFile);

}

void CRasterTranszDlg::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	const char* szFilter="BMP Files(*.BMP)|*.bmp|GeoTif Files(*.tif)|*.tif|IMG File(*.img)|*.img|Raster Matrix Format(*.rsw)|*.rsw|Raster Matrix Format(*.mtw)|*mtw||";
	CFileDialog dlg(false,"bmp",NULL,OFN_OVERWRITEPROMPT,szFilter);

	const char *pszFormat=NULL;

	if (dlg.DoModal()!=IDOK)
		return;
	CString ext=dlg.GetFileExt();
	ext.MakeLower();//ת��ΪСд
	if (ext=="bmp")
	{
		pszFormat="BMP";
	}
	if (ext=="tif")
	{
		pszFormat="GTiff";
	}
	if (ext=="img")
	{
		pszFormat="HFA";
	}
	if (ext=="jpg")
	{
		pszFormat="JPEG";
	}
	else
		pszFormat="BMP";
	m_strFormat=pszFormat;
	m_strOutFile=dlg.GetPathName();
	m_CtrEditOutPut.SetWindowText(m_strOutFile);
}

void CRasterTranszDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CProgressWnd *pro=new CProgressWnd(NULL,"����",FALSE);  
	pro->SetCurrentWnd(pro);

	int irev=RasterTrans(m_strInFile,m_strOutFile,m_strFormat,pro);
	if (irev==0)
	{
		MessageBox("��ʽת���ɹ���","��ʾ",MB_OK);
	}
	OnOK();
}
