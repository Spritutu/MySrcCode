// LibTest.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "stdafx.h"
#include "..\\BatLib\\BatFileSystem.h"
int _tmain(int argc, _TCHAR* argv[])
{
	path dir("c:\\xiang");
	vector<path>v;
	FindExtenFile(dir,v,".txt");
	int num=v.size();
	BatReName(v,"����");
	return 0;
}
