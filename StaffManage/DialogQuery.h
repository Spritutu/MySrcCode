#pragma once


// CDialogQuery �Ի���

class CDialogQuery : public CDialog
{
	DECLARE_DYNAMIC(CDialogQuery)

public:
	CDialogQuery(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogQuery();

// �Ի�������
	enum { IDD = IDD_DIALOG_QUERY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_query;
};
