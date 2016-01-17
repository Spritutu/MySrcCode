#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include "../commonheader/SysInfoClass.h"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

typedef CClientInf ServerInfo ;

string ConvertWStringToString(const wstring &ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";  
	setlocale(LC_ALL, "chs");  
	//��wstring��wchar *  
	const wchar_t* _Source = ws.c_str();  

	//string��Ӧ�ĳ���  
	size_t _Dsize = 2 * ws.size() + 1;  
	char *_Dest = new char[_Dsize];  
	//��ʼ���ڴ�  
	memset(_Dest,0,_Dsize);  

	//��unicode��ת��ANSI��  
	wcstombs(_Dest, _Source, _Dsize);  

	//��char *תΪstring  
	string result = _Dest;  

	delete []_Dest;  
	return result;  
}

bool GetServerInfo(const char * pszServerIP,ServerInfo &svrInfo)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return  false;
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
			return  false; 
	}

	// �����׽���
	SOCKET sockClent = socket(AF_INET,SOCK_STREAM,0);

	// �ͻ��˲���Ҫbind��ֱ������
	SOCKADDR_IN addrSrv; //�������˵�ַ
	addrSrv.sin_addr.S_un.S_addr = inet_addr(pszServerIP); // ���ػ�·��ַ����Ӧ��������ַ�����ʱ��Ӧ�������˵�ַ��
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // ������������˵ȴ��Ķ˿�һ�£���Ϊ�����ֽ���
	int iResult = connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if (iResult == SOCKET_ERROR)
	{	printf("�������%s����ʧ��!\n",pszServerIP);
	closesocket(sockClent);
	WSACleanup(); // ��ֹ���׽��ֿ��ʹ��
	return false;
	}

	int  iLen = recv(sockClent,(char*)&svrInfo,sizeof(ServerInfo),0);
	return true;
}
void main()
{
	cout << "�����뱻��ط�����IP��ַ������ 192.168.1.1\n";

	char szServerIP[20]={"172.172.4.158"};
	cin >> szServerIP;

	ServerInfo svrInfo;
	while(1)
	{
		if (!GetServerInfo(szServerIP,svrInfo))
		{
			printf("��ȡ����������״̬ʧ�ܣ�\n");
			break;
		}
		string  strSystemVersion =  ConvertWStringToString(svrInfo.wsSystemInf);

		printf("������������: %s \n",svrInfo.hostName);
		printf("������ϵͳ�汾: %s \n",strSystemVersion.c_str());
		printf("������IP��ַ: %s \n",szServerIP);
		printf("�������ڴ�ռ�: %.3f GB \n",svrInfo.nMemorySize *1.0 / 1000); 
		printf("���������̿ռ�: %u GB \n",svrInfo.nDiskSize);
		printf("������CPUƵ�� : %.3f GHz \n",svrInfo.nCPUFrequency*1.0/1000);

		printf("�����������ڴ�ʹ��: %.3f GB \n",svrInfo.nUsedMemSize *1.0 / 1000); 
		printf("�����������ڴ�ʣ��: %.3f GB \n",svrInfo.nFreeMemSize *1.0 / 1000); 
		printf("�����������ڴ�ռ�: %.3f GB \n",svrInfo.nTotalVirtualMemSize *1.0 / 1000); 
		printf("��������������ڴ�: %.3f GB \n",svrInfo.nVirtualFreeMemSize *1.0 / 1000); 

		Sleep(1000);
	}
}
