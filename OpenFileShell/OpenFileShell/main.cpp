#include<Windows.h>
#include <shellapi.h>
#include <stdio.h>

void main()
{
	// ��Ŀ¼���ļ���ѡ��
	const char * pszPath = " /select ,K:\\program\\Programing Tools\\���\\License.Manager.Center\\msvbvm60.dll";
	printf("��Ŀ¼���ļ���ѡ��%s\n",pszPath);
	ShellExecuteA(0,"open","explorer.exe",pszPath,NULL,SW_SHOWNORMAL);

	//����ָ���ļ����ָ��Ŀ¼
	const char * pszPath2 = " /select ,K:\\program\\Programing Tools\\���\\License.Manager.Center\\msvbvm60.dll";
	printf("����ָ���ļ����ָ��Ŀ¼%s\n",pszPath2);
	ShellExecuteA(0,"open","explorer.exe",pszPath2,NULL,SW_SHOWNORMAL);
}