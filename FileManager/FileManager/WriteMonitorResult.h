#pragma once
#include "TaskBase.h"
class CSqliteBase;
class CFileAction;

class CWriteMonitorResult :
	public CTaskBase
{
public:
	CWriteMonitorResult(CSqliteBase * pDataBase,HWND hReceiveMsg);
	~CWriteMonitorResult(void);
	virtual BOOL Run();
	virtual CTaskBase * GetCopy();
	void AddRecord(CFileAction * pAction); // �����ļ�
	void RemoveRecord(CFileAction * pAction); // ɾ���ļ�
	void ModifyRecord(CFileAction * pAction); // �������ļ�

private:
	CSqliteBase * m_pDataBase;
	HWND m_hReceiveMsg;
};

