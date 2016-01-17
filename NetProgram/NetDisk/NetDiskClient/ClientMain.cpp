#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <time.h>
using namespace std;
#include "CommonFun.h"
#include "LogTools.h"

#pragma comment(lib,"Ws2_32.lib")

extern DWORD WINAPI ProThreadClient(void * param);

extern string UploadToServer(SOCKET sockConn,const char * pszLocalFile,const char * pszNetDiskDir,const char *pszUserId);
extern bool DownToClient(SOCKET sockConn,const char * pszFileId,const char *pszDestFile);
extern bool GetNetDiskFileList(SOCKET sockConn,const char * pszUserName,const char * pszNetDiskPath,bool bRecursion,vector<FileListItem *> &pFileList);
extern bool RegisterUser(SOCKET sockConn,const char * pszUserId,const char *pszUserName,const char *pszPasswd);

SOCKET GetConnect(const char *pszServerIp)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return NULL;
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
			return NULL; 
	}

	// �����׽���
	SOCKET sockClent = socket(AF_INET,SOCK_STREAM,0);

	//�����׽��ַ��ͻ�����
	int nBuf = SOCKET_BUFF;
	int nBufLen = sizeof(nBuf);
	int nRe = setsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
	if(SOCKET_ERROR == nRe)
		LogMsg("setsockopt error!");

	//��黺�����Ƿ����óɹ�
	nRe = getsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
	if(SOCKET_BUFF != nBuf)
		LogMsg("������ò��ɹ�!");

	const char  * pszHostIp =  pszServerIp;
	// �ͻ��˲���Ҫbind��ֱ������
	SOCKADDR_IN addrSrv; //�������˵�ַ
	addrSrv.sin_addr.S_un.S_addr = inet_addr(pszHostIp); // ���ػ�·��ַ����Ӧ��������ַ�����ʱ��Ӧ�������˵�ַ��
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // ������������˵ȴ��Ķ˿�һ�£���Ϊ�����ֽ���
	LogMsg("�������ӷ�����%s...\n",pszHostIp);

	int iResult = connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if (iResult == SOCKET_ERROR)
	{
		LogMsg("�������%s����ʧ��!\n",pszHostIp);
		closesocket(sockClent);
		WSACleanup(); // ��ֹ���׽��ֿ��ʹ��
		return NULL;
	}

	int timeout = 3000; //3s

	//nRe = setsockopt(sockClent,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
	//if(0 != nRe)
	//	LogMsg("���÷�����Ϣ��ʱʧ��!\n");

	//nRe = setsockopt(sockClent,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
	//if(0 != nRe)
	//	LogMsg("���ý�����Ϣ��ʱʧ��!\n");

	return sockClent;
}
void main()
{
	SOCKET sockClent = GetConnect("127.0.0.1");
	RegisterUser(sockClent,"cugxiangzhenwei","���ʿƼ�","acfgkp1017");
	
	sockClent = GetConnect("127.0.0.1");
	string strFile = "L:\\�ĵ����\\2010Excel��ѵ�̳�������.pdf";
	UploadToServer(sockClent,strFile.c_str(),"/","cugxiangzhenwei");

	vector<FileListItem *> vFileList;
	sockClent = GetConnect("127.0.0.1");
	bool bok = GetNetDiskFileList(sockClent,"cugxiangzhenwei","/",false,vFileList);

	sockClent = GetConnect("127.0.0.1");
	string strDestFile = "L:\\Boost�������ȫ����ָ��_down.tif";
	DownToClient(sockClent,"1246D8A4-B585-4383-AB71-437AC8152E06",strDestFile.c_str());
	::system("pause");
}