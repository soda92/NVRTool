
// LT_LCWB_1ADlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"
#include "afxdialogex.h"

#include "HCNetSDK.h"

#include "VideoPlay.h"
#include "WaitDlg.h"
#include <Dbt.h>

#include <sapi.h> // 导入语音头文件
#include "sphelper.h"
#pragma comment(lib, "sapi.lib")

#include "log.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLT_WXCLDlg 对话框

extern char IPCName[12][50];
char FirstDriveFromMask(ULONG unitmask);

CString WarVoice;
int WINAPI Thread_Voice(LPVOID lpPara);



CLT_LCWB_1ADlg::CLT_LCWB_1ADlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLT_LCWB_1ADlg::IDD, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(lUserID, -1, sizeof(lUserID));
	memset(lRealPlayHandle, -1, sizeof(lRealPlayHandle));
	TaxStat = FALSE;
	memset(&TaxData, 0, sizeof(TaxData));
	OsdIndex = 0;
	BRecUdp = -1;
}

void CLT_LCWB_1ADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
    DDX_Control(pDX, IDC_BUTTON_STOPWARN, stop_warn);
}

BEGIN_MESSAGE_MAP(CLT_LCWB_1ADlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CLT_LCWB_1ADlg::OnTcnSelchangeTab1)
	ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON_STOPWARN, &CLT_LCWB_1ADlg::OnBnClickedStopWarn)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CLT_WXCLDlg 消息处理程序

int __stdcall VideoStateCB(LONG nPort, char type, char* error)
{
	CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)theApp.pMainDlg;
	TRACE("port = %d,type = %d,error = %s\n", nPort, type, error);
	//AfxMessageBox(error);
	if (dlg != NULL)
	{
		if (type == 0) //replay
		{
			//dlg->m_ReplayDlg.ReplayCallBack(nPort,error);
		}
		else if (type == 2) //record
		{
			dlg->m_ManageDlg.m_RecordCallBack(nPort, error);
		}
	}
	return 0;
}

int WINAPI Thread_SetOsd(LPVOID lpPara)
{
	CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;
	int i = dlg->OsdIndex;

	TRACE("osd i = %d\n", i);

	while (1)
	{
		char Speed[20] = "";
		char Mileage[20] = "";
		char CheCi[20] = "";
		char CheHao[20] = "";
		char SiJiHao[20] = "";

		double LiCheng = dlg->TaxData.Signpost / 1000.0;

		sprintf_s(CheCi, "%d", dlg->TaxData.TrainNum);

		sprintf_s(Speed, "%d", dlg->TaxData.Speed);
		sprintf_s(Mileage, "%.3lf", LiCheng);
		sprintf_s(CheHao, "%d", dlg->TaxData.EngineNo);
		sprintf_s(SiJiHao, "%d", dlg->TaxData.DriverNo);

		if (dlg->lUserID[i] >= 0)
		{
			dlg->VideoOSDSet(&dlg->lUserID[i], Speed, Mileage, CheCi, CheHao, (i > 7 ? i - 8 : i), SiJiHao);
		}

		Sleep(2 * 1000);
	}

	return 0;
}

int WINAPI Thread_Play(LPVOID lpPara)
{
	CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;
	char ip[30] = "";
	//Sleep(5 * 1000);

	int tmp = (theApp.Local[1] == 'A' ? 0 : 8);

	for (int i = tmp; i < (8 + tmp); i++)
	{
		dlg->OsdIndex = i;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_SetOsd, lpPara, 0, NULL); //osd叠加线程
		Sleep(50);
	}

	/*while (1)
	{*/
		for (int i = 0; i < 32; i++)
		{

			if (dlg->lUserID[i] == -1)
			{

				if (i <= 5 && i >= 0)
					sprintf_s(ip, "192.168.10%d.7%d", atoi(&theApp.Local[0]), i);
				else if (i <= 13 && i >= 8)
					sprintf_s(ip, "192.168.10%d.8%d", atoi(&theApp.Local[0]), i - 8);
				else
					continue;

				TRACE("ipc ip = %s\n", ip);
                strcpy_s(dlg->ip[i], ip);

				int res = dlg->VideoPlay(dlg->ip[i], &(dlg->lUserID[i]), &(dlg->lRealPlayHandle[i]),
                    dlg->m_VideoDlg.m_videoPlayWnd[i]->GetSafeHwnd());
				if (res < 0)
				{
					//TRACE("ipc %s error\n",ip);
					dlg->lUserID[i] = -1;
					dlg->lRealPlayHandle[i] = -1;
				}
			}
			Sleep(50);
		}

		Sleep(5 * 1000);
	//}

	return 0;
}

int WINAPI Thread_SetIpcTime(LPVOID lpPara)
{
	CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

	while (1)
	{
		Sleep(10 * 60 * 1000);
		dlg->TimeCFG();
	}
	return 0;
}

//UDP广播接收函数
//
//作用：接收UDP广播，识别报头，区分数据来源，然后将数据交给对应的处理函数
//
//数据分类：防火数据，采集盒数据，报警中断数据，空转数据
//
int WINAPI Thread_UDPBroadcastRecv(LPVOID lpPara)
{
	CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

	struct sockaddr_in addr = {0};
	int addrLen = sizeof(addr);
	unsigned char RecBuf[1024] = "";

	while (1)
	{
		int res = recvfrom(dlg->BRecUdp, (char*)RecBuf, sizeof(RecBuf), 0, (struct sockaddr*)&addr, &addrLen);
		if (res > 0)
		{
			if (RecBuf[0] == 0xFF && RecBuf[1] == 0x01) //防火
			{
				char Remote[10] = "";
				Remote[0] = RecBuf[2];
				Remote[1] = RecBuf[3];

				if (Remote[0] == theApp.Local[0] && Remote[1] == 'A') //本车A
				{
					dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 0);
				}
				if (Remote[0] == theApp.Local[0] && Remote[1] == 'B') //本车B
				{
					dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 1);
				}
				if (Remote[0] != theApp.Local[0] && Remote[1] == 'A') //他车A
				{
					dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 2);
				}
				if (Remote[0] != theApp.Local[0] && Remote[1] == 'B') //他车B
				{
					dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 3);
				}
			}
			else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x02) //采集盒
			{
				char Remote[10] = "";
				Remote[0] = RecBuf[2];
				Remote[1] = RecBuf[3];

			}
			else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x03) //校时
			{
				char Remote[10] = "";
				Remote[0] = RecBuf[2];
				Remote[1] = RecBuf[3];

				if (strcmp(Remote, theApp.Local) && Remote[0] == theApp.Local[0])
				{
					memcpy(&dlg->TaxData, &RecBuf[4], sizeof(dlg->TaxData));

					if (dlg->TaxData.TAXTime.Year != 0 && dlg->TaxData.TAXTime.Month != 0 && dlg->TaxData.TAXTime.Day != 0)
					{
						CTime curTime = CTime::GetCurrentTime();
						CTime TaxTime(dlg->TaxData.TAXTime.Year, dlg->TaxData.TAXTime.Month, dlg->TaxData.TAXTime.Day, dlg->TaxData.TAXTime.Hour, dlg->TaxData.TAXTime.Minute, dlg->TaxData.TAXTime.Second);
						CTimeSpan span = curTime - TaxTime;

						//if (time.wYear != MainDlg->TaxData.TAXTime.Year || time.wMonth != MainDlg->TaxData.TAXTime.Month || time.wDay != MainDlg->TaxData.TAXTime.Day || time.wHour != MainDlg->TaxData.TAXTime.Hour || time.wMinute != MainDlg->TaxData.TAXTime.Minute)
						if (span.GetTotalSeconds() > 60 || span.GetTotalSeconds() < -60)
						{
							//MainDlg->SetFireText("set time");
							TRACE("tax set time\n");
							SYSTEMTIME time;
							GetLocalTime(&time);
							time.wYear = dlg->TaxData.TAXTime.Year;
							time.wMonth = dlg->TaxData.TAXTime.Month;
							time.wDay = dlg->TaxData.TAXTime.Day;
							time.wHour = dlg->TaxData.TAXTime.Hour;
							time.wMinute = dlg->TaxData.TAXTime.Minute;
							time.wSecond = dlg->TaxData.TAXTime.Second;

							SetLocalTime(&time);
						}
					}
				}
			}
			else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x04) //报警中断
			{
				dlg->m_FireMsgDlg.StopWarFun();
			}
			else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x05) //空转
			{
				char Remote[10] = "";
				Remote[0] = RecBuf[2];
				Remote[1] = RecBuf[3];
			}
		}
		memset(RecBuf, 0, sizeof(RecBuf));
	}
	return 0;
}

BOOL CLT_LCWB_1ADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    // 初始化PLOG
	plog::init(plog::debug, "log.txt"); // Step2: initialize the logger

    // 故障记录列表
    logn::load();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);	 // 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_brush.CreateSolidBrush(RGB(0, 0, 0));
	newFont.CreatePointFont(200, "黑体");

	CArray<COLORREF, COLORREF> arColors;

	arColors.Add(RGB(121, 210, 231));
	arColors.Add(RGB(190, 218, 153));
	arColors.Add(RGB(255, 170, 100));

	//******************************

    // 停止报警 按钮
    stop_warn.SetBkColor(RGB(0, 0, 0));
    stop_warn.SetForeColor(RGB(255, 255, 255));

    CFont font;
    font.CreatePointFont(150, "黑体");
    stop_warn.SetFont(&font);

#if !defined(_DEBUG)
	CWaitDlg dlg;
	dlg.DoModal();
#endif

	theApp.pMainDlg = this;
	GetPrivateProfileString("LT_WXCLCFG", "Local", "", theApp.Local, 10, ".//LT_WXCLCFG.ini");
	theApp.Remote = GetPrivateProfileInt("LT_WXCLCFG", "Remote", 0, ".//LT_WXCLCFG.ini");

	///**************

	stop_warn.SetBkColor(RGB(0, 0, 0));
	stop_warn.SetForeColor(RGB(255, 255, 255));

	MoveWindow(0, 0, SCREEN_X, SCREEN_Y);
	CenterWindow();

	m_TabCtrl.SetItemSize(CSize(150, 40));

    m_TabCtrl.MoveWindow(-3, -3, SCREEN_X + 3, SCREEN_Y + 3);
    stop_warn.MoveWindow(SCREEN_X - 125, 3, 120, 35);

	m_TabCtrl.InsertItem(0, _T("视频预览"));
	m_VideoDlg.Create(IDD_DIALOG_VIDEO, GetDlgItem(IDC_TAB1));
	m_TabCtrl.InsertItem(1, _T("设备管理"));
	m_ManageDlg.Create(IDD_DIALOG_MANAGE, GetDlgItem(IDC_TAB1));
	m_TabCtrl.InsertItem(2, _T("火警信息"));
	m_FireMsgDlg.Create(IDD_DIALOG_FIRE, GetDlgItem(IDC_TAB1));
    m_TabCtrl.InsertItem(3, _T("事件记录"));
    m_logDlg.Create(IDD_DIALOG_LOG, GetDlgItem(IDC_TAB1));

	CRect rc; //标签页里的窗口大小
	m_TabCtrl.GetClientRect(&rc);
	ClientToScreen(rc);
	m_TabCtrl.SetMinTabWidth(rc.Width() / 7);

	rc.top += 40;

	m_VideoDlg.MoveWindow(&rc);
	m_ManageDlg.MoveWindow(&rc);
    m_FireMsgDlg.MoveWindow(&rc);
    m_logDlg.MoveWindow(&rc);

	m_VideoDlg.ShowWindow(TRUE);

	char TaxCom[20] = "";
	GetPrivateProfileString("LT_WXCLCFG", "TaxCom", "COM1", TaxCom, 20, ".//LT_WXCLCFG.ini");
	char Parity = GetPrivateProfileInt("LT_WXCLCFG", "TaxParity", 0, ".//LT_WXCLCFG.ini");

	if (TaxCOMInit(TaxCom, Parity) != -1)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_TaxData, this, 0, NULL);
		SetTimer(3, 20 * 1000, NULL);
	}
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_SetIpcTime, this, 0, NULL); //校时 10min

	char PweCom[20] = "";
	GetPrivateProfileString("LT_WXCLCFG", "PweCom", "COM3", PweCom, 20, ".//LT_WXCLCFG.ini");
	Parity = GetPrivateProfileInt("LT_WXCLCFG", "PweParity", 0, ".//LT_WXCLCFG.ini");

	if (PweCOMInit(PweCom, Parity) != -1)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_PweData, this, 0, NULL);
	}

	//ffmpeg dll
	Video_Init();
	Video_SetStateCallBack(VideoStateCB);

	//hk video
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(6000, true);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Play, this, 0, NULL); //开启视频线程

	//////////////////////////////////////////////////////////////////////////
	BROADCASTInit();
	if (FireUdpInit() != -1)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_UDPBroadcastRecv, this, 0, NULL);
	}

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Voice, this, 0, NULL);


    // 系统启动
    logn::system_start();
	return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLT_LCWB_1ADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		dc.SelectObject(newFont);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLT_LCWB_1ADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLT_LCWB_1ADlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int CurSel = m_TabCtrl.GetCurSel();

	m_VideoDlg.ShowWindow(false);
	m_ManageDlg.ShowWindow(false);
    m_FireMsgDlg.ShowWindow(false);
    m_logDlg.ShowWindow(false);

	switch (CurSel)
	{
	case 0:
		m_VideoDlg.ShowWindow(true);
		break;
	case 1:
		m_ManageDlg.ShowWindow(true);
		break;
    case 2:
        m_FireMsgDlg.ShowWindow(true);
        break;
    case 3:
        m_logDlg.ShowWindow(true);
        break;
	default:
		break;
	}

	*pResult = 0;

}

int CLT_LCWB_1ADlg::VideoOSDSet(long* pUid, char* Speed, char* Mileage, char* CheCi, char* CheHao, char pos, char* SiJiHao)
{
	char OSDOne[100] = "";	 //右上角
	char OSDTwo[100] = "";	 //左下角
	char OSDThree[100] = ""; //右下角
	NET_DVR_SHOWSTRING_V30 struShowString = { 0 };
	unsigned long dwReturned = 0;

	sprintf_s(OSDOne, "%skm/h %skm", Speed, Mileage);
	sprintf_s(OSDTwo, "车次:%s 车号:%s", CheCi, CheHao);
	sprintf_s(OSDThree, "%s 司机:%s", IPCName[pos], SiJiHao);

	struShowString.struStringInfo[0].wShowString = 1;
	struShowString.struStringInfo[0].wStringSize = static_cast<WORD>(strlen(OSDOne));
	strcpy(struShowString.struStringInfo[0].sString, OSDOne);
	struShowString.struStringInfo[0].wShowStringTopLeftX = 400;
	struShowString.struStringInfo[0].wShowStringTopLeftY = 32;

	struShowString.struStringInfo[1].wShowString = 1;
	struShowString.struStringInfo[1].wStringSize = static_cast<WORD>(strlen(OSDTwo));
	strcpy(struShowString.struStringInfo[1].sString, OSDTwo);
	struShowString.struStringInfo[1].wShowStringTopLeftX = 0;
	struShowString.struStringInfo[1].wShowStringTopLeftY = 540;

	struShowString.struStringInfo[2].wShowString = 1;
	struShowString.struStringInfo[2].wStringSize = static_cast<WORD>(strlen(OSDThree));
	strcpy(struShowString.struStringInfo[2].sString, OSDThree);
	struShowString.struStringInfo[2].wShowStringTopLeftX = 360; //320
	struShowString.struStringInfo[2].wShowStringTopLeftY = 540;

	if (m_ManageDlg.RecordFlag[pos])
	{
		struShowString.struStringInfo[3].wShowString = 1;
		struShowString.struStringInfo[3].wStringSize = static_cast<WORD>(strlen("REC"));
		strcpy(struShowString.struStringInfo[3].sString, "REC");
		struShowString.struStringInfo[3].wShowStringTopLeftX = 5;
		struShowString.struStringInfo[3].wShowStringTopLeftY = 80;
	}
	else
	{
		struShowString.struStringInfo[3].wShowString = 0;
	}

	struShowString.dwSize = sizeof(struShowString);

	if (!NET_DVR_SetDVRConfig(*pUid, NET_DVR_SET_SHOWSTRING_V30, 1, &struShowString, sizeof(NET_DVR_SHOWSTRING_V30)))
	{
		return -1;
	}
	return 0;
}

int CLT_LCWB_1ADlg::VideoPlay(char* ip, long* pUid, long* pHandle, HWND hWnd)
{
	//登录参数，包括设备地址、登录用户、密码等
	NET_DVR_USER_LOGIN_INFO struLoginInfo = { 0 };
	struLoginInfo.bUseAsynLogin = 0;			 //同步登录方式
	strcpy(struLoginInfo.sDeviceAddress, ip);	 //设备IP地址
	struLoginInfo.wPort = 8000;					 //设备服务端口
	strcpy(struLoginInfo.sUserName, "admin");	 //设备登录用户名
	strcpy(struLoginInfo.sPassword, "hk123456"); //设备登录密码
	//strcpy(struLoginInfo.sPassword, "123456"); //设备登录密码

	//设备信息, 输出参数
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = { 0 };

	*pUid = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (*pUid < 0)
	{
		TRACE("Login failed, error code: %d ip = %s\n", NET_DVR_GetLastError(), ip);
		return -1;
	}

	//---------------------------------------
	//启动预览并设置回调数据流
	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = hWnd;  //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	struPlayInfo.lChannel = 1;	   //预览通道号

	struPlayInfo.dwStreamType = 0; //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo.dwLinkMode = 0;   //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP

	*pHandle = NET_DVR_RealPlay_V40(*pUid, &struPlayInfo, NULL, NULL);
	if (*pHandle < 0)
	{
		TRACE("NET_DVR_RealPlay_V40 error code: %d ip = %s\n", NET_DVR_GetLastError(), ip);
		NET_DVR_Logout(*pUid);
		return -1;
	}
	//VideoOSDSet(pUid,"100","200","t23","12378",3,"12445");
	return 0;
}

void CLT_LCWB_1ADlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 3)
	{
		TaxStat = FALSE;
		memset(&TaxData, 0, sizeof(TaxData));
	}
	CDialogEx::OnTimer(nIDEvent);
}

int CLT_LCWB_1ADlg::TimeCFG()
{
	SYSTEMTIME Time;
	GetLocalTime(&Time);
	NET_DVR_TIME NvrTime = {0};

	NvrTime.dwYear = Time.wYear;
	NvrTime.dwMonth = Time.wMonth;
	NvrTime.dwDay = Time.wDay;

	NvrTime.dwHour = Time.wHour;
	NvrTime.dwMinute = Time.wMinute;
	NvrTime.dwSecond = Time.wSecond;

	int tmp = (theApp.Local[1] == 'A' ? 0 : 8);

	for (int i = tmp; i < (8 + tmp); i++)
	{
		if (lUserID[i] >= 0)
		{

			bool res = NET_DVR_SetDVRConfig(lUserID[i], NET_DVR_SET_TIMECFG, 0, &NvrTime, sizeof(NvrTime));
			if (!res)
			{
				TRACE("time set error \n");
			}
		}
	}

	TRACE("ipc time set\n");
	return 0;
}

int CLT_LCWB_1ADlg::BROADCASTInit()
{
	theApp.BSoc = -1;
	if ((theApp.BSoc = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		return -1;
	}
	int opt = -1;
	int nb = 0;
	nb = setsockopt(theApp.BSoc, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt)); //设置套接字类型
	if (nb == -1)
	{
		return -1;
	}

	return 0;
}

int CLT_LCWB_1ADlg::FireUdpInit()
{
	//接收防火信息
	BRecUdp = -1;
	if ((BRecUdp = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		TRACE("fire socket error!\n");
		return -1;
	}

	sockaddr_in Addr;
	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(8000);
	Addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(BRecUdp, (struct sockaddr*)&Addr, sizeof(sockaddr)) < 0)
		return -1;

	return 0;
}

char FirstDriveFromMask(ULONG unitmask)
{
	char i;

	for (i = 0; i < 26; ++i)
	{
		if (unitmask & 0x1)
			break;
		unitmask >>= 1;
	}

	return (i + 'A');
}

BOOL CLT_LCWB_1ADlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	//MessageBox("收到消息");
	CString detectMsg;
	PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)dwData;

	switch (nEventType)
	{
	case DBT_DEVICEREMOVECOMPLETE: //U盘拔出
	{
		if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) //逻辑卷
		{
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			switch (lpdbv->dbcv_flags)
			{
			case 0: //U盘
			{
				// 						CString decDriver;
				// 						decDriver = FirstDriveFromMask(lpdbv ->dbcv_unitmask);
				// 						detectMsg.Format(_T("检测到U盘:[%s]拔出!"), decDriver.GetBuffer(0));
				// 						MessageBox(detectMsg,0);
			}
			break;
			case DBTF_MEDIA: //光盘

				break;
			}
		}
	}
	break;
	case DBT_DEVICEARRIVAL: //U盘插入
	{
		if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) //逻辑卷
		{
			PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
			switch (lpdbv->dbcv_flags)
			{
			case 0: //U盘
			{
				CString decDriver, fileName;
				decDriver = FirstDriveFromMask(lpdbv->dbcv_unitmask);
				detectMsg.Format(_T("检测到U盘[%s：]插入!\n是否拷贝空转数据到 [%s：]盘？"), decDriver.GetBuffer(0), decDriver.GetBuffer(0));

				fileName.Format("%s:/license.txt", decDriver.GetBuffer(0));
				CFile file1;
				if (file1.Open(fileName, CFile::modeRead))
				{
					if (MessageBox(detectMsg, "U盘插入提示", MB_SYSTEMMODAL | MB_YESNO) == 6)
					{
						//拷贝文件
						/*m_RacingDlg.set_Upath(decDriver.GetBuffer(0));
						m_RacingDlg.copyToU();*/
					}
				}
				file1.Close();
			}
			break;
			case DBTF_MEDIA: //光盘
				break;
			}
		}
	}
	break;
	}

	return TRUE;
}

HBRUSH CLT_LCWB_1ADlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG) //对话框颜色
		return m_brush;

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT); //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合
		hbr = (HBRUSH)m_brush;
	}

	if (nCtlColor == CTLCOLOR_BTN || nCtlColor == CTLCOLOR_LISTBOX || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT); //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合
		hbr = (HBRUSH)m_brush;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CLT_LCWB_1ADlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnEraseBkgnd(pDC);
}


void CLT_LCWB_1ADlg::OnBnClickedStopWarn()
{
    // TODO: 在此添加控件通知处理程序代码
    struct sockaddr_in BAddr;
    memset(&BAddr, 0, sizeof(struct sockaddr_in));
    BAddr.sin_family = AF_INET;
    BAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);//套接字地址为广播地址
    BAddr.sin_port = htons(8000);//套接字广播端口号为8000

    //加入报文帧头和 本车他车AB节标志
    unsigned char SendBuf[5] = "";
    SendBuf[0] = 0xFF;
    SendBuf[1] = 0x04;
    SendBuf[2] = theApp.Local[0];
    SendBuf[3] = theApp.Local[1];
    for (int i = 0; i < 4; i++)
    {
        SendBuf[4] += SendBuf[i];
    }
    //MessageBox("停止报警");
    sendto(theApp.BSoc, (char*)SendBuf, sizeof(SendBuf), 0, (SOCKADDR*)&BAddr, sizeof(SOCKADDR));
    Sleep(1);
}


int WINAPI Thread_Voice(LPVOID lpPara)
{
	CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;
	//WarVoice = _T("1号火焰 2号烟雾 探测器报警");

	if (FAILED(CoInitialize(NULL)))
	{
		AfxMessageBox(_T("Error to intiliaze COM"));
		return -1;
	}

	Sleep(3 * 1000);

	HRESULT hr = E_FAIL;
	ISpVoice* pVoice = NULL; //初始化COM
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
	pVoice->SetRate(1);
	ISpObjectToken* p = NULL;

	char lan = -1;

	//WarVoice = "hello";
	while (1)
	{
		UCHAR f_add = 0;
		CString fWarVoice = "";
		for (int i = 0; i < 2; i++)
		{
			if (dlg->m_FireMsgDlg.f_fire_tex[i] != "")
			{
				fWarVoice += dlg->m_FireMsgDlg.f_fire_tex[i];
				f_add++;
			}
		}

		if (f_add > 0)
		{
			hr = SpFindBestToken(SPCAT_VOICES, L"language=804", NULL, &p);
			if (SUCCEEDED(hr))
			{
				pVoice->SetVoice(p);
			}
			pVoice->Speak(fWarVoice.AllocSysString(), 0, NULL);
			pVoice->WaitUntilDone(INFINITE);
			// TEST
#ifdef TEST_DISABLED
		}
		else {
			fWarVoice = "B节探头1报警.....";
#endif
			// END TEST

			// 联动
			//
			// 设置当前选项
			dlg->m_TabCtrl.SetCurSel(0);
			// 切换窗口
			dlg->m_VideoDlg.ShowWindow(SW_SHOW);
			dlg->m_ManageDlg.ShowWindow(SW_HIDE);
			dlg->m_FireMsgDlg.ShowWindow(SW_HIDE);
			// 切换按钮
			if (0)
			{
				dlg->m_VideoDlg.OnBnClickedButtonBb();
			}

			if (fWarVoice.GetAt(0) == 'A')
			{
				PLOGD << "switch to A...";
                if (dlg->m_VideoDlg.CurrentBox != 0) {
                    dlg->m_VideoDlg.OnBnClickedButtonBa();
                }
			}
			if (fWarVoice.GetAt(0) == 'B')
			{
				PLOGD << "switch to B...";
                if (dlg->m_VideoDlg.CurrentBox != 1) {
                    dlg->m_VideoDlg.OnBnClickedButtonBa();
                }
            }

			// A节探头2报警.....
			PLOGD << "fWarVoice.GetLength()" << fWarVoice.GetLength();
			PLOGD << "fWarVoice" << fWarVoice;
			PLOGD << "fWarVoice.GetAt(0)" << fWarVoice.GetAt(0);
			PLOGD << "fWarVoice.GetAt(7)" << fWarVoice.GetAt(7);

			// 最大化
			auto detector_num = 0;
			if (fWarVoice.GetAt(0) == 'A')
			{
				auto c1 = fWarVoice.GetAt(7);
				detector_num = fWarVoice.GetAt(7) - '0';
			}
			else
			{
				detector_num = fWarVoice.GetAt(7) - '0' + 4;
			}
			PLOGD << "detector num: " << detector_num;
			auto detector = "detector" + std::to_string(detector_num);
			char video_num_cstr[21] = { '\0' };
			// 读取对应IPC
			GetPrivateProfileString("LT_WXCLCFG", detector.c_str(), "0", video_num_cstr, 20, ".//LT_WXCLCFG.ini");
			int video_num = std::stoi(std::string(video_num_cstr));
			PLOGD << "video num: " << video_num;
			// 切换窗口
			dlg->m_VideoDlg.ChangeWndRects(0, video_num - 1);
		}

		Sleep(3 * 1000);
	}

	pVoice->Release();
	pVoice = NULL;

	CoUninitialize();
	return 0;
}


void CLT_LCWB_1ADlg::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类

    // 保存事件记录
    logn::system_exit();
    CDialogEx::OnCancel();
}


void CLT_LCWB_1ADlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    // 
    // 保存事件记录
    logn::system_exit();
    CDialogEx::OnOK();
}
