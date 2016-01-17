#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include <direct.h>
#include "../commonheader/SysInfoClass.h"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

extern bool CollectHostInf(CClientInf *pCClientInf);

void main()
{

	char szPath[256] ={0};
	getcwd(szPath,256);

	char LOG_FIlE[300] ={0};
	sprintf(LOG_FIlE,"%s\\\%s",szPath,"Monitor_log.txt");

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
	listen(sockSrv,/*5*/SOMAXCONN); //���õȴ������и������Ϊ5

	// ���ϵȴ��ͻ�����������
	SOCKADDR_IN  addrClient; //�洢�ͻ��˵ĵ�ַ��Ϣ
	int iLen = sizeof(SOCKADDR);

	FILE * pFileLog  = NULL;
	pFileLog = fopen(LOG_FIlE,"a+");

	while(1)
	{
		printf("�ȴ��ͻ�������...");
		// ���ܿͻ��˵��������󣬲����ظ����ӵ��׽���
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&iLen);
		if(pFileLog)
		{
			fprintf(pFileLog,"\n�ͻ���: %s���󱾻�����״̬��Ϣ...\n",inet_ntoa(addrClient.sin_addr));
			fflush(pFileLog);
		}

		printf("\n�Ѿ���ͻ�%s����������...\n",inet_ntoa(addrClient.sin_addr));
		// ��ͻ��˷�������
		printf("���ڻ�ȡϵͳ����״̬...\n");
		CClientInf ServiceSysInf;
		if (!CollectHostInf(&ServiceSysInf))
		{
			printf("��ȡϵͳ����״̬ʧ��!\n");
			if(pFileLog)
			{
				fprintf(pFileLog,"��ȡϵͳ����״̬ʧ��\n");
				fclose(pFileLog);
			}
			return; // ��ȡ��������״̬��Ϣʧ��
		}

		printf("������ͻ��˷��ͼ����Ϣ...\n");
		send(sockConn,(char*)&ServiceSysInf,sizeof(CClientInf) + 1,0); 		// �෢��һ���ֽڣ������ܶ�һ��'\0'��β

		closesocket(sockConn);
	}
}