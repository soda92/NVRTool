#include "stdafx.h"

#include "FireVideo.h"
#include "FireVideoDlg.h"
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

char Global_TrainNum[50] = {0};    // 车型车号
char Global_IPCName[12][50] = {0}; //保存通道名称

namespace ManageView
{
    void init()
    {
        std::string ret = http_get("/conf/TrainNum");
        strcpy_s(Global_TrainNum, ret.c_str());

        for (int i = 0; i < theApp.IPCNum; i++)
        {
            ret = http_get(fmt::format("/conf/IPC{}", i + 1));
            strcpy_s(Global_IPCName[i], ret.c_str());
        }
    }

    std::string get_version()
    {
        boost::filesystem::path full_path(boost::filesystem::current_path());

        std::vector<std::string> lines;
        std::ifstream in{"CHANGELOG.md"};
        std::string line;
        while (std::getline(in, line))
        {
            if (line.find('v') != std::string::npos)
            {
                lines.push_back(line);
            }
        }
        std::string version_str = lines[lines.size() - 1];
        std::string version = version_str.substr(version_str.find('v'),
                                                 version_str.length() - version_str.find('v'));
        return version;
    }
}

std::array<bool, 6> Global_UCFlag{false};

int WINAPI Thread_URecord(LPVOID lpPara)
{
    return 0;
}

int CManageDlg::StartURecord(char udisk)
{
    if (URecordFlag)
    {
        URecordFlag = false;
        this_thread::sleep_for(12s); // wait 12s
        for (int i = 0; i < 6; i++)
        {
            //               任务ID
            URecordStatus[i] = false;
        }
        this_thread::sleep_for(1s);
    }

    std::string url = fmt::format("/parse_channels/{}", udisk);
    auto res = http_get(url);

    boost::json::value jv = boost::json::parse(res);
    boost::json::array arr = jv.at("channels").as_array();
    for (size_t i = 0; i < 6; i++)
    {
        Global_UCFlag[i] = arr[i].as_bool();
    }

    std::thread(Thread_URecord, this).detach(); //开启录像线程

    return 0;
}

int WINAPI Thread_UDisk_Process(LPVOID lpPara)
{
    CFireVideoDlg *dlg = (CFireVideoDlg *)lpPara;

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

    // log
    PLOGD << "U盘下载";

    namespace bp = boost::process;
    auto curr = boost::filesystem::current_path();

#if defined(DEBUG)
    bp::system("pyw.exe", "copy_file.pyw", bp::start_dir(config::start_dir));
#else
    bp::system("pyw.exe", "copy_file.pyw", bp::start_dir(config::start_dir));
#endif

    return 0;
}
