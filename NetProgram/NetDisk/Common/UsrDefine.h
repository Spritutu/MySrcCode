#pragma once

#define NOT_FULL_DATA_BAG -1
#define SOCKET_BUFF 204800    //�׽��ֻ�������С
#define SERVER_PORT  6000     //�������˿�
#define MSG_CMD_LEN  500      // �����ַ�����󳤶�
#define SEND_RECV_LEN  131072 //һ�η��ͽ��յ��ļ���С

#define KB   1024
#define MB   ULONGLONG(1024*KB)
#define GB   ULONGLONG(1024*MB)
#define TB   ULONGLONG(1024*GB)

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

typedef enum{
	upload   = 0,
	download,
	getlist, 
	getuserInfo,
	registerUser,
	userLogin,
	addDir,       // �½�Ŀ¼
	reName,      // ������
	deleteItem ,  //ɾ������
	moveFile,    // �ƶ�
	copyFile,    //����

}Client_Op;

struct FileTransfer
{
	char szData[SEND_RECV_LEN];
	int  iDataLen;
};
class FileInfo
{
public:
	__int64 iFileLength;       // �ļ���С
	__int64 iLastModifyTime;  // ������ʱ��
	char szFileName[512];      // �ļ�����
	char szMd5[128];         // md5ֵ
	char szUserId[30];
	char szDir[512];
	FileInfo();
	FileInfo * GetCopied();
};

class FileListItem
{
public:
	char szFileId[128];        // ���Ϊ�ļ�,���ֶ�����Ϊ�ļ���id,��������
	char szItemName[512];      // �ļ������ļ��е�����
	char szItemType[10];       // �����ļ����ļ���
	char szMd5[128];          // md5ֵ;
	__int64 iFileLength;       // �ļ���С
	__int64 iLastModifyTime;   // ������ʱ��
	FileListItem();
	FileListItem * GetCopied();
};
class UploadStartInfo
{
public:
	UploadStartInfo();
	__int64 m_iFinishedSize; // ���ϴ����ļ���С���ϴ�����ʱ��Ӧ����ΪС��0��ֵ
	char m_szCmdMsg[MSG_CMD_LEN];// �ϴ��ɹ�ʱ���洢�ļ���FileId,ʧ��ʱ�洢������Ϣ
};

class CDataLogin
{
public:
	CDataLogin();
	char m_szUsrId[100];
	char m_szPassWd[50];
	void Code();
	void Decode();
};
class CDataLoginResult
{
public:
	CDataLoginResult();
	void Clear();
	char m_szUsrId[50];
	char m_szUserName[50];
	char m_szErrMsg[50];
	long long m_iUsedSpace;
	long long m_iAllSpace;
};

class CDataNewDir
{
public:
	char m_szDirName[MAX_PATH*2];
	char m_szParentDir[MAX_PATH*2]; 
	char m_szUsrId[50];

	CDataNewDir();
	void Clear();
};

class CDataNewDirResult
{
public:
	CDataNewDirResult();
	bool m_bFlag;
	char m_szErrMsg[500];
};

class CDataReNameItem
{
public:
	char m_szUsrId[50];         // �û�ID
	char szItemName[512];       // �ļ������ļ��е�����
	char szItemDir[512];        // ����Ŀ¼
	char szNewName[512];        // �µ��ļ������ļ��е�����
	CDataReNameItem();
};
typedef CDataNewDirResult CDataReNameResult; // ����������ṹ��

extern const char * g_strServerLocalDir; // �����������ļ�Ŀ¼