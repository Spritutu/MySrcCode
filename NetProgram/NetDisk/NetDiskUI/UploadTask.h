#pragma once
#include "TaskBase.h"
#include "NetDiskOperation.h"
class CUploadTask : public CTaskBase
{
public:
	CUploadTask(LPCTSTR strNetDiskDir,const CFileInfo & info ,LPCTSTR strLocalFile);
	~CUploadTask(void);
	CUploadTask(const CUploadTask & other);
	 BOOL Run();
	 CTaskBase * GetCopy();
	 CString GetTaskId();
public:
	// �������
	CString m_strNetDiskDir;
	CString m_strLocalFile;

	// �ϴ����Ȳ���
	long long m_iTotalSize;
	long long m_iFinishedSize;
	CString   m_strFileId;

	// �ļ�������Ϣ����
	CFileInfo m_FileInfo;
	CString   m_strMd5;
};

