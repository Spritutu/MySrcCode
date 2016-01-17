// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A4A8C089_0E83_4EA4_A694_CC86361DF43F__INCLUDED_)
#define AFX_STDAFX_H__A4A8C089_0E83_4EA4_A694_CC86361DF43F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

///////////////////////////////////////////////////////////////////////////////////////////////////
#include "OfficeXPMenu.h"
#include <math.h>
#include <wingdi.h>
#include <vector>
using namespace std;
#include "IniFile.h"


struct _tagGetPriceUps
{
	char         price[512];//�۸�
	char         picurl[512];//ͼƬ
	char         title[512];//˵��

} GetPriceUps,*pGetPriceUps;



struct _tagPriceUps
{
	int          shopid;  // ��ID
	int          utfflag;  // UTF��־
	char         shopname[64];//�̵���
	char         shopurl[128];//���ַ
	char         head[64];//ȥͷ
	int          flag1;//��־1
	char         left1[64];//��ؼ���
	char         right1[64];//�ҹؼ���
	int          flag2;//��־2
	char         left2[64];//��ؼ���
	char         right2[64];//�ҹؼ���
	int          flag3;//��־2
	char         left3[64];//��ؼ���
	char         right3[64];//�ҹؼ���
} PriceUps,*pPriceUps;



typedef	struct	tagProductors	//��Ʒ�ṹ����
{
	long nId;
	char  itemName[64]; // �豸����
	char  picpath[256]; // ͼƬ
	char  ipadd[256];   // �豸��ַ
	char  pwd[64];      // ����

}Productors,*pProductors;

typedef struct _tagItemProductors
{
	CBitmap         *pImage;  // ͼ���ڴ�
	Productors      productor;// ����
	CRect           rcPos;     //����ͼƬ����ʾ��λ��
	CRect           rcPosMain;     //�ⴰ�ڵ���ʾ��λ��
}ListItemProductors, *pListItemProductors;


struct _tagLpWnd
{
	LPARAM          lParam;  // ��Ϣ������
	WPARAM          hWnd;//��λ����
	CRect           rcPos;//δ�ƶ�ǰ����ԭ����λ��
	int             nFlag;
	BOOL            bMove;
};



#include <mshtml.h>
#include <afxinet.h>      //����CIneternetSession������




#define WM_ITEM_DEV_SELECT WM_USER+108
#define WM_ITEM_DEV_ON_CHANGE WM_USER+109
#define OPEN_MSG_ID_URL   WM_USER+112 // do other 



#ifndef	COLORREF2RGB
	#define COLORREF2RGB(Color) (Color&0x0000FF00)|((Color>>16)&0x000000FF)|((Color<<16)&0x00FF0000)
#endif
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A4A8C089_0E83_4EA4_A694_CC86361DF43F__INCLUDED_)
