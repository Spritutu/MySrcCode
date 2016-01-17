#include <WinSock2.h>
#include <iostream>
#include <string>
#include "CommonFun.h"
#include "SqliteBase.h"
#include "DataJsDefine.h"
#include "LogTools.h"
#include "CodeFunc.h"
using namespace std;

extern CSqliteBase * g_FileDataBase;
extern void UpdateUserSpace(const char *pszUsrId,LONGLONG iSpace,bool bAdd);
bool DeleteFileFromListTable(const char * pszUsrId,const char *pszItemDir,const char *pszItemName,const char *pszType)
{
	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select * from %s where ItemType ='%s'and ItemDir='%s'and ItemName='%s';",pszUsrId,pszType,pszItemDir,pszItemName);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows==0)
	{
		g_FileDataBase->FreeSelectResult(pszDbResult);
		return false;
	}

	LONGLONG iDelSize = 0;
	if (strcmp(pszType,"Folder")==0) // ɾ��Ŀ¼�ڵ���Ŀ��Ȼ��ɾ��Ŀ¼
	{
		g_FileDataBase->FreeSelectResult(pszDbResult);
		string strSubDir = string(pszItemDir) + pszItemName + "/";
		strSQL = FormatString("select sum(FileSize) from %s where ItemDir like '%s%%';",pszUsrId,strSubDir.c_str());
		g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
		if (pszDbResult[1]==NULL)
			iDelSize = 0;
		else
			iDelSize = _atoi64(pszDbResult[1]);

		g_FileDataBase->FreeSelectResult(pszDbResult);
		strSQL = FormatString("delete from %s  where ItemDir like '%s%%';",pszUsrId,strSubDir.c_str());
		if (!g_FileDataBase->ExcuteSqlCmd(strSQL.c_str())) //ɾ��Ŀ¼�ڵ��ļ�
			return false;
	}
	if (iDelSize > 21990232555520)
	{
		LogMsg("ɾ�������ݴ�С��ͳ����쳣,����21990232555520����СΪ%lld",iDelSize);
	}

	if (strcmp(pszType,"File")==0)
	{
		g_FileDataBase->FreeSelectResult(pszDbResult);
		strSQL = FormatString("select sum(FileSize) from %s where ItemDir='%s'And ItemName='%s';",pszUsrId,pszItemDir,pszItemName);
		g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
		if (pszDbResult[1]==NULL)
			iDelSize = 0;
		else
			iDelSize = _atoi64(pszDbResult[1]);

		g_FileDataBase->FreeSelectResult(pszDbResult);
	}

	strSQL = FormatString("delete from %s where ItemDir='%s'And ItemName='%s';",
		pszUsrId,pszItemDir,pszItemName);
	if (g_FileDataBase->ExcuteSqlCmd(strSQL.c_str())) //ɾ�������ļ����߿��ļ���
	{
		UpdateUserSpace(pszUsrId,iDelSize,false); // �������̿ռ�
		return true;
	}

	return false;
}

DWORD ServerDeleteFile(SOCKET sockConn)
{
	int iDataLen = -1;
	string strErr;
	if (!RecvData(sockConn,(char*)(&iDataLen),sizeof(int),strErr))
		return 0;

	if (iDataLen <=0)
	{
		LogMsg("��������,%s,%s,%s,json����=%d",__FILE__,__FUNCTION__,__LINE__,iDataLen);
		closesocket(sockConn);
		return 0;
	}

	char * pDataJs = new char[iDataLen];
	if (pDataJs==NULL)
	{
		LogMsg("�����ڴ�ʧ��,%s,%s,%s,�����ڴ��С%d",__FILE__,__FUNCTION__,__LINE__,iDataLen);
		closesocket(sockConn);
		return 0;
	}

	if (!RecvData(sockConn,pDataJs,iDataLen,strErr))
	{
		delete pDataJs;
		return 0;
	}

	CJsonDataDelteFile DataJsonDel;
	if (!DataJsonDel.Parse(pDataJs))
	{
		delete pDataJs;
		LogMsg("����Json����ʧ��,%s",pDataJs);
		closesocket(sockConn);
		return 0;
	}
	delete pDataJs;

	for (size_t i=0;i< DataJsonDel.m_vDelFileList.size();i++)
	{
		DeleteFileFromListTable(DataJsonDel.m_strUserId.c_str(),DataJsonDel.m_vDelFileList[i].m_strNetDiskDir.c_str(),
			DataJsonDel.m_vDelFileList[i].m_strItemName.c_str(),DataJsonDel.m_vDelFileList[i].m_strType.c_str());
	}
	CDataDelFileResult DataResult;
	DataResult.m_bFlag = true;
	if (!SendData(sockConn,(char*)&DataResult,sizeof(CDataDelFileResult),strErr))
		return false;

	closesocket(sockConn);
	return 0;
}