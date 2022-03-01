// ManageDlg.cpp : ʵ���ļ�
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

#include <boost/filesystem.hpp>
#include <thread>
using namespace std;
using namespace boost::filesystem;

// CManageDlg �Ի���

IMPLEMENT_DYNAMIC(CManageDlg, CDialogEx)

CManageDlg::CManageDlg(CWnd *pParent /*=NULL*/)
    : CDialogEx(CManageDlg::IDD, pParent)
{
    memset(RecordFlag, 0, sizeof(RecordFlag));
    num = 0;
}

CManageDlg::~CManageDlg()
{
}

void CManageDlg::DoDataExchange(CDataExchange *pDX)
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

// CManageDlg ��Ϣ�������

int CManageDlg::SetIPCState()
{
    m_IPCStateList.m_ItemTextColor.RemoveAll();
    for (int i = 0; i < 6; i++)
    {
        if (RecordFlag[i] == 1)
        {
            m_IPCStateList.SetItemText(i, 4, _T("¼��"));
            m_IPCStateList.SetItemTextColor(4, i, RGB(0, 255, 0));
        }
        else
        {
            m_IPCStateList.SetItemText(i, 4, _T("δ¼��"));
            m_IPCStateList.SetItemTextColor(4, i, RGB(255, 0, 0));
        }
    }

    return 0;
}

bool CManageDlg::IsHDD(char disk)
{
    namespace fs = boost::filesystem;
    if (fs::exists(fmt::format("{}:/force_disk.txt", disk)))
    {
        return true;
    }
    char disk_path[2]{'\0'};
    disk_path[0] = disk;
    if (GetDriveType(disk_path) == DRIVE_REMOVABLE)
    {
        return false;
    }
    return true;
}

int WINAPI Thread_Record(LPVOID lpPara)
{
    CManageDlg *dlg = (CManageDlg *)lpPara;

    for (;;)
    {
        auto test_path = fmt::format("{}:/lost+found", theApp.HDDPath);
        path p{test_path};
        if (exists(p))
        {
            remove_all(p);
        }
        try
        {
            create_directory(p);
            remove(p);
        }
        catch (...)
        {
            std::this_thread::sleep_for(3s);
            continue;
        }
        // if (!)
        // {
        //     dlg->MessageBox(TEXT("¼��洢·������ȷ�����޸������ļ��е�·����"));
        //     return -1;
        // }

        auto dir = fmt::format("{}:/LT-VIDEO-{}-���������ά/", theApp.HDDPath, Global_TrainNum);
        std::string cam_addr{};
        create_directory(dir);

        for (int i = 0; i < 12; i++)
        {
            if (dlg->RecordFlag[i] == 0)
            {
                char *ipcname;
                int nport;
                int tasknum;

                std::string ret = http_get(fmt::format("/cam/{}", i + 1));
                cam_addr = ret;

                ipcname = Global_IPCName[i];
                nport = i + 1;
                tasknum = nport - 1;
                auto rest = Video_StartRecord(
                    tasknum,
                    (char *)cam_addr.c_str(),
                    (char *)dir.c_str(),
                    Global_TrainNum, ipcname,
                    nport);

                if (rest != -1)
                {
                    dlg->RecordFlag[i] = 1;
                    PLOGD << fmt::format("{}ͨ����ʼ¼��...\n", i);
                }
                else
                {
                    dlg->RecordFlag[i] = 0;
                    PLOGD << fmt::format("{}ͨ��¼�����Ӵ���\n", i);
                    logn::camera_connect_failed(nport);
                    LogView::Update();
                }

                // ����ȴ�һ�ᡣ��������¼���ļ��浽��һ���ļ��е����⡣
                this_thread::sleep_for(50ms);
            }
        }

        this_thread::sleep_for(500ms);
    }

    return 0;
}

int WINAPI Thread_State(LPVOID lpPara)
{
    CManageDlg *dlg = (CManageDlg *)lpPara;

    // TRACE("ping = %d\n",dlg->DevPing.Ping(5,"192.168.101.1",NULL));
    for (;;)
    {
        dlg->SetIPCState();
        dlg->SetHDDState();
        this_thread::sleep_for(3s);
    }

    return 0;
}

int CManageDlg::SetHDDState()
{
    auto arr = {0, 1};
    for (auto i : arr)
    {
        auto ip = fmt::format("192.168.104.20{}", i);
        if ((theApp.Local == 'A' && i == 0) ||
            (theApp.Local == 'B' && i == 1))
        {
            ip = "127.0.0.1";
        }
        double total{0};
        double used{0};
        double free{0};
        bool ret{false};
        ret = get_hdd_state(ip, total, used, free);
        auto str_total = fmt::format("{:.1f}G", total);
        auto str_free = fmt::format("{:.1f}G", free);
        if (ret)
        {
            m_HDDStateList.SetItemText(i, 2, str_total.c_str());
            m_HDDStateList.SetItemText(i, 3, str_free.c_str());
            m_HDDStateList.SetItemText(i, 4, TEXT("����"));
            m_HDDStateList.m_ItemTextColor.RemoveAll();
            m_HDDStateList.SetItemTextColor(4, i, RGB(0, 255, 0));
        }
        else
        {
            m_HDDStateList.SetItemText(i, 2, TEXT("0G"));
            m_HDDStateList.SetItemText(i, 3, TEXT("0G"));
            m_HDDStateList.SetItemText(i, 4, TEXT("����"));
            m_HDDStateList.m_ItemTextColor.RemoveAll();
            m_HDDStateList.SetItemTextColor(4, i, RGB(255, 0, 0));
        }
    }

    return 0;
}

BOOL CManageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    SetWindowText("manage_dialog");

    ManageView::init();

    //������ˢ
    m_brush.CreateSolidBrush(RGB(0, 0, 0));

    //����
    newFont1.CreatePointFont(170, _T("����"));
    m_static_device.SetFont(&newFont1);
    m_static_hard.SetFont(&newFont1);

    //�б�����

    //�����С
    int fontSize = 20;
    m_IPCStateList.SetHeaderFontHW(fontSize, static_cast<int>(fontSize * 0.7));
    m_HDDStateList.SetHeaderFontHW(fontSize, static_cast<int>(fontSize * 0.7));
    m_IPCStateList.SetFontHW(fontSize, static_cast<int>(fontSize * 0.7));
    m_HDDStateList.SetFontHW(fontSize, static_cast<int>(fontSize * 0.7));

    //�и�
    m_IPCStateList.SetHeaderHeight(2); //��ͷ
    m_HDDStateList.SetHeaderHeight(2);
    m_IPCStateList.SetRowHeight(30); //��
    m_HDDStateList.SetRowHeight(30);

    m_IPCStateList.SetBkColor(RGB(0, 0, 0));
    m_IPCStateList.SetTextColor(RGB(255, 255, 255));
    m_HDDStateList.SetBkColor(RGB(0, 0, 0));
    m_HDDStateList.SetTextColor(RGB(255, 255, 255));

    std::string ret = http_get("/conf/Disk");
    theApp.HDDPath = ret[0];

    InitList();
    SetList();

    std::thread(Thread_State, this).detach();  //����״̬��ѯ�߳�
    std::thread(Thread_Record, this).detach(); //����¼���߳�

    // ����汾��ʾ
    std::string version = ManageView::get_version();
    CString version_cstr{version.c_str()};
    program_version.SetWindowText(version_cstr);
    //����
    program_version.SetFont(&newFont1);
    // λ��
    CRect rc;
    this->GetClientRect(&rc);
    program_version.MoveWindow(20, 530, 100, 50);

    return TRUE; // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

int CManageDlg::InitList()
{
    DWORD dwStyle = m_IPCStateList.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;          //ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;              //�����ߣ�ֻ������report����listctrl��
    m_IPCStateList.SetExtendedStyle(dwStyle); //������չ���
    m_HDDStateList.SetExtendedStyle(dwStyle);

    m_IPCStateList.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60);
    m_IPCStateList.InsertColumn(1, _T("�豸����"), LVCFMT_LEFT, 120);
    m_IPCStateList.InsertColumn(2, _T("   λ��"), LVCFMT_LEFT, 140);
    m_IPCStateList.InsertColumn(3, _T("�汾��"), LVCFMT_LEFT, 100);
    m_IPCStateList.InsertColumn(4, _T("  ״̬"), LVCFMT_LEFT, 120);
    m_IPCStateList.InsertColumn(5, _T("��Ӧ��"), LVCFMT_LEFT, 115);
    m_HDDStateList.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60);
    m_HDDStateList.InsertColumn(1, _T("�豸����"), LVCFMT_LEFT, 120);
    m_HDDStateList.InsertColumn(2, _T(" ������"), LVCFMT_LEFT, 120);
    m_HDDStateList.InsertColumn(3, _T(" ʣ��"), LVCFMT_LEFT, 100);
    m_HDDStateList.InsertColumn(4, _T("  ״̬"), LVCFMT_LEFT, 120);
    m_HDDStateList.InsertColumn(5, _T("��Ӧ��"), LVCFMT_LEFT, 135);

    return 0;
}

int CManageDlg::SetList()
{
    // IPC LIST
    for (int i = 0; i < 6; i++)
    {
        char a[20] = {'\0'};
        _itoa_s(i + 1, a, 10);
        m_IPCStateList.InsertItem(i, a);
        sprintf_s(a, "IPC");
        m_IPCStateList.SetItemText(i, 1, a);
        m_IPCStateList.SetItemText(i, 3, _T("V1.0.0"));
        m_IPCStateList.SetItemText(i, 4, _T("δ¼��"));
        m_IPCStateList.SetItemTextColor(4, i, RGB(255, 0, 0));
        m_IPCStateList.SetItemText(i, 5, _T("LTDW"));
    }

    // �����豸�������-ͨ������
    char local_name[20] = "";

    std::string ret = http_get("/local");
    strcpy_s(local_name, ret.c_str());

    int i_start = 0;
    if (local_name[0] == 'B')
    {
        i_start = 6;
    }
    for (int i = i_start; i < i_start + 6; i++)
    {
        m_IPCStateList.SetItemText(i - i_start, 2, Global_IPCName[i]);
    }

    // �����б�
    for (auto i : {0, 1})
    {
        if (i == 0)
        {
            m_HDDStateList.InsertItem(i, _T("A"));
        }
        else
        {
            m_HDDStateList.InsertItem(i, _T("B"));
        }
        m_HDDStateList.SetItemText(i, 1, _T("Ӳ��"));
        m_HDDStateList.SetItemText(i, 2, _T("0G"));
        m_HDDStateList.SetItemText(i, 3, _T("0G"));
        m_HDDStateList.SetItemText(i, 4, _T("����"));
        m_HDDStateList.SetItemTextColor(4, i, RGB(255, 0, 0));
        m_HDDStateList.SetItemText(i, 5, _T("LTDW"));
    }

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
        GetDlgItem(IDC_STATIC1)->MoveWindow(rc.right / 20, rc.bottom / 20, rc.right / 20 * 18, rc.bottom / 15 * 8 + 10);
        m_IPCStateList.MoveWindow(
            rc.right / 20 + 30, rc.bottom / 15 + 30,
            (rc.right / 20 * 18 - 40), (rc.bottom / 15 * 8 - 30));

        GetDlgItem(IDC_STATIC2)->MoveWindow(rc.right / 20, rc.bottom / 15 * 10, rc.right / 20 * 18, rc.bottom / 15 * 4 + 10);
        m_HDDStateList.MoveWindow(
            rc.right / 20 + 30, rc.bottom / 15 * 10 + 30,
            rc.right / 20 * 18 - 60, rc.bottom / 15 * 4 - 28);
    }
}

int CManageDlg::m_RecordCallBack(LONG nPort, char *error)
{
    RecordFlag[nPort - 1] = 0;
    return 0;
}

HBRUSH CManageDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (nCtlColor == CTLCOLOR_DLG) //�Ի�����ɫ
        return m_brush;

    if (nCtlColor == CTLCOLOR_STATIC)
    {
        pDC->SetTextColor(RGB(255, 255, 255));
        // ģʽ����͸���Ļ�������Ծ�̬�ؼ��ı�����ɫ���ã���Ի�����ɫ�ں�
        pDC->SetBkMode(TRANSPARENT);
        hbr = (HBRUSH)m_brush;
    }

    return hbr;
}
