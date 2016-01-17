#ifndef H_CORE_REG_SOFT
#define H_CORE_REG_SOFT
#include <string>
using namespace std;

#ifndef LICENSE_MGR_DLL_EXPORT
	#define LICENSE_MGR  __declspec(dllimport)
#else
	#define LICENSE_MGR  __declspec( dllexport) 
#endif

 void LICENSE_MGR FreeStrMen(char * pData);

// ��ȡ���
 char LICENSE_MGR * GetRegsiterKey(const char * pszClientPcCode,const char * pszSoftID,int iValidHourTime = 0);

// ��ȡӲ��ID
 char LICENSE_MGR * GetPcIdStr();

// ��֤��ɵ���Ч��
extern "C"  bool  LICENSE_MGR IsValidCode(const char * pszClientPcCode,const char * pszSoftID,const char * pszKeyValue,string & strErrMsg);

// �������Ϣд��ע���
extern "C"  bool LICENSE_MGR  WriteLicenseToReg(const char * pszKeyValue,const char * pszSoftID);

// ��ע����ȡ�����Ϣ�����ȡʧ�ܣ��򷵻ؿ��ַ���
 char  LICENSE_MGR * GetRegsiterKeyFromReg(const char * pszSoftID);

#endif //H_CORE_REG_SOFT