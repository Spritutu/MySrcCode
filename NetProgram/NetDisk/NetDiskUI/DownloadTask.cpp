#include "StdAfx.h"
#include "DownloadTask.h"
#include <imagehlp.h>
#include "ThreadPoolManager.h"
#include "NetDiskUI.h"
#include "LogTools.h"
CDownloadTask::CDownloadTask(LPCTSTR strNetDiskDir,const CFileInfo & info,LPCTSTR strLocalDir)
{
   
   m_iTotalSize    = 0;
   m_iFinishedSize = 0;
   m_bFlag = TRUE;
   m_strErrMsg.Empty();
   m_strOperation = "download";
   m_strNetDiskDir = strNetDiskDir==NULL ? "":strNetDiskDir;
   m_strDestDir  = strLocalDir==NULL ? "":strLocalDir;
   if ( m_strNetDiskDir.IsEmpty()
	   || m_strDestDir.IsEmpty()
	   || info.m_strFileId.IsEmpty())
   {
	   m_bFlag = FALSE;
	   m_strErrMsg = "���ֿղ�����";
   }
   m_strDestDir.Replace("/","\\");
   m_strDestDir.Replace("\\\\","\\");

   if (m_strDestDir.Right(1)!="\\")
	   m_strDestDir.AppendChar('\\');

   if (!MakeSureDirectoryPathExists(m_strDestDir))
   {
	   m_bFlag = FALSE;
	   m_strErrMsg.Format("��%s��Ŀ¼�����ڣ����޷�������");
   }
   m_FileInfo = info;
}


CDownloadTask::~CDownloadTask(void)
{
}
CDownloadTask::CDownloadTask(const CDownloadTask & other)
{
	m_strNetDiskDir = other.m_strNetDiskDir;
	m_strDestDir  = other.m_strDestDir;

	m_iTotalSize     = other.m_iTotalSize;
	m_iFinishedSize  = other.m_iFinishedSize;

	m_bFlag = other.m_bFlag;
	m_strErrMsg = other.m_strErrMsg;
	m_strOperation = other.m_strOperation;
	m_dProgress = other.m_dProgress;
	m_Status   = other.m_Status;
	m_FileInfo = other.m_FileInfo;
}
BOOL CDownloadTask::Run()
{
	CTaskBase *pTask = thePoolManager.GetTaskFromHistory(GetTaskId());
	CDownloadTask * pDownloadTaskHistory = NULL;
	if (pTask && pTask->m_strOperation=="download")
		pDownloadTaskHistory = dynamic_cast<CDownloadTask*>(pTask);

	ASSERT(pDownloadTaskHistory!=NULL);
	if (!m_bFlag)
	{
		pDownloadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	pDownloadTaskHistory->SetTaskStatus(CTaskBase::task_running);
	SOCKET sockConn = GetConnect(theApp.m_strServerIp);
	if (sockConn==0)
	{
		m_strErrMsg = "���ӷ�����ʧ�ܣ�";
		m_bFlag = FALSE;
		pDownloadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	CString strDestFile = m_strDestDir + m_FileInfo.m_strName;
	FileInfo info;

	LogMsg("���������ļ���FileId=%s������...\n",m_FileInfo.m_strFileId);
	Client_Op op = download;
	string strErrMsg;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
	{
		m_strErrMsg = strErrMsg.c_str();
		goto _ERROR;
	}

	if (!SendData(sockConn,(char*)(LPCTSTR)m_FileInfo.m_strFileId,64,strErrMsg))
	{
		m_strErrMsg = strErrMsg.c_str();
		goto _ERROR;
	}

	if (!RecvData(sockConn,&info,sizeof(FileInfo),strErrMsg))
	{
		m_strErrMsg = strErrMsg.c_str();
		goto _ERROR;
	}

	m_iTotalSize = info.iFileLength;
	clock_t t_start = clock();
	FILE *pFile = NULL;
	fopen_s(&pFile,strDestFile,"wb");
	if (pFile==NULL)
	{
		m_strErrMsg.Format("����Ŀ���ļ���%s��ʧ��!\n",strDestFile);
		m_strErrMsg = strErrMsg.c_str();
		LogMsg(m_strErrMsg);
		goto _ERROR;
	}

	LogMsg("��ʼд��Ŀ���ļ���%s��...",strDestFile);
	m_iFinishedSize = 0;
	bool bFailed = false;
	FileTransfer trans;
	while(1)
	{
		memset(trans.szData,0,sizeof(char)*SEND_RECV_LEN);
		if (!RecvData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg))
		{
			bFailed = true;
			m_strErrMsg = strErrMsg.c_str();
		}

		if (trans.iDataLen ==0)
			break;

		if (m_bExit)
		{
			LogMsg("���������յ��̳߳��˳���Ϣ���������...");
			break;
		}

		m_iFinishedSize += trans.iDataLen;
		pDownloadTaskHistory->m_iFinishedSize = m_iFinishedSize;
		pDownloadTaskHistory->m_dProgress = m_iFinishedSize*1.0/m_iTotalSize;
		int iWriteSize = fwrite(trans.szData,sizeof(char),trans.iDataLen,pFile);
		if (iWriteSize!=trans.iDataLen)
		{
			LogMsg("д��Ŀ���ļ���%s������",strDestFile);
			bFailed  = true;
		}
	}
	LogMsg("д��Ŀ���ļ���%s�����",strDestFile);
	fclose(pFile);
	if (bFailed || m_iTotalSize !=m_iFinishedSize)
	{
		LogMsg("�ļ�����ʧ��!,�ļ��ܴ�С��%lld��,����ɴ�С��%lld��\n",m_iTotalSize,m_iFinishedSize);
		goto _ERROR;
	}
	// ͨѶ��ɺ󣬹ر��׽��֣��ͷ��׽�����ռ��Դ
	closesocket(sockConn);
	clock_t t_End = clock();
	double dTimeEplised = t_End - t_start;
	LogMsg("�������,��ʱ%f��!\n",dTimeEplised/1000);
	pDownloadTaskHistory->SetTaskStatus(CTaskBase::task_finish);
	return TRUE;

_ERROR:
	m_bFlag = FALSE;
	closesocket(sockConn);
	pDownloadTaskHistory->SetTaskStatus(CTaskBase::task_error);
	return FALSE;
}
CTaskBase * CDownloadTask::GetCopy()
{
	CTaskBase *pTaskCopied = new CDownloadTask(*this);
	return pTaskCopied;
}
CString CDownloadTask::GetTaskId()
{
	return m_strNetDiskDir + m_FileInfo.m_strName;
}