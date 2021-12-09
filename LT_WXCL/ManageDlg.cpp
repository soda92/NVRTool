// ManageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "ManageDlg.h"
#include "afxdialogex.h"

#include "VideoPlay.h"

// CManageDlg �Ի���

//char RecordFlag[20] = "";//¼�񱣴��־
char TrainNum[50] = "";//���ͳ���
char IPCName[12][50] = {0};//����ͨ������


IMPLEMENT_DYNAMIC(CManageDlg, CDialogEx)

CManageDlg::CManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManageDlg::IDD, pParent)
{
	memset(RecordFlag,0,sizeof(RecordFlag));
	num = 0;
}

CManageDlg::~CManageDlg()
{
}

void CManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_IPCStateList);
	DDX_Control(pDX, IDC_LIST2, m_HDDStateList);
	DDX_Control(pDX, IDC_STATIC1, m_static_device);
	DDX_Control(pDX, IDC_STATIC2, m_static_hard);
}


BEGIN_MESSAGE_MAP(CManageDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CManageDlg ��Ϣ�������

int CManageDlg::SetIPCState()
{
	m_IPCStateList.m_ItemTextColor.RemoveAll();
	for (int i = 0;i < 6;i++)
	{
		if (RecordFlag[i] == 1)
		{
			m_IPCStateList.SetItemText(i,4,"¼��");
			
			m_IPCStateList.SetItemTextColor(4,i,RGB(0,255,0));
		}
		else
		{
			m_IPCStateList.SetItemText(i,4,"δ¼��");
			m_IPCStateList.SetItemTextColor(4,i,RGB(255,0,0));
		}
		Sleep(100);
	}

	return 0;
}

int CManageDlg::FindAndDeleteRecord(CString Path)
{
	CFileFind ff;
	CString FileName;
	//////////////////////////////////////////////////////////////////////////
	//�ҳ�����ķǿ��ļ���
	CString dir = Path;
	dir += "*.*";

	BOOL res = ff.FindFile(dir);
	while (res)
	{
		res = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots())
		{
			//////////////////////////////////////////////////////////////////////////
			//�ж��ļ����Ƿ��
			CFileFind ffEmpty;
			dir = Path;
			dir += ff.GetFileName();
			CString DirTemp = dir;
			dir += "/*.*";
			BOOL ffRes = ffEmpty.FindFile(dir);
			BOOL DirIsEmpty = TRUE;
			CString IdxFileName;			
			while(ffRes)
			{
				ffRes = ffEmpty.FindNextFile();
				if (!ffEmpty.IsDirectory() && !ffEmpty.IsDots())
				{
					if (ffEmpty.GetFileName().Find(".idx")>0)
					{
						IdxFileName = ffEmpty.GetFileName();
						continue;
					}
					DirIsEmpty = FALSE;
					break;
				}
			}
			ffEmpty.Close();
			//////////////////////////////////////////////////////////////////////////
			if (!DirIsEmpty)
			{
				if (FileName.IsEmpty())
				{
					FileName = ff.GetFileName();
				}else
				{
					if (strcmp(ff.GetFileName().GetBuffer(),FileName) < 0)
					{
						FileName = ff.GetFileName();
					}
				}
			}else
			{
				if (!IdxFileName.IsEmpty())
				{
					CString idxPath = DirTemp + "/";
					idxPath += IdxFileName;
					DeleteFile(idxPath);
				}
				RemoveDirectory(DirTemp);
			}
		}
	}
	ff.Close();
	//////////////////////////////////////////////////////////////////////////
	//�ҳ��������Ƶ
	dir = Path;
	dir += FileName;
	Path = dir;
	dir += "/*.mp4";
	res = ff.FindFile(dir);

	FileName.Empty();
	while (res)
	{
		res = ff.FindNextFile();
		if (!ff.IsDirectory() && !ff.IsDots())
		{
			if (FileName.IsEmpty())
			{
				FileName = ff.GetFileName();
			}else
			{
				
				char* FileNameTemp = FileName.GetBuffer();
				CString findFile = ff.GetFileName();
				char* FindFileName = findFile.GetBuffer();
				for (int i = 0;i < 4;i++)
				{
					char* temp = nullptr;
					temp = strchr(FileNameTemp,'_');
					if (temp)
					{
						FileNameTemp = temp+1;
					}
					temp = strchr(FindFileName,'_');
					if (temp)
					{
						FindFileName = temp+1;
					}
				}
						
				if (strcmp(FindFileName,FileNameTemp) < 0)
				{
					FileName = ff.GetFileName();
				}
			}
		}
	}
	ff.Close();
	if (!FileName.IsEmpty())
	{
		CString filePath;
		filePath.Format("%s/%s",Path,FileName);
		DeleteFile(filePath);
	}
	
	//////////////////////////////////////////////////////////////////////////
	return 0;
}

CString CManageDlg::FindDir(char* HddPath)
{
	CFileFind ff;
	CStringArray DirArray;
	int index = 0;
	CString DirBuf;
	CString ResDir;

	CString dir = HddPath;
	dir += "LT-VIDEO-*.*";

	BOOL res = ff.FindFile(dir);
	while (res)
	{
		res = ff.FindNextFile();
		if (ff.IsDirectory() && !ff.IsDots())
		{
			//TRACE("DIR NAME = %s\n",ff.GetFileName());
			CFileFind Subff;
			CString SubDir = HddPath + ff.GetFileName();
			SubDir += "/*.*";
			CString SubDirBuf;

			BOOL SubRes = Subff.FindFile(SubDir);
			while (SubRes)
			{
				SubRes = Subff.FindNextFile();
				if (Subff.IsDirectory() && !Subff.IsDots())
				{
					
					if (SubDirBuf.IsEmpty())
					{
						SubDirBuf = Subff.GetFileName();
					}else
					{
						if (strcmp(Subff.GetFileName(),SubDirBuf) < 0)
						{
							SubDirBuf = Subff.GetFileName();
						}
					}
				}
			}
			if (DirBuf.IsEmpty())
			{
				DirBuf = SubDirBuf;
				ResDir = ff.GetFileName();
			}else
			{
				if (strcmp(SubDirBuf,DirBuf) < 0)
				{
					DirBuf = SubDirBuf;
					ResDir = ff.GetFileName();
				}
			}
			//TRACE("SUB DIR NAME = %s,%s\n",DirBuf,ResDir);
			Subff.Close();
		}
	}
	ff.Close();

	return ResDir;
}

BOOL CManageDlg::IsHDD(char* Path)
{
	ULARGE_INTEGER FreeAv,TotalBytes,FreeBytes;
	if (GetDiskFreeSpaceEx(Path,&FreeAv,&TotalBytes,&FreeBytes))
	{		
		/*CString strTotalBytes,strFreeBytes;

		strTotalBytes.Format("%luG",TotalBytes.QuadPart/(ULONGLONG)(1024*1024*1024));
		strFreeBytes.Format("%luG",FreeBytes.QuadPart/(ULONGLONG)(1024*1024*1024));*/

		if (TotalBytes.QuadPart/(ULONGLONG)(1024*1024*1024) > 100)//�ж��ܿռ��Ƿ����100G
		{
			return TRUE;
		}
	}

	return FALSE;
}

int WINAPI Thread_Record(LPVOID lpPara)
{
	CManageDlg* dlg = (CManageDlg*) lpPara;

	
	/*for (int i=0;i<8;i++)
	{
		Path.Format("%s%d",HDDPath,i+1);
		CreateDirectory(Path,NULL);
	}*/
	//CStringArray IPCName;
	char sysLog[256] = "";

	while(1)
	{
		if (!dlg->IsHDD(theApp.HDDPath))
		{
			if (dlg->IsHDD("e://"))
			{
				strcpy(theApp.HDDPath,"e://");
			}
			else if (dlg->IsHDD("f://"))
			{
				strcpy(theApp.HDDPath,"f://");
			}
			else
			{
				Sleep(5*1000);
				continue;
			}
		}	

		//�����ļ���
		CString Path;
		CString File;

		Path.Format("%slost+found",theApp.HDDPath);
		int res = 0;
		if(CreateDirectory(Path,NULL) == 0)
		{
			res = GetLastError();
			if (res == 3)
			{
				dlg->MessageBox("¼��洢·������ȷ�����޸������ļ��е�·����");
				return -1;
			}
		}
		Path.Format("%s/LT-VIDEO-%s-���������ά/",theApp.HDDPath,TrainNum);	
		//Path.Format("%s/VIDEO_RECORD/",theApp.HDDPath);
		CreateDirectory(Path,NULL);

		for (int i=0;i < 6;i++)
		{
			if (dlg->RecordFlag[i] == 0)
			{
				
				//File.Format("rtsp://admin:hk123456@192.168.101.7%d:554/",i);
				File.Format("rtsp://admin:hk123456@192.168.10%d.%d%d:554/",atoi(&theApp.Local[0]),(theApp.Local[1]=='A'?7:8),i);
				/*if (Video_StartRecord(i+1,File.GetBuffer(File.GetLength()),Path.GetBuffer(Path.GetLength())) == -1)
				{
					RecordFlag[i] = 0;
					TRACE("%d port failed\n",i);
				}*/
				
				
				//if (Video_StartRecord(i+1,File.GetBuffer(File.GetLength()),Path.GetBuffer(Path.GetLength()),TrainNum,IPCName[i],i+1) == -1)
				if (Video_StartRecord(i+1,File.GetBuffer(File.GetLength()),Path.GetBuffer(Path.GetLength()),TrainNum,IPCName[(theApp.Local[1]=='A'?i:i+6)],(theApp.Local[1]=='A'?i:i+6)+1) == -1)
				{
					dlg->RecordFlag[i] = 0;
					TRACE("%d port failed\n",i);
					sprintf_s(sysLog,"%d ͨ��¼�����Ӵ���\n",i);
					//theApp.SysLogWr(theApp.SysLogPath,sysLog);
				}
				else
				{
					dlg->RecordFlag[i] = 1;
					TRACE("path = %s\n",Path);
					sprintf_s(sysLog,"%d ͨ����ʼ¼��...\n",i);					
					//theApp.SysLogWr(theApp.SysLogPath,sysLog);
				}
					
				Sleep(50);//����ȴ�һ�ᡣ��������¼���ļ��浽��һ���ļ��е����⡣
			}
		}

		//��һ��¼��
		for (int i=6;i < 12;i++)
		{
			if (dlg->RecordFlag[i] == 0)
			{
				File.Format("rtsp://admin:hk123456@192.168.10%d.%d%d:554/",atoi(&theApp.Local[0]),(theApp.Local[1]=='A'?8:7),i-6);
				/*if (Video_StartRecord(i+1,File.GetBuffer(File.GetLength()),Path.GetBuffer(Path.GetLength())) == -1)
				{
					RecordFlag[i] = 0;
					TRACE("%d port failed\n",i);
				}*/
				
				
				//if (Video_StartRecord(i+1,File.GetBuffer(File.GetLength()),Path.GetBuffer(Path.GetLength()),TrainNum,IPCName[i],i+1) == -1)
				if (Video_StartRecord(i+1,File.GetBuffer(File.GetLength()),Path.GetBuffer(Path.GetLength()),TrainNum,IPCName[(theApp.Local[1]=='A'?i:i-6)],(theApp.Local[1]=='A'?i:i-6)+1) == -1)
				{
					dlg->RecordFlag[i] = 0;
					TRACE("%d port failed\n",i);
					sprintf_s(sysLog,"%d ͨ��¼�����Ӵ���\n",i);
					//theApp.SysLogWr(theApp.SysLogPath,sysLog);
				}
				else
				{
					dlg->RecordFlag[i] = 1;
					TRACE("path = %s\n",Path);
					sprintf_s(sysLog,"%d ͨ����ʼ¼��...\n",i);					
					//theApp.SysLogWr(theApp.SysLogPath,sysLog);
				}
					
				Sleep(50);//����ȴ�һ�ᡣ��������¼���ļ��浽��һ���ļ��е����⡣
			}
		}

		Sleep(5*1000);
	}	

	return 0;
}

int WINAPI Thread_State(LPVOID lpPara)
{
	CManageDlg* dlg = (CManageDlg*) lpPara;

	//TRACE("ping = %d\n",dlg->DevPing.Ping(5,"192.168.101.1",NULL));
	while(1)
	{
		dlg->SetIPCState();
		dlg->SetHDDState();
		Sleep(5*1000);
	}

	return 0;
}

int CManageDlg::SetHDDState()
{
	//CString strAllInfo;
	ULARGE_INTEGER FreeAv,TotalBytes,FreeBytes;
	if (GetDiskFreeSpaceEx(theApp.HDDPath,&FreeAv,&TotalBytes,&FreeBytes))
	{
		//��ʽ����Ϣ������ʾ����
		CString strTotalBytes,strFreeBytes;

		strTotalBytes.Format("%luG",TotalBytes.QuadPart/(ULONGLONG)(1024*1024*1024));
		strFreeBytes.Format("%luG",FreeBytes.QuadPart/(ULONGLONG)(1024*1024*1024));
		//strAllInfo.Format("Info:[c] %s %s \nU can use %luG",strTotalBytes,strFreeBytes, FreeAv.QuadPart/(ULONGLONG)(1024*1024*1024));/* ��λΪG */
		//MessageBox(strAllInfo);

		if (TotalBytes.QuadPart/(ULONGLONG)(1024*1024*1024) > 100)
		{
			m_HDDStateList.SetItemText(0,2,strTotalBytes);
			m_HDDStateList.SetItemText(0,3,strFreeBytes);
			m_HDDStateList.SetItemText(0,4,"����");
			m_HDDStateList.m_ItemTextColor.RemoveAll();
			m_HDDStateList.SetItemTextColor(4,0,RGB(0,255,0));

			//Ӳ��С��10Gʱ���ֱ�ɾ��8��ͨ���������һ���ļ�
			TRACE("FreeBytes = %dG\n",FreeBytes.QuadPart/(ULONGLONG)(1024*1024*1024));
			if (FreeBytes.QuadPart/(ULONGLONG)(1024*1024*1024) <= 9 && (TotalBytes.QuadPart/(ULONGLONG)(1024*1024*1024)) > 0)
				//if(1)
			{

				CString Path;
				Path.Format("%s/%s/",theApp.HDDPath,FindDir(theApp.HDDPath));								
				//Path.Format("%s/LT-VIDEO-%s-���������ά/",theApp.HDDPath,TrainNum);
				//////////////////////////////////////////////////////////////////////////
				FindAndDeleteRecord(Path);			

			}
		}		

	}
	else
	{
		m_HDDStateList.SetItemText(0,2,"0");
		m_HDDStateList.SetItemText(0,3,"0");
		m_HDDStateList.SetItemText(0,4,"����");
		m_HDDStateList.m_ItemTextColor.RemoveAll();
		m_HDDStateList.SetItemTextColor(4,0,RGB(255,0,0));
		//((CLDFM4EVideoDlg*)theApp.pMainDlg)->SetFireText("Ӳ�̹��ϣ�����");
	}


	return 0;
}

BOOL CManageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//������ˢ
	m_brush.CreateSolidBrush(RGB(0,0,0));

	//����
	newFont1.CreatePointFont(170,"����");
	m_static_device.SetFont(&newFont1);
	m_static_hard.SetFont(&newFont1);

	//�б�����
	
	//�����С
	int fontSize=20;
	m_IPCStateList.SetHeaderFontHW(fontSize,fontSize*0.7);
	m_HDDStateList.SetHeaderFontHW(fontSize,fontSize*0.7);
	m_IPCStateList.SetFontHW(fontSize,fontSize*0.7);
	m_HDDStateList.SetFontHW(fontSize,fontSize*0.7);

	//�и�
	m_IPCStateList.SetHeaderHeight(2);//��ͷ
	m_HDDStateList.SetHeaderHeight(2);
	m_IPCStateList.SetRowHeight(30);//��
	m_HDDStateList.SetRowHeight(30);

	m_IPCStateList.SetBkColor(RGB(0,0,0));
	m_IPCStateList.SetTextColor(RGB(255,255,255));
	m_HDDStateList.SetBkColor(RGB(0,0,0));
	m_HDDStateList.SetTextColor(RGB(255,255,255));


	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetPrivateProfileString("LT_WXCLCFG","HDD","D://",theApp.HDDPath,20,".//LT_WXCLCFG.ini");
	GetPrivateProfileString("LT_WXCLCFG","TrainNum","No00000",TrainNum,50,".//LT_WXCLCFG.ini");

	InitList();
	SetList();

	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Thread_State,this,0,NULL);//����״̬��ѯ�߳�
	CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Thread_Record,this,0,NULL);//����¼���߳�

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


int CManageDlg::InitList()
{
	DWORD dwStyle = m_IPCStateList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��	
	m_IPCStateList.SetExtendedStyle(dwStyle); //������չ���
	m_HDDStateList.SetExtendedStyle(dwStyle);

	m_IPCStateList.InsertColumn(0,"���",LVCFMT_LEFT,60);
	m_IPCStateList.InsertColumn(1,"�豸����",LVCFMT_LEFT,120);
	m_IPCStateList.InsertColumn(2,"   λ��",LVCFMT_LEFT,140);
	m_IPCStateList.InsertColumn(3,"�汾��",LVCFMT_LEFT,100);
	m_IPCStateList.InsertColumn(4,"  ״̬",LVCFMT_LEFT,120);
	m_IPCStateList.InsertColumn(5,"��Ӧ��",LVCFMT_LEFT,115);

	m_HDDStateList.InsertColumn(0,"���",LVCFMT_LEFT,60);
	m_HDDStateList.InsertColumn(1,"�豸����",LVCFMT_LEFT,120);
	m_HDDStateList.InsertColumn(2," ������",LVCFMT_LEFT,120);
	m_HDDStateList.InsertColumn(3," ʣ��",LVCFMT_LEFT,100);
	m_HDDStateList.InsertColumn(4,"  ״̬",LVCFMT_LEFT,120);
	m_HDDStateList.InsertColumn(5,"��Ӧ��",LVCFMT_LEFT,135);

	return 0;
}

int CManageDlg::SetList()
{
	//IPC LIST
	for (int i =0;i<6;i++)
	{
		char a[20];
		itoa(i+1,a,10);
		m_IPCStateList.InsertItem(i,a);
		sprintf_s(a,"IPC");
		m_IPCStateList.SetItemText(i,1,a);
		m_IPCStateList.SetItemText(i,3,"V1.0.0");
		m_IPCStateList.SetItemText(i,4,"δ¼��");
		m_IPCStateList.SetItemTextColor(4,i,RGB(255,0,0));
		m_IPCStateList.SetItemText(i,5,"LTDW");
	}
	for (int i=0;i<12;i++)
	{
		char ipc[60] = "";
		char temp[20] = "";
		sprintf_s(temp,"IPC%d",i+1);
		GetPrivateProfileString("LT_WXCLCFG",temp,"��",ipc,60,".//LT_WXCLCFG.ini");
		strcpy(IPCName[i],ipc);
		//m_IPCStateList.SetItemText(i,2,ipc);
		
	}
	m_IPCStateList.SetItemText(0,2,"·��");
	m_IPCStateList.SetItemText(1,2,"˾����");
	m_IPCStateList.SetItemText(2,2,"��ѹ��1");
	m_IPCStateList.SetItemText(3,2,"��ѹ��2");
	m_IPCStateList.SetItemText(4,2,"������");
	m_IPCStateList.SetItemText(5,2,"������");

	/*m_IPCStateList.SetItemText(0,1,"IPC1");
	m_IPCStateList.SetItemText(1,1,"IPC2");
	m_IPCStateList.SetItemText(2,1,"IPC3");
	m_IPCStateList.SetItemText(3,1,"IPC4");
	m_IPCStateList.SetItemText(4,1,"IPC5");
	m_IPCStateList.SetItemText(5,1,"IPC6");
	m_IPCStateList.SetItemText(6,1,"IPC7");
	m_IPCStateList.SetItemText(7,1,"IPC8");*/

	//HDD LIST
	m_HDDStateList.InsertItem(0,"1");
	m_HDDStateList.SetItemText(0,1,"Ӳ��");
	m_HDDStateList.SetItemText(0,2,"0");
	m_HDDStateList.SetItemText(0,3,"0");
	m_HDDStateList.SetItemText(0,4,"����");
	m_HDDStateList.SetItemTextColor(4,0,RGB(255,0,0));
	m_HDDStateList.SetItemText(0,5,"LTDW");

	return 0;
}


void CManageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (num++ > 0)
	{		
		CRect rc;
		this->GetClientRect(&rc);
		GetDlgItem(IDC_STATIC1)->MoveWindow(rc.right/20,rc.bottom/20,rc.right/20*18,rc.bottom/15*8+10);
		m_IPCStateList.MoveWindow(rc.right/20 + 30,rc.bottom/15 + 30,(rc.right/20*18 - 60),(rc.bottom/15*8 - 50));

		GetDlgItem(IDC_STATIC2)->MoveWindow(rc.right/20,rc.bottom/15*10,rc.right/20*18,rc.bottom/15*4+10);
		m_HDDStateList.MoveWindow(rc.right/20 + 30,rc.bottom/15*10 + 40,rc.right/20*18 - 60,rc.bottom/15*4 - 48);
	}
}

int CManageDlg::m_RecordCallBack(LONG nPort,char* error)
{
	RecordFlag[nPort-1] = 0;
	return 0;
}


HBRUSH CManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	if(nCtlColor   ==CTLCOLOR_DLG)      //�Ի�����ɫ  
		return   m_brush;       //����ˢ��

	if(nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(255,255,255));  
		pDC->SetBkMode(TRANSPARENT);    //ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�  
		hbr=(HBRUSH)m_brush;
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
