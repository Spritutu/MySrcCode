
#ifndef BATFILESYSTEM_H
#define BATFILESYSTEM_H

#include <boost/assign.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/progress.hpp>
#include <boost/format.hpp>
#include <vector>
#include <BaseTsd.h >
using namespace boost;
using namespace boost::xpressive;
using namespace boost::filesystem;
using namespace boost::assign;
using namespace std; //��vector����ʹ�������ռ䣬�������

typedef recursive_directory_iterator rd_iterator;  //������Ŀ¼�ĵ�����
typedef directory_iterator       urd_iterator; //ֻ������Ŀ¼�����������ĵ�����
namespace fs=boost::filesystem;

typedef enum{
	/*! ����0, ��ȷ*/			RE_SUCCESS    =0,	
	/*! ����1��ʧ��*/			RE_FAILED     =1,		
	/*! ����2����������*/		RE_PARAMERROR =2,	
	/*! ����3���û�ȡ��*/		RE_USERCANCEL =3,	
	/*! ����4��δ�ҵ���Ӧ��Ŀ*/	RE_NOTFOUND	  =4,	
	/*! ����5�����ļ�ʧ��*/	RE_OPENFAILED =5	
} RE_VALUE;
struct FileInfo{
	/*! ��չ��*/
	string ext;
	/*! �ļ���*/
	string filename;
	/*! ��·��*/
	string ParentPath;
	/*! �ļ���С*/
	UINT64 filesize;
};
/*!�����ӿ�*/
#ifndef BAT_TOOL_API
#define BAT_TOOL_API _declspec(dllexport)
#else	
#define BAT_TOOL_API _declspec(dllimport)
#endif
#ifdef _cplusplus
extern "C"{
#endif
/** 
* @brief ��������Ŀ¼�µ�������Ŀ¼
* @param dir Ŀ¼��
* @param ResultPath ��Ų��ҵ���Ŀ¼�б��vector
* @param bIsRecurrence �Ƿ�ݹ���ң�Ϊtrueʱ������Ŀ¼����Ŀ¼��falseʱֻ���ҵ�ǰĿ¼
* @return ���ش�����Ϣ
*/
int BAT_TOOL_API FindSubDirectory(path &dir,vector<path> &ResultPath ,bool bIsRecurrence=true);
/** 
* @brief ��������Ŀ¼�µ������ļ�������Ŀ¼
* @param dir Ŀ¼��
* @param ResultFileList ��Ų��ҵ����ļ��б��vector
* @param bIsRecurrence �Ƿ�ݹ���ң�Ϊtrueʱ������Ŀ¼����Ŀ¼��falseʱֻ���ҵ�ǰĿ¼
* @return ���ش�����Ϣ
*/
int BAT_TOOL_API FindAllFile(path &dir,vector<path> &ResultFileList,bool bIsRecurrence=true);
/** 
* @brief ��������Ŀ¼�µ������ļ���Ŀ¼
* @param dir Ŀ¼��
* @param ResultList ��Ų��ҵ���Ŀ�б��vector
* @param bIsRecurrence �Ƿ�ݹ���ң�Ϊtrueʱ������Ŀ¼����Ŀ¼��falseʱֻ���ҵ�ǰĿ¼
* @return ���ش�����Ϣ
*/
int BAT_TOOL_API FindAllFileAndDirectory(path &dir,vector<path> &ResultList,bool bIsRecurrence=true);
/** 
* @brief ��������Ŀ¼�µ�ָ����չ�����ļ�
* @param dir Ŀ¼��
* @param ResultList ��Ų��ҵ��ļ��б��vector
* @param extension ��չ��
* @param bIsRecurrence �Ƿ�ݹ���ң�Ϊtrueʱ������Ŀ¼����Ŀ¼��falseʱֻ���ҵ�ǰĿ¼
* @return ���ش�����Ϣ
*/
int BAT_TOOL_API FindExtenFile(path &dir,vector<path> &ResultList,string extension=".*",bool bIsRecurrence=true );
/**
* @brief �ڸ����ļ������ָ�����ַ���
* @param filename  �ļ���
* @param strAppend ��ӵ��ַ���
* @return ���س�����Ϣ
*/
int BAT_TOOL_API AppendStrToFile(const char* filename,string strAppend);
/** 
* @brief �����ļ���������Ŀ¼
* @param theFile Ҫ���Ƶ��ļ���ȫ·��
* @param vpath Ŀ��Ŀ¼�б�
* @return ���س�����Ϣ
*/
int  BAT_TOOL_API CopyFileToDirectory(path &theFile,vector<path>& vpath,bool bOverWritten=false);
/** 
* @brief ��ȡ�����ļ�����Ϣ
* @param pathFile �����ļ���ȫ·��
* @param strInfo �ļ���Ϣ
* @return ���س�����Ϣ
*/
int  BAT_TOOL_API GetFileInfo(path & pathFile,FileInfo &strInfo);
/**
* @brief ɾ��ĳ��Ŀ¼��ָ�����ֵĶ���
* @param dir Ŀ¼��
* @param theName ָ��������
* @param IsFile ɾ����ʶ,true��ɾ���ļ���false ɾ��Ŀ¼
* @param bIsRecurrence Ϊtrue��ݹ飬���򲻵ݹ�
* @return ���س�����Ϣ
*/
int BAT_TOOL_API DelFile(path &dir,string theName,bool IsFile,bool bIsRecurrence=true);
/** 
* @brief ����Ŀ¼��ָ����ʽ���ļ�
* @param dir Ŀ¼��
* @param filename Ҫ���ҵ��ļ���
* @param v ���ص�·��vector
* @return �޷���ֵ
*/
void BAT_TOOL_API find_file(const path &dir,const string &filename,vector<path>&v);
/**
* @brief ����������
* @param vInpath ���������ļ��б�
* @param strPre ����ǰ׺
* @return ���س�����Ϣ
*/
int  BAT_TOOL_API BatReName(vector<path> vInpath,string strPre);
#ifdef _cplusplus
}
#endif
#endif