#include <WinSock2.h>
#include <iostream>
#include <io.h>
#include <time.h>
#include <string>
#include "CommonFun.h"
#include "DataJsDefine.h"
#include "LogTools.h"
using namespace std;

bool RegisterUser(SOCKET sockConn,const char * pszUserId,const char *pszUserName,const char *pszPasswd)
{
	LogMsg("����ע���û�����!\n");
	Client_Op op = registerUser;
	string strErrMsg;
	if (!SendData(sockConn,(char*)&op,sizeof(Client_Op),strErrMsg))
		return false;

	CJsonDataRegisterUser DataRegister;
	DataRegister.m_strUserId = pszUserId;
	DataRegister.m_strUserName = pszUserName;
	DataRegister.m_strPassWord = pszPasswd;

	string strJsParam = DataRegister.GetResultJson();
	int iLenParam = strJsParam.length();
	if (iLenParam <=0)
	{
		LogMsg("����Json��ʧ�ܣ�\n");
		closesocket(sockConn);
		return false;
	}

	iLenParam +=1; // �����ַ���ʱ�෢��һ���ֽ�
	if (!SendIntData(sockConn,iLenParam,strErrMsg))
			return false;

	if (!SendData(sockConn,(char*)strJsParam.c_str(),iLenParam,strErrMsg))
		return false;

	char chOK = 3;
	if (!RecvData(sockConn,&chOK,1,strErrMsg))
		return false;

	closesocket(sockConn);
	if (chOK=='0')
	{
		LogMsg("ע��ʧ��!\n");
		return false;
	}
	else
	{
		LogMsg("ע��ɹ�!\n");
		return true;
	}
}

bool Login(SOCKET socket,const char * pszUsrId,const char * pszPassWd,CDataLoginResult & LoginRes)
{
	LoginRes.Clear();
	if (pszUsrId==NULL || pszPassWd == NULL || strcmp(pszUsrId,"")==0 || strcmp(pszPassWd,"")==0 )
	{
		strcpy_s(LoginRes.m_szErrMsg,"�û��������벻��Ϊ��!");
		return false;
	}

	
	Client_Op op =  userLogin;
	string strErrMsg;
	if (!SendData(socket,(char*)&op,sizeof(Client_Op),strErrMsg))
	{
		strcpy_s(LoginRes.m_szErrMsg,"ͨѶ����,���͵�¼����ʧ��!");
		return false;
	}

	CDataLogin LoginData;
	strcpy_s(LoginData.m_szUsrId,pszUsrId);
	strcpy_s(LoginData.m_szPassWd,pszPassWd);

	if (!SendData(socket,(char*)&LoginData,sizeof(CDataLogin),strErrMsg))
	{
		strcpy_s(LoginRes.m_szErrMsg,"ͨѶ����,���͵�¼��Ϣʧ��!");
		return false;
	}

	if (!RecvData(socket,(char*)&LoginRes,sizeof(CDataLoginResult),strErrMsg))
	{
		LoginRes.Clear();
		strcpy_s(LoginRes.m_szErrMsg,"ͨѶ����,���յ�¼���ʧ��!");
		return false;
	}
	if (strcmp(LoginRes.m_szErrMsg,"")!=0)
		return false;

	return true;
}