#pragma once


// CDlgResLibImage �Ի���

class CDlgResLibImage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResLibImage)

public:
	CDlgResLibImage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgResLibImage();

// �Ի�������
	enum { IDD = IDD_JIANMO_COMPONENT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
