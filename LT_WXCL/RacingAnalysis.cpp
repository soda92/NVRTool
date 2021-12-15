#include "stdafx.h"
#include "resource.h"
#include "RacingAnalysis.h"
#include "RacingDlg.h"
#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"

using namespace std;

HANDLE LdlePort = 0;

int LdleDataComInit(char* com)
{
	DCB Dcb;
	COMMTIMEOUTS CommTimeouts;

	//int TAX_TYPE;
	//DWORD dwRet;
	LdlePort = CreateFile(com,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (LdlePort == INVALID_HANDLE_VALUE)
	{
		return -1;
	}


	GetCommState(LdlePort, &Dcb);
	//Dcb.BaudRate =28800;
	Dcb.BaudRate = 38400;
	Dcb.ByteSize = 8;
	Dcb.Parity = NOPARITY;
	//Dcb.Parity =ODDPARITY;
	Dcb.StopBits = ONESTOPBIT;
	//Dcb.fOutxCtsFlow = FALSE;

	SetCommState(LdlePort, &Dcb);

	GetCommTimeouts(LdlePort, &CommTimeouts);
	CommTimeouts.ReadIntervalTimeout = 10;
	CommTimeouts.ReadTotalTimeoutMultiplier = 10;
	CommTimeouts.ReadTotalTimeoutConstant = 10;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(LdlePort, &CommTimeouts);

	return 0;
}

//串口发送至空转
int LDFM_SendLdleDataMsg(unsigned char* buf, int len)
{
	if (LdlePort != 0)
	{
		DWORD dwRet;
		WriteFile(LdlePort, buf, len, &dwRet, NULL);
		if (dwRet > 0)
		{
			return dwRet;
		}
	}

	return -1;
}



//获取空转串口信息
UINT Thread_LdleDataAnalysis(LPVOID lParam)
{
	CRacingDlg* dlg = (CRacingDlg*)lParam;

	Sleep(10 * 1000);
	DWORD dwRet;
	unsigned char RecBuf[1024] = "";

	struct sockaddr_in BAddr;
	memset(&BAddr, 0, sizeof(struct sockaddr_in));
	BAddr.sin_family = AF_INET;
	BAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);//套接字地址为广播地址
	BAddr.sin_port = htons(8000);//套接字广播端口号为8000

	while (1)
	{
		//串口发送
		dlg->SendFireMsg();

		//串口读取
		memset(RecBuf, 0, sizeof(RecBuf));
		ReadFile(LdlePort, RecBuf, 1024, &dwRet, NULL);

		//存储空转数据


		//加入报文帧头和 本车他车AB节标志
		unsigned char SendBuf[1024] = "";
		SendBuf[0] = 0xFF;
		SendBuf[1] = 0x05;
		SendBuf[2] = theApp.Local[0];
		SendBuf[3] = theApp.Local[1];

		memcpy(&SendBuf[4], RecBuf, 18);

		if (dwRet > 0)
		{
			//发送重联机车 广播
			//广播数据(接收在LT_WXCLDlg->OnInitDialog->Thread_FireRec线程)
			sendto(theApp.BSoc, (char*)SendBuf, sizeof(SendBuf), 0, (SOCKADDR*)&BAddr, sizeof(SOCKADDR));
		}

		/*
		if (dwRet > 0)
		{
		dlg->LdleMsgAnalysis(RecBuf, dwRet, TRUE);
		}*/

		Sleep(1 * 1000);
	}

	return 0;
}
