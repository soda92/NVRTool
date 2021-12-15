//TexData.h
#pragma once

typedef struct _TAXTIME {
	unsigned short Year;
	unsigned char Month;
	unsigned char Day;
	unsigned char Hour;
	unsigned char Minute;
	unsigned char Second;
}TAXTIME, * pTAXTIME;

typedef struct _TAXDATA {
	TAXTIME  TAXTime;		//时间
	unsigned char FactRoute;	//实际交路号
	unsigned short ValvePressure;	//阀缸压力
	unsigned short DieselSpeedAndOriginalCurrent;	//柴油机转速、原边电流。
	unsigned short Speed;	//速度
	unsigned short MaxSpeed;//限速
	unsigned char TrainSign;//机车信号
	unsigned char TrainState;//机车工况
	unsigned int SignNo;	//信号机编号
	unsigned char SignType;	//信号机种类
	unsigned int Signpost;	//公里标
	unsigned short CarWeight;//总重
	unsigned short CarLong;	//计长
	unsigned char CarCount;	//辆数
	unsigned char TrainFlag;//客货、本补标志
	unsigned int TrainNum;	//车次
	unsigned char SectionNo;//区段号
	unsigned char StationNo;//车站号
	unsigned int DriverNo;	//司机号
	unsigned int CopilotNo;	//副司机号
	unsigned int EngineNo;	//机车号
	unsigned char EngineType;//机车型号
	unsigned char DevicePlace;//设备位置
	unsigned short PipePressure;//管压
	unsigned char TAXDataBuf[72];//TAX数组

}TAXDATA, * pTAXDATA;

int TaxCOMInit(char* COM, char Parity);
UINT Thread_TaxData(LPVOID lParam);
UINT Thread_UdpTax(LPVOID lParam);

int TaxNetInit(char* ip);

//int TaxRead(HANDLE Port,UCHAR* Buf,int BufLen);
int AnalyseTax(UCHAR* szBuf, TAXDATA* TaxData);

int PweCOMInit(char* COM, char Parity);
UINT Thread_PweData(LPVOID lParam);