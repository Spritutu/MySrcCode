#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include <ImageHlp.h>
#include "CommonFun.h"
#include "SqliteBase.h"
#include "LogTools.h"
#include <shlwapi.h>
using namespace std;

#pragma comment(lib,"imagehlp.lib")

CSqliteBase * g_FileDataBase = NULL;
extern void UpdateUserSpace(const char *pszUsrId,LONGLONG iSpace,bool bAdd);

bool AddFileToListTable(const char * pszFileId,FileInfo *pFileInfo)
{
	string strTable = pFileInfo->szUserId;

	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select * from %s where ItemType ='File'and ItemDir='%s'and ItemName='%s'and FileMd5='%s';",strTable.c_str(),pFileInfo->szDir,pFileInfo->szFileName,pFileInfo->szMd5);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows==1)
	{
		LogMsg("��ǰĿ¼���Ѿ�����ͬ���ļ�\n");
		g_FileDataBase->FreeSelectResult(pszDbResult);
		return false;
	}
	if (pszDbResult)
		g_FileDataBase->FreeSelectResult(pszDbResult);

	strSQL = FormatString("insert into %s(FileId,ItemName,ItemType,ItemDir,FileSize,FileTime,FileMd5) values('%s','%s','%s','%s',%lld,%lld,'%s');",
		strTable.c_str(),pszFileId,pFileInfo->szFileName,"File",pFileInfo->szDir,pFileInfo->iFileLength,pFileInfo->iLastModifyTime,pFileInfo->szMd5);
	bool bOK = g_FileDataBase->ExcuteSqlCmd(strSQL.c_str());
	if (!bOK)
		return false;

	UpdateUserSpace(pFileInfo->szUserId,pFileInfo->iFileLength,true);
	return true;
}
DWORD ReceiveToServer(SOCKET sockConn,FileInfo *pFileInfo)
{
	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strErrMsg;
	string strSQL = FormatString("select * from FILE where Md5 ='%s';",pFileInfo->szMd5);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows==1)
	{
		string strFileId = pszDbResult[nColums + 4];
		g_FileDataBase->FreeSelectResult(pszDbResult);

		AddFileToListTable(strFileId.c_str(),pFileInfo);

		UploadStartInfo StartInfo;
		StartInfo.m_iFinishedSize = pFileInfo->iFileLength;
		strcpy_s(StartInfo.m_szCmdMsg,strFileId.c_str());
		if (!SendData(sockConn,(char*)&StartInfo,sizeof(UploadStartInfo),strErrMsg))  //������ɹ����
			return 0;

		closesocket(sockConn);
		LogMsg("�봫�ɹ���FileId=%s\n",strFileId.c_str());
		return 0;
	}

	if (pszDbResult)
		g_FileDataBase->FreeSelectResult(pszDbResult);

	string strSumLen = GetFileLenStr(pFileInfo->iFileLength);
	LogMsg("�����ļ� : %s,�ļ���С%s\n",pFileInfo->szFileName,strSumLen.c_str());

	string strFileName = string(pFileInfo->szDir) + pFileInfo->szFileName;
	strFileName = RepalceAll(strFileName.c_str(),"/","\\");
	string strDestDir = string(g_strServerLocalDir) + strFileName;

	RepalceAll(strDestDir.c_str(),"/","\\");
	LogMsg("Ŀ���ļ�Ϊ:%s\n",strDestDir.c_str());
	MakeSureDirectoryPathExists(strDestDir.c_str());

	clock_t t_start = clock();
	FILE *pFile = NULL;
	fopen_s(&pFile,strDestDir.c_str(),"ab+");
	if (pFile==NULL)
	{
		LogMsg("����Ŀ���ļ�ʧ��!\n");
		closesocket(sockConn);
		return 0;
	}

	UploadStartInfo StartInfo;
	fseek(pFile,0,SEEK_END);
	StartInfo.m_iFinishedSize = _ftelli64(pFile);
	strcpy_s(StartInfo.m_szCmdMsg,"start");
	if (!SendData(sockConn,(char*)&StartInfo,sizeof(UploadStartInfo),strErrMsg))  //����ʼ����
		return 0;

	__int64 iCountBytes = StartInfo.m_iFinishedSize;
	FileTransfer trans;
	bool bFailed = false;
	while(1)
	{
		memset(trans.szData,0,sizeof(char)*SEND_RECV_LEN);
		if (!RecvData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg))
		{
			bFailed = true;
			LogMsg(strErrMsg.c_str());
			break;
		}
		if (trans.iDataLen ==0)
			break; // �������ݷ����ᷢ��һ��iDataLen=0��FileTransfer�ṹ��,���ڱ�ʶ�ļ��������

		iCountBytes += trans.iDataLen;
		fwrite(trans.szData,sizeof(char),trans.iDataLen,pFile);
	}
	fclose(pFile);

	if (iCountBytes!= pFileInfo->iFileLength)
		bFailed = true;

	clock_t t_End = clock();
	
	if (bFailed)
	{
		LogMsg("�ļ�����ʧ��!\n");
	}
	else
	{
		double dTimeEplised = t_End - t_start;
		LogMsg("�������,��ʱ%f��!\n",dTimeEplised/1000);

		string strFileId = GetNewGUID();
		strSQL = FormatString("insert into FILE(Md5,FileName,FileLength,FileTime,FileId) values('%s','%s',%lld,%lld,'%s');",
			pFileInfo->szMd5,strFileName.c_str(),pFileInfo->iFileLength,pFileInfo->iLastModifyTime,strFileId.c_str());

		bool bOk = g_FileDataBase->ExcuteSqlCmd(strSQL.c_str());
		if (!bOk)
		{
			LogMsg("�����ļ����ݿ�ʧ�ܣ�SQL =��%s��",strSQL.c_str());
			closesocket(sockConn);
			return 0;
		}
		bOk = AddFileToListTable(strFileId.c_str(),pFileInfo);
		if (bOk)
		{
			UploadStartInfo StartInfo;
			StartInfo.m_iFinishedSize = pFileInfo->iFileLength;
			strcpy_s(StartInfo.m_szCmdMsg,strFileId.c_str());
			if (!SendData(sockConn,(char*)&StartInfo,sizeof(UploadStartInfo),strErrMsg))  //������ɹ����
				return 0;

			LogMsg("�ϴ��ɹ���FileId=%s\n",strFileId.c_str());
		}
		else
			LogMsg("�������ݿ�ʧ��!");

		
	}
	
	// ͨѶ��ɺ󣬹ر��׽��֣��ͷ��׽�����ռ��Դ
	closesocket(sockConn);
	return 0;
}