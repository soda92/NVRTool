
// LT_LCWB_1A.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define SCREEN_X 800
#define SCREEN_Y 600
// CLT_WXCLApp:
// �йش����ʵ�֣������ LT_WXCL.cpp
//

class CLT_LCWB_1AApp : public CWinApp
{
public:
	CLT_LCWB_1AApp();
	void* pMainDlg;
	char HDDPath; //�����̷�

	char Local; //AB��
	char Remote;

	SOCKET BSoc;

	char m_FireCH[10];//������ͨ��
	char m_FireWarFlag;//������� 0-�� 1-���� 2-����ȡ��

    int IPCNum = 12;

// ��д
public:
	virtual BOOL InitInstance();

	// ʵ��

	DECLARE_MESSAGE_MAP()

    
};

extern CLT_LCWB_1AApp theApp;
