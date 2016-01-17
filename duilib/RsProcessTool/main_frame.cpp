#include "stdafx.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif

/**
* @brief ��ȫ��������Ϊ����ϵͳĬ������
*/
#define SET_LOCAL	{ locale::global(locale("")); setlocale(LC_ALL,"Chinese-simplified"); }
/**
* @brief ��ԭȫ�������趨
*/
#define REVERT_LOCAL	locale::global(locale("C"))


#include "UIGroups.hpp"
#include "UIMicroBlog.hpp"

#include "win_impl_base.hpp"
#include "main_frame.hpp"

#include "color_skin.hpp"
#include "chat_dialog.hpp"
#include "Py.h"

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

const TCHAR* const kTabControlName = _T("tabs");

const TCHAR* const kFriendButtonControlName = _T("friendbtn");
const TCHAR* const kGroupButtonControlName = _T("groupbtn");
const TCHAR* const kMicroBlogButtonControlName = _T("microblogbtn");

const TCHAR* const kFriendsListControlName = _T("friends");
const TCHAR* const kGroupsListControlName = _T("groups");
const TCHAR* const kMicroBlogListControlName = _T("microblog");

const TCHAR* const kHideLeftMainPannelControlName = _T("btnleft");
const TCHAR* const kShowLeftMainPannelControlName = _T("btnright");
const TCHAR* const kLeftMainPannelControlName = _T("LeftMainPanel");

const TCHAR* const kSignatureTipsControlName = _T("signaturetip");
const TCHAR* const kSignatureControlName = _T("signature");

const TCHAR* const kSearchEditTipControlName = _T("search_tip");
const TCHAR* const kSearchEditControlName = _T("search_edit");

const TCHAR* const kChangeBkSkinControlName = _T("bkskinbtn");
const TCHAR* const kChangeColorSkinControlName = _T("colorskinbtn");

const TCHAR* const kBackgroundControlName = _T("bg");

const int kBackgroundSkinImageCount = 3;

MainFrame::MainFrame()
: bk_image_index_(2)
{}

MainFrame::~MainFrame()
{
	PostQuitMessage(0);
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendList")) == 0)
	{
		return new CFriendsUI(paint_manager_);
	}
	else if (_tcsicmp(pstrClass, _T("GroupList")) == 0)
	{
		return new CGroupsUI(paint_manager_);
	}
	else if (_tcsicmp(pstrClass, _T("MicroBlog")) == 0)
	{
		return new CMicroBlogUI(paint_manager_);
	}

	return NULL;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

tString MainFrame::GetSkinFile()
{
	return _T("main_frame.xml");
}

tString MainFrame::GetSkinFolder()
{
	return tString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT MainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnTimer(TNotifyUI& msg)
{
}

void MainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}

void MainFrame::Init()
{}

DWORD MainFrame::GetBkColor()
{
	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
	if (background != NULL)
		return background->GetBkColor();

	return 0;
}

void MainFrame::SetBkColor(DWORD dwBackColor)
{
	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		background->SetBkImage(_T(""));
		background->SetBkColor(dwBackColor);
		background->NeedUpdate();

		SkinChangedParam param;
		param.bkcolor = background->GetBkColor();
		param.bgimage = background->GetBkImage();
		skin_changed_observer_.Broadcast(param);
	}
}

void MainFrame::UpdateFriendsList()
{
	CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(paint_manager_.FindControl(kFriendsListControlName));
	if (!pFriendsList)
		return;

	if (!friends_.empty())
		friends_.clear();
	if (pFriendsList->GetCount() > 0)
		pFriendsList->RemoveAll();

	int id  = 0;

	CPy py;
	for (int i=0;i< GetProductGroupCount();i++)
	{
		CProductGroup group = GetProductGroup(i);
		FriendListItemInfo item;
		char szId[100];
		item.id = _itoa(id,szId,10);;
		item.folder = true;
		item.empty = false;
		item.nick_name = group.GetGroupTitle().c_str();
		Node* GroupNode = pFriendsList->AddNode(item, NULL);
		friends_.push_back(item);
		for (int j=0;j<group.GetGroupItemCount();j++)
		{
			ProductStruct product = group.GetGroupItem(j);
			item.id = _T("1");
			item.folder = false;		
			item.logo = _T("man.png");
			item.nick_name = product.m_StrTitle.c_str();
			item.description = product.m_StrToolTip;
			item.strFirstLetter = py.MakeSpellCode(item.nick_name,0);
			item.strFullPy = py.MakeSpellCode(item.nick_name,1);

			pFriendsList->AddNode(item, GroupNode);
			friends_.push_back(item);
			id++;
		}
		id++;
	}
}

void MainFrame::UpdateGroupsList()
{
	CGroupsUI* pGroupsList = static_cast<CGroupsUI*>(paint_manager_.FindControl(kGroupsListControlName));
	if (pGroupsList == NULL) return;

	if (pGroupsList->GetCount() > 0)
		pGroupsList->RemoveAll();

	for ( size_t i=0;i<searchResultList.size();i++)
	{
		FriendListItemInfo info = searchResultList[i];
		GroupsListItemInfo item;

		item.folder = info.folder;
		item.empty = info.empty;
		item.id = info.id;
		item.logo = info.logo;
		item.nick_name = info.nick_name;
		item.description = info.description;
		item.strFirstLetter = info.strFirstLetter;
		item.strFullPy = info.strFullPy;
		Node* root_parent = pGroupsList->AddNode(item, NULL);
	}
}

void MainFrame::UpdateMicroBlogList()
{
	CMicroBlogUI* pMicroBlogList = static_cast<CMicroBlogUI*>(paint_manager_.FindControl(kMicroBlogListControlName));
	if (pMicroBlogList != NULL)
	{}
}

void MainFrame::OnPrepare(TNotifyUI& msg)
{
	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#endif
		background->SetBkImage(szBuf);
	}

	UpdateFriendsList();

	UpdateGroupsList();

	UpdateMicroBlogList();

	BuildSearchKey();
}

void MainFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kSignatureControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* signature_tip = paint_manager_.FindControl(kSignatureTipsControlName);
			if (signature_tip != NULL)
			{
				CRichEditUI* signature = static_cast<CRichEditUI*>(msg.pSender);
				if (signature != NULL)
					signature_tip->SetText(signature->GetText());
				signature_tip->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* search_tip = static_cast<CRichEditUI*>(paint_manager_.FindControl(kSearchEditTipControlName));
			if (search_tip != NULL)
			{
				CRichEditUI* search_edit = static_cast<CRichEditUI*>(msg.pSender);
				if (search_edit != NULL)
					search_tip->SetText(search_edit->GetText());
				search_tip->SetVisible(true);
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
		{
			OnExit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
#else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_RESTORE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
#else
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kHideLeftMainPannelControlName) == 0)
		{
			CControlUI* left_main_pannel = paint_manager_.FindControl(kLeftMainPannelControlName);
			CControlUI* hide_left_main_pannel = paint_manager_.FindControl(kHideLeftMainPannelControlName);
			CControlUI* show_left_main_pannel = paint_manager_.FindControl(kShowLeftMainPannelControlName);
			if ((left_main_pannel != NULL) && (show_left_main_pannel != NULL) && (hide_left_main_pannel != NULL))
			{
				hide_left_main_pannel->SetVisible(false);
				left_main_pannel->SetVisible(false);
				show_left_main_pannel->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kShowLeftMainPannelControlName) == 0)
		{
			CControlUI* left_main_pannel = paint_manager_.FindControl(kLeftMainPannelControlName);
			CControlUI* hide_left_main_pannel = paint_manager_.FindControl(kHideLeftMainPannelControlName);
			CControlUI* show_left_main_pannel = paint_manager_.FindControl(kShowLeftMainPannelControlName);
			if ((left_main_pannel != NULL) && (show_left_main_pannel != NULL) && (hide_left_main_pannel != NULL))
			{
				hide_left_main_pannel->SetVisible(true);
				left_main_pannel->SetVisible(true);
				show_left_main_pannel->SetVisible(false);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSignatureTipsControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* signature = static_cast<CRichEditUI*>(paint_manager_.FindControl(kSignatureControlName));
			if (signature != NULL)
			{
				signature->SetText(msg.pSender->GetText());
				signature->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditTipControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* search_edit = static_cast<CRichEditUI*>(paint_manager_.FindControl(kSearchEditControlName));
			if (search_edit != NULL)
			{
				search_edit->SetText(msg.pSender->GetText());
				search_edit->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeBkSkinControlName) == 0)
		{
			CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
			if (background != NULL)
			{
				TCHAR szBuf[MAX_PATH] = {0};
				++bk_image_index_;
				if (kBackgroundSkinImageCount < bk_image_index_)
					bk_image_index_ = 0;

#if defined(UNDER_WINCE)
				_stprintf(szBuf, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#else
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='bg%d.png' corner='600,200,1,1'"), bk_image_index_);
#endif
				background->SetBkImage(szBuf);

				SkinChangedParam param;
				CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
				if (background != NULL)
				{
					param.bkcolor = background->GetBkColor();
					if (_tcslen(background->GetBkImage()) > 0)
					{
#if defined(UNDER_WINCE)
						_stprintf(szBuf, _T("bg%d.png"), bk_image_index_);
#else
						_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.png"), bk_image_index_);
#endif
					}

					param.bgimage = szBuf;
				}
				skin_changed_observer_.Broadcast(param);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeColorSkinControlName) == 0)
		{
			CRect rcWindow;
			GetWindowRect(m_hWnd, &rcWindow);
			rcWindow.top = rcWindow.top + msg.pSender->GetPos().bottom;
			new ColorSkinWindow(this, rcWindow);
		}
	}
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kTabControlName));
		if (_tcsicmp(msg.pSender->GetName(), kFriendButtonControlName) == 0)
		{
			if (pTabControl && pTabControl->GetCurSel() != 0)
			{
				pTabControl->SelectItem(0);
				UpdateFriendsList();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kGroupButtonControlName) == 0)
		{
			if (pTabControl && pTabControl->GetCurSel() != 1)
			{
				pTabControl->SelectItem(1);
				UpdateGroupsList();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMicroBlogButtonControlName) == 0)
		{
			if (pTabControl && pTabControl->GetCurSel() != 2)
			{
				pTabControl->SelectItem(2);
				UpdateMicroBlogList();
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kTabControlName));
		if (pTabControl != NULL)
		{
			if (pTabControl->GetCurSel() == 1)
			{
				CGroupsUI* pSearchList = static_cast<CGroupsUI*>(paint_manager_.FindControl(kGroupsListControlName));
				if ((pSearchList != NULL) &&  pSearchList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
						if (!pSearchList->CanExpand(node) && (background != NULL))
						{
							FriendListItemInfo friend_info;

							for (std::vector<FriendListItemInfo>::const_iterator citer = friends_.begin(); citer != friends_.end(); ++citer)
							{
								if (_tcsicmp(citer->id.c_str(), node->data().value.c_str()) == 0)
								{
									friend_info = *citer;
									break;
								}
							}
							TCHAR szBuf[MAX_PATH] = {0};
							if (_tcslen(background->GetBkImage()) > 0)
							{
#if defined(UNDER_WINCE)
								_stprintf(szBuf, _T("bg%d.png"), bk_image_index_);
#else
								_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.png"), bk_image_index_);
#endif
							}

							ProductStruct product = FindProductByName(node->data().text_.c_str());
							tString strPath = "F:\\RsSrcDir\\bin\\"; /*CPaintManagerUI::GetInstancePath()*/;
							ShellExecute(NULL,NULL,product.m_StrExeName.c_str(),product.m_StrArguments.c_str(),strPath.c_str(),1);
						}
					}
				}
			}
			else if (pTabControl->GetCurSel() == 0)
			{
				CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(paint_manager_.FindControl(kFriendsListControlName));
				if ((pFriendsList != NULL) &&  pFriendsList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
						if (!pFriendsList->CanExpand(node) && (background != NULL))
						{
							FriendListItemInfo friend_info;

							for (std::vector<FriendListItemInfo>::const_iterator citer = friends_.begin(); citer != friends_.end(); ++citer)
							{
								if (_tcsicmp(citer->id.c_str(), node->data().value.c_str()) == 0)
								{
									friend_info = *citer;
									break;
								}
							}
							TCHAR szBuf[MAX_PATH] = {0};
							if (_tcslen(background->GetBkImage()) > 0)
							{
#if defined(UNDER_WINCE)
								_stprintf(szBuf, _T("bg%d.png"), bk_image_index_);
#else
								_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.png"), bk_image_index_);
#endif
							}

							ProductStruct product = FindProductByName(node->data().text_.c_str());
							tString strPath = "F:\\RsSrcDir\\bin\\"; /*CPaintManagerUI::GetInstancePath()*/;
							ShellExecute(NULL,NULL,product.m_StrExeName.c_str(),product.m_StrArguments.c_str(),strPath.c_str(),1);
						}
					}
				}
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kTabControlName));
		if (pTabControl != NULL)
		{
			if (pTabControl->GetCurSel() == 0)
			{
				CFriendsUI* pFriendsList = static_cast<CFriendsUI*>(paint_manager_.FindControl(kFriendsListControlName));
				if ((pFriendsList != NULL) &&  pFriendsList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						if (pFriendsList->CanExpand(node))
						{
							pFriendsList->SetChildVisible(node, !node->data().child_visible_);
						}
					}
				}
			}
			else if (pTabControl->GetCurSel() == 1)
			{
				CGroupsUI* pGroupsList = static_cast<CGroupsUI*>(paint_manager_.FindControl(kGroupsListControlName));
				if ((pGroupsList != NULL) &&  pGroupsList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						if (pGroupsList->CanExpand(node))
						{
							pGroupsList->SetChildVisible(node, !node->data().child_visible_);
						}
					}
				}
			}

		}
	}
	else if (_tcsicmp(msg.sType, _T("textchanged")) == 0)
	{		
		CControlUI* search_tip = static_cast<CRichEditUI*>(paint_manager_.FindControl(kSearchEditTipControlName));
		if (search_tip == NULL) return;

		CRichEditUI* search_edit = static_cast<CRichEditUI*>(msg.pSender);
		if (search_edit == NULL)return;

		LPCTSTR str = search_edit->GetText();

		searchResultList.clear(); //�������
		UpdateGroupsList();

		if (_tcsicmp(str, _T("")) == 0 ||_tcsicmp(str, _T("����ؼ�����������...")) == 0)
			return;

		CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kTabControlName));
		pTabControl->SelectItem(1);
		COptionUI *opt =  static_cast<COptionUI*>(paint_manager_.FindControl(kGroupButtonControlName));
		opt->Selected(true);
		searchResultList.clear(); //�������
		GetSearchResultList(str);
		UpdateGroupsList();
	}
	else if (_tcsicmp(msg.sType, _T("setfocus")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kSearchEditTipControlName) == 0)
		{
			CTabLayoutUI* pTabControl = static_cast<CTabLayoutUI*>(paint_manager_.FindControl(kTabControlName));

			if (pTabControl && pTabControl->GetCurSel() != 1)
			{
				pTabControl->SelectItem(1);
				COptionUI *opt =  static_cast<COptionUI*>(paint_manager_.FindControl(kGroupButtonControlName));
				opt->Selected(true);
				UpdateGroupsList();
			}
		}
	}
}

LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
int MainFrame::GetProductGroupCount()
{
	return m_vProductGroup.size();
}
CProductGroup MainFrame::GetProductGroup(int index)
{
	return m_vProductGroup[index];
}
CProductGroup MainFrame::GetProductGroup(tString strGroutTitle)
{
	for (int i=0;i<GetProductGroupCount();i++)
	{
		CProductGroup group = GetProductGroup(i);
		tString title  = group.GetGroupTitle();
		if (STREQUAL(title.c_str(),strGroutTitle.c_str()))
			return group;
	}

	return CProductGroup();
}
bool MainFrame::LoadConfigXml(tString strXml)
{
	CMarkup m_xml;
	if (!m_xml.LoadFromFile(strXml.c_str()))
		return false;

	CMarkupNode root = m_xml.GetRoot();
	for( CMarkupNode ApplicationNode = root.GetChild() ; ApplicationNode.IsValid(); ApplicationNode = ApplicationNode.GetSibling() )
	{
		tString strName = ApplicationNode.GetName();
		if (STREQUAL(strName.c_str(),"Application"))
		{
			for( CMarkupNode GroupNode = ApplicationNode.GetChild() ; GroupNode.IsValid(); GroupNode = GroupNode.GetSibling() )
			{
				strName = GroupNode.GetName();
				if (STREQUAL(strName.c_str(),"AppTab"))
				{
					CProductGroup group;
					tString strGroupTitle = GroupNode.GetAttributeValue(_T("TabName")); 
					group.SetGroupTitle(strGroupTitle.c_str());
					for( CMarkupNode NodeProduct = GroupNode.GetChild() ; NodeProduct.IsValid(); NodeProduct = NodeProduct.GetSibling() )
					{
						strName =  NodeProduct.GetName();
						if (STREQUAL(strName.c_str(),"ProductName"))
						{
							ProductStruct product;
							product.m_StrTitle = NodeProduct.GetAttributeValue(_T("title"));
							product.m_StrExeName = NodeProduct.GetAttributeValue(_T("exename"));
							product.m_StrArguments = NodeProduct.GetAttributeValue(_T("arguments"));
							product.m_StrToolTip = NodeProduct.GetAttributeValue(_T("tooltip"));
							product.m_strLogoPng = NodeProduct.GetAttributeValue(_T("identity"));
							tString strValid = NodeProduct.GetAttributeValue(_T("valid"));
							if (STREQUAL(strValid.c_str(),_T("1")))
								product.m_bValid = true;
							else
								product.m_bValid = false;

							group.AddProduct(product);
						}
					}

					m_vProductGroup.push_back(group);
				}
			}
		}
	}
	return true;
}

ProductStruct MainFrame::FindProductByName(const char * pszName)
{
	for (int i=0;i< GetProductGroupCount();i++)
	{
		CProductGroup group = GetProductGroup(i);
		for (int j=0;j<group.GetGroupItemCount();j++)
		{
			ProductStruct product = group.GetGroupItem(j);
			if ( STREQUAL(product.m_StrTitle.c_str(),pszName))
				return product;
		}
	}
	return ProductStruct();
}
void MainFrame::BuildSearchKey()
{
	CPy py;
	for (size_t i=0;i<friends_.size();i++)
	{
		FriendListItemInfo info = friends_[i];
		friends_[i].strFirstLetter =  py.MakeSpellCode(info.nick_name,0);
		friends_[i].strFullPy =  py.MakeSpellCode(info.nick_name,1);
	}
}
void MainFrame::GetSearchResultList(tString strKey)
{
	if (strKey.empty())
		return;
	for (size_t i=0;i<strKey.length();i++)
	{
		if (strKey[i] >= 97 && strKey[i]<= 122)
			strKey[i] = _toupper(strKey[i]);
	}

	for (size_t i=0;i< friends_.size();i++)
	{
		FriendListItemInfo info = friends_[i];
		if (info.nick_name.find(strKey.c_str())!=tString::npos || info.strFirstLetter.find(strKey.c_str())!=tString::npos || info.strFullPy.find(strKey.c_str())!=tString::npos)
		{
			if (!info.folder)
				searchResultList.push_back(info);
		}
	}
}