#include "stdafx.h"
#include "FireData.h"
#include "resource.h"
#include "FireMsgDlg.h"
#include "LT_LCWB_1A.h"

#include <iostream>
#include <iomanip>

template <typename T>
std::string HexToString(T uval)
{
	std::stringstream ss;
	ss << "0X" << std::setw(sizeof(uval) * 2) << std::setfill('0') << std::hex << +uval;
	return ss.str();
}

HANDLE fPort = 0;

int WXCL_FireComInit(char* com)
{
	DCB Dcb;
	COMMTIMEOUTS CommTimeouts;

	//int TAX_TYPE;
	//DWORD dwRet;
	fPort = CreateFile(com,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (fPort == INVALID_HANDLE_VALUE)
	{
		return -1;
	}


	GetCommState(fPort, &Dcb);
	//Dcb.BaudRate =28800;
	//Dcb.BaudRate =19200;
	Dcb.BaudRate = 9600;
	Dcb.ByteSize = 8;
	Dcb.Parity = NOPARITY;
	//Dcb.Parity =ODDPARITY;
	Dcb.StopBits = ONESTOPBIT;
	//Dcb.fOutxCtsFlow = FALSE;

	SetCommState(fPort, &Dcb);

	GetCommTimeouts(fPort, &CommTimeouts);
	CommTimeouts.ReadIntervalTimeout = 10;
	CommTimeouts.ReadTotalTimeoutMultiplier = 10;
	CommTimeouts.ReadTotalTimeoutConstant = 10;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(fPort, &CommTimeouts);

	return 1;
}

int WXCL_SendMsg(unsigned char* buf, int len)
{
	if (fPort != 0)
	{
		DWORD dwRet;
		WriteFile(fPort, buf, len, &dwRet, NULL);
		if (dwRet > 0)
		{
			return dwRet;
		}
	}

	return -1;
}


//向EF板卡发送TAX箱消息，然后接收EF板卡回复，并广播
UINT Thread_WXCL_FireData(LPVOID lParam)
{
	CFireMsgDlg* dlg = (CFireMsgDlg*)lParam;

	Sleep(1000);
	DWORD dwRet = 0;
	unsigned char RecBuf[256] = "";

	struct sockaddr_in BAddr;
	memset(&BAddr, 0, sizeof(struct sockaddr_in));
	BAddr.sin_family = AF_INET;
	BAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);//套接字地址为广播地址
	BAddr.sin_port = htons(8000);//套接字广播端口号为8000

	while (1)
	{
		//串口发送
		dlg->SendFireMsg();

		//串口接收
		memset(RecBuf, 0, sizeof(RecBuf));
		auto _ret = ReadFile(fPort, RecBuf, 100, &dwRet, NULL);//接收EF板卡信息

		//加入报文帧头和 本车他车AB节标志
		unsigned char SendBuf[100] = "";
		SendBuf[0] = 0xFF;
		SendBuf[1] = 0x01;
		SendBuf[2] = theApp.Local[0];
		SendBuf[3] = theApp.Local[1];

#ifdef _DEBUG
		std::string result;
		for (int i = 0; i < 256; i++) {
			result += HexToString<unsigned char>(RecBuf[i]).substr(2, 2);
		}

		PLOGD << "RecBuf: " << result;
#endif
		memcpy(&SendBuf[4], RecBuf, dwRet);

		// 		CString str="";
		// 		str.Format("dwRet=%d",dwRet);
		// 		dlg->MessageBox(str);

		if (dwRet > 0)
		{
			//dlg->FireDataAnalyse(RecBuf,dwRet,TRUE,0);//本车A节
			//广播数据(接收在LT_WXCLDlg->OnInitDialog->Thread_FireRec线程)
			sendto(theApp.BSoc, (char*)SendBuf, sizeof(SendBuf), 0, (SOCKADDR*)&BAddr, sizeof(SOCKADDR));
		}
		dwRet = 0;
		Sleep(1 * 1000);
	}

	return 0;
}

