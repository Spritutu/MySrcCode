#include <WinSock2.h>
#include <iostream>
#include <string>
#include "CommonFun.h"
#include "SqliteBase.h"
#include "LogTools.h"
#include "CodeFunc.h"
using namespace std;

extern CSqliteBase * g_FileDataBase;
bool ReNameFileFromListTable(const char * pszUsrId,const char *pszItemDir,const char *pszItemName,const char *pszNewName,string & strErrMsg)
{
	strErrMsg.empty();
	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select ItemType from %s where ItemDir='%s'and ItemName='%s';",pszUsrId,pszItemDir,pszNewName);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	g_FileDataBase->FreeSelectResult(pszDbResult);
	if (nRows==1)
	{
		strErrMsg = "��λ���Ѿ�����ͬ���ļ�";
		return false;
	}

	strSQL = FormatString("select ItemType from %s where ItemDir='%s'and ItemName='%s';",pszUsrId,pszItemDir,pszItemName);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows==0)
	{
		g_FileDataBase->FreeSelectResult(pszDbResult);
		strErrMsg = "������Դ�ļ�������";
		return false;
	}
	string strItemType = pszDbResult[1] ? pszDbResult[1]:"";

	g_FileDataBase->FreeSelectResult(pszDbResult);
	strSQL = FormatString("update %s set ItemName = '%s' where ItemDir='%s'and ItemName='%s';",pszUsrId,pszNewName,pszItemDir,pszItemName);
	if (!g_FileDataBase->ExcuteSqlCmd(strSQL.c_str()))// �������ļ������ļ��б���
	{
		strErrMsg = "������ʧ�ܣ�";
		return false;
	}

	if (strcmp(strItemType.c_str(),"Folder")==0) // �����������ļ���
	{
		// �޸����������ItemDirֵ
		string strSubDir = string(pszItemDir) + pszItemName + "/";
		string strNewSubDir =  string(pszItemDir) + pszNewName + "/";
		string strSQL = FormatString("select * from %s where  ItemDir like '%s%%';",pszUsrId,strSubDir.c_str());
		if (!g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult))
		{
			strErrMsg = "ִ��SQL��ѯ��Ŀ¼ʧ��";
			return false;
		}
		bool bSuccess = true;
		for (int i=0;i<nRows;i++)
		{ 
			// ѭ���޸���������ĸ�Ŀ¼
			string strItemName = CodeTools::UTF8_TO_MB(pszDbResult[nColums + 1 + i*nColums]);
			string strItemDir = CodeTools::UTF8_TO_MB(pszDbResult[nColums + 3 + i*nColums]);
			string strNewDir = strItemDir;
			strNewDir = strNewDir.replace(0,strSubDir.length(),strNewSubDir.c_str());
			strSQL = FormatString("update %s set ItemDir = '%s' where ItemDir='%s'and ItemName='%s';",pszUsrId,strNewDir.c_str(),strItemDir.c_str(),strItemName.c_str());
			bSuccess = g_FileDataBase->ExcuteSqlCmd(strSQL.c_str()); // �޸�����ĸ�Ŀ¼
			if (!bSuccess)
				break;
		}
		g_FileDataBase->FreeSelectResult(pszDbResult);
		if (!bSuccess)
		{
			strErrMsg = "�޸���Ŀ¼��Ĺ�����Ŀ¼���ִ���";
			return false;
		}
	}

	return true;
}
DWORD ServerReNameFile (SOCKET sockConn)
{
	string strErr;
	CDataReNameItem DataReName;
	if (!RecvData(sockConn,(char*)(&DataReName),sizeof(CDataReNameItem),strErr))
		return 0;

	CDataReNameResult ReNameResult;
	if(ReNameFileFromListTable(DataReName.m_szUsrId,DataReName.szItemDir,DataReName.szItemName,DataReName.szNewName,strErr))
		ReNameResult.m_bFlag = true;
	else
	{
		ReNameResult.m_bFlag = false;
		strcpy_s(ReNameResult.m_szErrMsg,500,strErr.c_str());
	}

	if (!SendData(sockConn,(char*)(&ReNameResult),sizeof(CDataReNameResult),strErr))
		return 0;

	closesocket(sockConn);
	return 0;
}