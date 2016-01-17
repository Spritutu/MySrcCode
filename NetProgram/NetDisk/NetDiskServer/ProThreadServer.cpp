#include <WinSock2.h>
#include <stdio.h>
#include "CommonFun.h"
#include "LogTools.h"

extern DWORD SendToClient(SOCKET sockConn);
extern DWORD ReceiveToServer(SOCKET sockConn,FileInfo *pFileInfo);
extern DWORD SendFileListToClient(SOCKET sockConn);
extern DWORD RegisterUser(SOCKET sockConn);
extern DWORD Login(SOCKET sockConn);
extern DWORD ServerNewDir(SOCKET sockConn);
extern DWORD ServerCopyFile(SOCKET sockConn);
extern DWORD ServerDeleteFile(SOCKET sockConn);
extern DWORD ServerMoveFile(SOCKET sockConn);
extern DWORD ServerReNameFile(SOCKET sockConn);


DWORD WINAPI ProThreadServer(void * param)
{
	SOCKET sockConn  = (SOCKET)param;
	// ��ȡ�û���������
	Client_Op opType;
	string strErrMsg;
	if (!RecvData(sockConn,&opType,sizeof(Client_Op),strErrMsg))
		return 0;

	DWORD dwRev = 0;
	if (opType == upload)
	{
		//�����û��ϴ�����
		LogMsg("�յ��ϴ��ļ�����...\n");
		FileInfo *pinfo = new FileInfo;
		if (!RecvData(sockConn,pinfo,sizeof(FileInfo),strErrMsg))
		{
			delete pinfo;
			return 0;
		}

		dwRev = ReceiveToServer(sockConn,pinfo);
		delete pinfo;
	}
	else if (opType == download)
	{
		//�����û���������
		LogMsg("�յ������ļ�����...\n");
		dwRev = SendToClient(sockConn);
	}
	else if (opType == getlist)
	{
		//�����û���ȡ�ļ��б�����
		LogMsg("�յ���ȡ�ļ��б�����...\n");
		dwRev = SendFileListToClient(sockConn);
	}
	else if (opType == getuserInfo)
	{
		//�����û���ȡ�û���Ϣ������������ʹ��������˻������
	}
	else if (opType == registerUser)
	{
		//����ע�����˻�����
		LogMsg("�յ�ע���û�����...\n");
		dwRev = RegisterUser(sockConn);
	}
	else if (opType == userLogin)
	{
		//�û���¼����
		LogMsg("�յ��û���¼����...\n");
		dwRev = Login(sockConn);
	}
	else if (opType == addDir)
	{
		//����Ŀ¼����
		LogMsg("�յ��û��½�Ŀ¼����...\n");
		dwRev = ServerNewDir(sockConn);
	}
	else if (opType == moveFile)
	{
		//�ƶ��ļ�����
		LogMsg("�յ��ƶ��ļ�����...\n");
		dwRev = ServerMoveFile(sockConn);
	}
	else if (opType == copyFile)
	{
		//�����ļ�����
		LogMsg("�յ������ļ�����...\n");
		dwRev = ServerCopyFile(sockConn);
	}
	else if (opType == reName)
	{
		//����������
		LogMsg("�յ��û�����������...\n");
		dwRev = ServerReNameFile(sockConn);
	}
	else if (opType == deleteItem)
	{
		//ɾ���ļ�����
		LogMsg("�յ�ɾ������...\n");
		dwRev = ServerDeleteFile(sockConn);
	}
	else
	{
		// �����������Ͳ�����
		LogMsg("���������%d\n",opType);
		closesocket(sockConn);
	}

	return dwRev;
}