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
		// ���ܿͻ��˵��������󣬲����ظ����ӵ��׽���
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&iLen);

		// ��ͻ��˷�������
		char Sendbuf[100];
		sprintf( Sendbuf,"Welcome %s to server...",inet_ntoa(addrClient.sin_addr) );
		send(sockConn,Sendbuf,strlen(Sendbuf)+1,0);// �෢��һ���ֽڣ������ܶ�һ��'\0'��β

		// �ӿͻ��˽�������
		char RecvBuf[100];
		recv(sockConn,RecvBuf,100,0);
		printf("%s\n",RecvBuf);

		closesocket(sockConn);
	}
}