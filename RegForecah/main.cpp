#include <stdio.h>
#include "RegRead.h"
void Test()
{
	vector<SNewFileTypeReg> vRegNew;
	if (!GetAllNewFileTypeReg(vRegNew))
	{
		printf("��ȡ�ļ��ļ�����ʧ��!\n");
		return;
	}

	for(size_t i=0;i< vRegNew.size();i++)
	{
		printf("�½��ļ����ͣ�%s\n",vRegNew[i].m_strExt.c_str());
		printf("ע����ֵ��%s\n",vRegNew[i].m_strKeyPath.c_str());
		printf("...................................\n\n");
	}

	printf("�����%d���½��ļ����ͣ�\n",vRegNew.size());
}

void Test2()
{
	vector<NewType> vStrList;
	GetAllDesktopRightMenuNewItems(vStrList);
	for (int i=0;i<vStrList.size();i++)
		if (!CreateNewItem(vStrList[i].m_strExt,"E:\\TestNewItem"))
			printf("�������͡�%s���ļ�ʧ��!\n",vStrList[i].m_strDesrciption.c_str());
}
void Test3()
{
	string strOut =  GetWindowVesion();
	printf("ϵͳ�汾Ϊ��%s\n",strOut.c_str());
}
void main()
{
	Test2();
	::system("pause");
}