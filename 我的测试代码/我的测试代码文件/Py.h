#ifndef PY_H
#define PY_H
/********************������ͷ�ļ�********************************/

/* ����: Py.h
* ʵ�ֹ���: ��ȡƴ��
* �汾: 1.00
* ˵��: ���汾�����ϻ�õ�һ��Dephile����"����"����
* ������: bohut
* ���ʱ�� : 2003.07.08
*/
#include <string>
using namespace std;
#define PYMUSICCOUNT   405

class CPy  
{
public:
	CPy(){}
	virtual ~CPy(){}
	string MakeSpellCode(string strText, int nMode);

	typedef struct 
	{
		char *PYCode[7];
	}TPYCode;
	TPYCode TFPYCodes[126][191];

};
#endif //PY_H