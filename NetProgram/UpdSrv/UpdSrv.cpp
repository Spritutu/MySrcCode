#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib,"Ws2_32.lib")

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

	SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM,0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);

	bind(sockSrv,(sockaddr*)&addrSrv,sizeof(sockaddr));

	while (1)
	{
		char szRecv[100]={0};
		SOCKADDR_IN addClient; //�������ݷ��ĵ�ַ��Ϣ
		int iLen =sizeof(SOCKADDR_IN);
		printf("���ڵȴ������û�������Ϣ...\n");
		recvfrom(sockSrv,szRecv,100,0,(sockaddr*)&addClient,&iLen);//�ȴ����ܷ��͵�����������Ϣ

		if (szRecv[0] =='q')
		{
			sendto(sockSrv,"q",strlen("q")+1,0,(sockaddr*)&addClient,iLen);
			printf("\a��[%s]�������!\n",inet_ntoa(addClient.sin_addr));
			break;
		}
		printf("\a[%s]˵:\"%s\"\n",inet_ntoa(addClient.sin_addr),szRecv);

		printf("������Ҫ���͵���Ϣ��\n");
		char szSend[100]={0};
		gets(szSend);
		sendto(sockSrv,szSend,strlen(szSend)+1,0,(sockaddr*)&addClient,iLen); 
		printf("�������!\n");
	}
	

	closesocket(sockSrv);
	WSACleanup();
}
