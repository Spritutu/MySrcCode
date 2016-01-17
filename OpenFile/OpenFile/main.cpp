#include <Windows.h>
#include <CommDlg.h>
#include <iostream>

using namespace std;

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

int main()
{
	static char     szFileName[MAX_PATH], szTitleName[MAX_PATH] ;
	HWND      hwnd;
	hwnd=GetForegroundWindow(); //��ȡǰ̨���ھ�����������е�ǰ̨���ھ��ǿ���̨���ڡ� 
	PopFileInitialize (hwnd);  //��ʼ��ofn
	PopFileOpenDlg(hwnd, szFileName, szTitleName);//���ļ��Ի���
	cout<<szFileName<<endl;  //�ڿ���̨����ʾѡ���ļ���·��
	return 0;
}