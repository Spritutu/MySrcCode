// Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include <stdio.h>
#include <Windows.h>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

unsigned  long   WINAPI  ThreadPro(PVOID   pvoid)   
{
	STARTUPINFO si;  
	PROCESS_INFORMATION pi;  

	ZeroMemory(&si,sizeof(STARTUPINFO));  

	si.cb = sizeof(STARTUPINFO);  
	GetStartupInfo(&si);  
	si.hStdError = NULL;  
	si.hStdOutput = NULL; //�´������̵ı�׼�������д�ܵ�һ��  
	si.wShowWindow = SW_SHOW; //��ʾ����  
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  


	char cmdline[200];  
	sprintf(cmdline,"%s","C:/Windows/rose.exe");  

	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) //�����ӽ���  
	{  
		//MessageBox("CreateProcess Failed!","��ʾ",MB_OK | MB_ICONWARNING);  
		return 1;  
	}

	//::system("E:\\WindowsFormsApplication1.scr");
	return 0;
}
int main(int argc, char* argv[])
{
	DWORD   dwThreadId;   
	void  * pThread   =   CreateThread(     
		NULL,       //   no   security   attributes     
		0,          //   use   default   stack   size       
		ThreadPro,  //   thread   function     
		0,       //   argument   to   thread   function     
		0,      //   use   default   creation   flags     
		&dwThreadId);     
	DWORD   dwwait=WaitForSingleObject(pThread,INFINITE);  

	return 0;
}

