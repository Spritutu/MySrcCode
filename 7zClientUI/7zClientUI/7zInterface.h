#ifndef _7Z_INTERFACE_H
#define _7Z_INTERFACE_H

#include "stdafx.h"
#include "../7z/Common/IntToString.h"
#include "../7z/Common/MyInitGuid.h"
#include "../7z/Common/StringConvert.h"
#include "../7z/Common/SetProperties.h"

#include "../7z/Windows/DLL.h"
#include "../7z/Windows/FileDir.h"
#include "../7z/Windows/FileFind.h"
#include "../7z/Windows/FileName.h"
#include "../7z/Windows/NtCheck.h"
#include "../7z/Windows/PropVariant.h"
#include "../7z/Windows/PropVariantConversions.h"
#include "../7z/FileStream/FileStreams.h"
#include "../7z/Archive/IArchive.h"
#include "../7z/IPassword.h"


struct CDirItem
{
	UInt64 Size;
	FILETIME CTime;
	FILETIME ATime;
	FILETIME MTime;
	UString Name;
	FString FullPath;
	UInt32 Attrib;

	bool isDir() const { return (Attrib & FILE_ATTRIBUTE_DIRECTORY) != 0 ; }
};


class CArchiveUpdateCallback:
	public IArchiveUpdateCallback2,
	public ICryptoGetTextPassword2,
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP2(IArchiveUpdateCallback2, ICryptoGetTextPassword2)

		// IProgress
		STDMETHOD(SetTotal)(UInt64 size);
	STDMETHOD(SetCompleted)(const UInt64 *completeValue);

	// IUpdateCallback2
	STDMETHOD(EnumProperties)(IEnumSTATPROPSTG **enumerator);
	STDMETHOD(GetUpdateItemInfo)(UInt32 index,
		Int32 *newData, Int32 *newProperties, UInt32 *indexInArchive);
	STDMETHOD(GetProperty)(UInt32 index, PROPID propID, PROPVARIANT *value);
	STDMETHOD(GetStream)(UInt32 index, ISequentialInStream **inStream);
	STDMETHOD(SetOperationResult)(Int32 operationResult);
	STDMETHOD(GetVolumeSize)(UInt32 index, UInt64 *size);
	STDMETHOD(GetVolumeStream)(UInt32 index, ISequentialOutStream **volumeStream);

	STDMETHOD(CryptoGetTextPassword2)(Int32 *passwordIsDefined, BSTR *password);

public:
	CRecordVector<UInt64> VolumesSizes;
	UString VolName;
	UString VolExt;

	FString DirPrefix;
	const CObjectVector<CDirItem> *DirItems;

	bool PasswordIsDefined;
	UString Password;
	bool AskPassword;

	bool m_NeedBeClosed;

	FStringVector FailedFiles;
	CRecordVector<HRESULT> FailedCodes;

	CArchiveUpdateCallback(): PasswordIsDefined(false), AskPassword(false), DirItems(0) {};

	virtual ~CArchiveUpdateCallback() { Finilize(); }
	HRESULT Finilize();

	void Init(const CObjectVector<CDirItem> *dirItems)
	{
		DirItems = dirItems;
		m_NeedBeClosed = false;
		FailedFiles.Clear();
		FailedCodes.Clear();
	}
};


class CArchiveExtractCallback:
	public IArchiveExtractCallback,
	public ICryptoGetTextPassword,
	public CMyUnknownImp
{
public:
	MY_UNKNOWN_IMP1(ICryptoGetTextPassword)

		// IProgress
		STDMETHOD(SetTotal)(UInt64 size);
	STDMETHOD(SetCompleted)(const UInt64 *completeValue);

	// IArchiveExtractCallback
	STDMETHOD(GetStream)(UInt32 index, ISequentialOutStream **outStream, Int32 askExtractMode);
	STDMETHOD(PrepareOperation)(Int32 askExtractMode);
	STDMETHOD(SetOperationResult)(Int32 resultEOperationResult);

	// ICryptoGetTextPassword
	STDMETHOD(CryptoGetTextPassword)(BSTR *aPassword);

private:
	CMyComPtr<IInArchive> _archiveHandler;

	UString _filePath;       // name inside arcvhive
	FString _diskFilePath;   // full path to file on disk
	bool _extractMode;
	struct CProcessedFileInfo
	{
		FILETIME MTime;
		UInt32 Attrib;
		bool isDir;
		bool AttribDefined;
		bool MTimeDefined;
	} _processedFileInfo;

	COutFileStream *_outFileStreamSpec;
	CMyComPtr<ISequentialOutStream> _outFileStream;

public:
	void Init(IInArchive *archiveHandler, const FString &directoryPath);

	UInt64 NumErrors;
	bool PasswordIsDefined;
	UString Password;
	FString _directoryPath;  // Output directory

	CArchiveExtractCallback() : PasswordIsDefined(false) {}
};


/**
* @brief ѹ������
* @param ArchiveName	ѹ����·��
* @param SwitchStrings	ѹ���ļ��б�
* @param password	    ѹ������
* @param level	        ѹ���ȼ�
*    ѹ���ȼ�            ˵��
*    0                  ��ѹ��.
*    1                  ����ѹ����LZMA �����㷨��32KB �ֵ��С��HC3 Match finder��BCJ ��������
*    5                  ����ѹ����LZMA ��׼�㷨��2 MB �ֵ��С��BT4 Match finder�����ʴ�СΪ 32��BCJ ��������
*    7                  ���ѹ����LZMA ����㷨��8 MB �ֵ��С��BT4 Match finder�����ʴ�СΪ 64��BCJ ��������
*    9                  ����ѹ����LZMA ����㷨��32 MB �ֵ��С��BT4b Match finder�����ʴ�СΪ 64��BCJ2 ��������
* @param pProgress      ѹ��������
* @return ����ֵ����ʾ��������г��ֵĸ��ִ�����Ϣ
*/
int compress(UString ArchiveName,UStringVector SwitchStrings,UString password = L"",int level = 1,CArchiveUpdateCallback * pProgress = NULL);

/**
* @brief ��ʾѹ�����ļ��б�
* @param ArchiveName	ѹ����·��
* @param ListVector	    ѹ���ļ��б�
* @return ����ֵ����ʾ��������г��ֵĸ��ִ�����Ϣ
*/
int showlist(UString ArchiveName,UStringVector &ListVector/*,UString password = L""*/);

/**
* @brief ��ȡѹ�������ض��ļ�
* @param ArchiveName	ѹ����·��
* @param FileList	    ������ȡ���ļ��б�
* @param FilePath	    ��ѹ·��
* @param password	    ��ѹ����
* @return ����ֵ����ʾ��������г��ֵĸ��ִ�����Ϣ
*/
int extract(UString ArchiveName,UStringVector FileList,UString FilePath,UString password = L"");

/**
* @brief ��ѹѹ����
* @param ArchiveName	ѹ����·��
* @param FilePath	    ��ѹ·��
* @param password	    ��ѹ����
* @param pProgress      ��ѹ��������
* @return ����ֵ����ʾ��������г��ֵĸ��ִ�����Ϣ
*/
int decompress(UString ArchiveName,UString FilePath,UString password = L"",CArchiveExtractCallback * pProgress = NULL);

/**
* @brief char*תUString
* @param ustr	        ת����UString�ַ���
* @param ch	            char* �ַ���
*/
void Char2UString(UString &ustr ,const char* ch);

#endif