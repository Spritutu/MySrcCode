// IniFile.h: interface for the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__B93960CE_A35E_4025_BAAE_6F28452C684D__INCLUDED_)
#define AFX_INIFILE_H__B93960CE_A35E_4025_BAAE_6F28452C684D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define    INIFILELPATH				TEXT("\\MeYe.ini")//�����ļ�·��
class CIniFile  
{
public:
	CIniFile();
	virtual ~CIniFile();
	static void WriteIniFile(const TCHAR* lpSection,const TCHAR* lpKey,const TCHAR* lpValue);	// дINI�ļ��@
	static int  GetIniFileInt(const TCHAR* lpSection,const TCHAR* lpKey);						// ��INI�ļ�,����INT
	static CString GetIniFileString(const TCHAR* lpSection,const TCHAR* lpKey);					// ��INI�ļ�
	static CString GetCurrentApPath();															// ������INI�ļ�·��
//	static CString GetProxyIPAddress();															// Ū���N�z�A�Ⱦ���IP
//	static BOOL GetDeviceAutoConnect(const int nDevice);
 //	static GetDeviceConnectLocal(const int nDevice);
//	static CString GetLanguageString(CString strCodeID);
};

#endif // !defined(AFX_INIFILE_H__B93960CE_A35E_4025_BAAE_6F28452C684D__INCLUDED_)
