#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include <ImageHlp.h>
#include "CommonFun.h"
#include "LogTools.h"
using namespace std;

#pragma comment(lib,"imagehlp.lib")

bool DownToClient(SOCKET sockConn,const char * pszFileId,const char *pszDestFile)
{
	LogMsg("���������ļ���FileId=%s������...\n",pszFileId);
	Client_Op op = download;
	string strErrMsg;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
		return false;

	if (!SendData(sockConn,(char*)pszFileId,64,strErrMsg))
		return false;

	FileInfo info;
	if (!RecvData(sockConn,&info,sizeof(FileInfo),strErrMsg))
		return false;

	string strSumLen = FormatString("%lld",info.iFileLength);

	clock_t t_start = clock();
	FILE *pFile = NULL;
	fopen_s(&pFile,pszDestFile,"wb");
	if (pFile==NULL)
	{
		LogMsg("����Ŀ���ļ�ʧ��!\n");
		closesocket(sockConn);
		return false;
	}

	__int64 iCountBytes = 0;
	FileTransfer DstTransferArry[2];

	int iTimerCount = 0;
	__int64 iLastDataLen = 0;
	double dLastTimeGo = 0;
	bool bFailed = false;
	while(1)
	{
		FileTransfer TmpTransfer;
		memset(TmpTransfer.szData,0,sizeof(char)*SEND_RECV_LEN);
		int iLen = recv(sockConn,(char*)&TmpTransfer,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			bFailed = true;
			LogMsg("\n�������\n");
			break;
		}
		else if(iLen==0)
		{
			bFailed = true;
			LogMsg("\n�����ж�,��������ֹ������\n");
			break;
		}

		int OterLength = CopyData(DstTransferArry,&TmpTransfer,iLen);// ���ö��еķ�ʽƴ��������һ��������֮�⻹ʣ�еİ���С
		if (OterLength == NOT_FULL_DATA_BAG)  
			continue; //������һ��������������ƴ��

		if (DstTransferArry[0].iDataLen ==0)
			break;

		iCountBytes +=DstTransferArry[0].iDataLen;
		fwrite(DstTransferArry[0].szData,sizeof(char),DstTransferArry[0].iDataLen,pFile);
		ClearReveiveQueen(DstTransferArry,OterLength);

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
		LogMsg("\r�ѽ���%s,�ܴ�С%s,��ʱ%f��,ƽ���ٶ�%s",GetFileLenStr(iCountBytes).c_str(),strSumLen.c_str()
			,dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock();
	double dTimeEplised = t_End - t_start;
	// ͨѶ��ɺ󣬹ر��׽��֣��ͷ��׽�����ռ��Դ
	closesocket(sockConn);
	LogMsg("\n");
	if (bFailed)
	{
		LogMsg("�ļ�����ʧ��!\n");
		return false;
	}
	else
	{
		LogMsg("�������,��ʱ%f��!\n",dTimeEplised/1000);
		return true;
	}
}