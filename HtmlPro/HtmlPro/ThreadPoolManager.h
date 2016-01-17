#pragma once
#include "stdafx.h"
#include <list>
using namespace std;

class CTaskBase;
class CPoolThread;
class CThreadPoolManager
{
	friend CTaskBase;
public:
	CThreadPoolManager(int iMaxThread = 10);
	~CThreadPoolManager();
	bool AddTask(CTaskBase & pTask,BOOL bHighLevel = FALSE);
	void WaitFinish();
	static DWORD WINAPI _ControlPoolThread(LPVOID);
private:
	CTaskBase * PeekOutOneTask(); //������������ȡ��һ�����Ŷӵ�����
private:
	size_t m_iMaxThreadCount;
	CRITICAL_SECTION m_TaskCs;
	list<CTaskBase * > m_TaskList;
	list<CPoolThread *> m_PoolThreadList;
	BOOL m_bRunContrl;
};
extern CThreadPoolManager theDownLoadPoolManager;