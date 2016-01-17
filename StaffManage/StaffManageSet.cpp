
// StaffManageSet.cpp : CStaffManageSet ���ʵ��
//

#include "stdafx.h"
#include "StaffManage.h"
#include "StaffManageSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStaffManageSet ʵ��

// ���������� 2013��7��29��, 23:13

IMPLEMENT_DYNAMIC(CStaffManageSet, CRecordset)

CStaffManageSet::CStaffManageSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	m_iStaffID = 0;
	m_strStaffName = L"";
	m_strStaffSex = L"";
	m_strDepartment = L"";
	m_iStaffAge = 0;
	m_WorkTime;
	m_fStaffPrice = 0.0;
	m_strDuty = L"";
	m_strWhereGradute = L"";
	m_nFields = 10;
	m_nDefaultType = dynaset;
}
// �������ַ����п��ܰ������������/��������Ҫ
// ��Ϣ�����ڲ鿴��������ַ������ҵ������밲ȫ
// �йص�������Ƴ� #error��������Ҫ���������
// ��Ϊ������ʽ��ʹ���������û������֤��
CString CStaffManageSet::GetDefaultConnect()
{
	return _T("DSN=���¹������ݿ�;DBQ=���¹������ݿ�.mdb;DriverId=25;FIL=MS Access;MaxBufferSize=2048;PageTimeout=5;UID=admin;");
}

CString CStaffManageSet::GetDefaultSQL()
{
	return _T("[���¹������ݱ�]");
}

void CStaffManageSet::DoFieldExchange(CFieldExchange* pFX)
{
	pFX->SetFieldType(CFieldExchange::outputColumn);
// RFX_Text() �� RFX_Int() �������������
// ��Ա���������ͣ����������ݿ��ֶε����͡�
// ODBC �����Զ�����ֵת��Ϊ�����������
	RFX_Long(pFX, _T("[ְ�����]"), m_iStaffID);
	RFX_Text(pFX, _T("[ְ������]"), m_strStaffName);
	RFX_Text(pFX, _T("[ְ���Ա�]"), m_strStaffSex);
	RFX_Text(pFX, _T("[���ڲ���]"), m_strDepartment);
	RFX_Int(pFX, _T("[ְ������]"), m_iStaffAge);
	RFX_Date(pFX, _T("[��ְʱ��]"), m_WorkTime);
	RFX_Single(pFX, _T("[��������]"), m_fStaffPrice);
	RFX_Text(pFX, _T("[ְ��]"), m_strDuty);
	RFX_Text(pFX, _T("[��ҵԺУ]"), m_strWhereGradute);
	RFX_LongBinary(pFX, _T("[��Ƭ]"),m_PtrPhoto);
}
/////////////////////////////////////////////////////////////////////////////
// CStaffManageSet ���

#ifdef _DEBUG
void CStaffManageSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CStaffManageSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG

