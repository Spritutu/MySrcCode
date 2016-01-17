// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣

#define _WIN32_WINNT  0x0500				// ����ϵͳ�汾, ȷ������ʹ�õײ���̹���
#include "stdafx.h"
#include "MouseCatch.h"
#include <Windows.h>
static HINSTANCE	g_hInstance = NULL;				// ģ��ʵ�����
static HHOOK		g_hHook = NULL;					// ���Ӿ��

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	// ����ģ��ʵ�����
	g_hInstance = (HINSTANCE)hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		if (g_hHook != NULL) UnhookWindowsHookEx(g_hHook);
		break;
	}
	return TRUE;
}
/// �ײ���̹��Ӻ���
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode==HC_ACTION)
	{
		if (wParam==WM_LBUTTONDOWN)
		{
			OutputDebugString("����������\n");
		}
	}
	
	// ����ϵͳ�е���һ������
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

BOOL WINAPI StartMouseHook()
{
	g_hHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, g_hInstance, NULL);
	if (g_hHook == NULL) 
		return FALSE;

	return TRUE;
}
void WINAPI StopMouseHook()
{
	if (g_hHook)
	{
		UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
}