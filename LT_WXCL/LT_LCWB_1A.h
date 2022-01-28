
// LT_LCWB_1A.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#define SCREEN_X 800
#define SCREEN_Y 600
// CLT_WXCLApp:
// 有关此类的实现，请参阅 LT_WXCL.cpp
//

class CLT_LCWB_1AApp : public CWinApp
{
public:
	CLT_LCWB_1AApp();
	void* pMainDlg;
	char HDDPath; //磁盘盘符

	char Local; //AB节
	char Remote;

	SOCKET BSoc;

	char m_FireCH[10];//报警的通道
	char m_FireWarFlag;//报警标记 0-无 1-报警 2-报警取消

    int IPCNum = 12;

// 重写
public:
	virtual BOOL InitInstance();

	// 实现

	DECLARE_MESSAGE_MAP()

    
};

extern CLT_LCWB_1AApp theApp;
