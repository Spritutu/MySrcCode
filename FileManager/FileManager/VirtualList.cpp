// VirtualList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileManager.h"
#include "VirtualList.h"
#include "CodeFunc.h"
#include "FileManagerDlg.h"
#include<algorithm>
using namespace std;
// CVirtualList

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{
}

CListCtrlEx::~CListCtrlEx()
{
	CompareObject::DelCache();
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CListCtrlEx::OnLvnGetdispinfo)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, &CListCtrlEx::OnLvnOdcachehint)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CListCtrlEx::OnLvnColumnclick)
END_MESSAGE_MAP()



// CVirtualList ��Ϣ�������




BOOL CListCtrlEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���
	dwStyle |= (LVS_EX_DOUBLEBUFFER|LVS_REPORT | LVS_OWNERDATA);
	if (!CListCtrl::Create(dwStyle, rect, pParentWnd, nID))
		return FALSE;

	CHeaderCtrl* pHdrCtrl = GetHeaderCtrl(); 
	return TRUE;
}


void CListCtrlEx::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	LV_ITEM* pItem= &(pDispInfo)->item;

	static CFileInfo info; // �����ļ���Ϣ
	static BOOL S_bNeedGetInfo = TRUE;
	int nItem = pItem->iItem;
	if (pItem->mask & LVIF_TEXT)
	{
		int nColum = pItem->iSubItem;
		if (S_bNeedGetInfo)
		{
			map<CString,CFileInfo *>::iterator iter = CompareObject::m_CacheInfo.find(m_strFileArry[nItem]);
			if (iter==CompareObject::m_CacheInfo.end())
			{
				CFileManagerDlg::GetFileInfo(m_strFileArry[nItem],info);
				CFileInfo * pInfoFile = info.Clone();
				CompareObject::m_CacheInfo.insert(make_pair(m_strFileArry[nItem],pInfoFile));
			}
			else
				info = *(iter->second);
			
		}

		if (nColum==0)
			lstrcpyn(pItem->pszText, info.m_strName, pItem->cchTextMax);
		else if (nColum==1)
			lstrcpyn(pItem->pszText, info.m_strDir, pItem->cchTextMax);
		else if (nColum==2)
			lstrcpyn(pItem->pszText, info.m_strFileSize, pItem->cchTextMax);
		else if (nColum==3)
			lstrcpyn(pItem->pszText, info.m_strCreateTime, pItem->cchTextMax);
		else if (nColum==4)
			lstrcpyn(pItem->pszText, info.m_strModifyTime, pItem->cchTextMax);

		if (nColum==4)
			S_bNeedGetInfo = TRUE;
		else
			S_bNeedGetInfo = FALSE;
	}

	//Does the list need image information?
	if( pItem->mask & LVIF_IMAGE)
	{
		if (pItem->iSubItem==0)
			pItem->iImage = info.m_iImageID;
	}
}


BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	 return FALSE;
	 //return CListCtrl::OnEraseBkgnd(pDC);
}


void CListCtrlEx::OnPaint()
{
	//��ӦWM_PAINT��Ϣ
	CPaintDC dc(this); // device context for painting
	CRect rect;
	CRect headerRect;
	CDC MenDC;//�ڴ�ID��  
	CBitmap MemMap;
	GetClientRect(&rect);   
	GetDlgItem(0)->GetWindowRect(&headerRect);  
	MenDC.CreateCompatibleDC(&dc);  
	MemMap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	MenDC.SelectObject(&MemMap);

	MenDC.FillSolidRect(&rect,RGB(255,255,255));  
	//��һ���ǵ���Ĭ�ϵ�OnPaint(),��ͼ�λ����ڴ�DC����  
	DefWindowProc(WM_PAINT,(WPARAM)MenDC.m_hDC,(LPARAM)0);      
	//���  
	dc.BitBlt(0,headerRect.Height(),rect.Width(),  rect.Height(),&MenDC,0, headerRect.Height(),SRCCOPY);  
	MenDC.DeleteDC();
	MemMap.DeleteObject();
}
void CListCtrlEx::InsertAll(int iTemCount,int iColumCount,char ** papszData)
{
	LockWindowUpdate();
	SetRedraw(FALSE);

	m_strFileArry.clear();
	DeleteAllItems();
	CompareObject::DelCache();
	UnlockWindowUpdate();

	if (papszData==NULL)
		return;

	m_strFileArry.resize(iTemCount);
	#pragma omp parallel for
	for (int i=0;i<iTemCount;i++)
	{
		CString strPath = papszData[iColumCount + iColumCount*i];
		strPath = CodeTools::UTF8_TO_MB((LPCTSTR)strPath).c_str();
		m_strFileArry[i] = strPath;
	}
	SetItemCountEx(iTemCount, LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);
	SetRedraw(TRUE);
}
void CListCtrlEx::DrawItem(_In_ LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC); 

	LVITEM lvi = {0}; 
	lvi.mask = LVIF_STATE|LVIF_IMAGE; 
	lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED ; 
	lvi.iItem = lpDrawItemStruct->itemID; 
	BOOL bGet = GetItem(&lvi); 

	BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)||((lvi.state & LVIS_SELECTED) && 
		((GetFocus() == this)|| (GetStyle() & LVS_SHOWSELALWAYS)))); 

	// ���ı����� 
	CRect rcBack = lpDrawItemStruct->rcItem; 
	pDC->SetBkMode(TRANSPARENT); 
	if( lpDrawItemStruct->itemState&ODS_SELECTED && lvi.state & LVIS_SELECTED) //�����ѡ��
	{ 
		COLORREF ref=RGB(0,0,0);
		pDC->SetTextColor(ref); 
		pDC->SetBkColor(ref); 

		pDC->FillRect(rcBack, &CBrush(RGB( 0xff, 0xe4, 0x91 )/*0xEEDED4*/));
	} 
	else 
	{ 

		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->FillRect(rcBack, &CBrush(0xFFFFFF/*0xEEF1E3*/));
	} 

	//����������ɫ 
	CString str; 

	//�õ������С 
	CRect rcFocus = lpDrawItemStruct->rcItem; 
	rcFocus.DeflateRect(1,1,1,1); 

	INT nColumnCount = GetHeaderCtrl()->GetItemCount();

	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) 
	{ 
		//д�ı� 
		CString szText; 
		for (int i = 0; i <nColumnCount; i++) 
		{ //ѭ���õ��ı� 
			CRect rcItem; 
			if ( !GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem )) 
				continue; 
			szText = GetItemText( lpDrawItemStruct->itemID, i ); 

			rcItem.left += 5; rcItem.right -= 1; 
			pDC->DrawText(szText/*szText*/, lstrlen(szText), &rcItem, DT_NOPREFIX|DT_SINGLELINE); 
		}

		CImageList * pImageList = GetImageList(LVSIL_SMALL);
		if (pImageList)
		{
			CRect rcItem; 
			if (GetSubItemRect(lpDrawItemStruct->itemID,0, LVIR_ICON, rcItem ))
			pImageList->Draw(pDC,lvi.iImage,rcItem.TopLeft(),TVSIL_NORMAL); // �����׻���ͼ��
		}
	} 
}
CString CListCtrlEx::GetItemTextEx(int iItem,int iSubItem)
{
	if (iItem <0 ||iItem >= (int)m_strFileArry.size())
		return "";

	static int s_iLastItem = -1;
	static CFileInfo info; // �����ļ���Ϣ
	if (iItem!=s_iLastItem)
	{
		s_iLastItem = iItem;
		CFileManagerDlg::GetFileInfo(m_strFileArry[iItem],info);
	}

	if (iSubItem==0)
		return info.m_strName;
	else if (iSubItem==1)
		return info.m_strDir;
	else if (iSubItem==2)
		return info.m_strFileSize;
	else if (iSubItem==3)
		return info.m_strCreateTime;
	else if (iSubItem==4)
		return info.m_strModifyTime;
	else
		return "";
}

void CListCtrlEx::OnLvnOdcachehint(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCACHEHINT pCacheHint = reinterpret_cast<LPNMLVCACHEHINT>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
void CListCtrlEx::SortItems(int iByColum)
{
	CompareObject SCmpObj(iByColum);
	sort(m_strFileArry.begin(),m_strFileArry.end(),SCmpObj);
	CompareObject::m_iLastSort[iByColum] = !CompareObject::m_iLastSort[iByColum]; //�л�����״̬
}

void CListCtrlEx::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	BeginWaitCursor();
	SortItems(pNMLV->iSubItem);
	CHeaderCtrl* pHdrCtrl = GetHeaderCtrl(); 
	int n = pHdrCtrl->GetItemCount();

	HDITEM hdi;  
	for( int i = 0; i < n; i++ )  
	{  
		hdi.mask = HDI_FORMAT;  // ��ȡ�����ͷͼ��״̬������ָ��  
		pHdrCtrl->GetItem(i, &hdi);  
		//Set sort image to this column
		if(i == pNMLV->iSubItem)
		{
			//Windows xp has a easier way to show the sort order
			//in the header: i just have to set a flag and windows
			//will do the drawing. No windows xp, no easy way.
			hdi.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
			hdi.fmt |= CompareObject::m_iLastSort[i] ? HDF_SORTUP : HDF_SORTDOWN;
		}
		//Remove sort image (if exists)
		//from other columns.
		else
		{
			hdi.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
		}
		pHdrCtrl->SetItem( i, &hdi );  
	} 
	EndWaitCursor();
	Invalidate();
}
int CompareObject::m_iSortByCollum = 0;
map<CString,CFileInfo *> CompareObject::m_CacheInfo;
int CompareObject::m_iLastSort[5];

CompareObject::~CompareObject()
{
	
} 
void CompareObject::DelCache()
{
	 map<CString,CFileInfo *>::iterator iter = m_CacheInfo.begin();
	 for (;iter!=m_CacheInfo.end();iter++)
	 {
		 delete iter->second;
	 }
	 m_CacheInfo.clear();
	 for (int i=0;i<5;i++)
	 {
		 m_iLastSort[i] = 0;
	 }
 }
bool CompareObject::operator()(const CString & str1,const CString &str2)
{
	map<CString,CFileInfo *>::iterator iter = m_CacheInfo.find(str1);

	static CFileInfo * pInfoFile1,*pInfoFile2;
	static CFileInfo info;
	if (iter==m_CacheInfo.end())
	{
		CFileManagerDlg::GetFileInfo(str1,info);
		pInfoFile1 = info.Clone();
		m_CacheInfo.insert(make_pair(str1,pInfoFile1));
	}
	else
		pInfoFile1 = iter->second;

	iter = m_CacheInfo.find(str2);
	if (iter==m_CacheInfo.end())
	{
		CFileManagerDlg::GetFileInfo(str2,info);
		pInfoFile2 = info.Clone();
		m_CacheInfo.insert(make_pair(str2,pInfoFile2));
	}
	else
		pInfoFile2 = iter->second;

	int iCmpResult = 0;
	switch (m_iSortByCollum)
	{
	case 0: // ����
		iCmpResult = pInfoFile1->m_strName.Compare(pInfoFile2->m_strName);
		break;
	case 1: // ·��
		iCmpResult = pInfoFile1->m_strDir.Compare(pInfoFile2->m_strDir);
		break;
	case 2: // ��С
		iCmpResult = pInfoFile1->m_strFileSize.Compare(pInfoFile2->m_strFileSize);
		break;
	case 3: // ����ʱ��
		iCmpResult = pInfoFile1->m_strCreateTime.Compare(pInfoFile2->m_strCreateTime);
		break;
	case 4: // ����ʱ��
		iCmpResult = pInfoFile1->m_strModifyTime.Compare(pInfoFile2->m_strModifyTime);
		break;
	}

	if (m_iLastSort[m_iSortByCollum])
	{
		if (iCmpResult<0)
			return true;

		return false;
	}
	else
	{
		if (iCmpResult>0)
			return true;

		return false;
	}
} 