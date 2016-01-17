#include "StdAfx.h"
#include "UploadTask.h"
#include "CommonFun.h"
#include "NetDiskUI.h"
#include "NetDiskOperation.h"
#include <fstream>
#include "ThreadPoolManager.h"
#include "LogTools.h"
using namespace std;

CUploadTask::CUploadTask(LPCTSTR strNetDiskDir,const CFileInfo & info,LPCTSTR strLocalFile)
{
	m_iTotalSize    = 0;
	m_iFinishedSize = 0;
	m_bFlag = TRUE;
	m_strErrMsg.Empty();
	m_strFileId.Empty();
	m_strOperation = "upload";
	m_strNetDiskDir = strNetDiskDir==NULL ? "":strNetDiskDir;
	m_strLocalFile  = strLocalFile==NULL ? "":strLocalFile;
	if ( m_strNetDiskDir.IsEmpty()
		|| m_strLocalFile.IsEmpty())
	{
		m_bFlag = FALSE;
		m_strErrMsg = "���ֿղ�����";
	}
	if (!IsFile(strLocalFile))
	{
		m_bFlag = FALSE;
		m_strErrMsg.Format("��%s�������ļ���",strLocalFile);
	}
	m_FileInfo = info;
	m_iTotalSize = FileSize(strLocalFile);
}
CUploadTask::CUploadTask(const CUploadTask & other)
{
	m_strNetDiskDir = other.m_strNetDiskDir;
	m_strLocalFile  = other.m_strLocalFile;

	m_iTotalSize     = other.m_iTotalSize;
	m_iFinishedSize  = other.m_iFinishedSize;

	m_bFlag = other.m_bFlag;
	m_strErrMsg = other.m_strErrMsg;
	m_strFileId = other.m_strFileId;
	m_strOperation = other.m_strOperation;
	m_dProgress = other.m_dProgress;
	m_Status = other.m_Status;
	m_FileInfo = other.m_FileInfo;
}

CUploadTask::~CUploadTask(void)
{

}
BOOL CUploadTask::Run()
{
	if (m_bExit)
		return FALSE;

	CTaskBase *pTask = thePoolManager.GetTaskFromHistory(GetTaskId());
	CUploadTask * pUploadTaskHistory = NULL;
	if (pTask && pTask->m_strOperation=="upload")
		pUploadTaskHistory = dynamic_cast<CUploadTask*>(pTask);

	ASSERT(pUploadTaskHistory!=NULL);

	if (!m_bFlag)
	{
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	pUploadTaskHistory->SetTaskStatus(CTaskBase::task_running);
	string strFile = m_strLocalFile;
	string strFileName = strFile.substr(strFile.rfind("\\")+1);

	if (m_bExit)
		return FALSE;

	FileInfo info;
	strcpy_s(info.szFileName,strFileName.c_str());
	info.iFileLength = m_iTotalSize;
	info.iLastModifyTime = FileTimeLastModify(strFile.c_str());
	LogMsg("���ڼ���MD5ֵ��%s��...",m_strLocalFile);
	m_strMd5 = GetMD5(strFile.c_str()).c_str();
	LogMsg("��%s���ļ���MD5ֵ=%s",m_strLocalFile,m_strMd5);
	if (m_bExit)
		return FALSE;

	strcpy_s(info.szMd5,m_strMd5);
	strcpy_s(info.szDir,m_strNetDiskDir);
	strcpy_s(info.szUserId,theApp.m_strUsrId);

	SOCKET sockConn = GetConnect(theApp.m_strServerIp);
	if (sockConn==0)
	{
		m_strErrMsg = "���ӷ�����ʧ�ܣ�";
		m_bFlag = FALSE;
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}
	LogMsg("�����ϴ��ļ���%s������...\n",strFile.c_str());
	Client_Op op = upload;
	string strErrMsg;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
	{
		m_strErrMsg = "ͨ�Ŵ���";
		m_bFlag = FALSE;
		closesocket(sockConn);
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	LogMsg("����FileInfo...");
	if (!SendData(sockConn,(char*)&info,sizeof(FileInfo),strErrMsg))
	{
		m_strErrMsg = "ͨ�Ŵ���";
		m_bFlag = FALSE;
		closesocket(sockConn);
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	UploadStartInfo StartInfo;
	LogMsg("���շ������ϴ��ļ�������Ϣ...");
	if (!RecvData(sockConn,&StartInfo,sizeof(StartInfo),strErrMsg))
	{
		m_strErrMsg = "ͨ�Ŵ���";
		m_bFlag = FALSE;
		closesocket(sockConn);
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	if (StartInfo.m_iFinishedSize == info.iFileLength && strcmp(StartInfo.m_szCmdMsg,"start")!=0)
	{
		LogMsg("�ļ��봫�ɹ�,FileId=%s\n",StartInfo.m_szCmdMsg);
		closesocket(sockConn);
		m_strFileId = StartInfo.m_szCmdMsg;
		pUploadTaskHistory->m_strFileId = m_strFileId;
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_finish);
		return TRUE;
	}
	else if (StartInfo.m_iFinishedSize < 0 || StartInfo.m_iFinishedSize > info.iFileLength)
	{
		LogMsg("�ļ��ϴ�ʧ��,%s\n",StartInfo.m_szCmdMsg);
		m_strErrMsg = StartInfo.m_szCmdMsg;
		closesocket(sockConn);
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	if (m_bExit)
	{
		LogMsg("�߳���Ҫ�˳����ϴ��������...");
		closesocket(sockConn);
		return FALSE;
	}

	std::wstring wstrFilePath = _bstr_t((LPCTSTR)m_strLocalFile);
	std::ifstream fIn(wstrFilePath,std::ios::binary);
	if (fIn == NULL || fIn.is_open() == false)
	{
		m_strErrMsg.Format( _T("��Դ�ļ���%s��ʧ�ܣ�"),m_strLocalFile);
		m_bFlag = FALSE;
		closesocket(sockConn);
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	if (StartInfo.m_iFinishedSize > 0)
	{
		m_iFinishedSize  = StartInfo.m_iFinishedSize;
		fIn.seekg(StartInfo.m_iFinishedSize,std::ios::beg); //�ϵ�����
		LogMsg("%s��ʼִ�жϵ�����,�Ѵ����С%lld,�ܴ�С%lld",m_FileInfo.m_strName,m_iFinishedSize,m_iTotalSize);
	}

	FileTransfer trans;
	int iReadCount = sizeof(trans.szData)/sizeof(char);
	clock_t t_start = clock(); //��ʼ��ʱ

	LogMsg("��ʼ���䡾%s���ļ�����...",m_FileInfo.m_strName);
	while(1)
	{
		int iReadBytes = fIn.read(trans.szData, iReadCount).gcount();
		if (0==iReadBytes)
			break;

		if (m_bExit)
		{
			LogMsg("�ϴ������յ��̳߳��˳���Ϣ���������...");
			break;
		}

		if(iReadBytes < 0)
		{
			LogMsg("\n��ȡ���ݳ��ִ���!\n");
			m_strErrMsg = "��ȡ���ݳ��ִ���";
			m_bFlag = FALSE;
			closesocket(sockConn);
			fIn.close();
			pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
			return FALSE;
		}

		trans.iDataLen = iReadBytes;
		if (!SendData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg))
		{
			m_strErrMsg = "ͨ�Ŵ���";
			m_bFlag = FALSE;
			closesocket(sockConn);
			fIn.close();
			pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
			return FALSE;
		}
		m_iFinishedSize +=iReadBytes;
		pUploadTaskHistory->m_iFinishedSize = m_iFinishedSize;
		pUploadTaskHistory->m_dProgress = m_iFinishedSize*1.0/m_iTotalSize;
	}

	LogMsg("��%s���ļ����ݴ�����ϣ�",m_FileInfo.m_strName);

	fIn.close();
	clock_t t_End = clock(); //������ʱ
	double dTimeEplised = t_End - t_start;

	LogMsg("�����ļ�������ϱ�־...");
	trans.iDataLen = 0;
	if (!SendData(sockConn,(char*)&trans,sizeof(FileTransfer),strErrMsg))
	{
		m_strErrMsg = "ͨ�Ŵ���";
		m_bFlag = FALSE;
		closesocket(sockConn);
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	LogMsg("���շ������������ļ�Id��Ϣ...");
	if (!RecvData(sockConn,&StartInfo,sizeof(StartInfo),strErrMsg))
	{
		m_strErrMsg = "ͨ�Ŵ���";
		m_bFlag = FALSE;
		closesocket(sockConn);
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}

	closesocket(sockConn);
	if (StartInfo.m_iFinishedSize == info.iFileLength  && m_iFinishedSize == StartInfo.m_iFinishedSize)
	{
		LogMsg("�ϴ��ɹ�,FileId=%s\n",StartInfo.m_szCmdMsg);
		m_strFileId = StartInfo.m_szCmdMsg;
		pUploadTaskHistory->m_strFileId = m_strFileId;
		pUploadTaskHistory->m_dProgress = 1.0;
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_finish);
		LogMsg("������ϣ���ʱ%f��!\n",dTimeEplised/1000);
		return TRUE;
	}
	else
	{
		LogMsg("�ϴ�ʧ�ܣ�%s",StartInfo.m_szCmdMsg);
		LogMsg("�������,�ļ���С��%lld��,�ϴ���С��%lld��,���������մ�С��%lld����"
			,info.iFileLength,m_iFinishedSize,StartInfo.m_iFinishedSize);

		m_strErrMsg = StartInfo.m_szCmdMsg;
		pUploadTaskHistory->SetTaskStatus(CTaskBase::task_error);
		return FALSE;
	}
}
CTaskBase * CUploadTask::GetCopy()
{
	CUploadTask * pTask = new CUploadTask(*this);
	return pTask;
}
 CString CUploadTask::GetTaskId()
 {
	 CString strTaskKey = m_strNetDiskDir + m_FileInfo.m_strName;
	 return  strTaskKey;
 }