#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
FILE *fp;
int vfpf(char *fmt, ...)

{

	va_list argptr;

	int cnt;

	va_start(argptr, fmt);

	cnt = vfprintf(fp, fmt, argptr);

	va_end(argptr);

	return(cnt);

}

int Fun(int iTotal,int iNum)
{
	if (iTotal<iNum)
		return 0;
	else
	{
		int iCount = iTotal / iNum; //����ֱ�ӻ�ȡ�ĸ���
		int iOther = iTotal % iNum; //���㻻ȡʱʣ��ĸ���
		return iCount + Fun(iOther + iCount,iNum); //������һ�λ���
	}
}
/**
*@brief ���������ң����ɳ����ӵĸ���
*@param iTotal
*@param iTotal
*@return �������ճ����ӵ��ܸ���
*/
int CalcResult(int iTotal ,int iNum)
{
	int iCount = Fun(iTotal,iNum); //���㻻ȡ���ܸ���
	return iTotal +iCount; //���ճ����ӵ��ܸ���
}
int main(int argc,char *argv[])
{
	int iResult = CalcResult (10,3);
	printf("�ܹ��ɳԵ�����%d��..\n",iResult);
	return 0;

	/*int inumber = 30;

	float fnumber = 90.0;

	char string[4] = "abc";

	fp = tmpfile();

	if (fp == NULL)

	{

		perror("tmpfile() call");

		exit(1);

	}

	vfpf("%d %f %s", inumber, fnumber, string);

	rewind(fp);

	fscanf(fp,"%d %f %s", &inumber, &fnumber, string);

	printf("%d %f %s\n", inumber, fnumber, string);

	fclose(fp);*/

}