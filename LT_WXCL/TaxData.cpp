//TexData.cpp

#include "stdafx.h"
#include "TaxData.h"
#include "resource.h"
#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"
#include "HCNetSDK.h"


template <typename T>
std::string HexToString(T uval)
{
    std::stringstream ss;
    ss << "0x" << std::setw(sizeof(uval) * 2) << std::setfill('0') << std::hex << +uval;
    return ss.str();
}


CString texportNo;

HANDLE tPort, pPort;
UCHAR DataBuf[256];


SOCKET TaxSoc;
sockaddr_in TaxAddr;

UINT Thread_TaxData(LPVOID lParam) {
	CLT_LCWB_1ADlg* MainDlg = (CLT_LCWB_1ADlg*)lParam;

	int TAX_TYPE;
	DWORD dwRet;
	char Num = 0;

	TAXDATA TaxBuf;

	struct sockaddr_in BAddr;
	memset(&BAddr, 0, sizeof(struct sockaddr_in));
	BAddr.sin_family = AF_INET;
	BAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);//套接字地址为广播地址
	BAddr.sin_port = htons(8000);//套接字广播端口号为8000
	unsigned char SendBuf[256] = "";

	unsigned char num = 0;
	while (1)
	{
		memset(&DataBuf, 0x00, sizeof(DataBuf));
        auto _ = ReadFile(tPort, &DataBuf, 256, &dwRet, NULL);

        std::string str;
        for (int i = 0; i < 256; i++) {
            str += HexToString(DataBuf[i]).substr(2, 2);
        }
        // PLOGD << "Tax Data: " << str;

		memset(&TaxBuf, 0, sizeof(TaxBuf));

		TAX_TYPE = AnalyseTax(DataBuf, &TaxBuf);
		if (TAX_TYPE > 0)
		{
			MainDlg->TaxStat = TRUE;
			memcpy(&MainDlg->TaxData, &TaxBuf, sizeof(MainDlg->TaxData));

			//////////////////////////////////////////////////////////////////////////
			if (++Num >= 10)
			{
				Num = 0;

                SendBuf[0] = 0xFF;
                SendBuf[1] = 0x03;
                SendBuf[2] = theApp.Local[0];
                SendBuf[3] = theApp.Local[1];
                memcpy(&SendBuf[4], &TaxBuf, sizeof(MainDlg->TaxData));
                sendto(theApp.BSoc, (char*)SendBuf, sizeof(MainDlg->TaxData) + 6, 0, (SOCKADDR*)&BAddr, sizeof(SOCKADDR));

				//校时
				if (MainDlg->TaxData.TAXTime.Year != 0 && MainDlg->TaxData.TAXTime.Month != 0 && MainDlg->TaxData.TAXTime.Day != 0)
				{
					CTime curTime = CTime::GetCurrentTime();
					CTime TaxTime(MainDlg->TaxData.TAXTime.Year, MainDlg->TaxData.TAXTime.Month, MainDlg->TaxData.TAXTime.Day, MainDlg->TaxData.TAXTime.Hour, MainDlg->TaxData.TAXTime.Minute, MainDlg->TaxData.TAXTime.Second);
					CTimeSpan span = curTime - TaxTime;

					if (span.GetTotalSeconds() > 60 || span.GetTotalSeconds() < -60)
					{
						TRACE("tax set time\n");
						SYSTEMTIME time;
						GetLocalTime(&time);
						time.wYear = MainDlg->TaxData.TAXTime.Year;
						time.wMonth = MainDlg->TaxData.TAXTime.Month;
						time.wDay = MainDlg->TaxData.TAXTime.Day;
						time.wHour = MainDlg->TaxData.TAXTime.Hour;
						time.wMinute = MainDlg->TaxData.TAXTime.Minute;
						time.wSecond = MainDlg->TaxData.TAXTime.Second;

						SetLocalTime(&time);
						NET_DVR_TIME NvrTime = {0};

						NvrTime.dwYear = time.wYear;
						NvrTime.dwMonth = time.wMonth;
						NvrTime.dwDay = time.wDay;

						NvrTime.dwHour = time.wHour;
						NvrTime.dwMinute = time.wMinute;
						NvrTime.dwSecond = time.wSecond;

						for (int i = 0; i < 16; i++)
						{
							if (MainDlg->lUserID[i] >= 0)
							{
								NET_DVR_SetDVRConfig(MainDlg->lUserID[i], NET_DVR_SET_TIMECFG, 0, &NvrTime, sizeof(NvrTime));
							}
						}
					}
				}
			}
		}
        Sleep(100);
	}
	return 0;
}

int TaxCOMInit(char* COM, char Parity) {
	DCB Dcb;
	COMMTIMEOUTS CommTimeouts;

	tPort = CreateFile(COM,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (tPort == INVALID_HANDLE_VALUE)
	{
		return -1;
	}


	GetCommState(tPort, &Dcb);
	Dcb.BaudRate = 28800;
	Dcb.ByteSize = 8;

	Dcb.Parity = Parity;
	Dcb.StopBits = ONESTOPBIT;
	SetCommState(tPort, &Dcb);

	GetCommTimeouts(tPort, &CommTimeouts);
	CommTimeouts.ReadIntervalTimeout = 10;
	CommTimeouts.ReadTotalTimeoutMultiplier = 10;
	CommTimeouts.ReadTotalTimeoutConstant = 10;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(tPort, &CommTimeouts);

	return 0;
}

int TaxNetInit(char* ip)
{
	if ((TaxSoc = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		TRACE("JWD socket error!\n");
		return -1;
	}

	sockaddr_in Addr;
	memset(&Addr, 0, sizeof(Addr));
	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(10010);
	Addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(TaxSoc, (struct sockaddr*)&Addr, sizeof(sockaddr)) < 0)
		return -1;

	memset(&TaxAddr, 0, sizeof(TaxAddr));
	TaxAddr.sin_family = AF_INET;
	TaxAddr.sin_port = htons(10010);
	InetPton(AF_INET, ip, &TaxAddr.sin_addr.s_addr);

	return 0;
}


// 从szBuf解析 TaxData 数据
int AnalyseTax(UCHAR* szBuf, TAXDATA* TaxData)
{
	UCHAR Tax40_Check, Tax32_Check, Tax72_Check;
	int i, k, m;
	i = 0, m = 0;
	Tax40_Check = 0;
	for (i = 0; i < 184; i++)
	{
		if ((DataBuf[i] == 0x38) && (DataBuf[i + 32] == 0x39))//72字节
		{
			Tax32_Check = 0;
			k = 0;
			for (k = 0; k < 31; k++)
				Tax32_Check = Tax32_Check + DataBuf[k + i];
			Tax32_Check = ~Tax32_Check + 1;
			Tax72_Check = 0;
			k = 0;
			for (k = 0; k < 39; k++)
				Tax72_Check = Tax72_Check + DataBuf[k + i + 32];
			Tax72_Check = ~Tax72_Check + 1;
			if ((Tax32_Check == DataBuf[i + 31]) && (Tax72_Check == DataBuf[i + 71]))
			{
				//实际交路号
				TaxData->FactRoute = DataBuf[15 + i];
				//阀缸压力
				TaxData->ValvePressure = DataBuf[17 + i] * 256 + DataBuf[16 + i];
				//柴油机转速、原边电流。
				TaxData->DieselSpeedAndOriginalCurrent = DataBuf[i + 20] * 256 + DataBuf[i + 19];
				//车次 
				TaxData->TrainNum = DataBuf[30 + i] * 65536 + DataBuf[29 + i] * 256 + DataBuf[28 + i];

                // 车次种类标识符
                memcpy_s(TaxData->TrainTypeId, 4,
                    &DataBuf[i + 6], 4);

               	//运行的时间
				TaxData->TAXTime.Second = DataBuf[35 + i] & 0x3f;
				TaxData->TAXTime.Minute = (DataBuf[36 + i] & 0x0f) * 4 + ((DataBuf[35 + i] & 0xc0) >> 6);
				TaxData->TAXTime.Hour = (DataBuf[37 + i] & 0x01) * 16 + ((DataBuf[36 + i] & 0xf0) >> 4);
				TaxData->TAXTime.Day = (DataBuf[37 + i] & 0x3e) / 2;
				TaxData->TAXTime.Month = (DataBuf[38 + i] & 0x03) * 4 + ((DataBuf[37 + i] & 0xc0) >> 6);
				TaxData->TAXTime.Year = ((DataBuf[38 + i] & 0xfc) >> 2) + 2000;
				//速度、限速
				TaxData->Speed = (DataBuf[40 + i] & 0x02) * 256 + DataBuf[39 + i];
				TaxData->MaxSpeed = (DataBuf[41 + i] & 0x0f) * 64 + (DataBuf[40 + i] >> 2);

				//机车信号	
				TaxData->TrainSign = DataBuf[42 + i];
				//机车工况
				TaxData->TrainState = DataBuf[43 + i];

				//信号机编号
				TaxData->SignNo = DataBuf[45 + i] * 256 + DataBuf[44 + i];
				//信号机种类
				TaxData->SignType = DataBuf[46 + i] & 0x07;
				//公里标
				TaxData->Signpost = (DataBuf[49 + i] & 0x003F) * 65536 + DataBuf[48 + i] * 256 + DataBuf[47 + i];
				//总重
				TaxData->CarWeight = DataBuf[51 + i] * 256 + DataBuf[50 + i];
				//计长
				TaxData->CarLong = DataBuf[53 + i] * 256 + DataBuf[52 + i];
				//辆数
				TaxData->CarCount = DataBuf[54 + i];
				//客/货、本/补 标志
				TaxData->TrainFlag = DataBuf[55 + i];
				//区段号
				TaxData->SectionNo = DataBuf[58 + i];
				//车站号
				TaxData->StationNo = DataBuf[59 + i];
				//司机号 
				TaxData->DriverNo = DataBuf[10 + i] * 65536 + DataBuf[61 + i] * 256 + DataBuf[60 + i];
				//副司机号 
				TaxData->CopilotNo = DataBuf[11 + i] * 65536 + DataBuf[63 + i] * 256 + DataBuf[62 + i];
				//机车号
				TaxData->EngineNo = DataBuf[65 + i] * 256 + DataBuf[64 + i];
				//机车型号
				TaxData->EngineType = (DataBuf[14 + i] & 0x3E) * 256 + DataBuf[66 + i];
				//设备位置 80 A节 40 B节
				TaxData->DevicePlace = DataBuf[14 + i] & 0xC0;
				//管压
				TaxData->PipePressure = (DataBuf[68 + i] & 0x03) * 256 + DataBuf[67 + i];
				memset(TaxData->TAXDataBuf, 0, sizeof(TaxData->TAXDataBuf));
				for (m = 0; m < 72; m++)
				{
					TaxData->TAXDataBuf[m] = DataBuf[i + m];
				}
				return 72;
			}
		}
	}
	i = 0;
	for (i = 0; i < 184; i++)
	{
		if (DataBuf[i] == 0x39)//40字节
		{

			Tax40_Check = 0;
			k = 0;
			for (k = 0; k < 39; k++)
				Tax40_Check = Tax40_Check + DataBuf[k + i];
			Tax40_Check = ~Tax40_Check + 1;
			if (Tax40_Check == DataBuf[i + 39])
			{
				//运行的时间
				TaxData->TAXTime.Second = DataBuf[i + 3] & 0x3f;
				TaxData->TAXTime.Minute = (DataBuf[i + 4] & 0x0f) * 4 + ((DataBuf[i + 3] & 0xc0) >> 6);
				TaxData->TAXTime.Hour = (DataBuf[i + 5] & 0x01) * 16 + ((DataBuf[i + 4] & 0xf0) >> 4);
				TaxData->TAXTime.Day = (DataBuf[i + 5] & 0x3e) / 2;
				TaxData->TAXTime.Month = (DataBuf[i + 6] & 0x03) * 4 + ((DataBuf[i + 5] & 0xc0) >> 6);
				TaxData->TAXTime.Year = ((DataBuf[i + 6] & 0xfc) >> 2) + 2000;
				//速度、限速
				TaxData->Speed = (DataBuf[i + 8] & 0x03) * 256 + DataBuf[i + 7];
				TaxData->MaxSpeed = (DataBuf[i + 9] & 0x0f) * 64 + (DataBuf[i + 8] >> 2);
				//信号机编号
				TaxData->SignNo = DataBuf[i + 13] * 256 + DataBuf[i + 12];
				//信号机种类
				TaxData->SignType = DataBuf[i + 14] & 0x07;
				//公里标
				TaxData->Signpost = DataBuf[i + 17] * 65536 + DataBuf[i + 16] * 256 + DataBuf[i + 15];
				//总重
				TaxData->CarWeight = DataBuf[i + 19] * 256 + DataBuf[i + 18];
				//计长
				TaxData->CarLong = DataBuf[i + 21] * 256 + DataBuf[i + 20];
				//辆数
				TaxData->CarCount = DataBuf[i + 22];
				//客/货、本/补 标志
				TaxData->TrainFlag = DataBuf[i + 23];
				//车次
				TaxData->TrainNum = ((DataBuf[i + 23] & 0x40) >> 6) * 65536 + DataBuf[i + 25] * 256 + DataBuf[i + 24];
				//区段号
				TaxData->SectionNo = DataBuf[i + 26];
				//车站号
				TaxData->StationNo = DataBuf[i + 27];
				//司机号
				TaxData->DriverNo = DataBuf[i + 28] * 256 + DataBuf[i + 29];
				//副司机号
				TaxData->CopilotNo = DataBuf[i + 30] * 256 + DataBuf[i + 31];
				//机车号
				TaxData->EngineNo = DataBuf[i + 33] * 256 + DataBuf[i + 32];
				//机车型号
				TaxData->EngineType = DataBuf[i + 34];
				//管压
				TaxData->PipePressure = (DataBuf[i + 36] & 0x03) * 256 + DataBuf[i + 35];
				memset(TaxData->TAXDataBuf, 0, sizeof(TaxData->TAXDataBuf));
				for (m = 0; m < 40; m++)
				{
					TaxData->TAXDataBuf[m] = DataBuf[i + m];
				}
				return 40;
			}
		}
	}
	return 0;
}


int PweCOMInit(char* COM, char Parity) {
	DCB Dcb;
	COMMTIMEOUTS CommTimeouts;

	pPort = CreateFile(COM,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (pPort == INVALID_HANDLE_VALUE)
	{
		TRACE("open PweCom fail ################");
		return -1;
	}

	GetCommState(pPort, &Dcb);
	Dcb.BaudRate = 9600;
	Dcb.ByteSize = 8;

	Dcb.Parity = Parity;
	Dcb.StopBits = ONESTOPBIT;
	SetCommState(pPort, &Dcb);

	GetCommTimeouts(pPort, &CommTimeouts);
	CommTimeouts.ReadIntervalTimeout = 10;
	CommTimeouts.ReadTotalTimeoutMultiplier = 10;
	CommTimeouts.ReadTotalTimeoutConstant = 10;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(pPort, &CommTimeouts);

	return 0;
}


// 智能电源
// 如果不发送该报文，系统会断电重启
UINT Thread_PweData(LPVOID lParam) {

	CLT_LCWB_1ADlg* MainDlg = (CLT_LCWB_1ADlg*)lParam;

	unsigned char SendBuf[3] = "";
	SendBuf[0] = 0x55;
	SendBuf[1] = 0x88;
	SendBuf[2] = 0x99;

	unsigned char num = 0;
	while (1)
	{
		DWORD dwRet;
		WriteFile(pPort, SendBuf, 3, &dwRet, NULL);

		Sleep(5000);
	}

	return 0;
}
