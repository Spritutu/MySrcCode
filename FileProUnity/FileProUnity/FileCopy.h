#pragma once
#include <map>
#include <string>
#include <queue>
using  namespace std;

class MsgWnd;
class CFileProCom;
class CFileCopy
{
public:
	typedef enum{
		COPY_RUNNING     = 0, //���ڿ���
		COPY_WAIT_SATRT  = 1, // �ȴ���ʼ
		COPY_FINISH		 = 2  //�������
	}COPY_STATE;
public:
	CFileCopy(const char *pszSrc,const char *pszDest);
	bool ExcuteCopy(bool m_bForce = false,MsgWnd *pMsgWnd = NULL);
	CString GetErrMsg(){return m_strErrMsg;}
	COPY_STATE GetCopyState(){ return m_CopyState; }
private:
	void CheckParam();

	static DWORD WINAPI CopyFileThreadPro(LPVOID lpParamter);
	static DWORD WINAPI SysCopyFileThreadPro(LPVOID lpParamter);

public:
	CString strSrc;
	CString strDst;
	CString m_strErrMsg; /* �洢������Ϣ*/
	bool    m_bForce;
	long long m_iTotalBytes;
	long long m_iCurrentBytes;
	COPY_STATE m_CopyState;
	BOOL     m_bIsCancel;
	MsgWnd  *m_pMsgWnd;
	HANDLE   m_hThreadHandle;
	bool     m_bParamOK;
};

class CCopyManager
{
public:
	static CCopyManager * Instance();
	CFileCopy * GetCopyProgressInfo(CString strIndex);
	void CancelCopy(CString strIndex);
	void DestoyrCopyProgressInfo();
	void AddToManager(CString str ,CFileCopy * pcp,bool & bStart);
	void SetMaxThreadCount(int iThreadCount);
	//������һ����ؿ�������״̬�ĺ�����������״̬ʵʱ�����µĿ����߳�
	void StartManagerThread();
protected:
	CCopyManager(){ StartManagerThread(); }
private:
	map<CString,CFileCopy *> g_CopyProgressInfo;
	queue<CString> m_CopyQueue;
	int m_MaxThreadCount;
	int m_iCurrentRunningThreadCount;
	static CCopyManager * m_PManagetr;
};