
// 7zClientUIDlg.h : ͷ�ļ�
//

#pragma once


// CMy7zClientUIDlg �Ի���
class CMy7zClientUIDlg : public CDialogEx
{
// ����
public:
	CMy7zClientUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY7ZCLIENTUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
