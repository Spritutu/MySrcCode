// HtmlPro.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "HtmlPro.h"
#include "ThreadPoolManager.h"
#include "DownloadTask.h"
#include "BaiduPicture.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  STREQUAL(a,b) _stricmp(a,b)==0

// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;
BOOL PeraseCmd(int argc, TCHAR* argv[],BOOL &bIsFile,CString & strOut,CString &strUrl)
{
	bIsFile = FALSE;
	strOut = "";
	strUrl  = "";

	for (int i=1;i< argc;i++)
	{
		if (STREQUAL(argv[i],"-bf"))
		{
			CString str = argv[++i];
			str.MakeLower();
			if (STREQUAL(str,"true"))
			{
				bIsFile =  TRUE;
			}
			else
				bIsFile =  FALSE;

		}
		else if (STREQUAL(argv[i],"-url"))
		{
			strUrl = argv[++i];
		}
		else if (STREQUAL(argv[i],"-out"))
		{
			strOut = argv[++i];
		}
	}

	if (strUrl.IsEmpty())
		return FALSE;

	if (strOut.IsEmpty())
	{
		if (bIsFile)
			printf("��ָ�����html�ļ���·����\n");
		else
			printf("��ָ�������ļ��洢Ŀ¼��\n");
		return FALSE;
	}

	return TRUE;
}

void PrintHelp()
{
	printf("ʹ�÷�����\nHtmlPro.exe -bf TRUE[FALSE],-url url -out out.html\
		   \nʾ��1��HtmlPro.exe -bf FALSE,-url \"www.baidu.com\" -out \"C:\\downloaddir\"\nʾ��2��HtmlPro.exe -bf TRUE,-url \"www.baidu.com\" -out \"C:\\out.html\"\n");
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣

			vector<CString> VBaiduImgLinks;
			CString strCon = GetJsFromFile("E:\\js����part.txt");
			if (strCon.IsEmpty())
				return 0;

			GetAllImgUrl(VBaiduImgLinks,strCon);
			CString strOutPath = "E:\\baidu";
			for (size_t i=0;i<VBaiduImgLinks.size();i++)
			{
				//DownloadFile(strOutDir,VImgLinks[i]);
				CDownloadTask DownloadTask(strOutPath,VBaiduImgLinks[i]);
				theDownLoadPoolManager.AddTask(DownloadTask);
			}
			while(1);

			CString strOut = "";
			BOOL bIsFile = FALSE;
			CString strUrl = "http://www.netbian.com/";

			if (!PeraseCmd(argc,argv,bIsFile,strOut,strUrl))
			{
				PrintHelp();
				return 1;
			}
			CString strOutDir = strOut;
			DownloadHtml(strOut,strUrl,bIsFile);
			//DownloadFile("C:\\","http://zhangmenshiting.baidu.com/data2/music/119826740/1197655931400515261128.mp3?xcode=e095060edcb512008a268d64a327f6a2e36664380262dc7e");
			
			if (!bIsFile)
			{
				vector<CString> VImgLinks;
				GetImgLink(VImgLinks,strOut,strUrl);
				for (size_t i=0;i<VImgLinks.size();i++)
				{
					//DownloadFile(strOutDir,VImgLinks[i]);
					CDownloadTask DownloadTask(strOutDir,VImgLinks[i]);
					theDownLoadPoolManager.AddTask(DownloadTask);
				}

				while(1);
				//theDownLoadPoolManager.WaitFinish();
			}
			
		}
	}
	else
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: GetModuleHandle ʧ��\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
