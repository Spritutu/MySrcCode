
// StaffManageSet.h: CStaffManageSet ��Ľӿ�
//


#pragma once

// ���������� 2013��7��29��, 23:13

class CStaffManageSet : public CRecordset
{
public:
	CStaffManageSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CStaffManageSet)

// �ֶ�/��������

// �����ַ�������(�������)��ӳ���ݿ��ֶ�(ANSI �������͵� CStringA �� Unicode
// �������͵� CStringW)��ʵ���������͡�
//  ����Ϊ��ֹ ODBC ��������ִ�п���
// ����Ҫ��ת�������ϣ�������Խ���Щ��Ա����Ϊ
// CString ���ͣ�ODBC ��������ִ�����б�Ҫ��ת����
// (ע��: ����ʹ�� 3.5 �����߰汾�� ODBC ��������
// ��ͬʱ֧�� Unicode ����Щת��)��

	long	m_iStaffID;
	CStringW	m_strStaffName;
	CStringW	m_strStaffSex;
	CStringW	m_strDepartment;
	int	m_iStaffAge;
	CTime	m_WorkTime;
	float	m_fStaffPrice;
	CStringW	m_strDuty;
	CStringW	m_strWhereGradute;
	CLongBinary m_PtrPhoto;

// ��д
	// �����ɵ��麯����д
	public:
	virtual CString GetDefaultConnect();	// Ĭ�������ַ���

	virtual CString GetDefaultSQL(); 	// ��¼����Ĭ�� SQL
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX ֧��

// ʵ��
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

