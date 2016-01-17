#include "stdafx.h"
#include "FileCopy.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include "FileProCom.h"
#include "comutil.h"

#define  COPY_ONCE_SIZE  (1024*1024)

CCopyManager * CCopyManager::m_PManagetr = NULL;

static DWORD WINAPI CopyFileCallBack(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred,
	LARGE_INTEGER StreamSize,LARGE_INTEGER StreamBytesTransferred,
	DWORD dwStreamNumber,DWORD dwCallbackReason,
	HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData)
{
	CFileCopy *parma = (CFileCopy*)lpData;
	parma->m_iTotalBytes = TotalFileSize.QuadPart;
	parma->m_iCurrentBytes = TotalBytesTransferred.QuadPart;

	if (parma->m_pMsgWnd)
		parma->m_pMsgWnd->PostMessage(WM_COMM,0,(LPARAM)parma);

	if (parma->m_bIsCancel)
		return PROGRESS_CANCEL;

	return PROGRESS_CONTINUE;
}
void CFileCopy::CheckParam()
{
	if (!m_bParamOK)
		return;

	struct stat buf;
	int result = stat (strSrc, &buf);
	if(result!=0)
	{
		m_strErrMsg.Format("�����ļ���%s���޷���",strSrc);
		m_bParamOK = false;
		return;
	}

	m_iTotalBytes = buf.st_size;

	if (access(strDst,0)==0) //����ļ��Ѿ�����
	{
		CString strMsg;
		strMsg.Format("���Ŀ���ļ���%s���Ѿ�����,�Ƿ񸲸�",strDst);
		if (MessageBox(NULL,strMsg,"��ʾ",MB_ICONQUESTION|MB_YESNO) ==IDNO)
		{
			m_strErrMsg.Format("���Ŀ���ļ���%s���Ѿ�����,���û�ѡ���˲�����",strDst);
			m_bParamOK = false;
			return;
		}
		else
		{
			if(remove(strDst)!=0)
			{
				m_strErrMsg.Format("���Ŀ���ļ���%s���Ѿ�����,���޷�ɾ����",strDst);
				m_bParamOK = false;
				return;
			}
		}
	}
}
CFileCopy::CFileCopy(const char *pszSrc,const char *pszDest)
{
	m_bParamOK = true;
	if (NULL == pszSrc)
	{
		m_strErrMsg = "����Դ�ļ�·��Ϊ��";
		m_bParamOK = false;
		strSrc = "";
	}
	else
		strSrc = pszSrc;

	if(NULL == pszDest)
	{
		m_strErrMsg = "���Ŀ��·��Ϊ��";
		m_bParamOK = false;
		strDst = "";
	}
	else
		strDst = pszDest;

	m_bForce = false;
	m_bIsCancel = false;
	m_iTotalBytes   = 0;
	m_iCurrentBytes = 0;
	m_pMsgWnd = NULL;
	m_hThreadHandle = NULL;
	m_CopyState = COPY_WAIT_SATRT;
	CheckParam();
}
bool CFileCopy::ExcuteCopy(bool bForce,MsgWnd *pMsgWnd)
{
	if (!m_bParamOK)
		return false;

	m_bForce = bForce;
	m_pMsgWnd = pMsgWnd;
	DWORD  ThreadId = 0;
	bool bStart = true;
	CCopyManager::Instance()->AddToManager(strDst,this,bStart);
	if (bStart)
		m_hThreadHandle = CreateThread(NULL,0,SysCopyFileThreadPro/*CopyFileThreadPro*/,this,0,&ThreadId);

	return true;
}
DWORD WINAPI CFileCopy::CopyFileThreadPro(LPVOID lpParamter)
{
	CFileCopy *param = (CFileCopy*)lpParamter;
	long iFileSize = param->m_iTotalBytes;

	int iReadBytes   = COPY_ONCE_SIZE;
	if (iFileSize<COPY_ONCE_SIZE)
		iReadBytes = iFileSize; //��������Сʱһ���Զ�д

	FILE *pSrcFile = fopen(param->strSrc,"rb");
	if (NULL== pSrcFile)
	{
		param->m_strErrMsg.Format( _T("��Դ�ļ���%s��ʧ�ܣ�"),param->strSrc);
		return 1;
	}

	FILE *pDstFile = fopen(param->strDst,"wb");
	if (NULL == pDstFile)
	{
		param->m_strErrMsg.Format( _T("����Ŀ���ļ���%s��ʧ�ܣ�"),param->strDst);
		fclose(pSrcFile);
		return 1;
	}
	int iAlloacSize = iReadBytes + 1;
	char *pData = new char[iAlloacSize];
	memset(pData,0,iAlloacSize);

	int iReaded = 0;
	int iWritten = 0;
	while(1)
	{
		int iReadThis =  fread(pData,1,iReadBytes,pSrcFile); //��
		if (0==iReadBytes)
			break;

		iReaded  += iReadThis;
		int iWriteThis = fwrite(pData,1,iReadThis,pDstFile); //д
		iWritten += iWriteThis;

		param->m_iCurrentBytes = iWritten;
		
		if ((iReaded + iReadBytes)>iFileSize)
			iReadBytes = iFileSize - iReaded;

		if (param->m_pMsgWnd)
			param->m_pMsgWnd->PostMessage(WM_COMM,0,(LPARAM)param);

		if (param->m_bIsCancel)
		{
			param->m_strErrMsg = _T("�û�ȡ����");
			break;
		}
	}

	delete []pData;
	fclose(pSrcFile);
	fclose(pDstFile);
	if (param->m_bIsCancel)
	{
		MessageBox(NULL,"�û�ȡ������!","��ʾ",MB_ICONINFORMATION);
		remove(param->strDst);
	}
	else
		MessageBox(NULL,"���Ƴɹ���","��ʾ",MB_ICONINFORMATION);
	return 0;
}

DWORD WINAPI CFileCopy::SysCopyFileThreadPro(LPVOID lpParamter)
{
	CFileCopy *param = (CFileCopy*)lpParamter;
	BOOL bok = CopyFileEx(param->strSrc,
		param->strDst,
		CopyFileCallBack,
		param,
		(LPBOOL)&(param->m_bIsCancel),
		COPY_FILE_FAIL_IF_EXISTS);

	if (!bok)
	{
		if (param->m_bIsCancel)
		{
			MessageBox(NULL,"�û�ȡ������!","��ʾ",MB_ICONINFORMATION);
		}
		else
		{
			CString str;
			str.Format("����ʧ�ܣ�Ŀ���ļ���%s�������Ѿ����ڣ�",param->strDst);
			MessageBox(NULL,str,"��ʾ",MB_ICONERROR);
			param->m_strErrMsg = str;
		}

		return 1;
	}
	MessageBox(NULL,"���Ƴɹ���","��ʾ",MB_ICONINFORMATION);
	return 0;
}

CCopyManager * CCopyManager::Instance()
{
	if (m_PManagetr == NULL)
		m_PManagetr = new CCopyManager;

	return m_PManagetr;
}
void CCopyManager::AddToManager(CString str ,CFileCopy * pcp,bool & bStart)
{
	g_CopyProgressInfo[str] = pcp;
	if (m_iCurrentRunningThreadCount == m_MaxThreadCount) //�Ѿ��ﵽ�߳�����
	{
		bStart = false; //�����ȴ�����
		pcp->m_CopyState = CFileCopy::COPY_WAIT_SATRT;
		return;
	}
	bStart = true; // �����µ��߳̿�ʼִ��
	pcp->m_CopyState = CFileCopy::COPY_RUNNING;
	m_iCurrentRunningThreadCount++; //��ִ�е��̸߳�������
	m_CopyQueue.push(str); // �����������һ������
}
void CCopyManager::DestoyrCopyProgressInfo()
{
	map<CString,CFileCopy *>::iterator iter = g_CopyProgressInfo.begin();
	for (iter;iter!=g_CopyProgressInfo.end();iter++)
	{
		if (iter->second->m_pMsgWnd)
		{
			iter->second->m_pMsgWnd->DestroyWindow();
			iter->second->m_pMsgWnd = NULL;
		}

		delete iter->second;
	}

	g_CopyProgressInfo.clear();
	delete this;
}
CFileCopy * CCopyManager::GetCopyProgressInfo(CString strIndex)
{
	map<CString,CFileCopy*>::iterator iter = g_CopyProgressInfo.find(strIndex);
	if (iter==g_CopyProgressInfo.end())
		return NULL;

	return g_CopyProgressInfo[strIndex];
}
void CCopyManager::CancelCopy(CString strIndex)
{
	map<CString,CFileCopy*>::iterator iter = g_CopyProgressInfo.find(strIndex);
	if (iter==g_CopyProgressInfo.end())
		return;

	g_CopyProgressInfo[strIndex]->m_bIsCancel = true;
}
void CCopyManager::SetMaxThreadCount(int iThreadCount)
{
	if (iThreadCount > 10 || iThreadCount < 1)
	{
		m_MaxThreadCount = 5;
		return;
	}
	m_MaxThreadCount = iThreadCount;
}

void CCopyManager::StartManagerThread()
{
	 //�ڴ�����һ���໤���̣��������̵߳�״̬
}