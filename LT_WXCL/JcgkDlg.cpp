// JcgkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "JcgkDlg.h"
#include "afxdialogex.h"
#include <string.h>

// CJcgkDlg �Ի���

IMPLEMENT_DYNAMIC(CJcgkDlg, CDialogEx)

CJcgkDlg::CJcgkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJcgkDlg::IDD, pParent)
{
	CurrentPage = 0;
	memset(buttonFlagTemp, -1, sizeof(buttonFlagTemp));
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			CH[i][j] = 0.0;
		}
	}

	memset(MeterValue, 0, sizeof(MeterValue));
	MeterShowFlag = 0;
}

CJcgkDlg::~CJcgkDlg()
{
}

void CJcgkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROGRESS1, m_pro1);
	DDX_Control(pDX, IDC_PROGRESS2, m_pro2);
	DDX_Control(pDX, IDC_PROGRESS3, m_pro3);
	DDX_Control(pDX, IDC_PROGRESS4, m_pro4);
	DDX_Control(pDX, IDC_PROGRESS5, m_pro5);
	DDX_Control(pDX, IDC_PROGRESS6, m_pro6);
	DDX_Control(pDX, IDC_PROGRESS7, m_pro7);
	DDX_Control(pDX, IDC_PROGRESS8, m_pro8);

	DDX_Control(pDX, IDC_BCA, m_button[0]);
	DDX_Control(pDX, IDC_BCB, m_button[1]);
	DDX_Control(pDX, IDC_TCA, m_button[2]);
	DDX_Control(pDX, IDC_TCB, m_button[3]);
	DDX_Control(pDX, IDC_BUTTON1, m_button[4]);
	DDX_Control(pDX, IDC_STATIC_JCXX, m_static_Train);
	// 	DDX_Control(pDX, IDC_BCA, m_button_BCA);
	// 	DDX_Control(pDX, IDC_BCB, m_button_BCB);
	// 	DDX_Control(pDX, IDC_TCA, m_button_TCA);
	// 	DDX_Control(pDX, IDC_TCB, m_button_TCB);
}


BEGIN_MESSAGE_MAP(CJcgkDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BCA, &CJcgkDlg::OnBnClickedBca)
	ON_BN_CLICKED(IDC_BCB, &CJcgkDlg::OnBnClickedBcb)
	ON_BN_CLICKED(IDC_TCA, &CJcgkDlg::OnBnClickedTca)
	ON_BN_CLICKED(IDC_TCB, &CJcgkDlg::OnBnClickedTcb)
	ON_BN_CLICKED(IDC_BUTTON1, &CJcgkDlg::OnBnClickedButton1)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CJcgkDlg ��Ϣ�������

UINT Thread_CJMKData(LPVOID lParam)
{
	CJcgkDlg* dlg = (CJcgkDlg*)lParam;

	struct sockaddr_in BAddr;
	memset(&BAddr, 0, sizeof(struct sockaddr_in));
	BAddr.sin_family = AF_INET;
	BAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);//�׽��ֵ�ַΪ�㲥��ַ
	BAddr.sin_port = htons(8000);//�׽��ֹ㲥�˿ں�Ϊ8000

	Sleep(5 * 1000);
	DWORD dwRet;
	unsigned char RecBuf[128] = "";

	unsigned char SendCJMKBuf[80] = "";

	while (1)
	{
		//���ڷ���
		dlg->CJMK_SendMsg(0);//״̬
		memset(RecBuf, 0, sizeof(RecBuf));

		//���ڽ���
		ReadFile(dlg->CPort, RecBuf, 100, &dwRet, NULL);

		if (dwRet > 0)
		{
			memset(SendCJMKBuf, 0, sizeof(SendCJMKBuf));
			memcpy(&SendCJMKBuf[5], RecBuf, dwRet);
			// 			if (dlg->CurrentPage == (theApp.Local[1]=='A'?0:1))
			// 			{
			// 				dlg->CJMK_MsgDec(RecBuf,dwRet,0);
			// 			}			
						//UDP�㲥
			SendCJMKBuf[0] = 0xFF;
			SendCJMKBuf[1] = 0x02;
			SendCJMKBuf[2] = theApp.Local[0];
			SendCJMKBuf[3] = theApp.Local[1];
			SendCJMKBuf[4] = 0;

			if (theApp.BSoc != -1)
			{
				sendto(theApp.BSoc, (char*)SendCJMKBuf, 50, 0, (sockaddr*)&BAddr, sizeof(BAddr));
			}
		}
		Sleep(500);
#if 0
		/////////////////////////////////////////////////////////////////////////////////////
		//���ڷ���
		dlg->CJMK_SendMsg(1);//�Ǳ�	

		memset(SendCJMKBuf, 0, sizeof(SendCJMKBuf));
		//���ڽ���
		ReadFile(dlg->CPort, RecBuf, 100, &dwRet, NULL);

		if (dwRet > 0)
		{
			memset(SendCJMKBuf, 0, sizeof(SendCJMKBuf));
			memcpy(&SendCJMKBuf[5], RecBuf, dwRet);
			// 			if (dlg->CurrentPage == (theApp.Local[1]=='A'?0:1))
			// 			{
			// 				dlg->CJMK_MsgDec(RecBuf,dwRet,1);
			// 			}
						//UDP�㲥
			SendCJMKBuf[0] = 0xFF;
			SendCJMKBuf[1] = 0x02;
			SendCJMKBuf[2] = theApp.Local[0];
			SendCJMKBuf[3] = theApp.Local[1];
			SendCJMKBuf[4] = 1;

			// 			for(int i=8;i<35;i=i+2)
			// 			{
			// 				SendCJMKBuf[i] =rand()%30;
			// 			}
			// 			SendCJMKBuf[33] =rand()%30;


			if (theApp.BSoc != -1)
			{
				sendto(theApp.BSoc, (char*)SendCJMKBuf, 50, 0, (sockaddr*)&BAddr, sizeof(BAddr));
			}
		}
#endif
		Sleep(500);
	}

	return 0;
}

int CJcgkDlg::CJMK_DecFun(unsigned int data, float VMax, float Max)
{
	//������
	//��ǰ��ѹ			data/1000	(data=��ѹ*1000)
	//��ѹ���ֵ		VMax
	//��״ͼ���ֵ		Max	
	//������״ͼ��ǰֵ	current
	if (data / 1000 > VMax)
	{
		//MessageBox("��ѹ�������Χ������");
		return (int)Max;
	}
	if (data > 0)
	{
		float current = (float)data / 1000;
		current = current / VMax * Max;
		return (int)current;
	}


	return 0;
}

//Э�����
int CJcgkDlg::CJMK_MsgDec(unsigned char* buf, int len, int Train, char Flag)
{
	if (Flag == 0)
	{
		if (buf[0] == 0xFF && buf[1] == 0x01)
		{
			lastDataTime[Train * 2 + Flag] = 0;	//��ʱ������


			SetButtonWar(8, !((buf[28] & 0x80) >> 7));
			SetButtonWar(7, !((buf[28] & 0x40) >> 6));
			SetButtonWar(6, !((buf[28] & 0x20) >> 5));
			SetButtonWar(5, !((buf[28] & 0x10) >> 4));
			SetButtonWar(4, !((buf[28] & 0x08) >> 3));
			SetButtonWar(3, !((buf[28] & 0x04) >> 2));
			SetButtonWar(2, !((buf[28] & 0x02) >> 1));
			SetButtonWar(1, !(buf[28] & 0x01));

			SetButtonWar(13, !((buf[29] & 0x10) >> 4));
			SetButtonWar(12, !((buf[29] & 0x08) >> 3));
			SetButtonWar(11, !((buf[29] & 0x04) >> 2));
			SetButtonWar(10, !((buf[29] & 0x02) >> 1));
			SetButtonWar(9, !(buf[29] & 0x01));

			unsigned int MSB;
			for (int i = 0; i < 8; i++)
			{
				int j = 4 + 2 * i;
				MSB = buf[j + 1] << 8 | buf[j];
				if (i == 7)
				{
					SetProValue(1, CJMK_DecFun(MSB, CH[0][0], CH[0][1]));
					MeterValue[0] = CJMK_DecFun(MSB, CH[i][0], CH[i][1]);
					//ˢ�±���������
					if (MeterShowFlag == 1)
					{
						//Invalidate(TRUE);
						CRect rect1(100, 30, 650, 450);
						InvalidateRect(rect1);
					}
				}
				else
				{
					SetProValue(i + 2, CJMK_DecFun(MSB, CH[i][0], CH[i][1]));
				}

			}

			// 			MSB=buf[5]<<8 & buf[6];
			// 			SetProValue(1,CJMK_DecFun(MSB,CH[0][0],CH[0][1])); 
			// 			MeterValue[0] = CJMK_DecFun(MSB,CH[0][0],CH[0][1]);
			// 
			// 			MSB=buf[7]<<8 & buf[8];
			// 			SetProValue(2,CJMK_DecFun(MSB,CH[1][0],CH[1][1]));
			// 
			// 			MSB=buf[9]<<8 & buf[10];
			// 			SetProValue(3,CJMK_DecFun(MSB,CH[2][0],CH[2][1]));
			// 
			// 			MSB=buf[11]<<8 & buf[12];
			// 			SetProValue(4,CJMK_DecFun(MSB,CH[3][0],CH[3][1])); 
			// 
			// 			MSB=buf[13]<<8 & buf[14];
			// 			SetProValue(5,CJMK_DecFun(MSB,CH[4][0],CH[4][1]));
			// 
			// 			MSB=buf[15]<<8 & buf[16];
			// 			SetProValue(6,CJMK_DecFun(MSB,CH[5][0],CH[5][1])); 
			// 
			// 			MSB=buf[17]<<8 & buf[18];
			// 			SetProValue(7,CJMK_DecFun(MSB,CH[6][0],CH[6][1])); 
			// 
			// 			MSB=buf[19]<<8 & buf[20];
			// 			SetProValue(8,CJMK_DecFun(MSB,CH[7][0],CH[7][1]));

			return 0;
		}
	}
	else if (Flag == 1)
	{
		if (buf[0] == 0xFF && buf[1] == 0x01)
		{
			lastDataTime[Train * 2 + Flag] = 0;//��ʱ������

			unsigned int MSB;

			MSB = buf[5] << 8 | buf[6];
			CJMK_MeterSet(1, CJMK_DecFun(MSB, CH[8][0], CH[8][1]));

			MSB = buf[7] << 8 | buf[8];
			CJMK_MeterSet(2, CJMK_DecFun(MSB, CH[9][0], CH[9][1]));

			MSB = buf[9] << 8 | buf[10];
			CJMK_MeterSet(3, CJMK_DecFun(MSB, CH[10][0], CH[10][1]));

			//ˢ�±���������
			if (MeterShowFlag == 1)
			{
				//Invalidate(TRUE);
				CRect rect1(100, 30, 650, 450);
				InvalidateRect(rect1);
			}

			return 0;
		}
	}
	return -1;
}

// int WINAPI Thread_CJMKUdpRec(LPVOID lpPara)
// {
// 	CJcgkDlg* dlg = (CJcgkDlg*) lpPara;
// 
// 	struct sockaddr_in BAddr;
// 	memset(&BAddr,0,sizeof(struct sockaddr_in));
// 	BAddr.sin_family=AF_INET;
// 	BAddr.sin_addr.s_addr=htonl(INADDR_BROADCAST);//�׽��ֵ�ַΪ�㲥��ַ
// 	BAddr.sin_port=htons(8000);//�׽��ֹ㲥�˿ں�Ϊ8000
// 
// 	struct sockaddr_in addr;
// 	int addrLen = sizeof(addr);
// 	unsigned char RecBuf[256] = "";
// 	unsigned char SendCJMKBuf[256] = "";
// 
// // 	while(1)
// // 	{
// // 		int res = recvfrom(dlg->CJMKRecUdpSoc,(char*)RecBuf,sizeof(RecBuf),0,(struct sockaddr *)&addr, &addrLen);
// // 		if (res > 0)
// // 		{
// // 			if (RecBuf[0] == 0x4D && RecBuf[1] == 0x41)
// // 			{
// // 				SendCJMKBuf[0] = 0xFF;
// // 				SendCJMKBuf[1] = 0x04;
// // 				SendCJMKBuf[2] = theApp.Local[0];
// // 				SendCJMKBuf[3] = theApp.Local[1];
// // 
// // 				memcpy(&SendCJMKBuf[4],RecBuf,res);
// // 
// // 				if (theApp.BSoc != -1)		
// // 					sendto(theApp.BSoc,(char*)SendCJMKBuf,sizeof(SendCJMKBuf),0,(sockaddr*)&BAddr,sizeof(BAddr));
// // 
// // 				memset(SendCJMKBuf,0,sizeof(SendCJMKBuf));
// // 			}
// // 		}
// // 
// // 		memset(RecBuf,0,sizeof(RecBuf));
// // 	}
// 
// 	return 0;
// }

int CJcgkDlg::CJMK_MeterSet(int No, unsigned int num)
{
	MeterValue[No] = num;
	// 	if (MeterShowFlag == 1)
	// 	{
	// 		Invalidate(TRUE);
	// 
	// 	}

	// 	USHORT max = 0xFFFF;
	// 	//USHORT p = 0xc0 << 8 | 0xa7;
	// 
	// 	//float a = (float)(20*p-10*max)/max;
	// 
	// 		for (int i = 2;i < 5;i++)
	// 		{
	// 			USHORT data = buf[i*2+22] << 8 | buf[i*2+23];
	// 			//float V = (float)(20*data-10*max)/max;
	// 			int temp = (4000*data-2000*max)/max;
	// 
	// 			if (temp > 1000)
	// 			{
	// 				MeterValue[i-1] = 1000;
	// 			}
	// 			else if (temp < 0)
	// 			{
	// 				MeterValue[i-1] = 0;
	// 			}
	// 			else
	// 				MeterValue[i-1] = temp;
	// 			
	// 		}
	// 
	// 		if (MeterShowFlag == 1)
	// 		{
	// 			Invalidate(TRUE);
	// 		}
	// 		
	// 	}
	// 	else
	// 		return -1;
	return 0;
}

BOOL CJcgkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	newFont1.CreatePointFont(170, "����");
	for (int i = 0; i < 5; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
		m_button[i].SetForeColor(RGB(255, 255, 255));
		m_button[i].SetTextFont(170, "����");
	}
	m_static_Train.SetFont(&newFont1);


	SetButton();

	m_brush.CreateSolidBrush(RGB(0, 0, 0));


	//��������ɫ����

	m_pro1.SetBkColor(RGB(0, 0, 0));
	m_pro2.SetBkColor(RGB(0, 0, 0));
	m_pro3.SetBkColor(RGB(0, 0, 0));
	m_pro4.SetBkColor(RGB(0, 0, 0));
	m_pro5.SetBkColor(RGB(0, 0, 0));
	m_pro6.SetBkColor(RGB(0, 0, 0));
	m_pro7.SetBkColor(RGB(0, 0, 0));
	m_pro8.SetBkColor(RGB(0, 0, 0));

	COLORREF clor(RGB(0, 0, 170));
	m_pro1.SetBarColor(clor);
	m_pro2.SetBarColor(clor);
	m_pro3.SetBarColor(clor);
	m_pro4.SetBarColor(clor);
	m_pro5.SetBarColor(clor);
	m_pro6.SetBarColor(clor);
	m_pro7.SetBarColor(clor);
	m_pro8.SetBarColor(clor);

	//////////////////////////////////////////////////////////////////////////
	//�������ļ��еı��̲���CH
	char ch[60] = "";
	char tmp[10] = "";
	for (int i = 0; i < 11; i++)
	{
		sprintf_s(tmp, "CH%d", i + 1);
		GetPrivateProfileString("LT_WXCLCFG", tmp, "", ch, 60, ".//LT_WXCLCFG.ini");
		char* chTmp = ch;

		if (strcmp(ch, ""))
		{
			for (int j = 0; j < 2; j++)
			{
				CH[i][j] = strtod(chTmp, NULL);
				chTmp = strchr(chTmp, ',') + 1;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//�������ļ��е�׳׳ͼ����
	for (int i = 0; i < 8; i++)
	{
		sprintf_s(tmp, "itemName%d", i + 1);
		GetPrivateProfileString("LT_WXCLCFG", tmp, "", ch, 60, ".//LT_WXCLCFG.ini");
		itemName[i].Format("%s", ch);
		sprintf_s(tmp, "itemUnit%d", i + 1);
		GetPrivateProfileString("LT_WXCLCFG", tmp, "", ch, 60, ".//LT_WXCLCFG.ini");
		itemUnit[i].Format("%s", ch);
	}

	//////////////////////////////////////////////////////////////////////////
	//��״ͼ��ʼ��
	ProCtrlInit();


	//���ڳ�ʼ��
	char CJMKCom[20] = "";
	GetPrivateProfileString("LT_WXCLCFG", "CJMKCom", "COM1", CJMKCom, 20, ".//LT_WXCLCFG.ini");
	if (CJMKComInit(CJMKCom) != -1)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_CJMKData, this, 0, NULL);
	}

	//ShowZhuZT(FALSE);
	//���ʼ��
	MeterInit();

	GetDlgItem(IDC_BUTTON1)->ShowWindow(FALSE);
	//SetTimer(1,10000,NULL);

	for (int i = 0; i < 8; i++)
	{
		lastDataTime[i] = 0;
	}
	SetTimer(201, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



void CJcgkDlg::ProCtrlInit()
{
	int IDC_lable = 2500;

	CString str;
	//////////////////////////////////////////////////////////////////////////
	int num_x = 20;//ƫ����
	int num_y1 = 4;
	int l_x = num_x;
	int l_y = 80;
	m_pro1.MoveWindow(l_x + 55, l_y + 5 + num_y1, 40, 300);
	str.Format("%s\n(%s)", itemName[0], itemUnit[0]);
	lable[0][3].Create(str, WS_CHILD | SS_CENTER | WS_VISIBLE, CRect(l_x + 30, l_y - 35, l_x + 105, l_y), this, IDC_lable++);
	CRect rc(l_x + 5, l_y, 50 + l_x + 5, 20 + l_y);
	str.Format("%d-", (int)CH[0][1]);
	lable[0][0].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5, l_y, 50 + l_x + 5, 20 + l_y);
	str = "";
	str.Format("%d-", (int)CH[0][1] / 2);
	lable[0][1].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5, l_y, 50 + l_x + 5, 20 + l_y);
	lable[0][2].Create("0-", WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	lable[0][4].Create("0", WS_CHILD | SS_RIGHT | WS_VISIBLE, CRect(l_x + 10, l_y + 15, l_x + 90, l_y + 40), this);

	l_x = 90 + num_x;
	l_y = 80;
	m_pro2.MoveWindow(l_x + 55, l_y + 5 + num_y1, 40, 300);
	str.Format("%s\n(%s)", itemName[1], itemUnit[1]);
	lable[1][3].Create(str, WS_CHILD | SS_CENTER | WS_VISIBLE, CRect(l_x + 30, l_y - 35, l_x + 100, l_y), this, IDC_lable++);
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str.Format("%d-", (int)CH[1][1]);
	lable[1][0].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str = "";
	str.Format("%d-", (int)CH[1][1] / 2);
	lable[1][1].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	lable[1][2].Create("0-", WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	lable[1][4].Create("0", WS_CHILD | SS_RIGHT | WS_VISIBLE, CRect(l_x + 10, l_y + 15, l_x + 90, l_y + 40), this);

	l_x = 180 + num_x;
	l_y = 80;
	m_pro3.MoveWindow(l_x + 55, l_y + 5 + num_y1, 40, 300 + num_y1);
	str.Format("%s\n(%s)", itemName[2], itemUnit[2]);
	lable[2][3].Create(str, WS_CHILD | SS_CENTER | WS_VISIBLE, CRect(l_x + 30, l_y - 35, l_x + 100, l_y), this, IDC_lable++);
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str.Format("%d-", (int)CH[2][1]);
	lable[2][0].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str = "";
	str.Format("%d-", (int)CH[2][1] / 2);
	lable[2][1].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	lable[2][2].Create("0-", WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	lable[2][4].Create("0", WS_CHILD | SS_RIGHT | WS_VISIBLE, CRect(l_x + 10, l_y + 15, l_x + 90, l_y + 40), this);

	l_x = 270 + num_x;
	l_y = 80;
	m_pro4.MoveWindow(l_x + 55, l_y + 5 + num_y1, 40, 300);
	str.Format("%s\n(%s)", itemName[3], itemUnit[3]);
	lable[3][3].Create(str, WS_CHILD | SS_CENTER | WS_VISIBLE, CRect(l_x + 30, l_y - 35, l_x + 100, l_y), this, IDC_lable++);
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str.Format("%d-", (int)CH[3][1]);
	lable[3][0].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str = "";
	str.Format("%d-", (int)CH[3][1] / 2);
	lable[3][1].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	lable[3][2].Create("0-", WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	lable[3][4].Create("0", WS_CHILD | SS_RIGHT | WS_VISIBLE, CRect(l_x + 10, l_y + 15, l_x + 90, l_y + 40), this);

	l_x = 360 + num_x;
	l_y = 80;
	m_pro5.MoveWindow(l_x + 55, l_y + 5 + num_y1, 40, 300);
	str.Format("%s\n(%s)", itemName[4], itemUnit[4]);
	lable[4][3].Create(str, WS_CHILD | SS_CENTER | WS_VISIBLE, CRect(l_x + 30, l_y - 35, l_x + 115, l_y), this, IDC_lable++);
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str.Format("%d-", (int)CH[4][1]);
	lable[4][0].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str = "";
	str.Format("%d-", (int)CH[4][1] / 2);
	lable[4][1].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	lable[4][2].Create("0-", WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	lable[4][4].Create("0", WS_CHILD | SS_RIGHT | WS_VISIBLE, CRect(l_x + 10, l_y + 15, l_x + 90, l_y + 40), this);

	l_x = 450 + num_x;
	l_y = 80;
	m_pro6.MoveWindow(l_x + 55, l_y + 5 + num_y1, 40, 300);
	str.Format("%s\n(%s)", itemName[5], itemUnit[5]);
	lable[5][3].Create(str, WS_CHILD | SS_CENTER | WS_VISIBLE, CRect(l_x + 30, l_y - 35, l_x + 115, l_y), this, IDC_lable++);
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str.Format("%d-", (int)CH[5][1]);
	lable[5][0].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str = "";
	str.Format("%d-", (int)CH[5][1] / 2);
	lable[5][1].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	lable[5][2].Create("0-", WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	lable[5][4].Create("0", WS_CHILD | SS_RIGHT | WS_VISIBLE, CRect(l_x + 10, l_y + 15, l_x + 90, l_y + 40), this);

	l_x = 540 + num_x;
	l_y = 80;
	m_pro7.MoveWindow(l_x + 55, l_y + 5 + num_y1, 40, 300);
	str.Format("%s\n(%s)", itemName[6], itemUnit[6]);
	lable[6][3].Create(str, WS_CHILD | SS_CENTER | WS_VISIBLE, CRect(l_x + 30, l_y - 35, l_x + 115, l_y), this, IDC_lable++);
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str.Format("%d-", (int)CH[6][1]);
	lable[6][0].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str = "";
	str.Format("%d-", (int)CH[6][1] / 2);
	lable[6][1].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	lable[6][2].Create("0-", WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	lable[6][4].Create("0", WS_CHILD | SS_RIGHT | WS_VISIBLE, CRect(l_x + 10, l_y + 15, l_x + 90, l_y + 40), this);

	l_x = 630 + num_x;
	l_y = 80;
	m_pro8.MoveWindow(l_x + 55, l_y + 5 + num_y1, 40, 300);
	str.Format("%s\n(%s)", itemName[7], itemUnit[7]);
	lable[7][3].Create(str, WS_CHILD | SS_CENTER | WS_VISIBLE, CRect(l_x + 30, l_y - 35, l_x + 115, l_y), this, IDC_lable++);
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str.Format("%d-", (int)CH[7][1]);
	lable[7][0].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	str = "";
	str.Format("%d-", (int)CH[7][1] / 2);
	lable[7][1].Create(str, WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	l_y += 150;
	rc.SetRect(l_x + 5 + 10, l_y, 45 + l_x + 10, 20 + l_y);
	lable[7][2].Create("0-", WS_CHILD | SS_RIGHT | WS_VISIBLE, rc, this);
	lable[7][4].Create("0", WS_CHILD | SS_RIGHT | WS_VISIBLE, CRect(l_x + 10, l_y + 15, l_x + 90, l_y + 40), this);

	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			lable[i][j].SetFont(&newFont);
		}
	}

	m_pro1.SetRange(0, CH[0][1]);
	m_pro2.SetRange(0, CH[1][1]);
	m_pro3.SetRange(0, CH[2][1]);
	m_pro4.SetRange(0, CH[3][1]);
	m_pro5.SetRange(0, CH[4][1]);
	m_pro6.SetRange(0, CH[5][1]);
	m_pro7.SetRange(0, CH[6][1]);
	m_pro8.SetRange(0, CH[7][1]);


}


void CJcgkDlg::ShowZhuZT(bool flag)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			lable[i][j].ShowWindow(flag);
		}
	}

	m_pro1.ShowWindow(flag);
	m_pro2.ShowWindow(flag);
	m_pro3.ShowWindow(flag);
	m_pro4.ShowWindow(flag);
	m_pro5.ShowWindow(flag);
	m_pro6.ShowWindow(flag);
	m_pro7.ShowWindow(flag);
	m_pro8.ShowWindow(flag);
}

int CJcgkDlg::MeterInit()
{

#if 1
	m_Meter[0] = new MyMeter(215, 135, 100, 4);
	m_Meter[1] = new MyMeter(525, 135, 100, 4);
	m_Meter[2] = new MyMeter(215, 345, 100, 4);
	m_Meter[3] = new MyMeter(525, 345, 100, 4);
#else
	m_Meter[0] = new MyMeter(100, 220, 90, 4);
	m_Meter[1] = new MyMeter(300, 220, 90, 4);
	m_Meter[2] = new MyMeter(500, 220, 90, 4);
	m_Meter[3] = new MyMeter(700, 220, 90, 4);
#endif

	for (int i = 0; i < 4; i++)
	{
		m_Meter[i]->set_pen(1, 4, RGB(255, 255, 255));	//���� �������ɫ�ʹ�ϸ  ���š���ϸ����ɫ  0����1Բ��2С�̶�3��̶� 4ָ��5ָ���
		m_Meter[i]->set_pen(2, 1, RGB(255, 255, 255));
		m_Meter[i]->set_pen(3, 1, RGB(255, 255, 255));
		m_Meter[i]->set_numcolor(RGB(255, 255, 255));
		m_Meter[i]->set_wordsize(140);
		m_Meter[i]->set_wordcolor(RGB(255, 255, 0));					//���� �ֵ���ɫ
	}


	char ch[60] = "";
	char tmp[10] = "";
	for (int i = 0; i < 3; i++)
	{
		sprintf_s(tmp, "MT%d", i + 1);
		GetPrivateProfileString("LT_WXCLCFG", tmp, "", ch, 60, ".//LT_WXCLCFG.ini");
		m_Meter[i + 1]->set_item(ch, "", 0);
	}

	m_Meter[0]->set_item("���ƶ���", "", 0);

	/*for (int i = 0;i < 4;i++)
	{
		m_Meter[i]->set_range()
	}*/
	return 0;
}

void CJcgkDlg::SetButtonWar(char num, BOOL WFlag)
{
	BOOL changeFlag = FALSE;
	HICON hIcon;
	int size = 54;
	switch (num)
	{
	case 1:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDI_ICON1:IDI_ICON1)));
			int IcoTmp = (WFlag ? IDI_ICON1_w : IDI_ICON1);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}
		break;
	case 2:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B2W:IDB_B2)));
			int IcoTmp = (WFlag ? IDI_ICON2_w : IDI_ICON2);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}
		break;
	case 3:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B3W:IDB_B3)));
			int IcoTmp = (WFlag ? IDI_ICON3_w : IDI_ICON3);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 4:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B4W:IDB_B4)));
			int IcoTmp = (WFlag ? IDI_ICON4_w : IDI_ICON4);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 5:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B5W:IDB_B5)));
			int IcoTmp = (WFlag ? IDI_ICON5_w : IDI_ICON5);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 6:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B6W:IDB_B6)));
			int IcoTmp = (WFlag ? IDI_ICON6_w : IDI_ICON6);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 7:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B7W:IDB_B7)));
			int IcoTmp = (WFlag ? IDI_ICON7_w : IDI_ICON7);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 8:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B8W:IDB_B8)));
			int IcoTmp = (WFlag ? IDI_ICON8_w : IDI_ICON8);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}
		break;
	case 9:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B9W:IDB_B9)));
			int IcoTmp = (WFlag ? IDI_ICON9_w : IDI_ICON9);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 10:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B10W:IDB_B10)));
			int IcoTmp = (WFlag ? IDI_ICON10_w : IDI_ICON10);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 11:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B11W:IDB_B11)));
			int IcoTmp = (WFlag ? IDI_ICON11_w : IDI_ICON11);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 12:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B12W:IDB_B12)));
			int IcoTmp = (WFlag ? IDI_ICON12_w : IDI_ICON12);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	case 13:
		if (WFlag != buttonFlagTemp[num - 1])
		{
			//m = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE((WFlag?IDB_B13W:IDB_B13)));
			int IcoTmp = (WFlag ? IDI_ICON13_w : IDI_ICON13);
			hIcon = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IcoTmp), IMAGE_ICON, size, size, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

			buttonFlagTemp[num - 1] = WFlag;
			changeFlag = TRUE;
		}

		break;
	}
	if (changeFlag)
	{
		//button[num-1].SetBitmap(m);
		button[num - 1].SetIcon(hIcon);
	}

}



void CJcgkDlg::SetButton()
{

	for (int i = 0; i < 13; i++)
	{
		int x = 60 * i + 10;
		//button[i].Create("",WS_CHILD|BS_MULTILINE|WS_VISIBLE|BS_BITMAP,CRect(x,450,x+50,500),this,20000+i);//BS_ICON
		button[i].Create("", WS_CHILD | BS_MULTILINE | WS_VISIBLE | BS_ICON | BS_FLAT, CRect(x, 450, x + 50, 500), this, 20000 + i);//BS_ICON
		//button[i].EnableWindow(FALSE);



		SetButtonWar(i + 1, FALSE);
	}

	//����A,B,����A,B,��ҳ��ť�飬ԭ������X��Y����ȣ��߶�,��ť���
	int ox = 45, oy = 510, width = 120, height = 45, space = 20;

	GetDlgItem(IDC_BCA)->MoveWindow(ox, oy, width, height);
	GetDlgItem(IDC_BCB)->MoveWindow(ox + width + space, oy, width, height);
	GetDlgItem(IDC_TCA)->MoveWindow(ox + 2 * width + 2 * space, oy, width, height);
	GetDlgItem(IDC_TCB)->MoveWindow(ox + 3 * width + 3 * space, oy, width, height);

	GetDlgItem(IDC_BUTTON1)->MoveWindow(ox + 4 * width + 5 * space, oy, width, height);

	GetDlgItem(IDC_STATIC_JCXX)->MoveWindow(310, 10, 150, 30);
}

void CJcgkDlg::SetProValue(char num, int value)
{
	switch (num)
	{
	case 1:
		m_pro1.SetPos(value);
		break;
	case 2:
		m_pro2.SetPos(value);
		break;
	case 3:
		m_pro3.SetPos(value);
		break;
	case 4:
		m_pro4.SetPos(value);
		break;
	case 5:
		m_pro5.SetPos(value);
		break;
	case 6:
		m_pro6.SetPos(value);
		break;
	case 7:
		m_pro7.SetPos(value);
		break;
	case 8:
		m_pro8.SetPos(value);
		break;
	default:
		break;
	}
	char temp[20] = "";
	sprintf_s(temp, "%d", value);
	lable[num - 1][4].SetWindowText(temp);
}

int CJcgkDlg::CJMKComInit(char* com)
{

	DCB Dcb;
	COMMTIMEOUTS CommTimeouts;

	//int TAX_TYPE;
	//DWORD dwRet;
	CPort = CreateFile(com,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (CPort == INVALID_HANDLE_VALUE)
	{
		return -1;
	}


	GetCommState(CPort, &Dcb);
	//Dcb.BaudRate =28800;
	Dcb.BaudRate = 38400;
	Dcb.ByteSize = 8;
	Dcb.Parity = NOPARITY;
	//Dcb.Parity =ODDPARITY;
	Dcb.StopBits = ONESTOPBIT;
	//Dcb.fOutxCtsFlow = FALSE;

	SetCommState(CPort, &Dcb);

	GetCommTimeouts(CPort, &CommTimeouts);
	CommTimeouts.ReadIntervalTimeout = 100;
	CommTimeouts.ReadTotalTimeoutMultiplier = 100;
	CommTimeouts.ReadTotalTimeoutConstant = 100;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(CPort, &CommTimeouts);

	return 0;

}

int CJcgkDlg::CJMK_SendMsg(char Flag)
{
	unsigned char buf[6] = "";
	buf[0] = 0xFF; ///��״̬
	buf[1] = 0x01;
	buf[2] = 0x06;
	buf[3] = 0x00;
	buf[4] = Flag;
	for (int i = 0; i < 5; i++)
	{
		buf[5] += buf[i];
	}
	if (CPort != 0)
	{
		DWORD dwRet;
		WriteFile(CPort, buf, 6, &dwRet, NULL);
		if (dwRet > 0)
		{
			return dwRet;
		}
	}

	return -1;
}



void CJcgkDlg::FlushAll()
{
	for (int i = 0; i < 8; i++)
	{
		SetProValue(i + 1, 0);
	}

	for (int i = 0; i < 13; i++)
	{
		SetButtonWar(i + 1, FALSE);
	}
}


void CJcgkDlg::OnBnClickedBca()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < 5; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[0].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC_JCXX, "��ǰ������A");
	CurrentPage = 0;
	FlushAll();
}


void CJcgkDlg::OnBnClickedBcb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < 5; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[1].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC_JCXX, "��ǰ������B");
	CurrentPage = 1;
	FlushAll();
}


void CJcgkDlg::OnBnClickedTca()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < 5; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[2].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC_JCXX, "��ǰ������A");
	CurrentPage = 2;
	FlushAll();
}


void CJcgkDlg::OnBnClickedTcb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < 5; i++)
	{
		m_button[i].SetBkColor(RGB(0, 0, 0));
	}
	m_button[3].SetBkColor(RGB(100, 200, 100));

	SetDlgItemText(IDC_STATIC_JCXX, "��ǰ������B");
	CurrentPage = 3;
	FlushAll();
}


//��ҳ��ť
void CJcgkDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_button[4].SetBkColor(RGB(100, 200, 100));

#if 0
	MeterValue[0] = 200;
	MeterValue[1] = 400;
	MeterValue[2] = 670;
	MeterValue[3] = 980;
	Invalidate(TRUE);
	//OnPaint();
#endif
	KillTimer(1);
	if (MeterShowFlag == 0)
	{
		MeterShowFlag = 1;
		ShowZhuZT(FALSE);
	}
	else if (MeterShowFlag == 1)
	{
		MeterShowFlag = 0;
		ShowZhuZT(TRUE);
	}
	//ˢ��ȫ��
	//Invalidate(TRUE);

	//ˢ�±���������
	CRect rect1(100, 30, 650, 450);
	InvalidateRect(rect1);


	SetTimer(1, 10000, NULL);
}


void CJcgkDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	// �����ڴ滺��DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bufferMemBitmap;
	CRect rect;
	GetClientRect(&rect);
	bufferMemBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.SelectObject(&bufferMemBitmap);
	CBrush whiteBrush;
	whiteBrush.CreateSolidBrush(RGB(0, 0, 0));
	memDC.FillRect(CRect(0, 0, rect.Width(), rect.Height()), &whiteBrush);

	// ʹ��memDC����ͼ����
	if (MeterShowFlag == 1)
	{
		//CDC *pCdc = &dc; 
		for (int i = 0; i < 4; i++)
		{
			m_Meter[i]->DrawMeter(&memDC, MeterValue[i]);
		}
	}


	// ���ڴ滺��DC��������ǰDC
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	bufferMemBitmap.DeleteObject();
	memDC.DeleteDC();


}


BOOL CJcgkDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//m_Meter[0]->DrawMeter(pDC,500);
	//return TRUE;

	//return CDialogEx::OnEraseBkgnd(pDC);
	return true;
	return CJcgkDlg::OnEraseBkgnd(pDC);
}

//������
int iii = 100;

void CJcgkDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	if (nIDEvent == 1)
	{
		OnBnClickedButton1();


	}
	else if (nIDEvent == 2)
	{
		//*******************************
		//����
		if (iii >= 1400)
		{
			iii = 100;
		}

		CJMK_MeterSet(1, iii);
		CJMK_MeterSet(2, iii);
		CJMK_MeterSet(3, iii);
		iii += 100;
		//ˢ�±���������
		if (MeterShowFlag == 1)
		{
			//Invalidate(TRUE);
			CRect rect1(100, 30, 650, 400);
			InvalidateRect(rect1);
		}
		//*******************************
	}
	else if (nIDEvent == 201)
	{
		for (int i = 0; i < 8; i++)
		{
			if (lastDataTime[i] > 3)
			{
				if (i % 2 == 0 && CurrentPage == i / 2)		//0ģ��
				{
					//��ť
					for (int j = 1; j < 14; j++)
					{
						SetButtonWar(j, 0);
					}

					//��״ͼ
					for (int j = 1; j <= 8; j++)
					{
						SetProValue(j, 0);
					}

					MeterValue[0] = 0;
					//ˢ�±���������
// 					if (MeterShowFlag == 1)
// 					{
// 						//Invalidate(TRUE);
// 						CRect rect1(100,450,650,650);
// 						InvalidateRect(rect1);
// 					}
				}
				else if (CurrentPage == i / 2)				//1ģ��
				{
					CJMK_MeterSet(1, 0);
					CJMK_MeterSet(2, 0);
					CJMK_MeterSet(3, 0);

					//ˢ�±���������
					if (MeterShowFlag == 1)
					{
						//Invalidate(TRUE);
						CRect rect1(100, 30, 650, 450);
						InvalidateRect(rect1);
					}
				}
				lastDataTime[i] = 0;
			}
			lastDataTime[i]++;
		}

	}


	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CJcgkDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_DLG)      //�Ի�����ɫ  

		return   m_brush;       //����ˢ��

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  
		hbr = (HBRUSH)m_brush;
	}

	for (int i = 0; i < 8; i++)
	{
		if (pWnd->GetDlgCtrlID() == 2500 + i)
		{
			pDC->SetTextColor(RGB(255, 255, 0));
			pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  
			hbr = (HBRUSH)m_brush;
		}
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
