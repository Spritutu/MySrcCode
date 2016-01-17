#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include <ImageHlp.h>
#include "CommonFun.h"
#include "SqliteBase.h"
#include "CodeFunc.h"
#include "LogTools.h"
using namespace std;

#pragma comment(lib,"imagehlp.lib")

extern CSqliteBase * g_FileDataBase;

DWORD SendToClient(SOCKET sockConn)
{
	char szFileId[64] = {0};
	string strErrMsg;
	if (!RecvData(sockConn,szFileId,64,strErrMsg))
		return 0;

	LogMsg("�����ļ�FileId=%s\n",szFileId);

	int nRows,nColums;
	char ** pszDbResult = NULL;
	string strSQL = FormatString("select * from FILE where FileId ='%s';",szFileId);
	g_FileDataBase->SelectRecord(strSQL.c_str(),nRows,nColums,pszDbResult);
	if (nRows <=0 )
	{
		LogMsg("�������˲�����FileId = %s���ļ�\n",szFileId);
		g_FileDataBase->FreeSelectResult(pszDbResult);
		closesocket(sockConn);
		return 0;
	}

	FileInfo info;
	string strFile = pszDbResult[nColums+1];
	strFile = CodeTools::UTF8_TO_MB(strFile.c_str());
	strcpy_s(info.szFileName,strFile.c_str());
	strcpy_s(info.szMd5,pszDbResult[nColums]);
	info.iFileLength = _atoi64(pszDbResult[nColums+2]);
	info.iLastModifyTime = _atoi64(pszDbResult[nColums+3]);

	if (!SendData(sockConn,(char*)&info,sizeof(FileInfo),strErrMsg))
		return 0;

	strFile = string(g_strServerLocalDir) + strFile;
	FILE *pFile = NULL;
	fopen_s(&pFile,strFile.c_str(),"rb");
	if(!pFile)
	{
		LogMsg("�޷��򿪴����ļ�[%s]\n",strFile.c_str());
		closesocket(sockConn);
		return 0;
	}

	__int64 iCountBytes = 0;
	FileTransfer trans;
	int iReadCount = sizeof(trans.szData)/sizeof(char);

	clock_t t_start = clock(); //��ʼ��ʱ

	__int64 iFileLen = FileSize(strFile.c_str());

	while(trans.iDataLen = fread(trans.szData,sizeof(char),iReadCount,pFile))
	{
		if(trans.iDataLen < 0)
		{
			LogMsg("\n��ȡ���ݳ��ִ���!\n");
			break;
		}

		if (!SendData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg))
		{
			LogMsg("������󣬷�������ʧ��");
			break;
		}
	}
	fclose(pFile);

	clock_t t_End = clock(); //������ʱ
	double dTimeEplised = t_End - t_start;

	LogMsg("\n");
	trans.iDataLen = 0;
	if (!SendData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg)) //���ͽ�����ʾ
		return 0;

	if (iCountBytes == iFileLen)
		LogMsg("������ϣ���ʱ%f��!\n",dTimeEplised/1000);

	closesocket(sockConn);
	return 0;
}