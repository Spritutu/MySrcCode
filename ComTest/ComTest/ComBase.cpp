// ComBase.cpp : CComBase ��ʵ��

#include "stdafx.h"
#include "ComBase.h"


// CComBase


STDMETHODIMP CComBase::PrintMsg(CHAR* msg)
{
	// TODO: �ڴ����ʵ�ִ���
	printf("%s",msg);

	return S_OK;
}

STDMETHODIMP CComBase::Sum(LONG iStart, LONG iEnd)
{
	// TODO: �ڴ����ʵ�ִ���

	LONG sum = 0;
	for (LONG i= iStart;i<=iEnd;i++)
	{
		sum += i;
	}
	printf("%d",sum);
	return S_OK;
}
