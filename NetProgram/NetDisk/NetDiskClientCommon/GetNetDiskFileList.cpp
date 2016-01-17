#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include "CommonFun.h"
#include "DataJsDefine.h"
#include "LogTools.h"
using namespace std;

bool GetNetDiskFileList(SOCKET sockConn,const char * pszUserName,const char * pszNetDiskPath,bool bRecursion,vector<FileListItem *> &pFileList)
{
	CJsonDataGetFileList DataGetList;
	DataGetList.m_strUsrId = pszUserName;
	DataGetList.m_strNetDiskDir = pszNetDiskPath;
	DataGetList.m_bRecursion = bRecursion;
	string strData = DataGetList.GetResultJson();
	int iDataParamLen = strData.length();
	if (iDataParamLen <=0)
		return false;

	LogMsg("���ͻ�ȡ�ļ��б�����...\n");
	Client_Op op = getlist;
	string strErrMsg;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
		return false;
	
	iDataParamLen +=1; //�����ַ���ʱ�෢��һ���ֽ�
	if (!SendIntData(sockConn,iDataParamLen,strErrMsg))
		return false;

	if (!SendData(sockConn,(char*)strData.c_str(),iDataParamLen,strErrMsg))
		return false;

	int iReturnJsLen = 0;
	if (!RecvIntData(sockConn,iReturnJsLen,strErrMsg))
		return false;

	string strReturnJs;
	if (!RecvStringData(sockConn,iReturnJsLen,strReturnJs,strErrMsg))
		return false;

	CJsonDataSendFileList DataSendList;
	if (!DataSendList.Parse(strReturnJs.c_str()))
	{
		LogMsg("�������JSʧ�ܣ�\n");
		return false;
	}

	DataSendList.GetSendListCopied(pFileList);
	return true;
}