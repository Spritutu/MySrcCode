#include <WinSock2.h>
#include <stdio.h>
#include <time.h>

#pragma comment(lib,"Ws2_32.lib")

char * GetRanUser()
{
	static char szUserArry[4][20] ={"���ɷ�","����ΰ","�ط�","������"};

	srand(time(0));
	int iIndex = rand()%4;
	return szUserArry[iIndex];
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

	// �����׽���
	SOCKET sockClent = socket(AF_INET,SOCK_STREAM,0);

	// �ͻ��˲���Ҫbind��ֱ������
	SOCKADDR_IN addrSrv; //�������˵�ַ
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // ���ػ�·��ַ����Ӧ��������ַ�����ʱ��Ӧ�������˵�ַ��
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // ������������˵ȴ��Ķ˿�һ�£���Ϊ�����ֽ���
	connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));


	char recvBuf[100];
	recv(sockClent,recvBuf,100,0);
	printf("%s\n",recvBuf);

	char * user = GetRanUser();
	char  SendMsg[100];
	sprintf(SendMsg,"This is %s",user);
	send(sockClent,SendMsg,strlen(SendMsg)+1,0);

	// ͨѶ��ɺ󣬹ر��׽��֣��ͷ��׽�����ռ��Դ
	closesocket(sockClent);
	WSACleanup(); // ��ֹ���׽��ֿ��ʹ��
}

