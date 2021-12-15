// RacingDlg.cpp : ʵ���ļ�
//
#include <iostream>
#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "RacingDlg.h"
#include "afxdialogex.h"
#include <list>
#include <string>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include "Date.h"
#include "FireData.h"
#include "RacingAnalysis.h"
#include "LT_LCWB_1ADlg.h"
#include  <io.h> //_access fun
#include <direct.h> //_mkdir fun

using namespace std;


//HANDLE fPort = 0;

// CRacingDlg �Ի���

IMPLEMENT_DYNAMIC(CRacingDlg, CDialogEx)

CRacingDlg::CRacingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRacingDlg::IDD, pParent)
{

}

CRacingDlg::~CRacingDlg()
{
}

void CRacingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_B, m_static_B);
	DDX_Control(pDX, IDC_STATIC_T, m_static_T);
}


BEGIN_MESSAGE_MAP(CRacingDlg, CDialogEx)

	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

CString staticText(int i)
{
	CString str;
	switch (i % 6)
	{
	case 0:
		return "��ת״̬ ��";
		break;
	case 1:
		return "��ת���� ��";
		break;
	case 2:
		return "�ֶ�1�ٶȣ�";
		break;
	case 3:
		return "�ֶ�2�ٶȣ�";
		break;
	case 4:
		return "�ֶ�3�ٶȣ�";
		break;
	case 5:
		return "�ֶ�4�ٶȣ�";
		break;
	default:
		return "����";
	}
}

void WINAPI Thread_FileManage(LPVOID lParam);


// CRacingDlg ��Ϣ�������

BOOL CRacingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_brush.CreateSolidBrush(RGB(0, 0, 0));


	//���ڳ�ʼ��
	char RacingCom[20] = "";
	GetPrivateProfileString("LT_WXCLCFG", "RacingCom", "COM1", RacingCom, 20, ".//LT_WXCLCFG.ini");
	if (LdleDataComInit(RacingCom) != -1)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_LdleDataAnalysis, this, 0, NULL);
	}


	//�������ļ��ж�ȡ�洢·��
	char FilePath[20] = "";
	GetPrivateProfileString("LT_WXCLCFG", "HDD", "e://", FilePath, 20, ".//LT_WXCLCFG.ini");
	strFilePath = CString(FilePath);

	char TrainNum[20];
	GetPrivateProfileString("LDFM-4E-VideoCFG", "TrainNum", "SS91456", TrainNum, 20, ".//LDFM-4E-VideoCFG.ini");
	strTrainNum = CString(TrainNum);

	//���������ļ�ά���߳�
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_FileManage, this, 0, NULL);

	//��ȡ��ת������Ϣ
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_LdleDataAnalysis, this, 0, NULL);
	//****************************************************************************************
	//��ǩ	
	//��̬�ı���㣬�����ߣ����¼��
	int num = 1200;


	int ox = 120, oy = 70, width = 160, height = 25, space = 8;
	int ox1 = ox + 350;		//B��ox
	int oy1 = oy + 80;		//����oy

	//����
	newFont.CreatePointFont(160, "����");


	for (int i = 0; i < 24; i++)
	{
		m_static_text[i] = new CStatic();
		CString str;
		str = staticText(i);
		if (i < 6)					//����A
		{
			CRect rect(ox, oy + i * (height + space), ox + width, oy + i * (space + height) + height);
			m_static_text[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_text[i]->SetFont(&newFont);
		}
		else if (i < 12)			//����B
		{
			CRect rect(ox1, oy + (i - 6) * (height + space), ox1 + width, oy + (i - 6) * (space + height) + height);
			m_static_text[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_text[i]->SetFont(&newFont);
		}
		else if (i < 18)			//����A
		{
			CRect rect(ox, oy1 + (i - 6) * (height + space), ox + width, oy1 + (i - 6) * (space + height) + height);
			m_static_text[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_text[i]->SetFont(&newFont);
		}
		else					//����B
		{
			CRect rect(ox1, oy1 + (i - 12) * (height + space), ox1 + width, oy1 + (i - 12) * (space + height) + height);
			m_static_text[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_text[i]->SetFont(&newFont);
		}
	}

	//****************************************************************************************
	//����
	//��̬�ı���㣬�����ߣ����¼��
	width = 140;
	ox = ox + 130;
	ox1 = ox + 350;
	num = 1224;
	static_value_handle1 = num;

	for (int i = 0; i < 24; i++)
	{
		m_static_value[i] = new CStatic();
		CString str;
		if (i % 6 == 0 || i % 6 == 1)
			str.Format("δ֪");
		else
			str.Format("-- km/h");

		if (i < 6)					//����A
		{
			CRect rect(ox, oy + i * (height + space), ox + width, oy + i * (space + height) + height);
			m_static_value[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_value[i]->SetFont(&newFont);
		}
		else if (i < 12)			//����B
		{
			CRect rect(ox1, oy + (i - 6) * (height + space), ox1 + width, oy + (i - 6) * (space + height) + height);
			m_static_value[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_value[i]->SetFont(&newFont);
		}
		else if (i < 18)			//����A
		{
			CRect rect(ox, oy1 + (i - 6) * (height + space), ox + width, oy1 + (i - 6) * (space + height) + height);
			m_static_value[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_value[i]->SetFont(&newFont);
		}
		else					//����B
		{
			CRect rect(ox1, oy1 + (i - 12) * (height + space), ox1 + width, oy1 + (i - 12) * (space + height) + height);
			m_static_value[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_value[i]->SetFont(&newFont);
		}
	}

	//********************************************************************************************


	//����
	newFont1.CreatePointFont(170, "����");

	m_static_B.SetFont(&newFont1);
	m_static_T.SetFont(&newFont1);


	for (int i = 0; i < 4; i++)
	{
		lastDataTime[i] = 0;
	}
	SetTimer(201, 1000, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



//�ж��ǲ�����ĸ
bool isLetter(char chr)
{
	if ((chr >= 97 && chr <= 122) || (chr >= 65 && chr <= 90))
	{
		return 1;
	}
	return 0;
}

//��ת�ַ���
string changeStr(string str)
{
	string str2;
	int sizeStr = str.length();
	//cout<<"�ַ�����="<<sizeStr<<endl;
	for (int i = 0; i < sizeStr; i++)
	{
		str2 += str[sizeStr - i - 1];
		//cout<<str[sizeStr-i-1]<<"->"<<str2<<endl;
	}


	return str2;
}

//��ǰ����ַ���
string findFront(ifstream* file)
{
	string str;
	bool is = 0; //��ĸ��־

	while (1)
	{
		char cur;
		file->seekg(-2, ios::cur);
		file->get(cur);

		if (isLetter(cur))
		{
			str += cur;
			is = 1;
			//cout<<"yes"<<endl;
		}
		else
		{
			if (is == 0)		//ǰһ���ַ�������ĸ
			{
				is = 0;
			}
			else			//ǰһ���ַ�����ĸ
			{
				//��ת�ַ���
				//cout<<"str="<<str<<endl;
				return changeStr(str);
			}
		}
		//cout<<cur<<endl;
	}

	return str;
}

//���Һ�����ַ���
string findBehind(ifstream* file)
{
	string str;
	bool is = 0; //��ĸ��־

	while (1)
	{
		char cur;
		file->get(cur);
		if (cur != ']')
		{
			str += cur;
			is = 1;
			//cout<<"yes"<<endl;
		}
		else
		{
			return str;
		}
		//cout<<cur<<endl;
	}

	return str;
}



//��ȡ�ض���ʽ���ļ���
void getAllFiles(string path, vector<string>& files, string format)
{
	intptr_t    hFile = 0;//�ļ����  64λ��long ��Ϊ intptr_t
	struct _finddata_t fileinfo;//�ļ���Ϣ 
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1) //�ļ�����
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//�ж��Ƿ�Ϊ�ļ���
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)//�ļ������в���"."��".."
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name)); //�����ļ�����
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, format); //�ݹ�����ļ���
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));//��������ļ��У������ļ���
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

//�жϸ��ļ�path�費��Ҫ����
bool CRacingDlg::isCopy(string path, string date, string days)
{
	//ת������
	string date1, date2, current;
	int n = 0;

	for (int i = 0; i < date.length(); i++)
	{
		if (date[i] != '-')
		{
			date1 += date[i];
		}
	}

	//cout<<date1<<endl;
	CDate cdate1(date1);

	//std::cout<<"��ʼ���ڣ�"<<cdate1.tostring()<<endl;
	OutputDebugStringA(cdate1.tostring().c_str());

	//������ת��Ϊint����
	for (int i = 0; i < days.length(); i++)
	{
		int num = days[i] - 48;
		n = n * 10 + num;
	}
	//std::cout<<"����n="<<n<<endl;



	//��ȡ��ֹ����
	CDate cdate2 = cdate1;
	cdate2.addDay(n);
	date2 = cdate2.tostring();
	//cout<<"��ֹ���ڣ�"<<date2<<endl;


	bool _is = 0;
	for (int i = path.length() - 1; i >= 0; i--)
	{

		if (_is == 1)
		{
			for (int j = 0; j < 8; j++)
			{
				current += path[i - 7 + j];
			}
			break;
		}
		if (path[i] == '_')
		{
			_is = 1;
		}
	}
	//cout<<"��ǰ�ļ���"<<path<<endl;
	//std::cout<<"��ǰ�ļ����ڣ�"<<current<<endl;


	//std::cout<<date1<<"<"<<current<<"<"<<date2<<endl;
	if (current.compare(date1) >= 0 && current.compare(date2) < 0)
	{
		//cout<<date1<<"<"<<current<<"<"<<date2<<endl;
		return 1;
	}
	//CString str;
	//str.Format("%s,%s,%s",cdate1.tostring().c_str(),current.c_str(),date2.c_str());
	//MessageBox(str);
	return 0;
}

bool isFolderExist(char* folder)
{
	int ret = 0;

	ret = _access(folder, 0);
	if (ret == 0)
		ret = true;
	else
		ret = false;

	return ret;
}

int createDirectory(char* directoryPath)
{
	int dirPathLen = 0;
	if (directoryPath != NULL) {
		dirPathLen = strlen(directoryPath);
	}
	if (dirPathLen > FILENAME_MAX)
	{
		return -1;
	}
	char tmpDirPath[FILENAME_MAX] = { 0 };
	for (int i = 0; i < dirPathLen; ++i)
	{
		tmpDirPath[i] = directoryPath[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			//cout << i<<endl;
			if (!isFolderExist(tmpDirPath))
			{
				int ret = _mkdir(tmpDirPath);
				//BOOL ret = CreateDirectory(tmpDirPath, NULL);
				if (ret != 0)
					return -1;
			}
		}
	}
	if (!isFolderExist(tmpDirPath))
	{
		int ret = _mkdir(tmpDirPath);
		//BOOL ret = CreateDirectory(tmpDirPath, NULL);
		if (ret != 0)
			return -1;
	}
	return 0;
}

//�����ļ�
int CRacingDlg::Copy_File(string date, string days, string Upath)
{
	int n = 0, fail = 0;
	//��ȡĿ¼������.skz�ļ�
	vector<string> files;
	getAllFiles(FILEPATH, files, ".skz");

	//cout<<"---------------------------------------------------\n�����ļ���"<<endl;

	vector<string>::iterator iter;
	for (iter = files.begin(); iter != files.end(); iter++)
	{
		string temp = *iter;
		//cout<<temp<<endl;
	}
	//cout<<"---------------------------------------------------"<<endl;

	//����LSKZ�ļ���
	string dirName;
	dirName += Upath[0];
	dirName += ":/LSKZ";
	createDirectory(const_cast<char*>(dirName.c_str()));


	//�����ļ�
	//vector<string>::iterator iter;
	for (iter = files.begin(); iter != files.end(); iter++)
	{
		string temp = *iter;
		string strUpath = Upath;
		if (isCopy(temp, date, days))
		{
			//����U��·��
			for (int i = temp.length() - 1; i >= 0; i--)
			{
				if (temp[i] == '\\' || temp[i] == '/')
				{
					for (int j = i + 1; j < temp.length(); j++)
					{
						strUpath += temp[j];
					}
					//cout<<"Upath="<<strUpath<<endl;
					break;
				}
			}

			//�����ļ�
			if (CopyFileA(temp.c_str(), strUpath.c_str(), 0))
			{
				//cout<<"����"<<temp<<"  ->  "<<strUpath.c_str()<<"�ɹ���"<<endl;
				//cout<<"---------------------------------------------------"<<endl;
			}
			else
			{
				fail++;
				//cout<<"����"<<temp<<"  ->  "<<strUpath.c_str()<<"ʧ�ܣ�"<<endl;
				//cout<<"---------------------------------------------------"<<endl;
			}
			n++;
		}
	}

	n = n * 1000 + fail;
	return n;

}

bool CRacingDlg::copyToU()
{

	HANDLE h1;//�����������
	h1 = CreateThread(NULL, 0, thread_copyDataToU, this, 0, NULL);//�����߳�1
	//MessageBox("�벻Ҫ�ڴ˹����аε�U�̣�\n����ת���У����Ժ󡭡�","���ƿ�ת���ݵ�U�̡���",MB_SYSTEMMODAL);

	CloseHandle(h1);
	return 1;
}



void CRacingDlg::set_Upath(string UP)
{
	Upath = UP + UPATH;
}

//���ݿ�������
//
//���ã��Զ�ʶ��U���е������ļ���Ϣ����������Ҫ�����ڷ�Χ�ڵ��ļ���U��
//
DWORD WINAPI CRacingDlg::thread_copyDataToU(LPVOID lpParameter)
{
	CRacingDlg* temp = (CRacingDlg*)lpParameter;

	list <string> strList;
	string date;
	string days;
	ifstream file1;
	CString liscensPath;

	liscensPath.Format("%c:\\license.txt", temp->Upath[0]);
	//temp->MessageBox(liscensPath);


	while (1)
	{
		file1.open(liscensPath, ios::in);
		if (!file1)
		{
			break;
		}

		if (temp->MessageBox("��ȷ��U����license.txt�ļ��Ƿ����", "�Ҳ��������ļ�", MB_SYSTEMMODAL | MB_RETRYCANCEL) == IDRETRY)
		{
			continue;
		}
		else
		{
			return 0;
		}
	}
	streamoff seekgNum;
	while (file1)
	{
		char current;
		file1.read(&current, 1);
		if (current == '[')
		{
			seekgNum = file1.tellg();
			strList.push_back(findFront(&file1));
			//cout<<"��ǰλ��1��"<<file.tellg()<<endl;
			file1.seekg(seekgNum);
			//cout<<"��ǰλ��2��"<<file.tellg()<<endl;
			strList.push_back(findBehind(&file1));
			//cout<<"��ǰλ��3��"<<file.tellg()<<endl;
		}
	}
	file1.close();

	list<string>::iterator iter;
	for (iter = strList.begin(); iter != strList.end(); iter++)
	{
		string temp = *iter;
		if (!temp.compare("Username"))	//��ȷ���0
		{
			//cout<<temp<<"=";
			string temp = *++iter;
			if (temp.compare("LSKZ"))		//�û���
			{
				//cout<<temp<<"  �û�������"<<endl;
				return 0;
			}
			else
			{
				//cout<<temp<<"  �û�����ȷ��"<<endl;
			}
			iter--;
		}
		else if (!temp.compare("Password"))
		{
			//cout<<temp<<"=";
			string temp = *++iter;
			if (temp.compare("SS4"))
			{
				//cout<<temp<<"  �������"<<endl;
				return 0;
			}
			else
			{
				//cout<<temp<<"  ������ȷ��"<<endl;
			}
			iter--;
		}
		else if (!temp.compare("TimeFrom"))
		{
			//cout<<temp<<"=";
			string temp = *++iter;
			//cout<<temp<<endl;
			date = temp;
		}
		else if (!temp.compare("TimeSpan"))
		{
			//cout<<temp<<"=";
			string temp = *++iter;
			//cout<<temp<<endl;
			days = temp;
		}
	}

	//�����ļ�
	int num = temp->Copy_File(date, days, temp->Upath);
	CString str;
	string path1 = temp->Upath;
	string path2 = FILEPATH;
	str.Format("������ɣ�\n�������ļ�������%d  \n�ɹ�%d  ʧ��:%d\n��·��%s  ������%s", num / 1000, num / 1000 - num % 1000, num % 1000, path2.c_str(), path1.c_str());
	HANDLE hWnd = ::FindWindowEx(NULL, NULL, NULL, _T("���ƿ�ת���ݵ�U�̡���"));
	::SendMessage((HWND)hWnd, WM_CLOSE, NULL, NULL);
	temp->MessageBox(str, "�������", MB_SYSTEMMODAL);

	return 0;
}


int CRacingDlg::SendFireMsg()
{

	unsigned char SendBuf[100] = "";

	SendBuf[0] = 0xBB;
	SendBuf[1] = 0xBB;
	SendBuf[2] = 0x4F;

	SendBuf[4] = 0x01;
	if (((CLT_LCWB_1ADlg*)theApp.pMainDlg)->TaxStat)
	{
		SendBuf[5] = 0x11;

		memcpy(&SendBuf[6], ((CLT_LCWB_1ADlg*)theApp.pMainDlg)->TaxData.TAXDataBuf, 72);
	}
	else
		SendBuf[5] = 0x22;

	for (int i = 0; i < 78; i++)
	{
		SendBuf[78] += SendBuf[i];
	}

	return LDFM_SendLdleDataMsg(SendBuf, 79);
}


//��ת���ݽ�������
//
//���ã�������ת�忨�����ݣ���ʾ������
//
//��������ת�忨�������ݣ����ݳ��ȣ���������AB�ڱ�־����־SendFlag��������,Ĭ��FALSE��
int CRacingDlg::LdleMsgAnalysis(unsigned char* buf, int len, int Train, BOOL SendFlag)
{
	//ʶ��ͷ
	if (!(buf[0] == 0xBB && buf[1] == 0xBB))
	{
		return 0;
	}

	lastDataTime[Train] = 0;	//��ʱ������

	//����
	CListCtrl* m_FireList;	//Train=0 ����A��1 ����B��2 ����A�� 3 ����B
	int startnum = 0;			//A��B�ڱ�־

	int Wheelset1, Wheelset2, Wheelset3, Wheelset4, WheelsetNum;
	CString WheelsetState, strWheelset1, strWheelset2, strWheelset3, strWheelset4, strWheelsetNum;

	//������ת����
	Wheelset1 = (buf[6] << 8) | buf[5];	//�ֶ�1
	Wheelset2 = (buf[8] << 8) | buf[7];
	Wheelset3 = (buf[10] << 8) | buf[9];
	Wheelset4 = (buf[12] << 8) | buf[11];
	if ((buf[13] & 0x01) == 1)				//״̬
		WheelsetState = "��ת";
	else
	{
		WheelsetState = "δ������ת";
	}
	WheelsetNum = (buf[15] << 8) | buf[14];//����

	//Ҫ��ʾ������ת�ַ���
	strWheelset1.Format("%3d km/h", Wheelset1);
	strWheelset2.Format("%3d km/h", Wheelset2);
	strWheelset3.Format("%3d km/h", Wheelset3);
	strWheelset4.Format("%3d km/h", Wheelset4);
	strWheelsetNum.Format("%3d", WheelsetNum);

	//��ʾ����
	SetDlgItemText(static_value_handle1 + (Train * 6) + 0, WheelsetState);
	SetDlgItemText(static_value_handle1 + (Train * 6) + 1, strWheelsetNum);
	SetDlgItemText(static_value_handle1 + (Train * 6) + 2, strWheelset1);
	SetDlgItemText(static_value_handle1 + (Train * 6) + 3, strWheelset2);
	SetDlgItemText(static_value_handle1 + (Train * 6) + 4, strWheelset3);
	SetDlgItemText(static_value_handle1 + (Train * 6) + 5, strWheelset4);

	/*
	if (SendFlag)
	{
	sendto(FireBSoc, (char*)buf, len, 0, (sockaddr*)&FireBAddr, sizeof(FireBAddr));
	}*/
}

//��ת���ݴ洢����
//
//���ã�����ת���ݴ洢��E��
//
void CRacingDlg::saveData(unsigned char* buf)
{
	//***********�洢����********************************************
	unsigned char SaveBuf[1024] = "";
	if (((buf[3] << 8) | buf[2]) == 0x377)
	{

		for (int i = 0; i < 869; i++)
		{
			SaveBuf[i] = buf[i + 17];
		}

		CString strPath = strFilePath;

		strPath += "LSKZ";
		CreateDirectory(strPath, NULL);

		SYSTEMTIME time;
		GetLocalTime(&time);

		CString m_year, m_month, m_day, m_date;
		m_year.Format("%d", time.wYear);
		m_month.Format("%.2d", time.wMonth);
		m_day.Format("%.2d", time.wDay);

		m_date += strPath;
		m_date += "//";
		m_date += strTrainNum;
		m_date += "_";
		m_date += m_year;
		m_date += m_month;
		m_date += m_day;
		m_date += "_01.skz";


		FILE* fh = fopen(m_date, "ab+");

		if (!fh)
		{
			TRACE("FILE not found\n");
			return;
		}
		//fseek(fh, 0L, SEEK_END);

		fwrite(SaveBuf, sizeof(unsigned char), 869, fh);

		fclose(fh);
		//***********************************************************
	}
}


void CRacingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	//��ת������ʾ����
	//��������
	CPen pen1(PS_SOLID, 2, RGB(255, 255, 255));
	CPen* oldPen;
	oldPen = dc.SelectObject(&pen1);

	dc.MoveTo(400, 70);
	dc.LineTo(400, 250);
	dc.MoveTo(400, 350);
	dc.LineTo(400, 530);
	dc.SelectObject(oldPen);

	//AB�� ��
	CFont font1, * oldfont;
	int textSize = 180, ox = 200, oy = 40;

	dc.SetTextColor(RGB(255, 255, 255));										//����������ɫ
	font1.CreatePointFont((int)(180), _T("����"), &dc);						//��������

	oldfont = dc.SelectObject(&font1);										//ѡ����������PDC
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(ox, oy, "A ��");
	dc.TextOut(ox + 350, oy, "B ��");
	dc.TextOut(ox, oy + 270, "A ��");
	dc.TextOut(ox + 350, oy + 270, "B ��");
	dc.SelectObject(oldfont);

}

void WINAPI Thread_FileManage(LPVOID lParam)
{
	CRacingDlg* dlg = (CRacingDlg*)lParam;

	SYSTEMTIME time;
	GetLocalTime(&time);


	CFileFind finder;
	std::vector<CString> fileList;
	CString findPath;
	findPath += dlg->strFilePath;
	findPath += "LSKZ//*.skz";

	while (1)
	{
		BOOL bHaveFiles = finder.FindFile(findPath);
		while (bHaveFiles)
		{
			bHaveFiles = finder.FindNextFile();
			fileList.push_back(finder.GetFileName());
		}

		for (int i = 0; i < fileList.size(); i++)
		{
			CString tempfilename = fileList[i];
			int n = tempfilename.Find('_');
			tempfilename = tempfilename.Right(tempfilename.GetLength() - n - 1);
			n = tempfilename.Find('_');
			tempfilename = tempfilename.Left(n);
			CString stryear, strmonth, strday;
			int nyear = 0, nmonth = 0, nday = 0;
			stryear = tempfilename.Left(4);
			strday = tempfilename.Right(2);
			tempfilename = tempfilename.Right(4);
			strmonth = tempfilename.Left(2);
			nyear = atoi(stryear);
			nmonth = atoi(strmonth);
			nday = atoi(strday);

			SYSTEMTIME filetime;
			filetime.wYear = nyear;
			filetime.wMonth = nmonth;
			filetime.wDay = nday;


			CTimeSpan sp;

			CTime tm1(time.wYear, time.wMonth, time.wDay, 0, 0, 0);
			CTime tm2(nyear, nmonth, nday, 0, 0, 0);

			sp = tm1 - tm2;

			if (sp.GetDays() > 100)
			{
				CString delfilepath = dlg->strFilePath;
				delfilepath += "LSKZ//";
				delfilepath += fileList[i];
				CFile TempFile;
				TempFile.Remove(delfilepath);
			}

		}

		fileList.clear();

		Sleep(1000 * 60 * 60 * 24);
	}
	return;
}


void CRacingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (nIDEvent == 201)
	{
		for (int i = 0; i < 4; i++)
		{
			if (lastDataTime[i] > 3)	//������15��û���յ��ýڳ�������ʱ����ʼ���ýڳ����б�
			{
				int Train = i;
				CString strWheelsetState, strWheelset;
				//Ҫ��ʾ������ת�ַ���
				strWheelset.Format("-- km/h");
				strWheelsetState.Format("��");
				//��ʼ���б�
				SetDlgItemText(static_value_handle1 + (Train * 6) + 0, strWheelsetState);
				SetDlgItemText(static_value_handle1 + (Train * 6) + 1, strWheelsetState);
				SetDlgItemText(static_value_handle1 + (Train * 6) + 2, strWheelset);
				SetDlgItemText(static_value_handle1 + (Train * 6) + 3, strWheelset);
				SetDlgItemText(static_value_handle1 + (Train * 6) + 4, strWheelset);
				SetDlgItemText(static_value_handle1 + (Train * 6) + 5, strWheelset);

				lastDataTime[i] = 0;
			}
			lastDataTime[i]++;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CRacingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

	for (int i = 0; i < 24; i++)
	{
		if (pWnd->GetDlgCtrlID() == static_value_handle1 + i)
		{
			pDC->SetTextColor(RGB(0, 255, 0));
			pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  
			hbr = (HBRUSH)m_brush;
		}
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
