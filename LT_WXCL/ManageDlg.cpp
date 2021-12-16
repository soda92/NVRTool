// ManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "ManageDlg.h"
#include "afxdialogex.h"

#include "VideoPlay.h"

// CManageDlg 对话框

//char RecordFlag[20] = "";//录像保存标志
char TrainNum[50] = "";//车型车号
char IPCName[12][50] = { 0 };//保存通道名称


IMPLEMENT_DYNAMIC(CManageDlg, CDialogEx)

CManageDlg::CManageDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CManageDlg::IDD, pParent)
{
    memset(RecordFlag, 0, sizeof(RecordFlag));
    num = 0;
}

CManageDlg::~CManageDlg()
{
}

void CManageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_IPC, m_IPCStateList);
    DDX_Control(pDX, IDC_LIST_HDD, m_HDDStateList);
    DDX_Control(pDX, IDC_STATIC1, m_static_device);
    DDX_Control(pDX, IDC_STATIC2, m_static_hard);
}


BEGIN_MESSAGE_MAP(CManageDlg, CDialogEx)
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CManageDlg 消息处理程序

int CManageDlg::SetIPCState()
{
    m_IPCStateList.m_ItemTextColor.RemoveAll();
    for (int i = 0; i < 6; i++)
    {
        if (RecordFlag[i] == 1)
        {
            m_IPCStateList.SetItemText(i, 4, _T("录像"));

            m_IPCStateList.SetItemTextColor(4, i, RGB(0, 255, 0));
        }
        else
        {
            m_IPCStateList.SetItemText(i, 4, _T("未录像"));
            m_IPCStateList.SetItemTextColor(4, i, RGB(255, 0, 0));
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
    //找出最早的非空文件夹
    CString dir = Path;
    dir += "*.*";

    BOOL res = ff.FindFile(dir);
    while (res)
    {
        res = ff.FindNextFile();
        if (ff.IsDirectory() && !ff.IsDots())
        {
            //////////////////////////////////////////////////////////////////////////
            //判断文件夹是否空
            CFileFind ffEmpty;
            dir = Path;
            dir += ff.GetFileName();
            CString DirTemp = dir;
            dir += "/*.*";
            BOOL ffRes = ffEmpty.FindFile(dir);
            BOOL DirIsEmpty = TRUE;
            CString IdxFileName;
            while (ffRes)
            {
                ffRes = ffEmpty.FindNextFile();
                if (!ffEmpty.IsDirectory() && !ffEmpty.IsDots())
                {
                    if (ffEmpty.GetFileName().Find(".idx") > 0)
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
                }
                else
                {
                    if (strcmp(ff.GetFileName().GetBuffer(), FileName) < 0)
                    {
                        FileName = ff.GetFileName();
                    }
                }
            }
            else
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
    //找出最早的视频
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
            }
            else
            {

                char* FileNameTemp = FileName.GetBuffer();
                CString findFile = ff.GetFileName();
                char* FindFileName = findFile.GetBuffer();
                for (int i = 0; i < 4; i++)
                {
                    char* temp = nullptr;
                    temp = strchr(FileNameTemp, '_');
                    if (temp)
                    {
                        FileNameTemp = temp + 1;
                    }
                    temp = strchr(FindFileName, '_');
                    if (temp)
                    {
                        FindFileName = temp + 1;
                    }
                }

                if (strcmp(FindFileName, FileNameTemp) < 0)
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
        filePath.Format("%s/%s", Path, FileName);
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
                    }
                    else
                    {
                        if (strcmp(Subff.GetFileName(), SubDirBuf) < 0)
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
            }
            else
            {
                if (strcmp(SubDirBuf, DirBuf) < 0)
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
    ULARGE_INTEGER FreeAv, TotalBytes, FreeBytes;
    if (GetDiskFreeSpaceEx(Path, &FreeAv, &TotalBytes, &FreeBytes))
    {

        if (TotalBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024) > 100)//判断总空间是否大于100G
        {
            return TRUE;
        }
    }

    return FALSE;
}

int WINAPI Thread_Record(LPVOID lpPara)
{
    CManageDlg* dlg = (CManageDlg*)lpPara;

    char sysLog[256] = "";

    while (1)
    {
        if (!dlg->IsHDD(theApp.HDDPath))
        {
            if (dlg->IsHDD("e://"))
            {
                strcpy_s(theApp.HDDPath, "e://");
            }
            else if (dlg->IsHDD("f://"))
            {
                strcpy_s(theApp.HDDPath, "f://");
            }
            else
            {
                Sleep(5 * 1000);
                continue;
            }
        }

        //创建文件夹
        CString Path;
        CString File;

        Path.Format("%slost+found", theApp.HDDPath);
        int res = 0;
        if (CreateDirectory(Path, NULL) == 0)
        {
            res = GetLastError();
            if (res == 3)
            {
                dlg->MessageBox(_T("录像存储路径不正确，请修改配置文件中的路径。"));
                return -1;
            }
        }
        Path.Format("%s/LT-VIDEO-%s-北京蓝天多维/", theApp.HDDPath, TrainNum);

        CreateDirectory(Path, NULL);

        for (int i = 0; i < 6; i++)
        {
            if (dlg->RecordFlag[i] == 0)
            {
                File.Format("rtsp://admin:hk123456@192.168.10%d.%d%d:554/",
                    atoi(&theApp.Local[0]), (theApp.Local[1] == 'A' ? 7 : 8), i);
                if (Video_StartRecord(i + 1, File.GetBuffer(File.GetLength()),
                    Path.GetBuffer(Path.GetLength()),
                    TrainNum, IPCName[(theApp.Local[1] == 'A' ? i : i + 6)],
                    (theApp.Local[1] == 'A' ? i : i + 6) + 1) == -1)
                {
                    dlg->RecordFlag[i] = 0;
                    TRACE("%d port failed\n", i);
                    sprintf_s(sysLog, "%d 通道录像连接错误！\n", i);
                }
                else
                {
                    dlg->RecordFlag[i] = 1;
                    TRACE("path = %s\n", Path);
                    sprintf_s(sysLog, "%d 通道开始录像...\n", i);
                }

                Sleep(50);//必须等待一会。否则会出现录像文件存到下一个文件夹的问题。
            }
        }

        //另一节录像
        for (int i = 6; i < 12; i++)
        {
            if (dlg->RecordFlag[i] == 0)
            {
                File.Format("rtsp://admin:hk123456@192.168.10%d.%d%d:554/",
                    atoi(&theApp.Local[0]), (theApp.Local[1] == 'A' ? 8 : 7), i - 6);
                if (Video_StartRecord(i + 1, File.GetBuffer(File.GetLength()),
                    Path.GetBuffer(Path.GetLength()),
                    TrainNum, IPCName[(theApp.Local[1] == 'A' ? i : i - 6)],
                    (theApp.Local[1] == 'A' ? i : i - 6) + 1) == -1)
                {
                    dlg->RecordFlag[i] = 0;
                    TRACE("%d port failed\n", i);
                    sprintf_s(sysLog, "%d 通道录像连接错误！\n", i);
                }
                else
                {
                    dlg->RecordFlag[i] = 1;
                    TRACE("path = %s\n", Path);
                    sprintf_s(sysLog, "%d 通道开始录像...\n", i);
                }

                Sleep(50);//必须等待一会。否则会出现录像文件存到下一个文件夹的问题。
            }
        }

        Sleep(5 * 1000);
    }

    return 0;
}

int WINAPI Thread_State(LPVOID lpPara)
{
    CManageDlg* dlg = (CManageDlg*)lpPara;

    //TRACE("ping = %d\n",dlg->DevPing.Ping(5,"192.168.101.1",NULL));
    while (1)
    {
        dlg->SetIPCState();
        dlg->SetHDDState();
        Sleep(5 * 1000);
    }

    return 0;
}

int CManageDlg::SetHDDState()
{
    //CString strAllInfo;
    ULARGE_INTEGER FreeAv, TotalBytes, FreeBytes;
    if (GetDiskFreeSpaceEx(theApp.HDDPath, &FreeAv, &TotalBytes, &FreeBytes))
    {
        //格式化信息，并显示出来
        CString strTotalBytes, strFreeBytes;

        strTotalBytes.Format("%luG", TotalBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024));
        strFreeBytes.Format("%luG", FreeBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024));
        //strAllInfo.Format("Info:[c] %s %s \nU can use %luG",strTotalBytes,strFreeBytes, FreeAv.QuadPart/(ULONGLONG)(1024*1024*1024));/* 单位为G */
        //MessageBox(strAllInfo);

        if (TotalBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024) > 100)
        {
            m_HDDStateList.SetItemText(0, 2, strTotalBytes);
            m_HDDStateList.SetItemText(0, 3, strFreeBytes);
            m_HDDStateList.SetItemText(0, 4, "正常");
            m_HDDStateList.m_ItemTextColor.RemoveAll();
            m_HDDStateList.SetItemTextColor(4, 0, RGB(0, 255, 0));

            //硬盘小于10G时将分别删除8个通道里最早的一个文件
            TRACE("FreeBytes = %dG\n", FreeBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024));
            if (FreeBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024) <= 9 && (TotalBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024)) > 0)
            {
                CString Path;
                Path.Format("%s/%s/", theApp.HDDPath, FindDir(theApp.HDDPath));
                //Path.Format("%s/LT-VIDEO-%s-北京蓝天多维/",theApp.HDDPath,TrainNum);
                //////////////////////////////////////////////////////////////////////////
                FindAndDeleteRecord(Path);

            }
        }

    }
    else
    {
        m_HDDStateList.SetItemText(0, 2, "0");
        m_HDDStateList.SetItemText(0, 3, "0");
        m_HDDStateList.SetItemText(0, 4, "错误");
        m_HDDStateList.m_ItemTextColor.RemoveAll();
        m_HDDStateList.SetItemTextColor(4, 0, RGB(255, 0, 0));
        //((CLDFM4EVideoDlg*)theApp.pMainDlg)->SetFireText("硬盘故障！！！");
    }


    return 0;
}

BOOL CManageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    //背景画刷
    m_brush.CreateSolidBrush(RGB(0, 0, 0));

    //字体
    newFont1.CreatePointFont(170, _T("黑体"));
    m_static_device.SetFont(&newFont1);
    m_static_hard.SetFont(&newFont1);

    //列表设置

    //字体大小
    int fontSize = 20;
    m_IPCStateList.SetHeaderFontHW(fontSize, static_cast<int>(fontSize * 0.7));
    m_HDDStateList.SetHeaderFontHW(fontSize, static_cast<int>(fontSize * 0.7));
    m_IPCStateList.SetFontHW(fontSize, static_cast<int>(fontSize * 0.7));
    m_HDDStateList.SetFontHW(fontSize, static_cast<int>(fontSize * 0.7));

    //行高
    m_IPCStateList.SetHeaderHeight(2);//表头
    m_HDDStateList.SetHeaderHeight(2);
    m_IPCStateList.SetRowHeight(30);//表
    m_HDDStateList.SetRowHeight(30);

    m_IPCStateList.SetBkColor(RGB(0, 0, 0));
    m_IPCStateList.SetTextColor(RGB(255, 255, 255));
    m_HDDStateList.SetBkColor(RGB(0, 0, 0));
    m_HDDStateList.SetTextColor(RGB(255, 255, 255));


    // TODO:  在此添加额外的初始化
    GetPrivateProfileString(_T("LT_WXCLCFG"), _T("HDD"), _T("D://"), theApp.HDDPath, 20, _T(".//LT_WXCLCFG.ini"));
    GetPrivateProfileString(_T("LT_WXCLCFG"), _T("TrainNum"), _T("No00000"), TrainNum, 50, _T(".//LT_WXCLCFG.ini"));

    InitList();
    SetList();

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_State, this, 0, NULL);//开启状态查询线程
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Record, this, 0, NULL);//开启录像线程

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}


int CManageDlg::InitList()
{
    DWORD dwStyle = m_IPCStateList.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）	
    m_IPCStateList.SetExtendedStyle(dwStyle); //设置扩展风格
    m_HDDStateList.SetExtendedStyle(dwStyle);

    m_IPCStateList.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
    m_IPCStateList.InsertColumn(1, _T("设备名称"), LVCFMT_LEFT, 120);
    m_IPCStateList.InsertColumn(2, _T("   位置"), LVCFMT_LEFT, 140);
    m_IPCStateList.InsertColumn(3, _T("版本号"), LVCFMT_LEFT, 100);
    m_IPCStateList.InsertColumn(4, _T("  状态"), LVCFMT_LEFT, 120);
    m_IPCStateList.InsertColumn(5, _T("供应商"), LVCFMT_LEFT, 115);
    m_HDDStateList.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
    m_HDDStateList.InsertColumn(1, _T("设备名称"), LVCFMT_LEFT, 120);
    m_HDDStateList.InsertColumn(2, _T(" 总容量"), LVCFMT_LEFT, 120);
    m_HDDStateList.InsertColumn(3, _T(" 剩余"), LVCFMT_LEFT, 100);
    m_HDDStateList.InsertColumn(4, _T("  状态"), LVCFMT_LEFT, 120);
    m_HDDStateList.InsertColumn(5, _T("供应商"), LVCFMT_LEFT, 135);

    return 0;
}

int CManageDlg::SetList()
{
    //IPC LIST
    for (int i = 0; i < 6; i++)
    {
        char a[20];
        _itoa_s(i + 1, a, 10);
        m_IPCStateList.InsertItem(i, a);
        sprintf_s(a, "IPC");
        m_IPCStateList.SetItemText(i, 1, a);
        m_IPCStateList.SetItemText(i, 3, _T("V1.0.0"));
        m_IPCStateList.SetItemText(i, 4, _T("未录像"));
        m_IPCStateList.SetItemTextColor(4, i, RGB(255, 0, 0));
        m_IPCStateList.SetItemText(i, 5, _T("LTDW"));
    }
    for (int i = 0; i < 12; i++)
    {
        char ipc[60] = "";
        char temp[20] = "";
        sprintf_s(temp, "IPC%d", i + 1);
        GetPrivateProfileString(_T("LT_WXCLCFG"), temp, _T("无"), ipc, 60, _T(".//LT_WXCLCFG.ini"));
        strcpy_s(IPCName[i], ipc);
    }

    m_IPCStateList.SetItemText(0, 2, _T("路况"));
    m_IPCStateList.SetItemText(1, 2, _T("司机室"));
    m_IPCStateList.SetItemText(2, 2, _T("高压室1"));
    m_IPCStateList.SetItemText(3, 2, _T("高压室2"));
    m_IPCStateList.SetItemText(4, 2, _T("左走廊"));
    m_IPCStateList.SetItemText(5, 2, _T("右走廊"));

    //HDD LIST
    m_HDDStateList.InsertItem(0, _T("1"));
    m_HDDStateList.SetItemText(0, 1, _T("硬盘"));
    m_HDDStateList.SetItemText(0, 2, _T("0"));
    m_HDDStateList.SetItemText(0, 3, _T("0"));
    m_HDDStateList.SetItemText(0, 4, _T("错误"));
    m_HDDStateList.SetItemTextColor(4, 0, RGB(255, 0, 0));
    m_HDDStateList.SetItemText(0, 5, _T("LTDW"));

    return 0;
}


void CManageDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    if (num++ > 0)
    {
        CRect rc;
        this->GetClientRect(&rc);
        GetDlgItem(IDC_STATIC1)->MoveWindow(
            rc.right / 20, rc.bottom / 20, rc.right / 20 * 18, rc.bottom / 15 * 8 + 10);
        m_IPCStateList.MoveWindow(
            rc.right / 20 + 30, rc.bottom / 15 + 30,
            (rc.right / 20 * 18 - 60), (rc.bottom / 15 * 8 - 50));

        GetDlgItem(IDC_STATIC2)->MoveWindow(
            rc.right / 20, rc.bottom / 15 * 10, rc.right / 20 * 18, rc.bottom / 15 * 4 + 10);
        m_HDDStateList.MoveWindow(
            rc.right / 20 + 30, rc.bottom / 15 * 10 + 40,
            rc.right / 20 * 18 - 60, rc.bottom / 15 * 4 - 48);
    }
}

int CManageDlg::m_RecordCallBack(LONG nPort, char* error)
{
    RecordFlag[nPort - 1] = 0;
    return 0;
}


HBRUSH CManageDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (nCtlColor == CTLCOLOR_DLG)      //对话框颜色  
        return m_brush;

    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetTextColor(RGB(255, 255, 255));
        // 模式设置透明的话，则忽略静态控件的背景颜色设置，与对话框颜色融合
        pDC->SetBkMode(TRANSPARENT);
        hbr = (HBRUSH)m_brush;
    }

    return hbr;
}
