// BatLibTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "..\BatFileSystem.h"
#pragma comment(lib,"..\\Debug\\BatLib.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	path dir("c:\\xiang");
	vector<path>v;
	FindExtenFile(dir,v,".txt",true);
	printf("���ҵ�%d��txt�ļ�\n",v.size());
	return 0;
}

