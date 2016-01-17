#include "stdafx.h"
#include "UserDefine.h"
#include <GdiplusBitmap.h>
using namespace Gdiplus;

 const char * g_strTitleJianMo = "��ģ����";
 const char * g_strTitleWorkHistory = "��ҵ����";

Image * LoadImage(LPCTSTR lpszPath)
{
	CStringW strPathW;
	CString strPath = lpszPath == NULL ? "" : lpszPath;

	strPathW = strPath;

	Image * pImg = Image::FromFile( (WCHAR *)(LPCWCH)strPathW );
	if (pImg == NULL) return NULL;
	return pImg;
}
Bitmap * GetNewBitmap(LPCTSTR lpszPath)
{
	Image * pImg = LoadImage(lpszPath);
	if (pImg == NULL) return NULL;
	if (pImg->GetHeight() < 1 || pImg->GetWidth() < 1)
	{
		delete pImg; pImg = NULL;
		return NULL;
	}

	Bitmap * pResult = new Bitmap( pImg->GetWidth(), pImg->GetHeight(), PixelFormat32bppARGB );
	if (pResult == NULL)
	{
		delete pImg; pImg = NULL;
		return NULL;
	}
	Graphics g(pResult);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetPixelOffsetMode(PixelOffsetModeHighQuality);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	Point ImgPoints[] = {
		Point(0, 0) //ԭ��
		, Point(pImg->GetWidth(), 0)
		, Point(0, pImg->GetHeight())
	};
	g.DrawImage(pImg, ImgPoints, 3);

	delete pImg; pImg = NULL;
	return pResult;
}