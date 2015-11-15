#include "SqliteBase.h"
#include "Record.h"
#include "CodeFunc.h"
#include <stdio.h>

int SelectResult(void*pFunData,int argc,char** argv,char **azColName)
{
	vector<CUserRecord> *pVectorUsr = (vector<CUserRecord>*)pFunData;
	if (pVectorUsr==NULL) return -1;

	MAP_ATTRIBUTES attMap;
	for (int i=0;i<argc;i++)
	{
		string strName = azColName[i];
		string strVal = argv[i] ? argv[i]:"";
		strName = CodeTools::UTF8_TO_MB(strName);
		strVal  = CodeTools::UTF8_TO_MB(strVal);
		attMap.insert(make_pair(strName,CAttirbuteValue(1,strVal)));
	}

	CUserRecord usr(attMap);
	pVectorUsr->push_back(usr);

	return 0;
}
void main()
{
	CSqliteBase * pDataBase = CSqliteBase::OpenDataBase("H:\\sqlite\\testllll.db");
	if (pDataBase==NULL)
	{
		printf("�����ݿ�ʧ�ܣ�\n");
		return;
	}
	bool bOK = pDataBase->CreateTable("hkop",true);

	bOK = pDataBase->IsTableExist("hkop");
	bOK = pDataBase->IsTableExist("user");

	CUserRecord usr1("����ΰ","26");
	pDataBase->AddRecord(usr1);

	CUserRecord usr2("����","25");
	pDataBase->AddRecord(usr2);

	CUserRecord usr3("����","29");
	pDataBase->AddRecord(usr3);

	CFileRecord f1("C:\\1.txt");
	pDataBase->AddRecord(f1);

	CFileRecord f2("H:\\software\\","a.pdf");
	pDataBase->AddRecord(f2);

	//pDataBase->DeleteRecord("user","name",CAttirbuteValue(1,"����ΰ"));

	MAP_ATTRIBUTES newMap;
	newMap.insert(make_pair("name",CAttirbuteValue(1,"������")));
	newMap.insert(make_pair("age",CAttirbuteValue(0,"87")));

	pDataBase->ModifyRecord("user","name",CAttirbuteValue(1,"����"),newMap);

	vector<CUserRecord> vUsr;
	pDataBase->SelectRecord("select * from user;",SelectResult,&vUsr);

	char **pszdbResult;
	int nRows,nColums;
	pDataBase->SelectRecord("select * from user;",nRows,nColums,pszdbResult);
	for (int i=0; i<nRows; i++)
	{
		printf("��%d����¼:",i+1);
		for (int j=0; j< nColums; j++)
		{
			string strName  = CodeTools::UTF8_TO_MB(pszdbResult[j]);
			string strValue = CodeTools::UTF8_TO_MB(pszdbResult[(i+1)*nColums + j]==NULL ? "": pszdbResult[(i+1)*nColums + j]); // ǰnColums�����ֶ��������涼���ֶ�ֵ
			printf("%s = %s  ",strName.c_str(),strValue.c_str());

		}
		printf("\n");
	}
	pDataBase->FreeSelectResult(pszdbResult);

	delete pDataBase;
	pDataBase = NULL;
}