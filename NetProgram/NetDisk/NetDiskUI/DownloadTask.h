#pragma once
#include "TaskBase.h"
#include "NetDiskOperation.h"
class CDownloadTask :public CTaskBase
{
public:
	CDownloadTask(LPCTSTR strNetDiskDir,const CFileInfo & info,LPCTSTR strLocalDir);
	~CDownloadTask(void);
	CDownloadTask(const CDownloadTask & other);

	BOOL Run();
	CTaskBase * GetCopy();
	CString GetTaskId();
	// �������
	CString m_strNetDiskDir;
	CString m_strDestDir;

	// ���ؽ��Ȳ���
	long long m_iTotalSize;
	long long m_iFinishedSize;

	// �ļ�������Ϣ������fileId�ر�
	CFileInfo m_FileInfo;
};

