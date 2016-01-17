#include "stdafx.h"
#include "TaskBase.h"
#include "PoolThread.h"
#include "ThreadPoolManager.h"
CThreadPoolManager theDownLoadPoolManager;

CThreadPoolManager::CThreadPoolManager(int iMaxThread /* = 10 */)
{
	m_PoolThreadList.clear();
	m_bRunContrl = TRUE;
	if (iMaxThread < 1 || iMaxThread > 20)
		iMaxThread = 5;

	m_iMaxThreadCount = iMaxThread;
	InitializeCriticalSection(&m_TaskCs);
	HANDLE h = CreateThread(NULL,0,CThreadPoolManager::_ControlPoolThread,this,CREATE_SUSPENDED,NULL);
	ResumeThread(h);
	CloseHandle(h);
}
CThreadPoolManager::~CThreadPoolManager()
{
	EnterCriticalSection(&m_TaskCs);
	list<CPoolThread*>::iterator iter = m_PoolThreadList.begin();
	for (; iter!=m_PoolThreadList.end();)
	{
		delete (*iter);
		iter = m_PoolThreadList.erase(iter);
	}
	LeaveCriticalSection(&m_TaskCs);

	DeleteCriticalSection(&m_TaskCs);
}
bool CThreadPoolManager::AddTask(CTaskBase & pTask,BOOL bHighLevel)
{
	EnterCriticalSection(&m_TaskCs);
	if (bHighLevel)
		m_TaskList.push_front(pTask.GetCopy());
	else
		m_TaskList.push_back(pTask.GetCopy());

	LeaveCriticalSection(&m_TaskCs);
	return true;
}
CTaskBase * CThreadPoolManager::PeekOutOneTask()
{
	if (m_TaskList.empty())
		return NULL;

	CTaskBase *pTask = m_TaskList.front();
	m_TaskList.erase(m_TaskList.begin());
	return pTask;
}
void CThreadPoolManager::WaitFinish()
{
	BOOL bHaveBusy = FALSE;
	while (1)
	{
		list<CPoolThread *>::iterator iter = m_PoolThreadList.begin();
		for (;iter!=m_PoolThreadList.end();iter++)
		{
			if ((*iter)->IsBusy())
			{
				bHaveBusy = TRUE;
				break;
			}
		}
		if (!bHaveBusy)
		{
			break;
		}
	}
}
DWORD WINAPI CThreadPoolManager::_ControlPoolThread(LPVOID pParam)
{
	CThreadPoolManager *pThis = (CThreadPoolManager*)pParam;
	// �����ȴ�����������߳�
	while(pThis->m_PoolThreadList.size() < pThis->m_iMaxThreadCount)
	{
		CPoolThread *pThread = new CPoolThread;
		pThis->m_PoolThreadList.push_back(pThread);
	}
	
	// ���̷߳�������
	while(pThis->m_bRunContrl)
	{
		CTaskBase *pTask = pThis->PeekOutOneTask();
		if (pTask == NULL)
		{
			Sleep(100);
			continue; // û�еȴ�������
		}
		BOOL bAttached = FALSE;
		while(1)
		{
			if (pThis->m_bRunContrl)
				break;

			list<CPoolThread *>::iterator iter = pThis->m_PoolThreadList.begin();
			for (;iter!=pThis->m_PoolThreadList.end();iter++)
			{
				if (!(*iter)->IsBusy())
				{
					(*iter)->AttchTask(pTask);
					bAttached = TRUE; // �ɹ���һ�����񸽼ӵ������߳�
					break;
				}
			}
			if (bAttached)
				break; 
			else
				Sleep(50); // �����̶߳���æµ����50���������¼����û�п����߳�
		}
		

	}
	return 0;
}