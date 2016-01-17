#include <WinSock2.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

#define NOT_FULL_DATA_BAG -1
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
#define KB   1024
#define MB   (1024*KB)
#define GB   (1024*MB)
string GetFileLenStr(__int64 iLen)
{
	char strFileSize[50] ={0};
	if (iLen > GB )
	{
		sprintf(strFileSize,"%fGB",iLen*1.0/GB);
	}
	else if (iLen > MB)
	{
		sprintf(strFileSize,"%fMB",iLen*1.0/MB);
	}
	else if (iLen > KB)
	{
		sprintf(strFileSize,"%fKB",iLen*1.0/KB);
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

static int CopyData( FileTransfer * Dst,FileTransfer * SrcTmp,int iSrcLen)
{
	const static int ONE_DATA_LEN = sizeof(FileTransfer);
	static int iRecvLength = 0;
	memcpy((char*)Dst+iRecvLength,SrcTmp,iSrcLen); //�����յ��Ĳ���������ƴ���������ݰ�������
	iRecvLength += iSrcLen;
	if(iRecvLength >= ONE_DATA_LEN )
	{
		iRecvLength = iRecvLength - ONE_DATA_LEN; //�Ѿ���һ����������
		return iRecvLength; //����������֮�⻹ʣ������ݴ�С
	}
	else
		return NOT_FULL_DATA_BAG;
}
void ClearReveiveQueen(FileTransfer *Dst,int iLen)
{
	const static int ONE_DATA_LEN = sizeof(FileTransfer);
	if (iLen == 0)
		return;

	char * pData = new char[iLen];
	memcpy(pData,(char*)Dst + ONE_DATA_LEN,iLen);
	memcpy(Dst,pData,iLen);
	delete pData;
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

	//�����׽��ַ��ͻ�����
	int nBuf = SOCKET_BUFF;
	int nBufLen = sizeof(nBuf);
	int nRe = setsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
	if(SOCKET_ERROR == nRe)
		printf("setsockopt error!");

	//��黺�����Ƿ����óɹ�
	nRe = getsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
	if(SOCKET_BUFF != nBuf)
		printf("������ò��ɹ�!");


	printf("�����������IP��ַ:\n");
	char szSrvI[30] ={"192.168.1.101"};
	gets(szSrvI);
	// �ͻ��˲���Ҫbind��ֱ������
	SOCKADDR_IN addrSrv; //�������˵�ַ
	addrSrv.sin_addr.S_un.S_addr = inet_addr(szSrvI); // ���ػ�·��ַ����Ӧ��������ַ�����ʱ��Ӧ�������˵�ַ��
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000); // ������������˵ȴ��Ķ˿�һ�£���Ϊ�����ֽ���
	printf("�������ӷ�����%s...\n",szSrvI);
	int iResult = connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	if (iResult == SOCKET_ERROR)
	{	printf("�������%s����ʧ��!\n",szSrvI);
		closesocket(sockClent);
		WSACleanup(); // ��ֹ���׽��ֿ��ʹ��
		return ;
	}

	printf("���ӷ�����%s�ɹ�\n",szSrvI);
	printf("�ȴ������ļ�...\n");
	FileInfo info;
	recv(sockClent,(char*)&info,sizeof(FileInfo),0);
	string strSumLen = GetFileLenStr(info.iFileLength);
	printf("�����ļ� : %s,�ļ���С%s\n",info.szFileName,strSumLen.c_str());
	printf("������洢Ŀ¼..\n");

	char szDstFolderName[256] ={0};
	gets(szDstFolderName);
	GetRightPath(szDstFolderName); //ȥ�����˵�����

	char szDstFinal[500] ={0};
	sprintf(szDstFinal,"%s\\%s",szDstFolderName,info.szFileName);
	printf("Ŀ���ļ�Ϊ:%s\n",szDstFinal);

	send(sockClent,"start",strlen("start")+1,0); //����ʼ����
	clock_t t_start = clock();

	FILE *pFile = fopen(szDstFinal,"wb");

	__int64 iCountBytes = 0;
	FileTransfer DstTransferArry[2];

	int iTimerCount = 0;
	__int64 iLastDataLen = 0;
	double dLastTimeGo = 0;
	bool bFailed = false;
	while(1)
	{
		FileTransfer TmpTransfer;
		strset(TmpTransfer.szData,0);
		int iLen = recv(sockClent,(char*)&TmpTransfer,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			bFailed = true;
			printf("\n�����ж�,��������������ѶϿ�\n");
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
			int iSpeed = (iCountBytes - iLastDataLen)*1000/dLastTimeGo; //��������ֵ��ʱ���ı�ֵΪ�ٶ�
			strSpeedMsg = GetFileLenStr(iSpeed);
			strSpeedMsg +="/s";
			iLastDataLen  = iCountBytes;
			dLastTimeGo  = dTimeEplisedCur; // �洢��ǰ��ʱ�����ڼ����ٶ�
			iTimerCount = 0;
		}
		printf("\r�ѽ���%s,�ܴ�С%s,��ʱ%f��,ƽ���ٶ�%s",GetFileLenStr(iCountBytes).c_str(),strSumLen.c_str()
			,dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock();
	double dTimeEplised = t_End - t_start;
	printf("\n");
	if (bFailed)
		printf("�ļ�����ʧ��!\n");
	else
		printf("�������,��ʱ%f��!\n",dTimeEplised/1000);


	// ͨѶ��ɺ󣬹ر��׽��֣��ͷ��׽�����ռ��Դ
	closesocket(sockClent);
	WSACleanup(); // ��ֹ���׽��ֿ��ʹ��
	::system("pause");
}

