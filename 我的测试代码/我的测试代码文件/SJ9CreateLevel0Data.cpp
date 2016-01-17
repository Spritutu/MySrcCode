/***************************************************************************
*
* Time: 2013-05-21
* Project: ң��ƽ̨
* Purpose: ʵ���ź�����Ԥ�����㷨
* Author:  ����¼
* Copyright (c) 2013, liminlu0314@gmail.com
* Describe:�ṩ����ʵ���źŶ�����������ɿ���ͼ�������ӿ�
*
****************************************************************************/
#include "SJ9DataProcessorAlg.h"
#include "ImageBatCommon.h"
#include "DataPreparation.h"

#include "gdal_priv.h"

#include "boost/format.hpp"
#include "boost/progress.hpp"	//boost��ʱ����
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/xpressive/xpressive_dynamic.hpp"

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#include "D:\RsSrcDir\Release\FindPro.h"
#include "stdlib.h"
#endif

using namespace boost;
using namespace boost::filesystem;
using namespace boost::xpressive;

typedef recursive_directory_iterator rdir_iterator;	//�ݹ�Ŀ¼������
typedef directory_iterator dir_iterator;			//��ǰĿ¼������

#define TRaceTime(MSG,T,dLastTime) printf(MSG);\
	printf("���ۼƺ�ʱ%fs,������ʱ%fs\n",T.elapsed(),T.elapsed() - dLastTime);\
	dLastTime = T.elapsed();

extern void WriteInSJ9XmlFile(const char* ExplainingFileIn,const char* XmlFilePath);//�ⲿ��������

//�����ļ��У������Ŀ¼�����ڣ��Զ�����
void RMakeDir(const char *pszPath)
{
	stack<string> st;
	string strTmp = pszPath;
	while(1)
	{
		if (chdir(strTmp.c_str())) //�����ǰĿ¼������
		{
			st.push(strTmp);
			strTmp = strTmp.substr(0,strTmp.find_last_of('\\')); //��ȡ��ǰĿ¼�ĸ�Ŀ¼
		}
		else
			break;
	}

	while(!st.empty())
	{
		string str =st.top();
		mkdir(str.c_str());
		st.pop();
	}
}

static string ConstructDirectory(const char *p1, const char *p2, const char *p3, const char *p4,
								 const char *p5 = "", const char *pszExt = ".raw")
{
	format fmtPath("%s_%s_%s_%s%s%s");	//�ļ�·����ʽ
	fmtPath % p1 % p2 % p3 % p4 % p5 % pszExt;
	return fmtPath.str();
}

static string ConstructFolderName(const char *pszInPutFileName)
{
	string strFileName = path(pszInPutFileName).stem();
	int ipos = strFileName.find("2");
	if (ipos!=string::npos)
		strFileName =strFileName.substr(ipos,strFileName.length());
	return strFileName;
}

struct CampareFileSize
{
	bool operator()(const path & leftFile,const path & rightFile) const
	{
		return (file_size(leftFile) > file_size(rightFile));
	}
};

CSJ9L0DataProduce::CSJ9L0DataProduce(const char *pszInPutFolder,const char *pszOutPutFolder,BOOL bBeijingStation)
{
	m_strInputFolder = pszInPutFolder;
	m_strParentOutputFolder = pszOutPutFolder;
	m_bBeijingStation = bBeijingStation;
	m_pBatProgress = NULL;
	m_pAllProcess  = NULL;
	m_pSubProcess  = NULL;

	string strTmp = SJ9_GetSystemParamters("SJ9A_TEMPDIR", CPLGenerateTempFilename("")); //��ʱ����Ŀ¼
	printf("%s\n", strTmp.c_str());
	CPLSetConfigOption("SJ9A_TEMPDIR", strTmp.c_str());
}

CSJ9L0DataProduce::~CSJ9L0DataProduce()
{
	if(0)
	{
		remove( m_strTempFolder.c_str());
	}
}

bool CSJ9L0DataProduce::ScanInputFolder()
{
	static sregex_compiler rc;	//������ʽ����
	path dir = GetInputFolder();
	dir_iterator end;		//��ǰ��������ĩβ

	bool bMatchedCCD =false;
	bool bMatchedHSI =false;

	if (m_bBeijingStation)
	{
		path dirCCD,dirHSI;

		string strCCD = ".*SJ9A_SEPAMODE_CCD_(1|2)\\d{3}(0|1)\\d[0-3]\\d\\d{6}_\\d{9}$";
		string strHSI = ".*SJ9A_SEPAMODE_HSI_(1|2)\\d{3}(0|1)\\d[0-3]\\d\\d{6}_\\d{9}$";
		rc[strCCD] = rc.compile(strCCD, icase);		//����һ��������ʽ��icase��ʾ��Сд�޹�
		rc[strHSI] = rc.compile(strHSI, icase);		//����һ��������ʽ��icase��ʾ��Сд�޹�
		for(dir_iterator pos(dir); pos != end; ++pos)
		{
			if(is_directory(*pos) && regex_match(pos->path().string(), rc[strCCD]))
			{
				bMatchedCCD = true;
				dirCCD = *pos;
			}

			if(is_directory(*pos) && regex_match(pos->path().string(), rc[strHSI]))
			{
				bMatchedHSI = true;
				dirHSI = *pos;
			}
		}
		if (!bMatchedCCD || !bMatchedHSI)
			return false;

		char SZPATH[1024];
		for (int i=0;i<4;i++) //�ж�ȫɫ�����Ƿ���ȫ
		{
			sprintf(SZPATH,"%s_%d.dat",dirCCD.filename().c_str(),i);
			path p = dirCCD /SZPATH;
			m_vstrCCDFile.push_back(p.string());
			if (!(!is_directory(p) &&exists(p)))
				return false;
		}
		for (int i=4;i<6;i++) //�ж϶���������Ƿ���ȫ
		{
			sprintf(SZPATH,"%s_%d.dat",dirHSI.filename().c_str(),i);
			path p = dirHSI /SZPATH;
			m_vstrHSIFile.push_back(p.string());
			if (!(!is_directory(p) &&exists(p)))
				return false;
		}
	}
	else
	{
		string strCCD = ".*SJ9A-PAN-(1|2)\\d{3}(0|1)\\d[0-3]\\d-\\d{6}-\\d{10}.dat$";
		string strHSI = ".*SJ9A-MUX-(1|2)\\d{3}(0|1)\\d[0-3]\\d-\\d{6}-\\d{10}.dat$";
		rc[strCCD] = rc.compile(strCCD, icase);		//����һ��������ʽ��icase��ʾ��Сд�޹�
		rc[strHSI] = rc.compile(strHSI, icase);		//����һ��������ʽ��icase��ʾ��Сд�޹�

		for(dir_iterator pos(dir); pos != end; ++pos)
		{
			if(!is_directory(*pos) && regex_match(pos->path().string(), rc[strCCD]))
			{
				m_vstrCCDFile.push_back(pos->path().string());
				bMatchedCCD = true;
			}
			if(!is_directory(*pos) && regex_match(pos->path().string(), rc[strHSI]))
			{
				m_vstrHSIFile.push_back(pos->path().string());
				bMatchedHSI = true;
			}
		}
		if (!bMatchedCCD || !bMatchedHSI)
			return false;
	}

	// �ж�˵��.txt�ļ��Ƿ����
	path p = dir / "˵��.txt";
	if (!(!is_directory(p) &&exists(p)))
		return false;

	return true;
}

void CSJ9L0DataProduce::MakeOutDir()
{
	m_strFinalOutputFolder = m_strParentOutputFolder + string("\\") + ConstructFolderName(m_vstrCCDFile[0].c_str());
	const char *pszOutFolderFinal = m_strFinalOutputFolder.c_str();
	RMakeDir(pszOutFolderFinal);
}

void CSJ9L0DataProduce::MakeTmpDir()
{
	string strTempFileName = CPLGenerateTempFilename("");
	const char* pszDir = CPLGetDirname(strTempFileName.c_str()); //��ȡ��Ŀ¼
	string strTemp 	=  CPLGetConfigOption("SJ9A_TEMPDIR",pszDir);
	m_strTempFolder = strTemp + "\\BeijingStation_" + ConstructFolderName(m_vstrCCDFile[0].c_str());;
}

int CSJ9L0DataProduce::DoDataExtractBeijing()
{
	string pInPutFile;
#ifdef _WIN32
	char ExeFile[260];
	CPLGetExecPath(ExeFile,260);
	string strExePath = CPLGetPath(ExeFile);
	for (int i=0;i<3;i++)
	{
		if (i<=3)
			pInPutFile = m_vstrCCDFile[i];
		else
			pInPutFile = m_vstrHSIFile[i-4];

		string strArgument = "-src ";
		strArgument += pInPutFile;
		strArgument += " -dst ";
		strArgument += m_strTempFolder;

		//strArgument = strExePath + string("\\SJ9DataExtract.exe ") + strArgument;
		//::system(strArgument.c_str());
		ShellExecute(NULL,NULL,"SJ9DataExtract.exe",strArgument.c_str(),strExePath.c_str(),1);
	}
	while(1)
	{
		if (FindProcByName("SJ9DataExtract.exe")==0)
			break;
		_sleep(5000);
	}
	for (int i=3;i<6;i++)
	{
		if (i<=3)
			pInPutFile = m_vstrCCDFile[i];
		else
			pInPutFile = m_vstrHSIFile[i-4];

		string strArgument = "-src ";
		strArgument += pInPutFile;
		strArgument += " -dst ";
		strArgument += m_strTempFolder;

		//strArgument = strExePath + string("\\SJ9DataExtract.exe ") + strArgument;
		//::system(strArgument.c_str());
		ShellExecute(NULL,NULL,"SJ9DataExtract.exe",strArgument.c_str(),strExePath.c_str(),1);
	}
	while(1)
	{
		if (FindProcByName("SJ9DataExtract.exe")==0)
			break;
		_sleep(5000);
	}
	return RE_SUCCESS;
#endif
	
	int  iRev = RE_FAILED;
	for (int i=0;i<6;i++)
	{
		if (i<=3)
			pInPutFile = m_vstrCCDFile[i];
		else
			pInPutFile = m_vstrHSIFile[i-4];

		iRev = SJ9_DataExtract(pInPutFile.c_str(), m_strTempFolder.c_str(),m_pSubProcess);
		if (m_pAllProcess)
			m_pAllProcess->StepIt();

		if (iRev != RE_SUCCESS)
		{
			printf("%s�������!\n",pInPutFile.c_str());
			return iRev;
		}
	}
	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoPanMergeBeijing(string & strOutPutMergedFile)
{
	char ** InPutList = NULL;
	int  iRev = FindMergeInPutFileList(&InPutList,true);

	int iRowStart[8], iRowCount;
	string strSatMetaFileNamePart = m_strFinalOutputFolder +  ConstructDirectory("\\SJ9A","PAN","20130530023033","002912","","");

	string txtFileIn = m_strInputFolder + "/˵��.txt";
	string CreateXmlFilePath = strSatMetaFileNamePart +"_meta.xml";

	WriteInSJ9XmlFile(txtFileIn.c_str(),CreateXmlFilePath.c_str());

	SJ9_CalcRowStart(InPutList, strSatMetaFileNamePart.c_str(), iRowStart, iRowCount, TRUE, m_pSubProcess); //����ƴ��ʱ��ȡ������ʼ��

	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	char ** InPutVrtList = NULL;
	double dGeotransform[6] = {0, 1, 0, 0, 0, -1};	//����������
	int iOverSize = lexical_cast<int>(SJ9_GetSystemParamters("SJ9A_PAN_OFFSET", "40"));

	for (int i=0;i<8;i++)
	{
		if(i == 4)	//��ǰƫ��80��
			dGeotransform[0] -= (2*iOverSize);

		string strVrtFile = string(InPutList[i]) + ".vrt";
		ConstructVrtFile(strVrtFile.c_str(), InPutList[i], 1536, iRowCount, 2, iRowStart[i]*3072, 3072, 2, TRUE, dGeotransform);
		InPutVrtList = CSLAddString(InPutVrtList, strVrtFile.c_str());

		dGeotransform[0] += 1536;
	}

	// ȫɫƴ��
	strOutPutMergedFile = m_strTempFolder + ConstructDirectory("\\SJ9A","PAN","20130530023033","002912","_Pan_Merge",".vrt");
	iRev = SJ9_DataPanMerge(InPutVrtList, strOutPutMergedFile.c_str(), "VRT", m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	if (iRev!=RE_SUCCESS)
	{
		printf("ȫɫƴ�ӳ���!\n");
		return iRev;
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::FindMergeInPutFileList(char ***OutPutFileList,bool bIsPan)
{
	vector<path> vPathFiles;
	FindFiles(path(m_strTempFolder.c_str()), "*.raw", vPathFiles,false);
	std::sort(vPathFiles.begin(),vPathFiles.end(),CampareFileSize());
	int count = vPathFiles.size();
	if (count < 16)
		return RE_PARAMERROR;

	int sequence[8] = {count + 1,count + 1,count + 1,count + 1,count + 1,count + 1,count + 1,count + 1};

	if(bIsPan)
	{
		for (int i = 0; i < 8; i++) //ȡ�ļ��������ϴ��8��
		{
			string str = vPathFiles[i].string();
			str = str.substr(str.length()-8,4);
			if (str == "0_aa"||str == "1_aa")
				sequence[0] = i;
			else if (str == "0_bb"||str == "1_bb")
				sequence[1] = i;
			else if (str == "0_cc"||str == "1_cc")
				sequence[2] = i;
			else if (str == "0_dd"||str == "1_dd")
				sequence[3] = i;
			else if (str == "2_aa"||str == "3_aa")
				sequence[4] = i;
			else if (str == "2_bb"||str == "3_bb")
				sequence[5] = i;
			else if (str == "2_cc"||str == "3_cc")
				sequence[6] = i;
			else if (str == "2_dd"||str == "3_dd")
				sequence[7] = i;
		}
	}
	else
	{
		for (int i = 0; i < count; i++)
		{
			string str = vPathFiles[i].string();
			str = str.substr(str.length()-8,4);

			if (str == "4_aa")
				sequence[0] = i;
			else if (str == "4_bb")
				sequence[1] = i;
			else if (str == "4_cc")
				sequence[2] = i;
			else if (str == "4_dd")
				sequence[3] = i;
			else if (str == "5_aa")
				sequence[4] = i;
			else if (str == "5_bb")
				sequence[5] = i;
			else if (str == "5_cc")
				sequence[6] = i;
			else if (str == "5_dd")
				sequence[7] = i;
		}
	}

	for (int i=0;i<8;i++)
	{
		string strRawfile = vPathFiles[sequence[i]].string();
		*OutPutFileList = CSLAddString(*OutPutFileList,strRawfile.c_str());
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoPanRadiancyAndQuickPic(const string & strOutPanMergeFile)
{
	progress_timer ti;   // ��ʼ��ʱ
	// ȫɫ����У��
	string strRadiancyFile = ConstructDirectory("\\SJ9A","PAN","20130530023033","002912","",".raw");
	strRadiancyFile = m_strFinalOutputFolder  + strRadiancyFile.c_str(); 
	int iRev = SJ9_DataPanRadiancy(strOutPanMergeFile.c_str(), strRadiancyFile.c_str(), FALSE, "ENVI", m_pSubProcess);
	printf("ȫɫ����У����ʱ%f\n",ti.elapsed());
	if (m_pAllProcess)
		m_pAllProcess->StepIt();
	if (iRev!=RE_SUCCESS)
	{
		printf("ȫɫ����У������!\n");
		return iRev;
	}
	// ȫɫ��������ͼ
	progress_timer tp;   // ��ʼ��ʱ
	iRev = SJ9_CreateQP(strRadiancyFile.c_str(), m_pSubProcess);
	printf("ȫɫ��������ͼ��ʱ%f\n",tp.elapsed());
	if (iRev!=RE_SUCCESS)
	{
		printf("ȫɫ�������ɿ���ͼ����!\n");
		return iRev;
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoMsiMergeBeijing(char *** OutputMsiMergeList)
{
	char ** InPutList = NULL;
	int  iRev = FindMergeInPutFileList(&InPutList,false);

	int iRowStart[8],iRowCount;
	string strSatMetaFileNamePart = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","","");

	string txtFileIn = m_strInputFolder + "/˵��.txt";
	string CreateXmlFilePath = strSatMetaFileNamePart + "_meta.xml";

	WriteInSJ9XmlFile(txtFileIn.c_str(),CreateXmlFilePath.c_str());

	iRev = SJ9_CalcRowStart(InPutList, strSatMetaFileNamePart.c_str(), iRowStart, iRowCount, TRUE,m_pSubProcess); //ȡ���е���ʼ�кͲ�������������XML

	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	if (iRev != RE_SUCCESS)
	{
		printf("�����ͷ�ļ�����!\n");
		return iRev;
	}

	char ** InPutVrtList = NULL;
	double dGeotransform[6] = {0, 1, 0, 0, 0, -1};	//����������
	int iOverSize = lexical_cast<int>(SJ9_GetSystemParamters("SJ9A_MUX_OFFSET", "10"));

	for (int i=0; i<8; i++)
	{
		if(i < 4)
			dGeotransform[0] = 0;
		else	//��ǰƫ��20��
			dGeotransform[0] = 1536 - (2*iOverSize);

		string strRawfile = InPutList[i];
		string strVrtFile = strRawfile + ".vrt";
		ConstructVrtFile(strVrtFile.c_str(), strRawfile.c_str(), 1536, iRowCount, 2, iRowStart[i]*3072, 3072, 2, TRUE, dGeotransform);
		InPutVrtList = CSLAddString(InPutVrtList, strVrtFile.c_str());

		dGeotransform[0] += 1536;
	}

	// �����ƴ��
	string file = m_strTempFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B1_Merge",".vrt").c_str();
	*OutputMsiMergeList = CSLAddString(*OutputMsiMergeList, file.c_str());
	file = m_strTempFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B2_Merge",".vrt").c_str();
	*OutputMsiMergeList = CSLAddString(*OutputMsiMergeList, file.c_str());
	file = m_strTempFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B3_Merge",".vrt").c_str();
	*OutputMsiMergeList = CSLAddString(*OutputMsiMergeList, file.c_str());
	file = m_strTempFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B4_Merge",".vrt").c_str();
	*OutputMsiMergeList = CSLAddString(*OutputMsiMergeList, file.c_str());

	iRev = SJ9_DataMsiMerge(InPutVrtList, *OutputMsiMergeList, "VRT", m_pSubProcess);
	if (iRev!=RE_SUCCESS)
	{
		printf("�����ƴ�ӳ���!\n");
		return iRev;
	}
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoMsiRadiancyAndQuickPic(char **OutputMsiMergeOutList)
{
	int iRev = RE_FAILED;
	char ** OutputMsiFinalList = NULL;
	string file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B1",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B2",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B3",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","002912","_B4",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());

	double dLastTimeTR(0),dLastTimeTQ(0);
	progress_timer tR;   // ��ʼ��ʱ
	progress_timer tQ;   // ��ʼ��ʱ

	for(int i=0;i<4;i++)
	{
		// ����׷���У��
		iRev = SJ9_DataMsiRadiancy(OutputMsiMergeOutList[i],OutputMsiFinalList[i],i,FALSE,"ENVI",m_pSubProcess);
		TRaceTime("����׷���У��",tR,dLastTimeTR);
		if (iRev!=RE_SUCCESS)
		{
			printf("����׷���У������!\n");
			return iRev;
		}
		// �������������ͼ
		iRev = SJ9_CreateQP(OutputMsiFinalList[i],m_pSubProcess);
		TRaceTime("�������������ͼ",tQ,dLastTimeTQ);
		if (iRev!=RE_SUCCESS)
		{
			printf("��������ɿ���ͼ����!\n");
			return iRev;
		}

		if (m_pAllProcess)
			m_pAllProcess->StepIt();
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::DoOtherStationProduce()
{
	int iRev = RE_FAILED;
	if (m_pAllProcess)
		m_pAllProcess->SetStepCount(13);

	double dLastTime = 0;
	progress_timer TimeDataExtract;   // ��ʼ��ʱ
	// ȫɫ����
	iRev = SJ9_DataExtract2(m_vstrCCDFile[0].c_str(), m_strTempFolder.c_str(), TRUE, m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("ȫɫ�������",TimeDataExtract,dLastTime);

	if (iRev!=RE_SUCCESS)
	{
		printf("%s�������!\n", m_vstrCCDFile[0].c_str());
		return iRev;
	}

	// ����׽���
	iRev = SJ9_DataExtract2(m_vstrHSIFile[0].c_str(), m_strTempFolder.c_str(), FALSE, m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("����׽������",TimeDataExtract,dLastTime);
	if (iRev!=RE_SUCCESS)
	{
		printf("%s�������!\n", m_vstrHSIFile[0].c_str());
		return iRev;
	}

	// ȫɫ��������
	string strFileName = path(m_vstrCCDFile[0].c_str()).stem();
	string strPanExtract = m_strTempFolder + "\\" +strFileName + ".dat";
	string strSatMetaFileNamePart = m_strFinalOutputFolder +  ConstructDirectory("\\SJ9A","PAN","20130530023033","111111","","");
	iRev = SJ9_ParseAuxData(strPanExtract.c_str(), strSatMetaFileNamePart.c_str(), 0, 0, TRUE, m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("ȫɫ��������",TimeDataExtract,dLastTime);
	if (iRev!=RE_SUCCESS)
	{
		printf("%s������������!\n", strPanExtract.c_str());
		return iRev;
	}

	// ����׽�������
	strFileName = path(m_vstrHSIFile[0].c_str()).stem();
	string strMuxExtractAux = m_strTempFolder + "\\" +strFileName + ".dat";
	strSatMetaFileNamePart = m_strFinalOutputFolder +  ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","","");
	iRev = SJ9_ParseAuxData(strMuxExtractAux.c_str(), strSatMetaFileNamePart.c_str(), 0, 0, FALSE, m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("����׽�������",TimeDataExtract,dLastTime);
	if (iRev!=RE_SUCCESS)
	{
		printf("%s������������!\n",strMuxExtractAux.c_str());
		return iRev;
	}


	// ȫɫ����У��
	strFileName = path(m_vstrCCDFile[0].c_str()).stem();
	strPanExtract = m_strTempFolder + "\\" +strFileName + ".raw";
	string strRadiancyFile = ConstructDirectory("\\SJ9A","PAN","20130530023033","111111","",".raw");
	strRadiancyFile = m_strFinalOutputFolder  + strRadiancyFile.c_str(); 

	string strPanVrt = m_strTempFolder + "\\" +strFileName + ".vrt";

	filesystem::path pt(strPanExtract.c_str());
	int lineCount = static_cast<int>(file_size(pt) / 24416);

	iRev = ConstructVrtFile(strPanVrt.c_str(), strPanExtract.c_str(), 12208, lineCount, 2, 0, 24416, 2, TRUE);
	if (iRev!=RE_SUCCESS)
	{
		printf("����VRT�ļ�ʧ��!\n");
		return iRev;
	}

	iRev = SJ9_DataPanRadiancy(strPanVrt.c_str(), strRadiancyFile.c_str(), FALSE, "ENVI", m_pSubProcess);
	if (m_pAllProcess)
		m_pAllProcess->StepIt();

	TRaceTime("ȫɫ����У��",TimeDataExtract,dLastTime);
	if (iRev!=RE_SUCCESS)
	{
		printf("ȫɫ����У������!\n");
		return iRev;
	}

	// ȫɫ���ɿ���ͼ
	iRev = SJ9_CreateQP(strRadiancyFile.c_str(), m_pSubProcess);
	TRaceTime("ȫɫ���ɿ���ͼ",TimeDataExtract,dLastTime);


	// ���ɷ���У�������ļ�·��
	char ** OutputMsiExtractOutList = NULL;
	strFileName = path(m_vstrHSIFile[0].c_str()).stem();
	string strMuxExtract = m_strTempFolder + "\\" +strFileName;
	string  strBandFile  = strMuxExtract + "_B1.raw";
	OutputMsiExtractOutList = CSLAddString(OutputMsiExtractOutList,strBandFile.c_str());
	strBandFile  = strMuxExtract + "_B2.raw";
	OutputMsiExtractOutList = CSLAddString(OutputMsiExtractOutList,strBandFile.c_str());
	strBandFile  = strMuxExtract + "_B3.raw";
	OutputMsiExtractOutList = CSLAddString(OutputMsiExtractOutList,strBandFile.c_str());
	strBandFile  = strMuxExtract + "_B4.raw";
	OutputMsiExtractOutList = CSLAddString(OutputMsiExtractOutList,strBandFile.c_str());

	// ���ɷ���У������ļ�·��
	char ** OutputMsiFinalList = NULL;
	string file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","_B1",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","_B2",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","_B3",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());
	file = m_strFinalOutputFolder + ConstructDirectory("\\SJ9A","MUX","20130530023033","111111","_B4",".raw").c_str();
	OutputMsiFinalList = CSLAddString(OutputMsiFinalList,file.c_str());

	char strNameNum[100];
	for(int i=0;i<4;i++)
	{
		// ����׷���У��
		sprintf(strNameNum,"_B%d.vrt",i+1);
		strFileName = path(m_vstrHSIFile[0].c_str()).stem();
		string strMuxVrt = m_strTempFolder + "\\" +strFileName + string(strNameNum);

		filesystem::path pt(OutputMsiExtractOutList[i]);
		int lineCount = static_cast<int>(file_size(pt) / 6104);

		iRev = ConstructVrtFile(strMuxVrt.c_str(), OutputMsiExtractOutList[i], 3052, lineCount, 2, 0, 6104, 2, TRUE);
		if (iRev!=RE_SUCCESS)
		{
			printf("����VRT�ļ�ʧ��!\n");
			return iRev;
		}

		iRev = SJ9_DataMsiRadiancy(strMuxVrt.c_str(), OutputMsiFinalList[i], i, FALSE, "ENVI", m_pSubProcess);
		TRaceTime("����׷���У��",TimeDataExtract,dLastTime);
		if (iRev!=RE_SUCCESS)
		{
			printf("����׷���У������!\n");
			return iRev;
		}

		if (m_pAllProcess)
			m_pAllProcess->StepIt();

		// ��������ɿ���ͼ
		iRev = SJ9_CreateQP(OutputMsiFinalList[i], m_pSubProcess);
		TRaceTime("��������ɿ���ͼ",TimeDataExtract,dLastTime);
		if (iRev!=RE_SUCCESS)
		{
			printf("��������ɿ���ͼ����!\n");
			return iRev;
		}

		if (m_pAllProcess)
			m_pAllProcess->StepIt();
	}

	return RE_SUCCESS;
}

int CSJ9L0DataProduce::ExcuteProduce(CBatProcessBase *pBatProgress)
{
	m_pBatProgress = pBatProgress;
	int iRev = RE_FAILED;
	if (m_pBatProgress!=NULL)
	{
		m_pBatProgress->SetMessage("����ִ��ʵ����0����������...");
		m_pAllProcess = m_pBatProgress->m_pAllProcess;
		m_pSubProcess = m_pBatProgress->m_pSubProcess;
	}

	bool bIsFolderOK = ScanInputFolder();
	if (!bIsFolderOK)
		return RE_PARAMERROR;

	MakeOutDir();
	MakeTmpDir();

	if (!m_bBeijingStation && m_vstrCCDFile.size()==1 && m_vstrHSIFile.size()==1)
		return DoOtherStationProduce();

	if (m_vstrCCDFile.size() + m_vstrHSIFile.size() !=6)
		return RE_PARAMERROR;

	if (m_pAllProcess)
		m_pAllProcess->SetStepCount(15);

	double dLastTime = 0;
	progress_timer TimeDataExtract;   // ��ʼ��ʱ
	iRev = DoDataExtractBeijing();
	TRaceTime("�������",TimeDataExtract,dLastTime);
	if (iRev !=RE_SUCCESS)
		return iRev;

	string strOutPanMergeFile;
	iRev =  DoPanMergeBeijing(strOutPanMergeFile);
	TRaceTime("ȫɫƴ�����",TimeDataExtract,dLastTime);
	if (iRev !=RE_SUCCESS)
		return iRev;

	iRev =  DoPanRadiancyAndQuickPic(strOutPanMergeFile);
	TRaceTime("ȫɫ����У�������ɿ���ͼ���",TimeDataExtract,dLastTime);
	if (iRev !=RE_SUCCESS)
		return iRev;

	char ** OutputMsiFinalList = NULL;
	iRev = DoMsiMergeBeijing(&OutputMsiFinalList);
	TRaceTime("�����ƴ�����",TimeDataExtract,dLastTime);
	if (iRev !=RE_SUCCESS)
		return iRev;

	iRev = DoMsiRadiancyAndQuickPic(OutputMsiFinalList);
	TRaceTime("����׷���У�������ɿ���ͼ���",TimeDataExtract,dLastTime);

	return iRev;
}