// RsProcDoc.cpp : implementation of the CRsProcDoc class
//

#include "stdafx.h"
#include "ChildFrm.h"
#include "RsProc.h"
#include "RsProcView.h"
#include "HistDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRsProcDoc

IMPLEMENT_DYNCREATE(CRsProcDoc, CDocument)

BEGIN_MESSAGE_MAP(CRsProcDoc, CDocument)
	//{{AFX_MSG_MAP(CRsProcDoc)
	ON_COMMAND(ID_HIST_SHOW, OnHistShow)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SAVE_AS, &CRsProcDoc::OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, &CRsProcDoc::OnFileSave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRsProcDoc construction/destruction

CRsProcDoc::CRsProcDoc()
{
	// TODO: add one-time construction code here
	m_nbands=0;
	m_nHeigth=0;
	m_nWidth=0;
	for (int i=0;i<3;i++)
	{
		max[i]=0;
		max_grey[i]=0;
		min_grey[i]=0;
		for (int j=0;j<256;j++)
		{
			count[i][j]=0;
		}
	}
}

CRsProcDoc::~CRsProcDoc()
{
}

BOOL CRsProcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRsProcDoc serialization

void CRsProcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRsProcDoc diagnostics

#ifdef _DEBUG
void CRsProcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRsProcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
/**
*	@brief	 ����OnOpenDocument���ļ�
*	@param	 lpszPathName  �򿪵��ļ�·����
*	@return	 ���ش��ĵ��Ƿ�ɹ�,�ɹ���ΪTRUE,����ΪFALSE
*/
BOOL CRsProcDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO: Add your specialized creation code here
	CFile file;
	CFileException fe;
	if (!file.Open(lpszPathName,CFile::modeRead|CFile::shareDenyWrite,&fe))
	{
		ReportSaveLoadException(lpszPathName,&fe,FALSE,AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}

	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	poDataset=(GDALDataset*)GDALOpen(lpszPathName,GA_ReadOnly);
	if (poDataset==NULL)
	{
		AfxMessageBox("��դ�����ݼ�ʧ��");
		return FALSE;
	}
	m_nbands=poDataset->GetRasterCount();
	pbands=new GDALRasterBand *[m_nbands];  //���벨��ָ�����飿
	if (pbands==NULL)
	{
		AfxMessageBox("��ò������ݼ�ʧ��");
		return FALSE;
	}
	for (int i=0;i<m_nbands;i++)
	{
		pbands[i]=poDataset->GetRasterBand(i+1);
	}
	//��ȡͼ��Ŀ�Ⱥ͸߶�
	m_nWidth=pbands[0]->GetXSize();
	m_nHeigth=pbands[0]->GetYSize();
	if (m_nbands==1)
	{
		for (int i=0;i<3;i++)
		{
			pBandBlock[i]=(BYTE*)CPLMalloc(sizeof(BYTE)*m_nHeigth*m_nWidth);
			pbands[0]->RasterIO(GF_Read,0,0,m_nWidth,m_nHeigth,pBandBlock[i],
				m_nWidth,m_nHeigth,GDT_Byte,0,0);
		}
	}
	if (m_nbands!=1)
	{
		for (int i=0;i<m_nbands;i++)
		{
			pBandBlock[i]=(BYTE*)CPLMalloc(sizeof(BYTE)*m_nHeigth*m_nWidth);
			pbands[i]->RasterIO(GF_Read,0,0,m_nWidth,m_nHeigth,pBandBlock[i],
				m_nWidth,m_nHeigth,GDT_Byte,0,0);
		}	
	}
	Calculate();
	UpdateAllViews(NULL);
	return TRUE;
}
/************************************************************************/
/* ͳ�ƻҶ���Ϣ������ֱ��ͼ��ʾ                                                  */
/************************************************************************/
void CRsProcDoc::Calculate()
{
	for (int i=0;i<m_nHeigth;i++)
	{
		for (int j=0;j<m_nWidth;j++)
		{
			count[0][pBandBlock[0][i*m_nWidth+j]]++;
			count[1][pBandBlock[1][i*m_nWidth+j]]++;
			count[2][pBandBlock[2][i*m_nWidth+j]]++;

			if (pBandBlock[0][i*m_nWidth+j]>max_grey[0])
			{
				max_grey[0]=pBandBlock[0][i*m_nWidth+j];
			}
			if (pBandBlock[1][i*m_nWidth+j]>max_grey[1])
			{
				max_grey[1]=pBandBlock[1][i*m_nWidth+j];
			}
			if (pBandBlock[2][i*m_nWidth+j]>max_grey[2])
			{
				max_grey[2]=pBandBlock[2][i*m_nWidth+j];
			}
			if (pBandBlock[0][i*m_nWidth+j]<min_grey[0])
			{
				min_grey[0]=pBandBlock[0][i*m_nWidth+j];
			}
			if (pBandBlock[1][i*m_nWidth+j]<min_grey[1])
			{
				min_grey[1]=pBandBlock[1][i*m_nWidth+j];
			}
			if (pBandBlock[2][i*m_nWidth+j]<min_grey[2])
			{
				min_grey[2]=pBandBlock[2][i*m_nWidth+j];
			}
		}
	}
	for (int k=0;k<256;k++)
	{
		if (count[0][k]>max[0])
		{
			max[0]=count[0][k];
		}
		if (count[1][k]>max[1])
		{
			max[1]=count[1][k];
		}
		if (count[2][k]>max[2])
		{
			max[2]=count[2][k];
		}
	}
}
void CRsProcDoc::OnHistShow() 
{
	// TODO: Add your command handler code here	
	CHistDlg *pdlg=new CHistDlg();
	pdlg->Create(IDD_HIST_DLG);
	pdlg->ShowWindow(SW_SHOW);
	pdlg->CenterWindow();
}


void CRsProcDoc::OnFileSaveAs()
{
	// TODO: �ڴ���������������
	const char* szFilter="BMP Files(*.BMP)|*.bmp|GeoTif Files(*.tif)|*.tif|IMG File(*.img)|*.img||";
	CFileDialog filedlg(false,"bmp",NULL,OFN_OVERWRITEPROMPT,szFilter);
	const char *pszFormat=NULL;
	CString FileSaveAsPathName;
	if (filedlg.DoModal()==IDOK)
	{
		CString ext=filedlg.GetFileExt();
		ext.MakeLower();//ת��ΪСд
		if (ext=="bmp")
			pszFormat="BMP";
		else if (ext=="tif")
			pszFormat="GTiff";
		else if (ext=="img")
			pszFormat="HFA";
		else if (ext=="jpg")
			pszFormat="JPEG";
		else
			pszFormat="GTiff";
		FileSaveAsPathName=filedlg.GetPathName();
	}
	else
		return;
	GDALDataset *poDsDataSet;
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8","NO");
	GDALDriver *poDriver;
	poDriver=GetGDALDriverManager()->GetDriverByName(pszFormat);
	if (poDriver==NULL)
	{
		AfxMessageBox("����ļ���ʽ��֧��!");
		return;
	}

	poDsDataSet=poDriver->Create(FileSaveAsPathName,m_nWidth,m_nHeigth,
		m_nbands,pbands[0]->GetRasterDataType(),NULL);

	if (poDsDataSet==NULL)
	{
		AfxMessageBox("����Ŀ���ļ�ʧ��!");
		return;
	}
	for (int i=0;i<m_nbands;i++)
	{
		GDALRasterBand *poband;
		poband=poDsDataSet->GetRasterBand(i+1);
		poband->RasterIO(GF_Write,0,0,m_nWidth,m_nHeigth,pBandBlock[i],
			m_nWidth,m_nHeigth,pbands[i]->GetRasterDataType(),0,0);
	}
	GDALClose((GDALDatasetH)poDsDataSet);
}
/************************************************************************/
/*  ����1: CopyScreenToBitmap //����Ļָ��������ͼƬ                                                                    */
/************************************************************************/
HBITMAP CRsProcDoc::CopyScreenToBitmap(LPRECT lpRect) //lpRect ����ѡ������
{
	HDC hScrDC, hMemDC;      
	// ��Ļ���ڴ��豸������
	HBITMAP hBitmap,hOldBitmap;   
	// λͼ���
	int       nX, nY, nX2, nY2;      
	// ѡ����������
	int       nWidth, nHeight;      
	// λͼ��Ⱥ͸߶�
	int       xScrn, yScrn;         
	// ��Ļ�ֱ���
	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
		return NULL;
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	// �����Ļ�ֱ���
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//ȷ��ѡ�������ǿɼ���
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > xScrn)
		nX2 = xScrn;
	if (nY2 > yScrn)
		nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap=CreateCompatibleBitmap(hScrDC,nWidth,nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	BitBlt(hMemDC,0,0, nWidth,nHeight,hScrDC, nX, nY, SRCCOPY);
	//�õ���Ļλͼ�ľ��
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// ����λͼ���
	return hBitmap;
}
/************************************************************************/
/*   ����2 : SaveBitmapToFile ��ͼƬ����ļ�                                                                    */
/************************************************************************/
int CRsProcDoc::SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName) //hBitmap Ϊ�ղŵ���Ļλͼ���
{      //lpFileName Ϊλͼ�ļ���
	HDC     hDC;         
	//�豸������
	int     iBits;      
	//��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���
	WORD    wBitCount;   
	//λͼ��ÿ��������ռ�ֽ���
	//�����ɫ���С�� λͼ�������ֽڴ�С ��  λͼ�ļ���С �� д���ļ��ֽ���
	DWORD           dwPaletteSize=0,dwBmBitsSize,dwDIBSize, dwWritten;
	BITMAP          Bitmap;        
	//λͼ���Խṹ
	BITMAPFILEHEADER   bmfHdr;        
	//λͼ�ļ�ͷ�ṹ
	BITMAPINFOHEADER   bi;            
	//λͼ��Ϣͷ�ṹ 
	LPBITMAPINFOHEADER lpbi;          
	//ָ��λͼ��Ϣͷ�ṹ
	HANDLE          fh, hDib, hPal;
	HPALETTE     hOldPal=NULL;
	//�����ļ��������ڴ�������ɫ����

	//����λͼ�ļ�ÿ��������ռ�ֽ���
	hDC = CreateDC("DISPLAY",NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * 
		GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		wBitCount = 32;
	//�����ɫ���С
	if (wBitCount <= 8)
		dwPaletteSize=(1<<wBitCount)*sizeof(RGBQUAD);

	//����λͼ��Ϣͷ�ṹ
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = Bitmap.bmWidth;
	bi.biHeight          = Bitmap.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount         = wBitCount;
	bi.biCompression      = BI_RGB;
	bi.biSizeImage        = 0;
	bi.biXPelsPerMeter     = 0;
	bi.biYPelsPerMeter     = 0;
	bi.biClrUsed         = 0;
	bi.biClrImportant      = 0;

	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)/32)*4*Bitmap.bmHeight;
	//Ϊλͼ���ݷ����ڴ�

	/*xxxxxxxx����λͼ��С�ֽ�һ��(����һ����������)xxxxxxxxxxxxxxxxxxxx 
	//ÿ��ɨ������ռ���ֽ���Ӧ��Ϊ4���������������㷨Ϊ:
	int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
	if((Bitmap.bmWidth*wBitCount) % 32)
	biWidth++; //�����������ļ�1
	biWidth *= 4;//���������õ���Ϊÿ��ɨ���е��ֽ�����
	dwBmBitsSize = biWidth * Bitmap.bmHeight;//�õ���С
	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/


	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// �����ɫ��   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	// ��ȡ�õ�ɫ�����µ�����ֵ
	GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	//�ָ���ɫ��   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//����λͼ�ļ�    
	fh=CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh==INVALID_HANDLE_VALUE)
		return FALSE;
	// ����λͼ�ļ�ͷ
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
	// д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// д��λͼ�ļ���������
	WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize , &dwWritten, NULL); 
	//���   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}
void CRsProcDoc::OnFileSave()
{
	// TODO: �ڴ���������������
}

