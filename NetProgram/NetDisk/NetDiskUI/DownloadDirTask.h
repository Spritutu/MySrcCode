#pragma once
#include "Taskbase.h"
#include "NetDiskOperation.h"
class CDownloadDirTask :
	public CTaskBase
{
public:
	CDownloadDirTask(LPCTSTR strNetDiskDir,const CFileInfo & info,LPCTSTR strLocalDir);
	~CDownloadDirTask(void);
	CDownloadDirTask(const CDownloadDirTask & other);
	BOOL Run();
	CTaskBase * GetCopy();
	CString GetTaskId();
	BOOL DownloadDirRecusion(CString strDir,const CString & strDirItemName,CString strNetDiskDir);
public:
	// �������
	CString m_strNetDiskDir;
	CString m_strLocalDir;
	// �ļ�������Ϣ������fileId�ر�
	CFileInfo m_FileInfo;
};

