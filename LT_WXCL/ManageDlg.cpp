// ManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "ManageDlg.h"
#include "afxdialogex.h"

#include "VideoPlay.h"
#include "ManageView.h"
#include "hdd_state.h"
#include "log.h"
#include "LogView.h"

// CManageDlg 对话框

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
    DDX_Control(pDX, IDC_PROGRAM_VERSION, program_version);
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

            OnlineDev[i] = 1;
        }
        else
        {
            m_IPCStateList.SetItemText(i, 4, _T("未录像"));
            m_IPCStateList.SetItemTextColor(4, i, RGB(255, 0, 0));

            OnlineDev[i] = 0;
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
        PLOGD << "Deleting..." << filePath;
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
                if (theApp.Local[1] == 'A') {
                    File.Format("rtsp://admin:hk123456@192.168.104.7%d:554/Streaming/Channels/101", i);
                }
                else {
                    File.Format("rtsp://admin:hk123456@192.168.104.8%d:554/Streaming/Channels/101", i);
                }

                if (Video_StartRecord(i + 1, File.GetBuffer(File.GetLength()),
                    Path.GetBuffer(Path.GetLength()),
                    TrainNum, IPCName[(theApp.Local[1] == 'A' ? i : i + 6)],
                    (theApp.Local[1] == 'A' ? i : i + 6) + 1) == -1)
                {
                    dlg->RecordFlag[i] = 0;
                    TRACE("%d port failed\n", i);
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
                if (theApp.Local[1] == 'A') {
                    File.Format("rtsp://admin:hk123456@192.168.104.8%d:554/Streaming/Channels/101", i - 6);
                }
                else {
                    File.Format("rtsp://admin:hk123456@192.168.104.7%d:554/Streaming/Channels/101", i - 6);
                }

                if (Video_StartRecord(i + 1, File.GetBuffer(File.GetLength()),
                    Path.GetBuffer(Path.GetLength()),
                    TrainNum, IPCName[(theApp.Local[1] == 'A' ? i : i - 6)],
                    (theApp.Local[1] == 'A' ? i : i - 6) + 1) == -1)
                {
                    dlg->RecordFlag[i] = 0;
                    TRACE("%d port failed\n", i);
                    sprintf_s(sysLog, "%d 通道录像连接错误！\n", i);
                    auto IPCNum = (theApp.Local[1] == 'A' ? i : i + 6);
                    logn::camera_connect_failed(IPCNum);
                    LogView::Update();
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
    auto arr = { 0, 1 };
    for (auto i : arr) {
        auto ip = fmt::format("192.168.104.20{}", i);
        if ((theApp.Local[1] == 'A' && i == 0) ||
            (theApp.Local[1] == 'B' && i == 1)) {
            ip = "localhost";
        }
        double total{ 0 };
        double used{ 0 };
        double free{ 0 };
        bool ret{ false };
        ret = get_hdd_state(ip, total, used, free);
        auto str_total = fmt::format("{:.1f}G", total);
        auto str_free = fmt::format("{:.1f}G", free);
        if (ret) {
            m_HDDStateList.SetItemText(i, 2, str_total.c_str());
            m_HDDStateList.SetItemText(i, 3, str_free.c_str());
            m_HDDStateList.SetItemText(i, 4, TEXT("正常"));
            m_HDDStateList.m_ItemTextColor.RemoveAll();
            m_HDDStateList.SetItemTextColor(4, i, RGB(0, 255, 0));
        }
        else {
            m_HDDStateList.SetItemText(i, 2, TEXT("0G"));
            m_HDDStateList.SetItemText(i, 3, TEXT("0G"));
            m_HDDStateList.SetItemText(i, 4, TEXT("错误"));
            m_HDDStateList.m_ItemTextColor.RemoveAll();
            m_HDDStateList.SetItemTextColor(4, i, RGB(255, 0, 0));
        }
    }



    //U
    if (URecordFlag && strcmp(UPath, ""))
    {
        //////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////
        ULARGE_INTEGER uFreeAv, uTotalBytes, uFreeBytes;
        if (GetDiskFreeSpaceEx(UPath, &uFreeAv, &uTotalBytes, &uFreeBytes))
        {
            //////////////////////////////////////////////////////////////////////////
            TRACE("u = %d\n", uFreeBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024));


            //////////////////////////////////////////////////////////////////////////
            //小于2G时删除
            if (uFreeAv.QuadPart / (ULONGLONG)(1024 * 1024 * 1024) <= 1 && (uTotalBytes.QuadPart / (ULONGLONG)(1024 * 1024 * 1024)) > 0)
            {

                CString uPath;
                uPath.Format("%s/%s/", UPath, FindDir(UPath));
                //uPath.Format("%s/LT-VIDEO-%s-北京蓝天多维/",UPath,TrainNum);
                //uPath.Format("%s/6A-VIDEO-%s-北京蓝天多维/",UPath,TrainNum);

                FindAndDeleteRecord(uPath);

            }
        }
    }



    return 0;
}

BOOL CManageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetWindowText("manage_dialog");

    ManageView::init();

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


    GetPrivateProfileString(_T("LT_WXCLCFG"), _T("HDD"), _T("D://"), theApp.HDDPath, 20, _T(".//LT_WXCLCFG.ini"));

    InitList();
    SetList();

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_State, this, 0, NULL);//开启状态查询线程
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Record, this, 0, NULL);//开启录像线程

    // 软件版本显示
    std::string version = ManageView::get_version();
    CString version_cstr{ version.c_str() };
    program_version.SetWindowText(version_cstr);
    //字体
    program_version.SetFont(&newFont1);
    // 位置
    CRect rc;
    this->GetClientRect(&rc);
    program_version.MoveWindow(20, 530, 100, 50);

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
        char a[20] = {'\0'};
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
    char local_name[20] = "";
    GetPrivateProfileString(TEXT("LT_WXCLCFG"), "Local", "4A", local_name, 20, _T(".//LT_WXCLCFG.ini"));
    int i_start = 0;
    if (local_name[1] == 'B') {
        i_start = 6;
    }
    for (int i = i_start; i < i_start + 6; i++) {
        m_IPCStateList.SetItemText(i - i_start, 2, IPCName[i]);
    }


    //HDD LIST
    for (auto i : { 0,1 }) {
        if (i == 0) {
            m_HDDStateList.InsertItem(i, _T("A"));
        }
        else {
            m_HDDStateList.InsertItem(i, _T("B"));
        }
        m_HDDStateList.SetItemText(i, 1, _T("硬盘"));
        m_HDDStateList.SetItemText(i, 2, _T("0G"));
        m_HDDStateList.SetItemText(i, 3, _T("0G"));
        m_HDDStateList.SetItemText(i, 4, _T("错误"));
        m_HDDStateList.SetItemTextColor(4, i, RGB(255, 0, 0));
        m_HDDStateList.SetItemText(i, 5, _T("LTDW"));
    }


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
            (rc.right / 20 * 18 - 40), (rc.bottom / 15 * 8 - 30));

        GetDlgItem(IDC_STATIC2)->MoveWindow(
            rc.right / 20, rc.bottom / 15 * 10, rc.right / 20 * 18, rc.bottom / 15 * 4 + 10);
        m_HDDStateList.MoveWindow(
            rc.right / 20 + 30, rc.bottom / 15 * 10 + 30,
            rc.right / 20 * 18 - 60, rc.bottom / 15 * 4 - 28);
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
