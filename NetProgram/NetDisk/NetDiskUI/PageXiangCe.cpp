// PageXiangCe.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetDiskUI.h"
#include "PageXiangCe.h"
#include "afxdialogex.h"


// CPageXiangCe �Ի���

IMPLEMENT_DYNAMIC(CPageXiangCe, CDialogEx)

CPageXiangCe::CPageXiangCe(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageXiangCe::IDD, pParent)
{

}

CPageXiangCe::~CPageXiangCe()
{
}

void CPageXiangCe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageXiangCe, CDialogEx)
END_MESSAGE_MAP()


// CPageXiangCe ��Ϣ�������


BOOL CPageXiangCe::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
