// SystemProDll.h : SystemProDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSystemProDllApp
// �йش���ʵ�ֵ���Ϣ������� SystemProDll.cpp
//

class CSystemProDllApp : public CWinApp
{
public:
	CSystemProDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
