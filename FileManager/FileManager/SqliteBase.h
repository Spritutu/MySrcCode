#pragma once
#include <map>
#include <vector>
#include <string>
using namespace std;

typedef  int(*SelectResultFunc)(void *params,int column_size,char **column_value,char **column_name);

class CAttirbuteValue
{
public:
	CAttirbuteValue(int iType,string strValue)
	{
		m_iType = iType;
		m_strValue = strValue;
	}
protected:
	CAttirbuteValue()
	{
		m_iType = 0;
		m_strValue = "";
	}
public:
	int m_iType;
	string m_strValue;
};

typedef  map<string,CAttirbuteValue> MAP_ATTRIBUTES;

class CRecordBase
{
public:
	CRecordBase(){}
	virtual ~CRecordBase(){};
	virtual size_t GetColumSize() const
	{
		return m_AttributeMap.size();
	}
	virtual const map<string,CAttirbuteValue> & GetAttributeMap() const
	{
		return m_AttributeMap;
	}
	virtual const string & GetTableName() const
	{
		return m_strTable;
	}
protected:
	MAP_ATTRIBUTES m_AttributeMap;
	string m_strTable;
};

class CSqliteBase
{
protected:
	CSqliteBase(void);
public:
	~CSqliteBase(void);
	// �����ݿ�
	static CSqliteBase * OpenDataBase(const char * pszDataBaseName);

	// ���������ڿ�ʼ��������ǰ����
	bool StartTransaction();
	// ����ʱ�ع�
	bool Rollback();
	// ��������
	bool EndTransaction(); 
	// ִ��SQL����
	bool ExcuteSqlCmd(const char * pszSQL);
	// ��Ӽ�¼
	bool AddRecord(const CRecordBase &);
	// ɾ����¼
	bool DeleteRecord(const char * pszTable,const char * pszConditonName,const CAttirbuteValue & ConditonValue);
	// ���¼�¼
	bool ModifyRecord(const char * pszTable,const char * pszConditonName,const CAttirbuteValue & ConditonValue,const MAP_ATTRIBUTES & newValueMap);
	// ��ѯ��¼
	bool SelectRecord(const char * pszSQL,SelectResultFunc pSelResult = NULL,void *pFunData = NULL);
	// ��ʹ�ûص��Ĳ�ѯ
	bool SelectRecord(const char * pszSQL,int &nRows,int &nColums,char ** &pszDbReslut);
	// �ͷŲ�ѯ���
	void FreeSelectResult(char ** & pszDbReslut);
	// �½������
	bool CreateTable(const char * pszTable,bool DropIfExist = true);
	// �жϱ��Ƿ����
	bool IsTableExist(const char * pszTable);
	struct sqlite3 * GetDB(){return m_pDB;}
	static bool loadOrSaveDb(CSqliteBase *pInMemeoryBase, const char *zFilename, bool isSave);
private:
	static bool loadOrSaveDb(sqlite3 * & pInMemeory, const char *zFilename, bool isSave);
private:
	struct sqlite3 * m_pDB; 
};

