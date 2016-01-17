#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <time.h>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

#define SOCKET_BUFF 204800    //�׽��ֻ�������С

struct FileTransfer
{
	char szData[153600];
	int  iDataLen;
};
struct FileInfo
{
	__int64 iFileLength;
	char szFileName[256];
};
string GetFileName(const char * pszFilePath)
{
	char *pEnd = (char*)pszFilePath + strlen(pszFilePath);
	while(*pEnd !='\\' && *pEnd !='/')
		pEnd--;
	return pEnd+1;
}
#define KB   1024
#define MB  ( 1024*KB)
#define GB  ( 1024*MB)
string GetFileLenStr(__int64 iLen)
{
	char strFileSize[50] ={0};
	if (iLen > GB )
	{
		sprintf(strFileSize,"%.2fGB",iLen*1.0/GB);
	}
	else if (iLen > MB)
	{
		sprintf(strFileSize,"%.2fMB",iLen*1.0/MB);
	}
	else if (iLen > KB)
	{
		sprintf(strFileSize,"%.2fKB",iLen*1.0/KB);
	}
	else
		sprintf(strFileSize,"%dByte",iLen);

	return strFileSize;
}
void GetRightPath(char *path)
{
	if (NULL == path)
		return;

	int iLen = strlen(path);
	if (iLen < 3)
		return;

	if (path[0] == '"' && path[iLen - 1] == '"')
	{
		char NewPath[2000] = {0};
		strcpy(NewPath,path+1);
		iLen = strlen(NewPath);
		NewPath[iLen-1] = 0;
		strset(path,0);
		strcpy(path,NewPath);
	}
}
//���ÿ���̨�����ɫ  
BOOL SetConsoleColor(WORD wAttributes)  
{  
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  
	if (hConsole == INVALID_HANDLE_VALUE)  
		return FALSE;     
	return SetConsoleTextAttribute(hConsole, wAttributes);  
}

struct ThreadParam
{
	SOCKET sockConn;
	string  strFile;
};
DWORD WINAPI ThreadSendData(void * param)
{
	SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // �ָ�Ϊ��ɫ��
	ThreadParam *Myparam = (ThreadParam*)(param);
	SOCKET sockConn = Myparam->sockConn;
	string strFile  = Myparam->strFile;
	
	FILE *pFile = fopen(strFile.c_str(),"rb");
	if(!pFile)
	{
		printf("�޷��򿪴����ļ�[%s]\n",strFile.c_str());
		return 0;
	}

	FileInfo info;
	strcpy(info.szFileName,GetFileName(strFile.c_str()).c_str());
	info.iFileLength =  _filelengthi64(_fileno(pFile));

	send(sockConn,(char*)&info,sizeof(FileInfo),0);

	char szCommandMsg[50] ={0};
	bool bStart = true;
	while(1)
	{
		SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // �ָ�Ϊ��ɫ��
		printf("�ȴ��ͻ��˷��ʹ�����������...");
		int iLen = recv(sockConn,szCommandMsg,50,0);
		if (iLen == -1)
		{
			SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // �ָ�Ϊ��ɫ��
			printf("�ͻ����ж���������!\n");
			closesocket(sockConn);
			bStart = false;
			break;
		}
		if (strcmp("start",szCommandMsg)==0)
			break;
		else
		{
			printf("������Ϣ����ȷ!\n");
		}
	}
	if (!bStart)
		return 0;


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
			printf("\n��ȡ���ݳ��ִ���!\n");
			break;
		}
		int iTryCount = 0;
RE_TRY:
		iLen = send(sockConn,(char*)&trans,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // �ָ�Ϊ��ɫ��
			printf("\n�����ж�,��ͻ��˵������ѶϿ�\n");
			break;
		}
		if (iLen != sizeof(FileTransfer))
		{
			if (iTryCount > 3)
			{
				printf("�����޷��ɹ�����!\n");
				break;
			}

			iTryCount++;
			SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // �ָ�Ϊ��ɫ��
			printf("\n���ݷ��Ͳ�����,���ڳ��Ե�%d�����·���\n",iTryCount);
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
			int iSpeed = (iCountBytes - iLastDataLen)*1000/dLastTimeGo; //��������ֵ��ʱ���ı�ֵΪ�ٶ�
			strSpeedMsg = GetFileLenStr(iSpeed);
			strSpeedMsg +="/s";
			iLastDataLen  = iCountBytes;
			dLastTimeGo  = dTimeEplisedCur; // �洢��ǰ��ʱ�����ڼ����ٶ�
			iTimerCount = 0;
		}
		SetConsoleColor(FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE); // �ָ�Ϊ��ɫ��
		printf("\r�Ѵ���%s,�ܴ�С%s,��ʱ%f��,ƽ���ٶ�%s",GetFileLenStr(iCountBytes).c_str(),
			strSumLen.c_str(),dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock(); //������ʱ
	double dTimeEplised = t_End - t_start;

	printf("\n");
	trans.iDataLen = 0;
	send(sockConn,(char*)&trans,sizeof(FileTransfer),0); //���ͽ�����ʾ
	if (iCountBytes == info.iFileLength)
		printf("������ϣ���ʱ%f��!\n",dTimeEplised/1000);
	closesocket(sockConn);
	return 0;
}
void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return ;
	}

	/* Confirm that the WinSock DLL supports 1.1.*/
	/* Note that if the DLL supports versions greater    */
	/* than 1.1 in addition to 1.1, it will still return */
	/* 1.1 in wVersion since that is the version we      */
	/* requested.                                        */

	if ( LOBYTE( wsaData.wVersion ) != 1||
		HIBYTE( wsaData.wVersion ) != 1 ) {
			/* Tell the user that we could not find a usable */
			/* WinSock DLL.                                  */
			WSACleanup( );
			return ; 
	}

	// ������ʽ�׽���(���ڼ����ͻ�����������)
	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // �����ֽ���
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); //�˿ں���ת��Ϊ�����ֽ���

	// �����ص�ַ���׽��ֽ��й���
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	//���׽�������Ϊ����ģʽ
	listen(sockSrv,5/*SOMAXCONN*/); //���õȴ������и������Ϊ5

	// ���ϵȴ��ͻ�����������
	SOCKADDR_IN  addrClient; //�洢�ͻ��˵ĵ�ַ��Ϣ
	int iLen = sizeof(SOCKADDR);

	while(1)
	{
		SetConsoleColor(FOREGROUND_GREEN);
		printf("\n�ȴ��ͻ�������...");
		// ���ܿͻ��˵��������󣬲����ظ����ӵ��׽���
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&iLen);
		SetConsoleColor(FOREGROUND_GREEN);
		printf("\n�Ѿ���ͻ�%s����������...\n",inet_ntoa(addrClient.sin_addr));

		//�����׽��ַ��ͻ�����
		int nBuf = SOCKET_BUFF;
		int nBufLen = sizeof(nBuf);
		int nRe = setsockopt(sockConn, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
		if(SOCKET_ERROR == nRe)
			printf("setsockopt error!");

		//��黺�����Ƿ����óɹ�
		nRe = getsockopt(sockConn, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
		if(SOCKET_BUFF != nBuf)
			printf("������ò��ɹ�!");

		//const char *pszFileName = "E:\\[C++ STL������������].pdf";
		SetConsoleColor(FOREGROUND_GREEN);
		printf("������Ҫ���͵��ļ�...\n");
		char pszFileName[500] ={0};
		gets(pszFileName);
		GetRightPath(pszFileName);
		ThreadParam param;
		param.sockConn = sockConn;
		param.strFile = pszFileName;
		void *pSendThread = CreateThread(NULL,0,ThreadSendData,&param,0,NULL);
		WaitForSingleObject(pSendThread,5);
	}
}