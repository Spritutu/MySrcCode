// VirtualList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VirtualList.h"
#include<algorithm>
using namespace std;
// CVirtualList

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
{
}

CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CListCtrlEx::OnLvnGetdispinfo)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(LVN_ODCACHEHINT, &CListCtrlEx::OnLvnOdcachehint)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CListCtrlEx::OnLvnColumnclick)
	ON_NOTIFY_REFLECT(NM_CLICK, &CListCtrlEx::OnNMClick)
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()



// CVirtualList ��Ϣ�������




BOOL CListCtrlEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���
	dwStyle |= (LVS_EX_DOUBLEBUFFER|LVS_REPORT | LVS_OWNERDATA |LVS_OWNERDRAWFIXED);
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

	int nItem = pItem->iItem;
	CFileInfo &info = m_FileInfoArry[nItem]; 
	if (pItem->mask & LVIF_TEXT)
	{
		int nColum = pItem->iSubItem;
		if (nColum==0)
			lstrcpyn(pItem->pszText, info.m_strName, pItem->cchTextMax);
		else if (nColum==1)
			lstrcpyn(pItem->pszText, info.m_strModifyTime, pItem->cchTextMax);
		else if (nColum==2)
			lstrcpyn(pItem->pszText, info.m_strType, pItem->cchTextMax);
		else if (nColum==3)
			lstrcpyn(pItem->pszText, info.m_strFileSize, pItem->cchTextMax);
		else if (nColum==4)
			lstrcpyn(pItem->pszText, info.m_strProgressText, pItem->cchTextMax);
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
void CListCtrlEx::InsertAll(const vector<CFileInfo> & vInfo )
{
	/*LockWindowUpdate();
	SetRedraw(FALSE);
*/
	DeleteAllItems();
	//UnlockWindowUpdate();
	m_FileInfoArry.insert(m_FileInfoArry.begin(),vInfo.begin(),vInfo.end());
	SetItemCountEx(m_FileInfoArry.size(), LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);
	//SetRedraw(TRUE);
}
 void CListCtrlEx::InsertItemEx(const CFileInfo & info)
 {
	/* LockWindowUpdate();
	 SetRedraw(FALSE);
*/
	 DeleteAllItems();
	// UnlockWindowUpdate();
	 m_FileInfoArry.push_back(info);
	 SetItemCountEx(m_FileInfoArry.size(), LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);

	 //SetRedraw(TRUE);
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
		LOGFONT lf;
		pDC->GetCurrentFont()->GetLogFont(&lf);

		lf.lfCharSet = GB2312_CHARSET;
		lf.lfHeight = 12;
		lf.lfWidth  = 0;
		lf.lfWeight = FW_NORMAL;
		strcpy_s(lf.lfFaceName,"����");

		CFont font;
		font.CreateFontIndirect(&lf);
		CFont *pOldFont = pDC->SelectObject(&font);

		//д�ı� 
		CString szText; 
		CSize si;
		for (int i = 0; i <nColumnCount; i++) 
		{ //ѭ���õ��ı� 
			CRect rcItem; 
			if ( !GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem )) 
				continue; 
			szText = GetItemText( lpDrawItemStruct->itemID, i ); 

			si = pDC->GetTextExtent(szText);
			rcItem.left += 5; rcItem.right -= 1;
			rcItem.top = (rcItem.bottom  + rcItem.top - si.cy)/2;
			pDC->DrawText(szText/*szText*/, lstrlen(szText), &rcItem, DT_NOPREFIX|DT_SINGLELINE); 
		}

		pDC->SelectObject(pOldFont);

		CImageList * pImageList = GetImageList(LVSIL_SMALL);
		if (pImageList)
		{
			CRect rcItem; 
			if (GetSubItemRect(lpDrawItemStruct->itemID,0, LVIR_ICON, rcItem ))
			{
				rcItem.top += 3;
				pImageList->Draw(pDC,lvi.iImage,rcItem.TopLeft(),TVSIL_NORMAL); // �����׻���ͼ��
			}
		}
	} 
}
CString CListCtrlEx::GetItemTextEx(int iItem,int iSubItem)
{
	if (iItem <0 ||iItem >= (int)m_FileInfoArry.size())
		return "";

	int nColum = iSubItem;
	if (nColum==0)
		return m_FileInfoArry[iItem].m_strName;
	else if (nColum==1)
		return m_FileInfoArry[iItem].m_strModifyTime;
	else if (nColum==2)
		return m_FileInfoArry[iItem].m_strType;
	else if (nColum==3)
		return m_FileInfoArry[iItem].m_strFileSize;;

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
	sort(m_FileInfoArry.begin(),m_FileInfoArry.end(),SCmpObj);
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
bool CompareObject::operator()(const CFileInfo & str1,const CFileInfo &str2)
{
	return false;
} 

void CListCtrlEx::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
void CListCtrlEx::UpdateProgress(double dProgress,CString strOperation,CString strTaskKey)
{
	for (size_t i=0;i<m_FileInfoArry.size();i++)
	{
		CString strKey = m_strNetDiskDir  + m_FileInfoArry[i].m_strName;
		if (m_FileInfoArry[i].m_strType == "�ļ���")
			strKey += "/";

		if (strKey.Compare(strTaskKey)==0)
		{
			int iProgress = int(dProgress*100);
			m_FileInfoArry[i].m_strProgressText.Format("����%s(%d%%)",strOperation,iProgress);
			break;
		}
	}
}
void  CListCtrlEx:: ClearProgressMsg(CString strTaskKey)
{
	for (size_t i=0;i<m_FileInfoArry.size();i++)
	{
		CString strKey = m_strNetDiskDir  + m_FileInfoArry[i].m_strName;
		if (m_FileInfoArry[i].m_strType == "�ļ���")
			strKey += "/";

		if (strKey.Compare(strTaskKey)==0)
		{
			m_FileInfoArry[i].m_strProgressText.Empty();
			break;
		}
	}
}
void CListCtrlEx::SetProgressText(CString strText,CString strTaskKey)
{
	for (size_t i=0;i<m_FileInfoArry.size();i++)
	{
		CString strKey = m_strNetDiskDir + m_FileInfoArry[i].m_strName;
		if (m_FileInfoArry[i].m_strType == "�ļ���")
			strKey += "/";

		if (strKey.Compare(strTaskKey)==0)
		{
			m_FileInfoArry[i].m_strProgressText = strText;
			break;
		}
	}
}
void CListCtrlEx::SetNetDiskDir(CString strDir)
{
	m_strNetDiskDir = strDir;
}
void CListCtrlEx::SetItemFileId(CString strFileName,CString strFileId)
{
	for (size_t i=0;i<m_FileInfoArry.size();i++)
	{
		if (m_FileInfoArry[i].m_strName.Compare(strFileName)==0)
		{
			m_FileInfoArry[i].m_strFileId = strFileId;
			break;
		}
	}
}

BOOL CListCtrlEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY   *pHDNotify   =   (HD_NOTIFY*)lParam;   

	switch(   pHDNotify->hdr.code   )   // ���Ƹ����п����СΪ100
	{   
	case   HDN_ITEMCHANGINGA:   
	case   HDN_ITEMCHANGINGW:   
	case   HDN_ENDTRACK:   
		{
			if(   pHDNotify->pitem->mask   &   HDI_WIDTH   &&   
					pHDNotify->pitem->cxy   <   100   &&   
					pHDNotify->pitem->cxy   >=   0   )   
				{   
					pHDNotify->pitem->cxy   =   100; //   Set   the   column   width  
				} 
			else if (pHDNotify->pitem->mask   &   HDI_WIDTH   &&
				  pHDNotify->pitem->cxy > 500)
			{
				pHDNotify->pitem->cxy   =   500;
			}
		}
		break;
	}
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

void CListCtrlEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (lpMeasureItemStruct->itemHeight > 0)
		lpMeasureItemStruct->itemHeight = 23;
}