#include "stdafx.h"


#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"

#include "VideoPlay.h"
#include "ManageDlg.h"

#include "ManageView.h"

#include "util.h"
#include "resource.h"

#include <string>
#include <boost/filesystem.hpp>
#include <fstream>
#include <boost/process.hpp>
#include <boost/process/start_dir.hpp>

#include "config.h"
#include <boost/json.hpp>
#include "D:/src/vcpkg/installed/x86-windows/include/httplib.h"
#include "fmt/core.h"
namespace fs = boost::filesystem;

#include "log.h"
#include "logView.h"

char Global_TrainNum[50] = { 0 }; // ���ͳ���
char Global_IPCName[12][50] = { 0 }; //����ͨ������

namespace ManageView {
    void init() {
        GetPrivateProfileString("LT_WXCLCFG", "TrainNum", "No0000", Global_TrainNum, 50, ".//LT_WXCLCFG.ini");

        for (int i = 0; i < theApp.IPCNum; i++)
        {
            char ipc[60] = "";
            char temp[20] = "";
            sprintf_s(temp, "IPC%d", i + 1);
            GetPrivateProfileString("LT_WXCLCFG", temp, "IPC", ipc, 60, ".//LT_WXCLCFG.ini");

            strcpy_s(Global_IPCName[i], ipc);
        }
    }

    std::string get_version() {
        boost::filesystem::path full_path(boost::filesystem::current_path());
        
        std::vector<std::string> lines;
        std::ifstream in{ "CHANGELOG.md" };
        std::string line;
        while (std::getline(in, line)) {
            if (line.find('v') != std::string::npos) {
                lines.push_back(line);
            }
        }
        std::string version_str = lines[lines.size() - 1];
        std::string version = version_str.substr(version_str.find('v'),
            version_str.length() - version_str.find('v'));
        return version;
    }
}

std::array<bool, 6> Global_UCFlag{ false };

int WINAPI Thread_URecord(LPVOID lpPara)
{
    CManageDlg* dlg = (CManageDlg*)lpPara;

    //log
    PLOGD << "U�����";

    std::string path;
    path = fmt::format("{}lost+found", dlg->udisk_path);
    if (fs::exists(path)) {
        fs::remove_all(path);
    }
    auto ret = fs::create_directory(path);
    if (!ret)
    {
        AfxMessageBox("¼��洢·������ȷ�����޸������ļ��е�·����");
        return -1;
    }

    httplib::Client cli("http://localhost:5000");
    std::string url;

    path = fmt::format("{}/LT-VIDEO-{}-���������ά/",
        dlg->udisk_path, Global_TrainNum);

    if (!fs::exists(path)) {
        fs::create_directory(path);
    }

    while (dlg->URecordFlag)
    {
        for (int i = 0; i < 6; i++)
        {
            if (Global_UCFlag[i] == true)
            {
                if (dlg->URecordStatus[i] == false)
                {
                    auto ip_first = (theApp.Local[1] == 'A') ? 7 : 8;
                    auto ip_last = i;
                    auto port_number = (theApp.Local[1] == 'A') ? i + 1 : i + 6 + 1;
                    auto cam_addr = fmt::format("rtsp://admin:hk123456@192.168.104.{}{}:554/Streaming/Channels/101", ip_first, ip_last);

                    auto IPCNum = (theApp.Local[1] == 'A' ? i : i + 6);
                    auto IPCName = Global_IPCName[IPCNum];

                    auto ret = Video_StartRecord(
                        // ���������
                        12 + i + 1,
                        (char*)cam_addr.c_str(),
                        (char*)path.c_str(),
                        Global_TrainNum, IPCName,
                        // �ļ������ͨ����
                        port_number);

                    if (ret != -1)
                    {
                        dlg->URecordStatus[i] = true;
                        PLOGD << IPCName << " ͨ����ʼ¼��...";
                    }
                    else
                    {
                        dlg->URecordStatus[i] = false;
                        PLOGD << IPCName << " ͨ��¼�����Ӵ���";

                        url = fmt::format("/add/IPC{}[{}]���Ӵ���", IPCNum, IPCName);
                        //cli.Get(url.c_str());
                        logn::camera_connect_failed(IPCNum);
                        LogView::Update();
                    }

                    // ����ȴ�һ��, ��������¼���ļ��浽��һ���ļ��е����⡣
                    Sleep(50);
                }
            }

        }
        Sleep(5 * 1000);
    }
    for (int i = 0; i < 6; i++)
    {
        Video_StopRecord(12 + i + 1);
        dlg->URecordStatus[i] = false;
    }
    return 0;
}


int CManageDlg::StartURecord(char udisk)
{
    if (URecordFlag)
    {
        URecordFlag = false;
        Sleep(12 * 1000);//wait 12s
        for (int i = 0; i < 6; i++)
        {
            //               ����ID
            Video_StopRecord(12 + i + 1);
            URecordStatus[i] = false;
        }
        Sleep(1000);
    }

    std::string url;
    httplib::Client cli("http://localhost:5000");
    url = fmt::format("/parse_channels/{}", udisk);
    auto res = cli.Get(url.c_str());

    if (res && res->status == 200) {
        printf("%d %s\n", res->status, res->body.c_str());
        boost::json::value jv = boost::json::parse(res->body);
        auto arr = jv.at("channels").as_array();
        for (size_t i = 0; i < 6; i++) {
            Global_UCFlag[i] = arr[i].as_bool();
        }
    }

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_URecord, this, 0, NULL);//����¼���߳�
    return 0;
}

int WINAPI Thread_UDisk_Process(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

    std::string url;
    httplib::Client cli("http://localhost:5000");
    url = fmt::format("/is_record/{}", dlg->m_ManageDlg.udisk_path[0]);
    auto res = cli.Get(url.c_str());

    bool is_record = true;
    if (res && res->status == 200) {
        printf("%d %s\n", res->status, res->body.c_str());
        boost::json::value jv = boost::json::parse(res->body);
        is_record = jv.at("record").as_bool();
    }

    if (is_record)
    {
        dlg->m_ManageDlg.StartURecord(dlg->m_ManageDlg.udisk_path[0]);
        return -1;
    }

    //log
    PLOGD << "U������";

    namespace bp = boost::process;
    auto curr = boost::filesystem::current_path();

#if defined(DEBUG)
    bp::system("copy_file.exe", bp::start_dir(config::start_dir));
#else
    bp::system("copy_file.exe", "--release", "True", bp::start_dir(config::start_dir));
#endif

    return 0;
}
