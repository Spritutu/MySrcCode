#pragma once


// CPageXiangCe �Ի���

class CPageXiangCe : public CDialogEx
{
	DECLARE_DYNAMIC(CPageXiangCe)

public:
	CPageXiangCe(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageXiangCe();

// �Ի�������
	enum { IDD = IDD_DLG_PAGE_XIANGCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
