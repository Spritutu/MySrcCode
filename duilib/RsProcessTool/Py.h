#ifndef _PY_H_
#define _PY_H_

#include<string>
using namespace std;
/********************������ͷ�ļ�********************************/

/* ����: Py.h
 * ʵ�ֹ���: ��ȡƴ��
 * �汾: 1.00
 * ˵��: ���汾�����ϻ�õ�һ��Dephile����"����"����
 * ������: bohut
 * ���ʱ�� : 2003.07.08
 */
#pragma once
#define PYMUSICCOUNT   405

class CPy  
{
public:
	CPy(){}
	virtual ~CPy(){}
 string MakeSpellCode(string strText, int nMode);
};

#endif //_PY_H_