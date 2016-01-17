#pragma once

extern const int SELECT_MAX_FILE_NUMBER; 
// CFileSelectDlg �Ի���

class CFileSelectDlg : public CFileDialog
{
	DECLARE_DYNAMIC(CFileSelectDlg)

public:
	CFileSelectDlg(BOOL bOpenFileDialog,
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL,
		DWORD dwSize = 0,
		BOOL bVistaStyle = FALSE);   // ��׼���캯��
	virtual ~CFileSelectDlg();

	static LRESULT CALLBACK WindowProcNew(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam);
	CStringArray & GetSelectList();
// �Ի�������
//	enum { IDD = IDD_DIALOG_MINE_OPENFILE };

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitDone();
	virtual void OnFolderChange();
	virtual void OnFileNameChange();
	CString GetDisplayNameOf(LPCITEMIDLIST pidl, DWORD flags);
	virtual BOOL OnNotify(WPARAM wp, LPARAM lp, LRESULT* pRes);
	CListCtrl* GetListCtrl();
	int GetSelectCount();
	DECLARE_MESSAGE_MAP()
	
	static WNDPROC m_wndProc;
public:
	static CStringArray m_SelectedItemList;
	static CString m_strCurrendDirectory;
private:
	CStringArray m_strSelectListTemp;
	CStringArray m_strSelectListNow;
};

CString GetSelectedFolder(HWND hParent = NULL);