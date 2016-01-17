#pragma once
#include <vector>
#include <string>
using  namespace std;

typedef struct 
{
	string m_strExt; // ��׺��
	string m_strDesrciption; //������
}NewType;

class CFilePro
{
public:
	typedef struct 
	{
		long long iFileSize;
		CString m_strFileName;
		CString m_strFullPath;
		CString m_strCreateTime;
		CString m_strLastModifyTime;
		CString m_strLastAccessTime;
	}SFileInfo;

	CFilePro(void);
	~CFilePro(void);
	CString GetErrMsg(){return m_strErrMsg;}
	void ClearErrMsg(){m_strErrMsg = _T("");}

	/*
	ѡ���ļ��Ի���
	*/
	vector<CString> GetSelectedFile(bool bMutilSelect = true, const char * pszDefExt = NULL,const char *pszFilter = NULL);
	/*
	ѡ��Ŀ¼�Ի���
	*/
	CString GetSelectedFolder();
	/*
	��ָ�����ļ�
	*/
	bool OpenFile(CString strFile);

	/*
	��ȡ�ļ���MD5ֵ
	*/
	bool GetFileMD5(CString strInFile,CString & strOutMd5);

	/*
	��ȡ�ļ�����Ϣ
	*/
	bool GetFileInfo(CString strInFile,SFileInfo &fileInfo,const char *pszTimeFormat = "%Y-%m-%d %H:%M:%S");

	/*
	ɾ�������ļ�
	*/
	bool DeleteLocalFile(CString strFile);
	
	/* 
	��ȡ�����Ҽ��˵��½��ļ�����
	*/
	void GetAllDesktopRightMenuNewItems(vector<NewType> &vAllItems);
	/*
	��ָ��Ŀ¼�´���ָ�����͵����ļ�
	*/
	bool CreateNewItem(string strItemType,string strDir);
private:
	CString m_strErrMsg;
};
