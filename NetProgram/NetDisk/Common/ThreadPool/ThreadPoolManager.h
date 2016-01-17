#pragma once
#include "stdafx.h"
#include <list>
#include <map>
#include <vector>
using namespace std;

class CTaskBase;
class CPoolThread;
class CThreadPoolManager
{
	friend CTaskBase;
public:
	CThreadPoolManager(int iMaxThread = 5);
	~CThreadPoolManager();
	bool AddTask(CTaskBase & pTask,BOOL bHighLevel = FALSE);
	void ExitAll();
	static DWORD WINAPI _ControlPoolThread(LPVOID);
	CTaskBase * GetTaskFromHistory(const CString & strTaskId);// ���ھ���������ȡ����ָ�룬Ȼ����������״̬
	void RemoveFinishedTask(const CString & strTaskId);
	void GetFrontMatchTask(const CString & strTaskIdFront,vector<CTaskBase *> & vOut);
	BOOL HasFrontMatchTask(const CString & strTaskIdFront);
	void ClearAllFinishedTask();
private:
	void WaitFinish();
	void WaitPoolManagerExit();
	CTaskBase * PeekOutOneTask(); //������������ȡ��һ�����Ŷӵ�����
private:
	size_t m_iMaxThreadCount;
	CRITICAL_SECTION m_TaskCs;
	list<CTaskBase * > m_TaskList;
	list<CPoolThread *> m_PoolThreadList;
	BOOL m_bRunContrl;
	HANDLE m_hManagerThread;
	map<CString,CTaskBase*> m_TaskHistoryMap;
};
extern CThreadPoolManager thePoolManager;