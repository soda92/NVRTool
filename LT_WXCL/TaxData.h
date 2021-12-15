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
	TAXTIME  TAXTime;		//ʱ��
	unsigned char FactRoute;	//ʵ�ʽ�·��
	unsigned short ValvePressure;	//����ѹ��
	unsigned short DieselSpeedAndOriginalCurrent;	//���ͻ�ת�١�ԭ�ߵ�����
	unsigned short Speed;	//�ٶ�
	unsigned short MaxSpeed;//����
	unsigned char TrainSign;//�����ź�
	unsigned char TrainState;//��������
	unsigned int SignNo;	//�źŻ����
	unsigned char SignType;	//�źŻ�����
	unsigned int Signpost;	//�����
	unsigned short CarWeight;//����
	unsigned short CarLong;	//�Ƴ�
	unsigned char CarCount;	//����
	unsigned char TrainFlag;//�ͻ���������־
	unsigned int TrainNum;	//����
	unsigned char SectionNo;//���κ�
	unsigned char StationNo;//��վ��
	unsigned int DriverNo;	//˾����
	unsigned int CopilotNo;	//��˾����
	unsigned int EngineNo;	//������
	unsigned char EngineType;//�����ͺ�
	unsigned char DevicePlace;//�豸λ��
	unsigned short PipePressure;//��ѹ
	unsigned char TAXDataBuf[72];//TAX����

}TAXDATA, * pTAXDATA;

int TaxCOMInit(char* COM, char Parity);
UINT Thread_TaxData(LPVOID lParam);
UINT Thread_UdpTax(LPVOID lParam);

int TaxNetInit(char* ip);

//int TaxRead(HANDLE Port,UCHAR* Buf,int BufLen);
int AnalyseTax(UCHAR* szBuf, TAXDATA* TaxData);

int PweCOMInit(char* COM, char Parity);
UINT Thread_PweData(LPVOID lParam);