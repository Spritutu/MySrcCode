#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include <ImageHlp.h>
#include "CommonFun.h"
#include "UsrDefine.h"
#include "LogTools.h"
using namespace std;

#pragma comment(lib,"imagehlp.lib")

string UploadToServer(SOCKET sockConn,const char * pszLocalFile,const char * pszNetDiskDir,const char *pszUserId)
{
	FILE *pFile = NULL;
	fopen_s(&pFile,pszLocalFile,"rb");
	if(!pFile)
	{
		LogMsg("�޷��򿪴����ļ�[%s]\n",pszLocalFile);
		return "";
	}
	string strFile = pszLocalFile;
	string strFileName = strFile.substr(strFile.rfind("\\")+1);

	FileInfo info;
	strcpy_s(info.szFileName,strFileName.c_str());
	info.iFileLength =  _filelengthi64(_fileno(pFile));
	info.iLastModifyTime = FileTimeLastModify(strFile.c_str());
	string strMd5 = GetMD5(strFile.c_str());
	strcpy_s(info.szMd5,strMd5.c_str());
	strcpy_s(info.szDir,pszNetDiskDir);
	strcpy_s(info.szUserId,pszUserId);

	LogMsg("�����ϴ��ļ���%s������...\n",strFile.c_str());
	Client_Op op = upload;
	string strErrMsg;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
		return "";

	if (!SendData(sockConn,(char*)&info,sizeof(FileInfo),strErrMsg))
		return "";

	UploadStartInfo StartInfo;
	if (!RecvData(sockConn,&StartInfo,sizeof(StartInfo),strErrMsg))
		return "";

	if (StartInfo.m_iFinishedSize == info.iFileLength)
	{
		LogMsg("�ļ��봫�ɹ�,FileId=%s\n",StartInfo.m_szCmdMsg);
		closesocket(sockConn);
		return StartInfo.m_szCmdMsg;
	}
	else if (StartInfo.m_iFinishedSize < 0 || StartInfo.m_iFinishedSize > info.iFileLength)
	{
		LogMsg("�ļ��ϴ�ʧ��,%s\n",StartInfo.m_szCmdMsg);
		closesocket(sockConn);
		return "";
	}

	__int64 iCountBytes = 0;
	FileTransfer trans;
	int iReadCount = sizeof(trans.szData)/sizeof(char);

	clock_t t_start = clock(); //��ʼ��ʱ

	string strSumLen = GetFileLenStr(info.iFileLength);

	int iLen = 0;
	int iTimerCount = 0;
	__int64 iLastDataLen = 0;
	double dLastTimeGo = 0;
	while(trans.iDataLen = fread(trans.szData,sizeof(char),iReadCount,pFile))
	{
		if(trans.iDataLen < 0)
		{
			LogMsg("\n��ȡ���ݳ��ִ���!\n");
			break;
		}
		int iTryCount = 0;
RE_TRY:
		iLen = send(sockConn,(char*)&trans,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			LogMsg("\n�������\n");
			break;
		}
		else if(iLen ==0)
		{
			LogMsg("\n�����ж�,��������ֹ������\n");
			break;
		}
		if (iLen != sizeof(FileTransfer))
		{
			if (iTryCount > 3)
			{
				LogMsg("�����޷��ɹ�����!\n");
				break;
			}

			iTryCount++;
			LogMsg("\n���ݷ��Ͳ�����,���ڳ��Ե�%d�����·���\n",iTryCount);
			Sleep(1);
			goto RE_TRY;
		}
		iCountBytes += __int64(trans.iDataLen);
		double dTimeEplisedCur = clock() - t_start;
		iTimerCount++;
		string strSpeedMsg;
		if (iTimerCount%50==0) //ÿ50��ͳ��һ��ƽ���ٶ�
		{
			dLastTimeGo = dTimeEplisedCur - dLastTimeGo; // ��ȡ�����ۼƺ�ʱʱ���
			int iSpeed = int((iCountBytes - iLastDataLen)*1000/dLastTimeGo); //��������ֵ��ʱ���ı�ֵΪ�ٶ�
			strSpeedMsg = GetFileLenStr(iSpeed);
			strSpeedMsg +="/s";
			iLastDataLen  = iCountBytes;
			dLastTimeGo  = dTimeEplisedCur; // �洢��ǰ��ʱ�����ڼ����ٶ�
			iTimerCount = 0;
		}
		LogMsg("\r�Ѵ���%s,�ܴ�С%s,��ʱ%f��,ƽ���ٶ�%s",GetFileLenStr(iCountBytes).c_str(),
			strSumLen.c_str(),dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock(); //������ʱ
	double dTimeEplised = t_End - t_start;

	LogMsg("\n");
	trans.iDataLen = 0;
	if (!SendData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg))
		return "";

	if (iCountBytes == info.iFileLength)
		LogMsg("������ϣ���ʱ%f��!\n",dTimeEplised/1000);

	if (!RecvData(sockConn,&StartInfo,sizeof(StartInfo),strErrMsg))
		return "";

	closesocket(sockConn);
	if (StartInfo.m_iFinishedSize == info.iFileLength)
	{
		LogMsg("�ϴ��ɹ�,FileId=%s\n",StartInfo.m_szCmdMsg);
		return StartInfo.m_szCmdMsg;
	}
	else
		LogMsg("�ϴ�ʧ�ܣ�%s",StartInfo.m_szCmdMsg);

		return "";
}