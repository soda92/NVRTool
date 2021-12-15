// RacingDlg.cpp : 实现文件
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

// CRacingDlg 对话框

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
		return "空转状态 ：";
		break;
	case 1:
		return "空转次数 ：";
		break;
	case 2:
		return "轮对1速度：";
		break;
	case 3:
		return "轮对2速度：";
		break;
	case 4:
		return "轮对3速度：";
		break;
	case 5:
		return "轮对4速度：";
		break;
	default:
		return "错误";
	}
}

void WINAPI Thread_FileManage(LPVOID lParam);


// CRacingDlg 消息处理程序

BOOL CRacingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_brush.CreateSolidBrush(RGB(0, 0, 0));


	//串口初始化
	char RacingCom[20] = "";
	GetPrivateProfileString("LT_WXCLCFG", "RacingCom", "COM1", RacingCom, 20, ".//LT_WXCLCFG.ini");
	if (LdleDataComInit(RacingCom) != -1)
	{
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_LdleDataAnalysis, this, 0, NULL);
	}


	//从配置文件中读取存储路径
	char FilePath[20] = "";
	GetPrivateProfileString("LT_WXCLCFG", "HDD", "e://", FilePath, 20, ".//LT_WXCLCFG.ini");
	strFilePath = CString(FilePath);

	char TrainNum[20];
	GetPrivateProfileString("LDFM-4E-VideoCFG", "TrainNum", "SS91456", TrainNum, 20, ".//LDFM-4E-VideoCFG.ini");
	strTrainNum = CString(TrainNum);

	//启动磁盘文件维护线程
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_FileManage, this, 0, NULL);

	//获取空转串口信息
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_LdleDataAnalysis, this, 0, NULL);
	//****************************************************************************************
	//标签	
	//静态文本起点，长，高，上下间隔
	int num = 1200;


	int ox = 120, oy = 70, width = 160, height = 25, space = 8;
	int ox1 = ox + 350;		//B节ox
	int oy1 = oy + 80;		//他车oy

	//字体
	newFont.CreatePointFont(160, "黑体");


	for (int i = 0; i < 24; i++)
	{
		m_static_text[i] = new CStatic();
		CString str;
		str = staticText(i);
		if (i < 6)					//本车A
		{
			CRect rect(ox, oy + i * (height + space), ox + width, oy + i * (space + height) + height);
			m_static_text[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_text[i]->SetFont(&newFont);
		}
		else if (i < 12)			//本车B
		{
			CRect rect(ox1, oy + (i - 6) * (height + space), ox1 + width, oy + (i - 6) * (space + height) + height);
			m_static_text[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_text[i]->SetFont(&newFont);
		}
		else if (i < 18)			//他车A
		{
			CRect rect(ox, oy1 + (i - 6) * (height + space), ox + width, oy1 + (i - 6) * (space + height) + height);
			m_static_text[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_text[i]->SetFont(&newFont);
		}
		else					//他车B
		{
			CRect rect(ox1, oy1 + (i - 12) * (height + space), ox1 + width, oy1 + (i - 12) * (space + height) + height);
			m_static_text[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_text[i]->SetFont(&newFont);
		}
	}

	//****************************************************************************************
	//数据
	//静态文本起点，长，高，上下间隔
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
			str.Format("未知");
		else
			str.Format("-- km/h");

		if (i < 6)					//本车A
		{
			CRect rect(ox, oy + i * (height + space), ox + width, oy + i * (space + height) + height);
			m_static_value[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_value[i]->SetFont(&newFont);
		}
		else if (i < 12)			//本车B
		{
			CRect rect(ox1, oy + (i - 6) * (height + space), ox1 + width, oy + (i - 6) * (space + height) + height);
			m_static_value[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_value[i]->SetFont(&newFont);
		}
		else if (i < 18)			//他车A
		{
			CRect rect(ox, oy1 + (i - 6) * (height + space), ox + width, oy1 + (i - 6) * (space + height) + height);
			m_static_value[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_value[i]->SetFont(&newFont);
		}
		else					//他车B
		{
			CRect rect(ox1, oy1 + (i - 12) * (height + space), ox1 + width, oy1 + (i - 12) * (space + height) + height);
			m_static_value[i]->Create(str, WS_CHILD | WS_VISIBLE, rect, this, num + i);
			m_static_value[i]->SetFont(&newFont);
		}
	}

	//********************************************************************************************


	//字体
	newFont1.CreatePointFont(170, "黑体");

	m_static_B.SetFont(&newFont1);
	m_static_T.SetFont(&newFont1);


	for (int i = 0; i < 4; i++)
	{
		lastDataTime[i] = 0;
	}
	SetTimer(201, 1000, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



//判断是不是字母
bool isLetter(char chr)
{
	if ((chr >= 97 && chr <= 122) || (chr >= 65 && chr <= 90))
	{
		return 1;
	}
	return 0;
}

//翻转字符串
string changeStr(string str)
{
	string str2;
	int sizeStr = str.length();
	//cout<<"字符长度="<<sizeStr<<endl;
	for (int i = 0; i < sizeStr; i++)
	{
		str2 += str[sizeStr - i - 1];
		//cout<<str[sizeStr-i-1]<<"->"<<str2<<endl;
	}


	return str2;
}

//找前面的字符串
string findFront(ifstream* file)
{
	string str;
	bool is = 0; //字母标志

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
			if (is == 0)		//前一个字符不是字母
			{
				is = 0;
			}
			else			//前一个字符是字母
			{
				//翻转字符串
				//cout<<"str="<<str<<endl;
				return changeStr(str);
			}
		}
		//cout<<cur<<endl;
	}

	return str;
}

//查找后面的字符串
string findBehind(ifstream* file)
{
	string str;
	bool is = 0; //字母标志

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



//获取特定格式的文件名
void getAllFiles(string path, vector<string>& files, string format)
{
	intptr_t    hFile = 0;//文件句柄  64位下long 改为 intptr_t
	struct _finddata_t fileinfo;//文件信息 
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1) //文件存在
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//判断是否为文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)//文件夹名中不含"."和".."
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name)); //保存文件夹名
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, format); //递归遍历文件夹
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));//如果不是文件夹，储存文件名
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

//判断该文件path需不需要复制
bool CRacingDlg::isCopy(string path, string date, string days)
{
	//转换日期
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

	//std::cout<<"起始日期："<<cdate1.tostring()<<endl;
	OutputDebugStringA(cdate1.tostring().c_str());

	//将天数转换为int类型
	for (int i = 0; i < days.length(); i++)
	{
		int num = days[i] - 48;
		n = n * 10 + num;
	}
	//std::cout<<"天数n="<<n<<endl;



	//获取截止日期
	CDate cdate2 = cdate1;
	cdate2.addDay(n);
	date2 = cdate2.tostring();
	//cout<<"截止日期："<<date2<<endl;


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
	//cout<<"当前文件："<<path<<endl;
	//std::cout<<"当前文件日期："<<current<<endl;


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

//拷贝文件
int CRacingDlg::Copy_File(string date, string days, string Upath)
{
	int n = 0, fail = 0;
	//获取目录下所有.skz文件
	vector<string> files;
	getAllFiles(FILEPATH, files, ".skz");

	//cout<<"---------------------------------------------------\n所有文件："<<endl;

	vector<string>::iterator iter;
	for (iter = files.begin(); iter != files.end(); iter++)
	{
		string temp = *iter;
		//cout<<temp<<endl;
	}
	//cout<<"---------------------------------------------------"<<endl;

	//创建LSKZ文件夹
	string dirName;
	dirName += Upath[0];
	dirName += ":/LSKZ";
	createDirectory(const_cast<char*>(dirName.c_str()));


	//拷贝文件
	//vector<string>::iterator iter;
	for (iter = files.begin(); iter != files.end(); iter++)
	{
		string temp = *iter;
		string strUpath = Upath;
		if (isCopy(temp, date, days))
		{
			//计算U盘路径
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

			//复制文件
			if (CopyFileA(temp.c_str(), strUpath.c_str(), 0))
			{
				//cout<<"复制"<<temp<<"  ->  "<<strUpath.c_str()<<"成功！"<<endl;
				//cout<<"---------------------------------------------------"<<endl;
			}
			else
			{
				fail++;
				//cout<<"复制"<<temp<<"  ->  "<<strUpath.c_str()<<"失败！"<<endl;
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

	HANDLE h1;//声明句柄变量
	h1 = CreateThread(NULL, 0, thread_copyDataToU, this, 0, NULL);//创建线程1
	//MessageBox("请不要在此过程中拔掉U盘！\n数据转移中，请稍后……","复制空转数据到U盘……",MB_SYSTEMMODAL);

	CloseHandle(h1);
	return 1;
}



void CRacingDlg::set_Upath(string UP)
{
	Upath = UP + UPATH;
}

//数据拷贝函数
//
//作用：自动识别U盘中的配置文件信息，拷贝所需要的日期范围内的文件到U盘
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

		if (temp->MessageBox("请确认U盘中license.txt文件是否存在", "找不到配置文件", MB_SYSTEMMODAL | MB_RETRYCANCEL) == IDRETRY)
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
			//cout<<"当前位置1："<<file.tellg()<<endl;
			file1.seekg(seekgNum);
			//cout<<"当前位置2："<<file.tellg()<<endl;
			strList.push_back(findBehind(&file1));
			//cout<<"当前位置3："<<file.tellg()<<endl;
		}
	}
	file1.close();

	list<string>::iterator iter;
	for (iter = strList.begin(); iter != strList.end(); iter++)
	{
		string temp = *iter;
		if (!temp.compare("Username"))	//相等返回0
		{
			//cout<<temp<<"=";
			string temp = *++iter;
			if (temp.compare("LSKZ"))		//用户名
			{
				//cout<<temp<<"  用户名错误！"<<endl;
				return 0;
			}
			else
			{
				//cout<<temp<<"  用户名正确！"<<endl;
			}
			iter--;
		}
		else if (!temp.compare("Password"))
		{
			//cout<<temp<<"=";
			string temp = *++iter;
			if (temp.compare("SS4"))
			{
				//cout<<temp<<"  密码错误！"<<endl;
				return 0;
			}
			else
			{
				//cout<<temp<<"  密码正确！"<<endl;
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

	//拷贝文件
	int num = temp->Copy_File(date, days, temp->Upath);
	CString str;
	string path1 = temp->Upath;
	string path2 = FILEPATH;
	str.Format("拷贝完成！\n日期内文件总数：%d  \n成功%d  失败:%d\n从路径%s  拷贝到%s", num / 1000, num / 1000 - num % 1000, num % 1000, path2.c_str(), path1.c_str());
	HANDLE hWnd = ::FindWindowEx(NULL, NULL, NULL, _T("复制空转数据到U盘……"));
	::SendMessage((HWND)hWnd, WM_CLOSE, NULL, NULL);
	temp->MessageBox(str, "拷贝完成", MB_SYSTEMMODAL);

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


//空转数据解析函数
//
//作用：解析空转板卡的数据，显示到界面
//
//参数：空转板卡来的数据，数据长度，本车他车AB节标志，标志SendFlag（暂无用,默认FALSE）
int CRacingDlg::LdleMsgAnalysis(unsigned char* buf, int len, int Train, BOOL SendFlag)
{
	//识别报头
	if (!(buf[0] == 0xBB && buf[1] == 0xBB))
	{
		return 0;
	}

	lastDataTime[Train] = 0;	//计时器归零

	//变量
	CListCtrl* m_FireList;	//Train=0 本车A，1 本车B，2 他车A， 3 他车B
	int startnum = 0;			//A、B节标志

	int Wheelset1, Wheelset2, Wheelset3, Wheelset4, WheelsetNum;
	CString WheelsetState, strWheelset1, strWheelset2, strWheelset3, strWheelset4, strWheelsetNum;

	//解析空转数据
	Wheelset1 = (buf[6] << 8) | buf[5];	//轮对1
	Wheelset2 = (buf[8] << 8) | buf[7];
	Wheelset3 = (buf[10] << 8) | buf[9];
	Wheelset4 = (buf[12] << 8) | buf[11];
	if ((buf[13] & 0x01) == 1)				//状态
		WheelsetState = "空转";
	else
	{
		WheelsetState = "未发生空转";
	}
	WheelsetNum = (buf[15] << 8) | buf[14];//次数

	//要显示的数据转字符串
	strWheelset1.Format("%3d km/h", Wheelset1);
	strWheelset2.Format("%3d km/h", Wheelset2);
	strWheelset3.Format("%3d km/h", Wheelset3);
	strWheelset4.Format("%3d km/h", Wheelset4);
	strWheelsetNum.Format("%3d", WheelsetNum);

	//显示数据
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

//空转数据存储函数
//
//作用：将空转数据存储到E盘
//
void CRacingDlg::saveData(unsigned char* buf)
{
	//***********存储数据********************************************
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

	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	//空转数据显示界面
	//两个竖线
	CPen pen1(PS_SOLID, 2, RGB(255, 255, 255));
	CPen* oldPen;
	oldPen = dc.SelectObject(&pen1);

	dc.MoveTo(400, 70);
	dc.LineTo(400, 250);
	dc.MoveTo(400, 350);
	dc.LineTo(400, 530);
	dc.SelectObject(oldPen);

	//AB节 字
	CFont font1, * oldfont;
	int textSize = 180, ox = 200, oy = 40;

	dc.SetTextColor(RGB(255, 255, 255));										//设置字体颜色
	font1.CreatePointFont((int)(180), _T("黑体"), &dc);						//设置字体

	oldfont = dc.SelectObject(&font1);										//选择该字体进入PDC
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(ox, oy, "A 节");
	dc.TextOut(ox + 350, oy, "B 节");
	dc.TextOut(ox, oy + 270, "A 节");
	dc.TextOut(ox + 350, oy + 270, "B 节");
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 201)
	{
		for (int i = 0; i < 4; i++)
		{
			if (lastDataTime[i] > 3)	//当超过15秒没有收到该节车厢数据时，初始化该节车厢列表
			{
				int Train = i;
				CString strWheelsetState, strWheelset;
				//要显示的数据转字符串
				strWheelset.Format("-- km/h");
				strWheelsetState.Format("无");
				//初始化列表
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

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)      //对话框颜色  
		return   m_brush;       //返加刷子

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkMode(TRANSPARENT);    //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合  
		hbr = (HBRUSH)m_brush;
	}

	for (int i = 0; i < 24; i++)
	{
		if (pWnd->GetDlgCtrlID() == static_value_handle1 + i)
		{
			pDC->SetTextColor(RGB(0, 255, 0));
			pDC->SetBkMode(TRANSPARENT);    //模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合  
			hbr = (HBRUSH)m_brush;
		}
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
