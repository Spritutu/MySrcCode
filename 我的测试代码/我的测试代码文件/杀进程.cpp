/*
*���ݽ���IDɱ������
*/
BOOL KillProcess(DWORD nProcessID)
{
	HANDLE hProcessHandle; 
	hProcessHandle =::OpenProcess( PROCESS_TERMINATE,FALSE, nProcessID );
	if (hProcessHandle == NULL)
		return FALSE;

	return ::TerminateProcess( hProcessHandle, 4 );
}

/*
*���ݽ�����ʾ�ı�������ɱ������
*/
BOOL KillProcess(LPCSTR pszWindowTitle)
{
	HWND TheWindow;

	TheWindow = ::FindWindow( NULL, pszWindowTitle );
	if (TheWindow == NULL)
		return FALSE;

	DWORD nProcessID;
	::GetWindowThreadProcessId( TheWindow, &nProcessID );

	return KillProcess(nProcessID);
}