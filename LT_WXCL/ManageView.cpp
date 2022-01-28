#include "stdafx.h"


#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"

#include "VideoPlay.h"
#include "ManageDlg.h"

#include "ManageView.h"

#include "util.h"
#include "resource.h"

#include <boost/filesystem.hpp>
#include <fstream>
#include <boost/process.hpp>
#include <boost/process/start_dir.hpp>

#include "config.h"
#include <boost/json.hpp>
#include "fmt/core.h"
namespace fs = boost::filesystem;
using namespace boost::json;

#include "log.h"
#include "logView.h"

char Global_TrainNum[50] = { 0 }; // 车型车号
char Global_IPCName[12][50] = { 0 }; //保存通道名称

namespace ManageView {
    void init() {
        std::string ret = http_get("/conf/TrainNum");
        strcpy_s(Global_TrainNum, ret.c_str());

        for (int i = 0; i < theApp.IPCNum; i++)
        {
            ret = http_get(fmt::format("/conf/IPC{}", i + 1));
            strcpy_s(Global_IPCName[i], ret.c_str());
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
    PLOGD << "U盘外挂";

    std::string path;
    path = fmt::format("{}lost+found", dlg->udisk_path);
    if (fs::exists(path)) {
        fs::remove_all(path);
    }
    auto ret = fs::create_directory(path);
    if (!ret)
    {
        AfxMessageBox("录像存储路径不正确，请修改配置文件中的路径。");
        return -1;
    }
    std::string url;

    path = fmt::format("{}/LT-VIDEO-{}-北京蓝天多维/",
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
                    auto ip_first = (theApp.Local == 'A') ? 7 : 8;
                    auto ip_last = i;
                    auto port_number = (theApp.Local == 'A') ? i + 1 : i + 6 + 1;

                    std::string cam_addr = http_get(fmt::format("/cam/{}", port_number));

                    auto IPCNum = (theApp.Local == 'A' ? i : i + 6);
                    auto IPCName = Global_IPCName[IPCNum];

                    auto rest = Video_StartRecord(
                        // 进程任务号
                        12 + i + 1,
                        (char*)cam_addr.c_str(),
                        (char*)path.c_str(),
                        Global_TrainNum, IPCName,
                        // 文件名里的通道号
                        port_number);

                    if (rest != -1)
                    {
                        dlg->URecordStatus[i] = true;
                        PLOGD << IPCName << " 通道开始录像...";
                    }
                    else
                    {
                        dlg->URecordStatus[i] = false;
                        PLOGD << IPCName << " 通道录像连接错误！";

                        url = fmt::format("/add/IPC{}[{}]连接错误", IPCNum, IPCName);
                        //cli.Get(url.c_str());
                        logn::camera_connect_failed(IPCNum);
                        LogView::Update();
                    }

                    // 必须等待一会, 否则会出现录像文件存到下一个文件夹的问题。
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
            //               任务ID
            Video_StopRecord(12 + i + 1);
            URecordStatus[i] = false;
        }
        Sleep(1000);
    }

    std::string url = fmt::format("/parse_channels/{}", udisk);
    auto res = http_get(url);

    boost::json::value jv = boost::json::parse(res);
    boost::json::array arr = jv.at("channels").as_array();
    for (size_t i = 0; i < 6; i++) {
        Global_UCFlag[i] = arr[i].as_bool();
    }

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_URecord, this, 0, NULL);//开启录像线程
    return 0;
}

int WINAPI Thread_UDisk_Process(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

    std::string url = fmt::format("/is_record/{}", dlg->m_ManageDlg.udisk_path[0]);
    auto res = http_get(url);

    bool is_record = true;
    boost::json::value jv = boost::json::parse(res);
    is_record = jv.at("record").as_bool();

    if (is_record)
    {
        dlg->m_ManageDlg.StartURecord(dlg->m_ManageDlg.udisk_path[0]);
        return -1;
    }

    //log
    PLOGD << "U盘下载";

    namespace bp = boost::process;
    auto curr = boost::filesystem::current_path();

#if defined(DEBUG)
    bp::system("copy_file.exe", bp::start_dir(config::start_dir));
#else
    bp::system("copy_file.exe", "--release", "True", bp::start_dir(config::start_dir));
#endif

    return 0;
}
