#include <iostream>
using namespace std;
typedef int BOOL;
#define FALSE 0
#define TRUE  1

typedef struct _TagNode
{
	_TagNode()
	{
		m_ch = 0;
		m_pNext = NULL;
		m_pPrev = NULL;
	}
	char m_ch;
	_TagNode * m_pNext;
	_TagNode * m_pPrev;
}CharNode;

void PrintfCircleList(CharNode * pNodeFirst)
{
	CharNode * pCurNode = pNodeFirst;
	while(1)
	{
		printf("%c ",pCurNode->m_ch);
		if (pCurNode->m_pNext == pNodeFirst)
			break;

		pCurNode = pCurNode->m_pNext; //������� 
	}

}
BOOL DeleteCircleNode()
{
	const char * strFile = "E:\\data.txt";
	FILE * pFile = NULL;
	fopen_s(&pFile,strFile,"r+");
	if (pFile==NULL)
	{
		printf("��ȡ�����ļ�[%s]ʧ��!",strFile);
		return FALSE;
	}

	CharNode *pPreNode   = NULL;
	CharNode *pNextNode  = NULL;
	CharNode *pNodeCur   = NULL; 
	CharNode *pNodeFirst = NULL;
	char * pChar = NULL;
	char szBuf[4] ={0};
	while(pChar=fgets(szBuf,2,pFile))
	{
		if (szBuf[0]==' ')
			continue;

		pNodeCur = new CharNode;
		if (pNodeFirst==NULL)
			pNodeFirst = pNodeCur;
		if (pPreNode) 
			pPreNode->m_pNext = pNodeCur;

		pNodeCur->m_ch = szBuf[0];
		pNodeCur->m_pPrev = pPreNode;
		pNodeCur->m_pNext = NULL;
		pPreNode = pNodeCur;
	}
	fclose(pFile);

	fopen_s(&pFile,"E:\\out_data.txt","a+");
	if (pFile==NULL)
	{
		printf("��������ļ�ʧ��!\n");
		return FALSE;
	}

	pNodeCur->m_pNext = pNodeFirst;  //�����һ���ڵ����һ���ڵ�ָ���׽ڵ㣬�γɻ�״
	pNodeFirst->m_pPrev = pNodeCur;  //���׽ڵ��ǰһ���ڵ�ָ��β�ڵ㣬�γɻ�״

	pNextNode = NULL;
	pPreNode  = NULL;

	//���ԭʼ�б�
	printf("ԭʼ�ڵ��б�Ϊ��\n");
	PrintfCircleList(pNodeFirst);
	printf("\n");

	pNodeCur = pNodeFirst; // �ӽڵ�A��ʼ
	int iStart = 0; //��A��ʼ�ĵڼ����ڵ�
	int iDis  = 0; //ƫ�Ʋ���
	cout << "������ɾ������ʼλ�úͼ����\n";
	cin >>iStart>>iDis;
	fprintf(pFile,"��ʼλ�ã�%d , ��� %d\n",iStart,iDis);

	iStart  = iStart - 1;
	iDis    = iDis - 1;

	if (iStart>=0) // ��ʼλ�ô��ڵ���0
	{
		for (int i=0;i<iStart;i++)
			pNodeCur = pNodeCur->m_pNext;
	}
	else // ��ʼλ��С��0
	{
		for (int i=0;i<iStart;i++)
			pNodeCur = pNodeCur->m_pPrev;
	}

	printf("��ʼɾ���ڵ�:\n");
	while(pNodeCur)
	{
		if (iDis>=0) // ������ڵ���0
		{
			for (int i=0;i<iDis;i++)
				pNodeCur = pNodeCur->m_pNext;
		}
		else // ���С��0
		{
			for (int i=0;i<iDis;i++)
				pNodeCur = pNodeCur->m_pPrev;
		}

		pNextNode = pNodeCur->m_pNext;
		if (pNextNode == pNodeCur)
		{
			printf("%c �Ƴ�\n",pNodeCur->m_ch);
			fprintf(pFile,"%c\n\n",pNodeCur->m_ch);
			delete pNodeCur;
			break;
		}
		pNodeCur->m_pPrev->m_pNext = pNextNode;
		pNextNode->m_pPrev = pNodeCur->m_pPrev;
		if (pNodeCur==pNodeFirst) // �׽ڵ��Ƴ��ˣ����׽ڵ�ָ����һ�ڵ�
			pNodeFirst = pNextNode;

		printf("%c �Ƴ�\n",pNodeCur->m_ch);
		fprintf(pFile,"%c ",pNodeCur->m_ch);

		delete pNodeCur;
		pNodeCur = pNextNode; //������� 
		PrintfCircleList(pNodeFirst);
		printf("\n");
	}

	return TRUE;
}