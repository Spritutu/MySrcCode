// MyListBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyListBox.h"


// CMyListBox

IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox()
{
	m_nMaxWidth   =   0;  
}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
END_MESSAGE_MAP()



// CMyListBox ��Ϣ�������

int   CMyListBox::AddString(LPCTSTR   lpszItem)  
{  
	//�˴������ַ�����ȸ��١�ˮƽ��������ʾ�ȴ���  

	int   nRet   =   CListBox::AddString(lpszItem);  

	/*	----   ����������GetScrollInfo()�Ի�ô�ֱ�������������Ϣ����Щ��Ϣ��  
	ͨ��һ��SCROLLINFO�ṹ���ݵģ������ǶԸýṹ��ʼ��������  
	GetScrollInfo()�Ĵ��룺   */ 

	SCROLLINFO   scrollInfo;  
	memset(&scrollInfo,   0,   sizeof(SCROLLINFO));  
	scrollInfo.cbSize   =   sizeof(SCROLLINFO);  
	scrollInfo.fMask     =   SIF_ALL;  
	GetScrollInfo(SB_VERT,   &scrollInfo,   SIF_ALL);  

	/*----   �ڵ������ڹ۲�SCROLLINFO�����Է���Ҫ���nMax��nPage����ȷ��ֵ��  
	�б������Ӧ����һ���ַ�����SCROLLINFO�ĳ�ԱnPage�������б��ÿҳ��  
	�ܹ���ʾ����Ŀ����nMax���б������Ŀ��������nMax���ڻ����nPage����  
	�����˴�ֱ��������������Ҫ֪����ֱ�������Ŀ������ȷ�����б�����Ч  
	��ʾ��ȡ�����ʹ��һ����ʼֵΪ0������nScrollWidth��ʾ�����ڴ�ֱ������  
	��ʾʱ������ֵ��    */

	int   nScrollWidth   =   0;  
	if(GetCount() >1 && ((int)scrollInfo.nMax >= (int)scrollInfo.nPage))  
	{  
		nScrollWidth   =   GetSystemMetrics(SM_CXVSCROLL);  
	}  

	/*----   ����������һ��SIZE����sSize����ʵ�����Ի����CClientDC��    */

	SIZE             sSize;  
	CClientDC   myDC(this);  

	/*----   �Ի��������õ����壬�п�����ȱʡ���壬Ҳ�п�������Ŀ�ĵ�ѡ��  
	�ڶԻ���༭�����һ��Ի��򣬲�ѡ��Properties���Բ鿴��ǰֵ����Ȼ  
	MyDC�Ǵ��б��ȡ�õģ����б��������Ϣ��δ������MyDC�С�Ҳ����˵��  
	�Ի��򴴽�ʱ�������岢û�С�ѡ�롱CClientDC��Ҫ��  
	GetTextExtentPoint32()����������ַ�����С��Ӧ���ȵ���GetFont()��  
	���б���������Ϣ��Ȼ�󽫴�����ѡ��MyDC������Ϊ��    */

	CFont*   pListBoxFont   =   GetFont();  
	if(pListBoxFont   !=   NULL)  
	{  
		CFont*   pOldFont   =    
			myDC.SelectObject(pListBoxFont);  

		/*----   ���ڿ��Ե���GetTextExtendPoint32()����������ַ����Ŀ���ˡ�  
		�ַ����Ŀ����sSize�ṹ��cx��Ա���أ�����ֵ�������������Ƚϣ�*/    

		GetTextExtentPoint32(myDC.m_hDC,    
			lpszItem,   strlen(lpszItem),   &sSize);  
		m_nMaxWidth   =   max(m_nMaxWidth,   (int)sSize.cx);  

		/*----   ʣ�µ���Ҫ����֮һ����������ˮƽ�������Ĵ�С�ˡ������ͨ����  
		��SetHorizontalExtent()��ɡ�������ݸ��������β������б������С��  
		��ˮƽ�����������ء�    

		----   ������һ�����ױ����Եĵط��������ϸ�۲�CListBox�����Է�����  
		�������һ��СС�Ŀհף����Ĵ�СΪ3   ���ⲿ�ֿ��Ӧ�üӵ��ı����  
		�ϡ����ϣ�����ı��ұ�Ҳͬ���ճ�һ������������ı�������ټ�3�� */   

		SetHorizontalExtent(m_nMaxWidth   +   3);  

		/*----   �ڽ���֮ǰ��������ҪΪMyDCѡ��ԭ�����塣ԭ�����屣����pOldFont  
		�У�   */ 

		myDC.SelectObject(pOldFont);  
	}  

	return   nRet;  
}  
void   CMyListBox::ResetContent()  
{  
	CListBox::ResetContent();  

	m_nMaxWidth   =   0;  
	SetHorizontalExtent(0);  
}   

