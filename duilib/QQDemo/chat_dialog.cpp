#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>

#include "win_impl_base.hpp"
#include "chat_dialog.hpp"
#include "ColorPicker.hpp"
#include "main_frame.hpp"
#include <time.h>
#include <WinSock2.h>
#include <io.h>

#if USE(ZIP_SKIN)
static const TCHAR* const kResourceSkinZipFileName = _T("QQRes.zip");
#endif

const TCHAR* const kTitleControlName = _T("apptitle");
const TCHAR* const kCloseButtonControlName = _T("closebtn");
const TCHAR* const kMinButtonControlName = _T("minbtn");
const TCHAR* const kMaxButtonControlName = _T("maxbtn");
const TCHAR* const kRestoreButtonControlName = _T("restorebtn");

const TCHAR* const kBackgroundControlName = _T("bg");

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const kDescriptionControlName = _T("description");

const TCHAR* const kFontButtonControlName = _T("fontbtn");
const TCHAR* const kFontbarControlName = _T("fontbar");
const TCHAR* const kFontTypeControlName = _T("font_type");
const TCHAR* const kFontSizeControlName = _T("font_size");
const TCHAR* const kBoldButtonControlName = _T("boldbtn");
const TCHAR* const kItalicButtonControlName = _T("italicbtn");
const TCHAR* const KUnderlineButtonControlName = _T("underlinebtn");
const TCHAR* const kColorButtonControlName = _T("colorbtn");

const TCHAR* const kInputRichEditControlName = _T("input_richedit");
const TCHAR* const kViewRichEditControlName = _T("view_richedit");

const TCHAR* const kEmotionButtonControlName = _T("emotionbtn");
const TCHAR* const kSelectImageButtonControlName = _T("imagebtn");

const TCHAR* const kSendButtonControlName = _T("sendbtn");
const TCHAR* const kShakeWindowBtn = _T("shockbtn");

const int kEmotionRefreshTimerId = 1001;
const int kEmotionRefreshInterval = 150;

extern void SendMsgToFriend(const char *pszIp,NEW_MSG Msg);

#define SOCKET_BUFF 204800    //�׽��ֻ�������С
struct ThreadParamSend
{
	SOCKET sockConn;
	string  strFile;
};
struct ThreadParamReceive
{
	string strSrvIp;
	string  strDestFile;
	ChatDialog *m_pChatDlg;
};
struct FileTransfer
{
	char szData[153600];
	int  iDataLen;
};
struct FileInfo
{
	__int64 iFileLength;
	char szFileName[256];
};
#define KB   1024
#define MB   (1024*KB)
#define GB   (1024*MB)
string GetTimeStr(int iSecond)
{
	char strTime[50] ={0};
	char szHour[3]   = {"00"};
	char szMinute[3] = {"00"};
	char szSecond[3] = {"00"};

	if (iSecond >=3600)
	{
		int iHour = iSecond / 3600;
		int iMinute = iSecond - iHour*3600;
		iSecond = iSecond - iHour*3600 - iMinute*60;

		if (iHour <10)
			sprintf(szHour,"0%d",iHour);
		else
			sprintf(szHour,"%d",iHour);

		if (iMinute <10)
			sprintf(szMinute,"0%d",iMinute);
		else
			sprintf(szMinute,"%d",iMinute);

		if (iSecond < 10)
			sprintf(szSecond,"0%d",iSecond);
		else
			sprintf(szSecond,"%d",iSecond);
	}
	else if (iSecond > 60 )
	{
		int iMinute = iSecond/60;
		iSecond = iSecond - 60*iMinute;

		if (iMinute <10)
			sprintf(szMinute,"0%d",iMinute);
		else
			sprintf(szMinute,"%d",iMinute);

		if (iSecond < 10)
			sprintf(szSecond,"0%d",iSecond);
		else
			sprintf(szSecond,"%d",iSecond);
	}
	else
	{
		if (iSecond < 10)
			sprintf(szSecond,"0%d",iSecond);
		else
			sprintf(szSecond,"%d",iSecond);
	}

	sprintf(strTime,"%s:%s:%s",szHour,szMinute,szSecond);
	return strTime;
}
string GetFileLenStr(__int64 iLen)
{
	char strFileSize[50] ={0};
	if (iLen > GB )
	{
		sprintf(strFileSize,"%.2fGB",iLen*1.0/GB);
	}
	else if (iLen > MB)
	{
		sprintf(strFileSize,"%.2fMB",iLen*1.0/MB);
	}
	else if (iLen > KB)
	{
		sprintf(strFileSize,"%.2fKB",iLen*1.0/KB);
	}
	else
		sprintf(strFileSize,"%dByte",iLen);

	return strFileSize;
}
string GetFileName(const char * pszFilePath)
{
	char *pEnd = (char*)pszFilePath + strlen(pszFilePath);
	while(*pEnd !='\\' && *pEnd !='/')
		pEnd--;
	return pEnd+1;
}

static int CopyData( FileTransfer * Dst,FileTransfer * SrcTmp,int iSrcLen)
{
	const static int ONE_DATA_LEN = sizeof(FileTransfer);
	static int iRecvLength = 0;
	memcpy((char*)Dst+iRecvLength,SrcTmp,iSrcLen); //�����յ��Ĳ���������ƴ���������ݰ�������
	iRecvLength += iSrcLen;
	if(iRecvLength >= ONE_DATA_LEN )
	{
		iRecvLength = iRecvLength - ONE_DATA_LEN; //�Ѿ���һ����������
		return iRecvLength; //����������֮�⻹ʣ������ݴ�С
	}
	else
		return -1;
}
static void ClearReveiveQueen(FileTransfer *Dst,int iLen)
{
	const static int ONE_DATA_LEN = sizeof(FileTransfer);
	if (iLen == 0)
		return;

	char * pData = new char[iLen];
	memcpy(pData,(char*)Dst + ONE_DATA_LEN,iLen);
	memcpy(Dst,pData,iLen);
	delete pData;
}

DWORD WINAPI ThreadReceiveFile(void *param)
{
	ThreadParamReceive *MyParam = (ThreadParamReceive*)param;
	string strSrvIp = MyParam->strSrvIp;
	string strDestFile = MyParam->strDestFile;
	ChatDialog *pChatDlg = MyParam->m_pChatDlg;

	// �����׽���
	SOCKET sockClent = socket(AF_INET,SOCK_STREAM,0);

	//�����׽��ַ��ͻ�����
	int nBuf = SOCKET_BUFF;
	int nBufLen = sizeof(nBuf);
	int nRe = setsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
	if(SOCKET_ERROR == nRe)
		MessageBox(NULL,"setsockopt error!","����",MB_ICONWARNING);

	//��黺�����Ƿ����óɹ�
	nRe = getsockopt(sockClent, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
	if(SOCKET_BUFF != nBuf)
		MessageBox(NULL,"���������ò��ɹ�!","����",MB_ICONWARNING);

	// �ͻ��˲���Ҫbind��ֱ������
	SOCKADDR_IN addrSrv; //�������˵�ַ
	addrSrv.sin_addr.S_un.S_addr = inet_addr(strSrvIp.c_str()); // ���ػ�·��ַ����Ӧ��������ַ�����ʱ��Ӧ�������˵�ַ��
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6100); // ������������˵ȴ��Ķ˿�һ�£���Ϊ�����ֽ���
	int iResult = connect(sockClent,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	if (iResult == SOCKET_ERROR)
	{	
		char szMsg[200] ={0};
		sprintf(szMsg,"���ӵ�%sʧ��!",strSrvIp.c_str());
		MessageBox(NULL,szMsg,"����",MB_ICONERROR);
		closesocket(sockClent);
		return -1;
	}

	printf("�ȴ������ļ�...\n");
	FileInfo info;
	recv(sockClent,(char*)&info,sizeof(FileInfo),0);
	string strSumLen = GetFileLenStr(info.iFileLength);
	string strFileName = GetFileName(strDestFile.c_str());

	double dTmpPro = info.iFileLength*1.0/100;
	if (pChatDlg)
	{
		pChatDlg->SetProgressFileName(strFileName);
		pChatDlg->SetProgressRange(0,100);
	}

	send(sockClent,"start",strlen("start")+1,0); //����ʼ����
	clock_t t_start = clock();

	FILE *pFile = fopen(strDestFile.c_str(),"wb"); // ����Ŀ���ļ�

	__int64 iCountBytes = 0;
	FileTransfer DstTransferArry[2];

	int iTimerCount = 0;
	__int64 iLastDataLen = 0;
	double dLastTimeGo = 0;
	bool bFailed = false;
	while(1)
	{
		FileTransfer TmpTransfer;
		strset(TmpTransfer.szData,0);
		int iLen = recv(sockClent,(char*)&TmpTransfer,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			bFailed = true;
			printf("\n�����ж�,��������������ѶϿ�\n");
			break;
		}

		int OterLength = CopyData(DstTransferArry,&TmpTransfer,iLen);// ���ö��еķ�ʽƴ��������һ��������֮�⻹ʣ�еİ���С
		if (OterLength == -1)  
			continue; //������һ��������������ƴ��

		if (DstTransferArry[0].iDataLen ==0)
			break;

		iCountBytes +=DstTransferArry[0].iDataLen;
		fwrite(DstTransferArry[0].szData,sizeof(char),DstTransferArry[0].iDataLen,pFile);
		ClearReveiveQueen(DstTransferArry,OterLength);

		double dTimeEplisedCur = clock() - t_start;
		iTimerCount++;
		string strSpeedMsg;
		if (iTimerCount%50==0) //ÿ50��ͳ��һ��ƽ���ٶ�
		{
			dLastTimeGo = dTimeEplisedCur - dLastTimeGo; // ��ȡ�����ۼƺ�ʱʱ���
			int iSpeed = (iCountBytes - iLastDataLen)*1000/dLastTimeGo; //��������ֵ��ʱ���ı�ֵΪ�ٶ�
			strSpeedMsg = GetFileLenStr(iSpeed);
			strSpeedMsg +="/s";
			iLastDataLen  = iCountBytes;
			dLastTimeGo  = dTimeEplisedCur; // �洢��ǰ��ʱ�����ڼ����ٶ�
			iTimerCount = 0;
		}

		char szMSg[500] ={0};
		sprintf(szMSg,"%s %s/%s  %s",GetTimeStr(dTimeEplisedCur/1000).c_str(),
			GetFileLenStr(iCountBytes).c_str(),strSumLen.c_str(),strSpeedMsg.c_str());

		if (pChatDlg)
		{
			pChatDlg->SetProgressMsg(szMSg);
			pChatDlg->UpdateProgressValue(iCountBytes/dTmpPro);
		}
	}
	fclose(pFile);

	clock_t t_End = clock();
	double dTimeEplised = t_End - t_start;
	printf("\n");
	if (bFailed)
		MessageBox(NULL,"�ļ�����ʧ��!","����",MB_ICONERROR);
	else
	{
		char szMsg[500] ={0};
		sprintf(szMsg,"�������,��ʱ%.1f��!\n",dTimeEplised/1000);
		MessageBox(NULL,szMsg,"��ʾ",MB_ICONINFORMATION);
	}
	if (pChatDlg)
		pChatDlg->HideProgressAndLable();// ���ؽ������ؼ�

	// ͨѶ��ɺ󣬹ر��׽��֣��ͷ��׽�����ռ��Դ
	closesocket(sockClent);

	if (!bFailed)
	{
		strDestFile = "/select ,"  + strDestFile;
		ShellExecuteA(0,"open","explorer.exe",strDestFile.c_str(),NULL,SW_SHOWNORMAL); //�ɹ�����ļ�����Ŀ¼
	}
	return 0;
}
DWORD WINAPI ThreadSendData(void * param)
{
	ThreadParamSend *Myparam = (ThreadParamSend*)(param);
	string strFile  = Myparam->strFile;

	// ������ʽ�׽���(���ڼ����ͻ�����������)
	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // �����ֽ���
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6100); //�˿ں���ת��Ϊ�����ֽ���

	// �����ص�ַ���׽��ֽ��й���
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	//���׽�������Ϊ����ģʽ
	listen(sockSrv,1/*SOMAXCONN*/); //���õȴ������и������Ϊ1

	// ���ϵȴ��ͻ�����������
	SOCKADDR_IN  addrClient; //�洢�ͻ��˵ĵ�ַ��Ϣ
	int iLen = sizeof(SOCKADDR);

	printf("\n�ȴ��ͻ�������...");
	// ���ܿͻ��˵��������󣬲����ظ����ӵ��׽���
	SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&iLen);
	printf("\n�Ѿ���ͻ�%s����������...\n",inet_ntoa(addrClient.sin_addr));

	//�����׽��ַ��ͻ�����
	int nBuf = SOCKET_BUFF;
	int nBufLen = sizeof(nBuf);
	int nRe = setsockopt(sockConn, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, nBufLen);
	if(SOCKET_ERROR == nRe)
		MessageBox(NULL,"setsockopt error!","����",MB_ICONWARNING);

	//��黺�����Ƿ����óɹ�
	nRe = getsockopt(sockConn, SOL_SOCKET, SO_SNDBUF, (char*)&nBuf, &nBufLen);
	if(SOCKET_BUFF != nBuf)
		MessageBox(NULL,"���������ò��ɹ�!","����",MB_ICONWARNING);


	FILE *pFile = fopen(strFile.c_str(),"rb");
	if(!pFile)
	{
		char szMsg[500] ={0};
		sprintf(szMsg,"�޷��򿪴����ļ�[%s]",strFile.c_str());
		MessageBox(NULL,szMsg,"����",MB_ICONERROR);
		return 0;
	}

	FileInfo info;
	strcpy(info.szFileName,GetFileName(strFile.c_str()).c_str());
	info.iFileLength =  _filelengthi64(_fileno(pFile));

	send(sockConn,(char*)&info,sizeof(FileInfo),0);

	char szCommandMsg[50] ={0};
	bool bStart = true;
	while(1)
	{
		printf("�ȴ��ͻ��˷��ʹ�����������...");
		int iLen = recv(sockConn,szCommandMsg,50,0);
		if (iLen == -1)
		{
			MessageBox(NULL,"�ͻ����ж���������!","��ʾ",MB_ICONERROR);
			closesocket(sockConn);
			bStart = false;
			break;
		}
		if (strcmp("start",szCommandMsg)==0)
			break;
		else
		{
			MessageBox(NULL,"������Ϣ����ȷ!","��ʾ",MB_ICONERROR);
		}
	}
	if (!bStart)
		return 0;


	__int64 iCountBytes = 0;
	FileTransfer trans;
	int iReadCount = sizeof(trans.szData)/sizeof(char);

	clock_t t_start = clock(); //��ʼ��ʱ

	string strSumLen = GetFileLenStr(info.iFileLength);

	iLen = 0;
	int iTimerCount = 0;
	__int64 iLastDataLen = 0;
	double dLastTimeGo = 0;
	while(trans.iDataLen = fread(trans.szData,sizeof(char),iReadCount,pFile))
	{
		if(trans.iDataLen < 0)
		{
			printf("\n��ȡ���ݳ��ִ���!\n");
			break;
		}
		int iTryCount = 0;
RE_TRY:
		iLen = send(sockConn,(char*)&trans,sizeof(FileTransfer),0);
		if(iLen ==-1)
		{
			printf("\n�����ж�,��ͻ��˵������ѶϿ�\n");
			break;
		}
		if (iLen != sizeof(FileTransfer))
		{
			if (iTryCount > 3)
			{
				printf("�����޷��ɹ�����!\n");
				break;
			}

			iTryCount++;
			printf("\n���ݷ��Ͳ�����,���ڳ��Ե�%d�����·���\n",iTryCount);
			Sleep(1);
			goto RE_TRY;
		}
		iCountBytes += __int64(trans.iDataLen);
		double dTimeEplisedCur = clock() - t_start;
		iTimerCount++;
		string strSpeedMsg;
		if (iTimerCount%50==0) //ÿ50��ͳ��һ��ƽ���ٶ�
		{
			dLastTimeGo = dTimeEplisedCur - dLastTimeGo; // ��ȡ�����ۼƺ�ʱʱ���
			int iSpeed = (iCountBytes - iLastDataLen)*1000/dLastTimeGo; //��������ֵ��ʱ���ı�ֵΪ�ٶ�
			strSpeedMsg = GetFileLenStr(iSpeed);
			strSpeedMsg +="/s";
			iLastDataLen  = iCountBytes;
			dLastTimeGo  = dTimeEplisedCur; // �洢��ǰ��ʱ�����ڼ����ٶ�
			iTimerCount = 0;
		}
		printf("\r�Ѵ���%s,�ܴ�С%s,��ʱ%f��,ƽ���ٶ�%s",GetFileLenStr(iCountBytes).c_str(),
			strSumLen.c_str(),dTimeEplisedCur/1000,strSpeedMsg.c_str());
	}
	fclose(pFile);

	clock_t t_End = clock(); //������ʱ
	double dTimeEplised = t_End - t_start;

	printf("\n");
	trans.iDataLen = 0;
	send(sockConn,(char*)&trans,sizeof(FileTransfer),0); //���ͽ�����ʾ
	if (iCountBytes == info.iFileLength)
		printf("������ϣ���ʱ%f��!\n",dTimeEplised/1000);
	closesocket(sockConn);
	return 0;
}
ChatDialog::ChatDialog(MainFrame *pMain,const tString& bgimage, DWORD bkcolor, const FriendListItemInfo& myselft_info, const FriendListItemInfo& friend_info)
: bgimage_(bgimage)
, bkcolor_(bkcolor)
, myselft_(myselft_info)
, friend_(friend_info)
, emotion_timer_start_(false)
, text_color_(0xFF000000)
, bold_(false)
, italic_(false)
, underline_(false)
, font_size_(12)
, font_face_name_(_T("΢���ź�"))
,m_pMainUi(pMain)
{}

ChatDialog::~ChatDialog()
{
	return;
}

LPCTSTR ChatDialog::GetWindowClassName() const
{
	return _T("ChatDialog");
}

CControlUI* ChatDialog::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void ChatDialog::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

BOOL ChatDialog::Receive(SkinChangedParam param)
{
	bgimage_ = param.bgimage;
	bkcolor_ = param.bkcolor;
	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		if (!param.bgimage.empty())
		{
			TCHAR szBuf[MAX_PATH] = {0};
#if defined(UNDER_WINCE)
			_stprintf(szBuf, _T("file='%s' corner='600,200,1,1'"), param.bgimage.c_str());
#else
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), param.bgimage.c_str());
#endif
			background->SetBkImage(szBuf);
		}
		else
			background->SetBkImage(_T(""));

		background->SetBkColor(param.bkcolor);
	}

	return TRUE;
}

tString ChatDialog::GetSkinFile()
{
	return _T("chatbox.xml");
}

tString ChatDialog::GetSkinFolder()
{
	return tString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT ChatDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pMainUi)
		m_pMainUi->DestroyChatDialogPointer(friend_.strIp);
	bHandled = FALSE;
	return 0;
}

LRESULT ChatDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT ChatDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
#else
	return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif

	return 0;
}

LRESULT ChatDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ChatDialog::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void ChatDialog::OnTimer(TNotifyUI& msg)
{}

void ChatDialog::OnExit(TNotifyUI& msg)
{
	Close();
}

void ChatDialog::Init()
{}

void ChatDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	CControlUI* background = paint_manager_.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("file='%s' corner='600,200,1,1'"), bgimage_.c_str());
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), bgimage_.c_str());
#endif
		background->SetBkImage(szBuf);
		background->SetBkColor(bkcolor_);
	}

	CButtonUI* log_button = static_cast<CButtonUI*>(paint_manager_.FindControl(kLogoButtonControlName));
	if (log_button != NULL)
	{
#if defined(UNDER_WINCE)
		_stprintf(szBuf, _T("%s"), friend_.logo.c_str());
#else
		_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), friend_.logo.c_str());
#endif
		log_button->SetNormalImage(szBuf);
	}

	CControlUI* nick_name = paint_manager_.FindControl(kNickNameControlName);
	if (nick_name != NULL)
		nick_name->SetText(friend_.nick_name.c_str());

	CControlUI* desciption = paint_manager_.FindControl(kDescriptionControlName);
	if (desciption != NULL)
		desciption->SetText(friend_.description.c_str());

	CContainerUI* pFontbar = static_cast<CContainerUI*>(paint_manager_.FindControl(kFontbarControlName));
	if (pFontbar != NULL)
		pFontbar->SetVisible(!pFontbar->IsVisible());

	CProgressUI *pro = static_cast<CProgressUI*>(paint_manager_.FindControl("processbar"));
	if (pro)
		pro->SetVisible(false);
	CLabelUI *ProLable = static_cast<CLabelUI*>(paint_manager_.FindControl("processlable"));
	if (ProLable)
		ProLable->SetVisible(false);
	CLabelUI *ProLableFileName = static_cast<CLabelUI*>(paint_manager_.FindControl("filename"));
	if (ProLableFileName)
		ProLableFileName->SetVisible(false);
}

tString GetCurrentTimeString()
{
	SYSTEMTIME time = {0};
	TCHAR szTime[MAX_PATH] = {0};
	::GetLocalTime( &time );
#if defined(UNDER_CE)
	_stprintf( szTime, _T("%04d��%02d��%02d�� %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
#else
	_stprintf_s( szTime, MAX_PATH, _T("%04d��%02d��%02d�� %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
#endif

	return szTime;
}

void ChatDialog::SendMsg()
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(paint_manager_.FindControl(kInputRichEditControlName));
	if( pRichEdit == NULL ) return;
	pRichEdit->SetFocus();
	CStdString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength()); // ��ȡҪ���͵��ı�
	if( sText.IsEmpty() ) return;
	pRichEdit->SetText(_T(""));

	pRichEdit = static_cast<CRichEditUI*>(paint_manager_.FindControl(kViewRichEditControlName));
	if( pRichEdit == NULL ) return;
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE | CFM_UNDERLINETYPE;
	cf.dwEffects = CFE_LINK;
	cf.bUnderlineType = CFU_UNDERLINE;
	cf.crTextColor = RGB(220, 0, 0);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);

	tString strMine = myselft_.nick_name;
	pRichEdit->ReplaceSel(strMine.c_str(), false);

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);

	tString strTime = GetCurrentTimeString();
	strTime = tString("˵:\t") + strTime + tString("\n");
	pRichEdit->ReplaceSel(strTime.c_str(), false);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 0); //����ʱ����ʾ����ɫ
	cf.dwEffects = 0;
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(sText.GetData(), false);

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(_T("\n"), false);

	cf.crTextColor = RGB(255, 0, 0); //���÷�����Ϣ������ʾ����ɫ
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();

	NEW_MSG msg;
	msg.iMsgType = ChatMsg;
	strcpy(msg.szIp,friend_.strIp.c_str());
	strcpy(msg.szNewMsg,sText.GetData());

	SendMsgToFriend(friend_.strIp.c_str(),msg);
}
/************************************************************************/
/* �����յ�����Ϣ��ʾ�����촰��                                                                     */
/************************************************************************/
void ChatDialog::ReceivedMsg(tString strMsg)
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(paint_manager_.FindControl(kViewRichEditControlName));
	if( pRichEdit == NULL ) return;

	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR | CFM_LINK | CFM_UNDERLINE | CFM_UNDERLINETYPE;
	cf.dwEffects = CFE_LINK;
	cf.bUnderlineType = CFU_UNDERLINE;
	cf.crTextColor = RGB(220, 0, 0);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);

	tString strMine = friend_.nick_name;
	pRichEdit->ReplaceSel(strMine.c_str(), false);

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);

	tString strTime = GetCurrentTimeString();
	strTime = tString("˵:\t") + strTime + tString("\n");
	pRichEdit->ReplaceSel(strTime.c_str(), false);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 0);
	cf.dwEffects = 0;
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(strMsg.c_str(), false);

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(_T("\n"), false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();
}
void ChatDialog::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
		{
			OnExit(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kShakeWindowBtn) == 0)
		{
			ShakeWindow(true);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
#else
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
		{
#if defined(UNDER_CE)
			::ShowWindow(m_hWnd, SW_RESTORE);
			CControlUI* pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(paint_manager_.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
#else
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), kFontButtonControlName) == 0)
		{
			CContainerUI* pFontbar = static_cast<CContainerUI*>(paint_manager_.FindControl(kFontbarControlName));
			if (pFontbar != NULL)
				pFontbar->SetVisible(!pFontbar->IsVisible());
		}
		else if (_tcsicmp(msg.pSender->GetName(), kEmotionButtonControlName) == 0)
		{
			POINT pt = {0};
			CRect rcEmotionBtn = msg.pSender->GetPos();
			CRect rcWindow;
			GetWindowRect(m_hWnd, &rcWindow);

			pt.y = rcWindow.top + rcEmotionBtn.top;
			pt.x = rcWindow.left + rcEmotionBtn.left;
			//emotion_list_window_.SelectEmotion(pt);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSendButtonControlName) == 0)
		{
			// ������Ͱ�ť
			SendMsg();
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSelectImageButtonControlName) == 0)
		{
			// ѡ���ļ���ť����
			SelectSendFile();
		}
	}
	else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
	{
		if (_tcsicmp(msg.pSender->GetName(), kInputRichEditControlName) == 0)
		{
			SendMsg();
		}
	}
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kColorButtonControlName) == 0)
		{
			CContainerUI* pFontbar = static_cast<CContainerUI*>(paint_manager_.FindControl(kFontbarControlName));
			if (pFontbar != NULL)
			{
				POINT pt = {0};
				CRect rcFontbar = pFontbar->GetPos();
				CRect rcColorBtn = msg.pSender->GetPos();
				CRect rcWindow;
				GetWindowRect(m_hWnd, &rcWindow);

				pt.y = rcWindow.top + rcFontbar.top;
				pt.x = rcWindow.left + rcColorBtn.left + static_cast<LONG>(rcColorBtn.right - rcColorBtn.left / 2);
				new CColorPicker(this, pt);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kBoldButtonControlName) == 0)
		{
			COptionUI* bold_button = static_cast<COptionUI*>(msg.pSender);
			if (bold_button != NULL)
			{
				bold_ = bold_button->IsSelected();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kItalicButtonControlName) == 0)
		{
			COptionUI* italic_button = static_cast<COptionUI*>(msg.pSender);
			if (italic_button != NULL)
			{
				italic_ = italic_button->IsSelected();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), KUnderlineButtonControlName) == 0)
		{
			COptionUI* underline_button = static_cast<COptionUI*>(msg.pSender);
			if (underline_button != NULL)
			{
				underline_ = underline_button->IsSelected();
				FontStyleChanged();
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemselect")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kFontTypeControlName) == 0)
		{
			CComboUI* font_type = static_cast<CComboUI*>(msg.pSender);
			if (font_type != NULL)
			{
				font_face_name_ = font_type->GetText();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kFontSizeControlName) == 0)
		{
			CComboUI* font_size = static_cast<CComboUI*>(msg.pSender);
			if (font_size != NULL)
			{
				font_size_ = _ttoi(font_size->GetText());
				FontStyleChanged();
			}
		}
	}
}

void ChatDialog::SetTextColor(DWORD dwColor)
{
	COptionUI* color_button = static_cast<COptionUI*>(paint_manager_.FindControl(kColorButtonControlName));
	if (color_button != NULL)
	{
		color_button->Selected(false);
		if (dwColor != 0)
		{
			text_color_ = dwColor;
			FontStyleChanged();
		}
	}
}

LRESULT ChatDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_USER_RECEIVED_NEW_MSG)
	{
		NEW_MSG *Msg = (NEW_MSG*)lParam;
		tString  strMsg = Msg->szNewMsg;

		if(Msg->iMsgType == CommondMsg)
		{
			if (strMsg == "ShakeWindow")
			{
				ShakeWindow(false);
			}
			else if (strMsg.length() >15  &&  strMsg.substr(0,15) == "SendFile /FILE:")
			{
				tString strFile = strMsg.substr(15);
				char szMsg[500] = {0};
				sprintf(szMsg,"���ѷ����ļ���%s��,�Ƿ����",strFile.c_str());
				int iRev = MessageBox(m_hWnd,szMsg,"��ʾ",MB_ICONQUESTION|MB_YESNO);
				if (iRev != IDYES)
					return 0;

				SelectSaveFile(strFile);
			}
		}
		else
		{
			ReceivedMsg(strMsg);
		}

		delete Msg;

	}
	bHandled = FALSE;
	return 0;
}

void ChatDialog::FontStyleChanged()
{}
void ChatDialog::ShakeWindow(bool bBtnClicked)
{
	NEW_MSG msg;
	msg.iMsgType = CommondMsg;
	strcpy(msg.szIp,friend_.strIp.c_str());
	strcpy(msg.szNewMsg,"ShakeWindow");
	if (bBtnClicked)
		SendMsgToFriend(friend_.strIp.c_str(),msg);

	RECT rcDlg = { 0 };
	::GetWindowRect(m_hWnd, &rcDlg);

	int recordx = rcDlg.left;
	int recordy = rcDlg.top;

	srand(time(NULL));
	for (int i = 0; i < 100; i++)
	{
		int x =   rand()%10;
		int y =   rand()%10;
		if (x % 2 == 0)
		{
			rcDlg.left = rcDlg.left + x;
		}
		else
		{
			rcDlg.left = rcDlg.left - x;
		}
		if (y % 2 == 0)
		{
			rcDlg.top = rcDlg.top + y;
		}
		else
		{
			rcDlg.top = rcDlg.top - y;
		}

		::SetWindowPos(m_hWnd, NULL, rcDlg.left, rcDlg.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		Sleep(1);
	}
	rcDlg.left = recordx;
	rcDlg.top  = recordy;
	::SetWindowPos(m_hWnd, NULL, rcDlg.left, rcDlg.top, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

#include <CommDlg.h>
static OPENFILENAMEA ofn ;

void PopFileInitialize (HWND hwnd)
{
	static char szFilter[] = "ͼ��(*.jpg;*.bmp;*.gif;*.tif;*.img)\0*.jpg;*.bmp;*.gif;*.tif;*.img\0"\
		"�ĵ�(*.doc;*.docx;*.txt;*.cpp;*.h)\0*.doc;*.docx;*.txt;*.cpp;*.h\0" \
		"ѹ���ļ�(*.zip;*.rar;*.7z)\0*.zip;*.rar;*.7z\0" \
		"�����ļ�(*.mp3;*.wma;*.wav)\0*.mp3;*.wma;*.wav\0" \
		"��Ƶ�ļ�(*.rmvb;*.mp4;*.mkv;*.flv;*.avi)\0*.rmvb;*.mp4;*.mkv;*.flv;*.avi\0" \
		"�����ļ�(*.*)\0*.*\0\0";

	ofn.lStructSize       = sizeof (OPENFILENAME) ;
	ofn.hwndOwner         = hwnd ;
	ofn.hInstance         = NULL ;
	ofn.lpstrFilter       = szFilter ;
	ofn.lpstrCustomFilter = NULL ;
	ofn.nMaxCustFilter    = 0 ;
	ofn.nFilterIndex      = 0 ;
	ofn.lpstrFile         = NULL ;          // Set in Open and Close functions
	ofn.nMaxFile          = MAX_PATH ;
	ofn.lpstrFileTitle    = NULL ;          // Set in Open and Close functions
	ofn.nMaxFileTitle     = MAX_PATH ;
	ofn.lpstrInitialDir   = NULL ;
	ofn.lpstrTitle        = NULL ;
	ofn.Flags             = 0 ;             // Set in Open and Close functions
	ofn.nFileOffset       = 0 ;
	ofn.nFileExtension    = 0 ;
	ofn.lpstrDefExt       = "jpg" ;
	ofn.lCustData         = 0L ;
	ofn.lpfnHook          = NULL ;
	ofn.lpTemplateName    = NULL ;
}

BOOL PopFileOpenDlg (HWND hwnd, LPSTR pstrFileName, LPSTR pstrTitleName)
{
	ofn.hwndOwner         = hwnd ;
	ofn.lpstrFile         = pstrFileName ;
	ofn.lpstrFileTitle    = pstrTitleName ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;

	return GetOpenFileName (&ofn) ;
}
BOOL PopFileSaveDlg (HWND hwnd, LPSTR pstrFileName, LPSTR pstrTitleName)
{
	ofn.hwndOwner         = hwnd ;
	ofn.lpstrFile         = pstrFileName ;
	ofn.lpstrFileTitle    = pstrTitleName ;
	ofn.Flags             = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;
	ofn.lpstrFile  = pstrTitleName;

	return GetSaveFileName (&ofn) ;
}
void ChatDialog::SelectSendFile()
{
	static char     szFileName[MAX_PATH], szTitleName[MAX_PATH] ;
	HWND      hwnd;
	hwnd=GetForegroundWindow(); //��ȡǰ̨���ھ�����������е�ǰ̨���ھ��ǿ���̨���ڡ� 
	PopFileInitialize (hwnd);  //��ʼ��ofn
	PopFileOpenDlg(hwnd, szFileName, szTitleName);//���ļ��Ի���

	if (szFileName== "")
		return;

	// �����ͷ��ʹ����ļ���֪ͨ��Ϣ
	NEW_MSG msg;
	msg.iMsgType = CommondMsg;
	strcpy(msg.szIp,friend_.strIp.c_str());
	char strMsg[500] ={0};
	sprintf(strMsg,"SendFile /FILE:%s",szTitleName);
	strcpy(msg.szNewMsg,strMsg);
	SendMsgToFriend(friend_.strIp.c_str(),msg);
	// ����tcp�������������ȴ���������
	ThreadParamSend param;
// 	param.sockConn = sockConn;
	param.strFile = szFileName;
	void *pSendThread = CreateThread(NULL,0,ThreadSendData,&param,0,NULL);
	WaitForSingleObject(pSendThread,5); //�ȴ��ļ��������
}
void ChatDialog::SelectSaveFile(tString strFile)
{
	// ѡ��洢·��
	static char     szFileName[MAX_PATH], szTitleName[MAX_PATH] ;
	HWND      hwnd;
	hwnd=GetForegroundWindow(); //��ȡǰ̨���ھ�����������е�ǰ̨���ھ��ǿ���̨���ڡ� 
	PopFileInitialize (hwnd);  //��ʼ��ofn
	strcpy(szTitleName,strFile.c_str()); // ����ѡ��Ի���ĳ�ʼ�ļ���Ϊ���������ļ���
	BOOL bOK = PopFileSaveDlg(hwnd, szFileName, szTitleName);//�����ļ��Ի���
	if (!bOK)
		return; 

	// Ŀ��洢�ļ�ȫ·��
	tString strDestFile = szTitleName;
	//����TCP���ӿ�ʼ�����ļ�
	ThreadParamReceive param;
	param.m_pChatDlg = this;
	param.strSrvIp = myselft_.strIp;
	param.strDestFile = strDestFile;
	void *pReceiveThread = CreateThread(NULL,0,ThreadReceiveFile,&param,0,NULL);
	WaitForSingleObject(pReceiveThread,5); //�ȴ������ļ����߳�ִ��5����
}
void ChatDialog::SetProgressRange(int iMin,int iMax)
{
	CProgressUI *pro = static_cast<CProgressUI*>(paint_manager_.FindControl("processbar"));
	if (pro)
	{
		pro->SetMinValue(iMin);
		pro->SetMaxValue(iMax);
		pro->SetValue(iMin);
		pro->SetVisible(true);
	}
}
void ChatDialog::SetProgressMsg(tString strMsg)
{
	CLabelUI *ProLable = static_cast<CLabelUI*>(paint_manager_.FindControl("processlable"));
	if (ProLable)
	{
		ProLable->SetVisible(true);
		ProLable->SetText(strMsg.c_str());
	}
}
void ChatDialog::SetProgressFileName(tString strFileName)
{
	CLabelUI *ProLableFilename = static_cast<CLabelUI*>(paint_manager_.FindControl("filename"));
	if (ProLableFilename)
	{
		ProLableFilename->SetVisible(true);
		ProLableFilename->SetText(strFileName.c_str());
	}

}
void ChatDialog::UpdateProgressValue(int iValue)
{
	CProgressUI *pro = static_cast<CProgressUI*>(paint_manager_.FindControl("processbar"));
	if (pro)
	{
		pro->SetVisible(true);
		pro->SetValue(iValue);
	}
}
void ChatDialog::HideProgressAndLable()
{
	CLabelUI *ProLable = static_cast<CLabelUI*>(paint_manager_.FindControl("processlable"));
	if (ProLable)
		ProLable->SetVisible(false);

	CLabelUI *ProLableFilename = static_cast<CLabelUI*>(paint_manager_.FindControl("filename"));
	if (ProLableFilename)
		ProLableFilename->SetVisible(false);

	CProgressUI *pro = static_cast<CProgressUI*>(paint_manager_.FindControl("processbar"));
	if (pro)
		pro->SetVisible(false);

}