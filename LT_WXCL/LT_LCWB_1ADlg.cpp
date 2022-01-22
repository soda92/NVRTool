
// LT_LCWB_1ADlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"

#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"
#include "afxdialogex.h"

#include "HCNetSDK.h"

#include "VideoPlay.h"
#include "WaitDlg.h"
#include <Dbt.h>

#include "log.h"
#include "LogView.h"
#include "LT_LCWB_1AView.h"

#include "ManageView.h"

#include <boost/filesystem.hpp>
#include "progress_bar.h"
#include <string>
#include "D:/src/vcpkg/installed/x86-windows/include/httplib.h"
#include "fmt/core.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLT_WXCLDlg 对话框

extern char Global_IPCName[12][50];
char FirstDriveFromMask(ULONG unitmask);





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
    ON_WM_DEVICECHANGE()
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


BOOL CLT_LCWB_1ADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
    SetWindowText("太原机车视频监控");

    constexpr static GUID GUID_DEVINTERFACE_LIST[] =
    {
        // GUID_DEVINTERFACE_USB_DEVICE
        { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },

        // GUID_DEVINTERFACE_DISK
        { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },

        // GUID_DEVINTERFACE_HID,
        { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },

        // GUID_NDIS_LAN_CLASS
        { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } }

    };
    // Register DEVICE message
    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
    ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
    NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    for (int i = 0; i < sizeof(GUID_DEVINTERFACE_LIST) / sizeof(GUID); i++)
    {
        NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
        hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
        if (!hDevNotify)
        {
            return FALSE;
        }
    }

    boost::filesystem::path full_path(boost::filesystem::current_path());

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

    font_150.CreatePointFont(150, "黑体");
    stop_warn.SetFont(&font_150);

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

    m_logDlg.Create(IDD_DIALOG_LOG, GetDlgItem(IDC_TAB1));

    logn::system_start();
    LogView::Update();
	m_TabCtrl.InsertItem(0, _T("视频预览"));
	m_VideoDlg.Create(IDD_DIALOG_VIDEO, GetDlgItem(IDC_TAB1));
	m_TabCtrl.InsertItem(1, _T("设备管理"));
	m_ManageDlg.Create(IDD_DIALOG_MANAGE, GetDlgItem(IDC_TAB1));
	m_TabCtrl.InsertItem(2, _T("火警信息"));
	m_FireMsgDlg.Create(IDD_DIALOG_FIRE, GetDlgItem(IDC_TAB1));
    m_TabCtrl.InsertItem(3, _T("事件记录"));

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
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_UDrive, this, 0, NULL); // 开启u盘检测线程
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Index, this, 0, NULL); // 开启公共信息记录线程

    // 系统启动
    std::string url;
    httplib::Client cli("http://localhost:5000");
    url = fmt::format("/add/系统启动");
    //cli.Get(url.c_str());
    
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
        progress_bar::show();
		break;
	case 1:
		m_ManageDlg.ShowWindow(true);
        progress_bar::hide();
		break;
    case 2:
        m_FireMsgDlg.ShowWindow(true);
        progress_bar::hide();
        break;
    case 3:
        m_logDlg.ShowWindow(true);
        progress_bar::hide();
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
	sprintf_s(OSDThree, "%s 司机:%s", Global_IPCName[pos], SiJiHao);

	struShowString.struStringInfo[0].wShowString = 1;
	struShowString.struStringInfo[0].wStringSize = static_cast<WORD>(strlen(OSDOne));
	strcpy_s(struShowString.struStringInfo[0].sString, OSDOne);
	struShowString.struStringInfo[0].wShowStringTopLeftX = 400;
	struShowString.struStringInfo[0].wShowStringTopLeftY = 32;

	struShowString.struStringInfo[1].wShowString = 1;
	struShowString.struStringInfo[1].wStringSize = static_cast<WORD>(strlen(OSDTwo));
	strcpy_s(struShowString.struStringInfo[1].sString, OSDTwo);
	struShowString.struStringInfo[1].wShowStringTopLeftX = 0;
	struShowString.struStringInfo[1].wShowStringTopLeftY = 540;

	struShowString.struStringInfo[2].wShowString = 1;
	struShowString.struStringInfo[2].wStringSize = static_cast<WORD>(strlen(OSDThree));
	strcpy_s(struShowString.struStringInfo[2].sString, OSDThree);
	struShowString.struStringInfo[2].wShowStringTopLeftX = 360; //320
	struShowString.struStringInfo[2].wShowStringTopLeftY = 540;

    if (m_ManageDlg.URecordStatus[pos] && m_ManageDlg.RecordFlag[pos]) {
        struShowString.struStringInfo[3].wShowString = 1;
        struShowString.struStringInfo[3].wStringSize = static_cast<WORD>(strlen("REC USB"));
        strcpy_s(struShowString.struStringInfo[3].sString, "REC USB");
        struShowString.struStringInfo[3].wShowStringTopLeftX = 30;
        struShowString.struStringInfo[3].wShowStringTopLeftY = 100;
    }
    else {
        if (m_ManageDlg.RecordFlag[pos])
        {
            struShowString.struStringInfo[3].wShowString = 1;
            struShowString.struStringInfo[3].wStringSize = static_cast<WORD>(strlen("REC"));
            strcpy_s(struShowString.struStringInfo[3].sString, "REC");
            struShowString.struStringInfo[3].wShowStringTopLeftX = 30;
            struShowString.struStringInfo[3].wShowStringTopLeftY = 100;
        }
        else if (m_ManageDlg.URecordStatus[pos]) {
            struShowString.struStringInfo[3].wShowString = 1;
            struShowString.struStringInfo[3].wStringSize = static_cast<WORD>(strlen("USB"));
            strcpy_s(struShowString.struStringInfo[3].sString, "USB");
            struShowString.struStringInfo[3].wShowStringTopLeftX = 30;
            struShowString.struStringInfo[3].wShowStringTopLeftY = 100;
        }
        else
        {
            struShowString.struStringInfo[3].wShowString = 0;
        }
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
	strcpy_s(struLoginInfo.sDeviceAddress, ip);	 //设备IP地址
	struLoginInfo.wPort = 8000;					 //设备服务端口
	strcpy_s(struLoginInfo.sUserName, "admin");	 //设备登录用户名
	strcpy_s(struLoginInfo.sPassword, "hk123456"); //设备登录密码
	//strcpy(struLoginInfo.sPassword, "123456"); //设备登录密码

	//设备信息, 输出参数
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = { 0 };

    // PLOGD << "window handle: " << (long)pHandle;

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

	struPlayInfo.dwStreamType = 1; //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
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

	if (::bind(BRecUdp, (struct sockaddr*)&Addr, sizeof(sockaddr)) < 0)
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

char get_device_letter_from_mask(DWORD unitmask)
{
    int i;
    for (i = 0; i < 26; i++)
    {
        if (unitmask & 1)
            break;
        unitmask = unitmask >> 1;
    }
    return i + 'A';
}

BOOL CLT_LCWB_1ADlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
    DEV_BROADCAST_VOLUME* pDevVolume = (DEV_BROADCAST_VOLUME*)dwData;
    if (nEventType != DBT_DEVICEARRIVAL)
        return TRUE;

    if (pDevVolume->dbcv_devicetype != DBT_DEVTYP_VOLUME)
        return TRUE;

    char disk = get_device_letter_from_mask(pDevVolume->dbcv_unitmask);

    auto uPath = fmt::format("{}:\\", disk);

    if (m_ManageDlg.IsHDD((char*)uPath.c_str()))
    {
        this->usb_flag = false;
        return FALSE;
    }

    m_ManageDlg.udisk_path = uPath;

    this->usb_flag = true;

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_UDisk_Process, this, 0, NULL);//开启下载线程

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


void CLT_LCWB_1ADlg::OnCancel()
{
    // TODO: 在此添加专用代码和/或调用基类

    // 保存事件记录
    std::string url;
    httplib::Client cli("http://localhost:5000");
    url = fmt::format("/add/系统退出");
    //cli.Get(url.c_str());
    LogView::Update();
    logn::system_exit();
    CDialogEx::OnCancel();
}


void CLT_LCWB_1ADlg::OnOK()
{
    // TODO: 在此添加专用代码和/或调用基类
    //
    // 保存事件记录
    std::string url;
    httplib::Client cli("http://localhost:5000");
    url = fmt::format("/add/系统退出");
    //cli.Get(url.c_str());
    LogView::Update();
    logn::system_exit();
    CDialogEx::OnOK();
}
