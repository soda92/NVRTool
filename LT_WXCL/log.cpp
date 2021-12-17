#include "log.h"

#include "LogDlg.h"
#include "stdafx.h"
#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"

// 需要注意引用顺序，否则会导致编译错误
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/json/src.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost;
using boost::json::value_to_tag;
using boost::json::value_from_tag;
using boost::json::value;
using boost::json::value_to;
using boost::json::object;


namespace logn {
    // 日志消息结构
    struct log
    {
        std::string time;
        int level;
        std::string message;
    };
    
    value log_value;

    // 序列化
    void tag_invoke(value_from_tag, json::value& jv, log const& c)
    {
        jv = {
            { "time" , c.time },
            { "level", c.level },
            { "message", c.message }
        };
    }

    // 反序列化
    log tag_invoke(value_to_tag< log >, json::value const& jv)
    {
        object const& obj = jv.as_object();
        return log{
            value_to<std::string>(obj.at("time")),
            value_to<int>(obj.at("level")),
            value_to<std::string>(obj.at("message"))
        };
    }


    constexpr auto filename{ "systemlog.txt" };

    // 解析vector到json
    json::value parse_value(std::vector<std::string> values)
    {
        json::stream_parser p;
        json::error_code ec;
        for (const auto& value : values) {
            p.write(value);
        }
        if (ec)
            return nullptr;
        p.finish(ec);
        if (ec)
            return nullptr;
        return p.release();
    }

    /// <summary>
    /// 读取文件
    /// </summary>
    /// <returns></returns>
    std::vector<std::string> readfile() {
#ifdef DEBUG
        boost::filesystem::path full_path(boost::filesystem::current_path());
#endif // DEBUG

        std::ifstream f{ filename };
        f.imbue(std::locale(std::cout.getloc()));
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(f, line)) {
            lines.push_back(line);
        }
        return lines;
    }

    std::vector<logn::log> logs;

    /// <summary>
    /// 更新到List
    /// </summary>
    /// <param name="logs"></param>
    void update() {
        CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)theApp.m_pMainWnd;
        dlg->m_logDlg.log_list;
        auto len = logs.size();
        for (size_t i = 0; i < len; i++) {
            logn::log _log = logs[i];
            int nItem;
            // translation
            if (_log.message == "system exit") {
                nItem = dlg->m_logDlg.log_list.InsertItem(0, _TEXT("系统退出"));
            }
            else if (_log.message == "system startup") {
                nItem = dlg->m_logDlg.log_list.InsertItem(0, _TEXT("系统启动"));
            }
            else {
                nItem = dlg->m_logDlg.log_list.InsertItem(0, _TEXT("未知事件"));
            }
            dlg->m_logDlg.log_list.SetItemText(nItem, 1, CString(_log.time.c_str()));
        }
    }

    /// <summary>
    /// 加载Log
    /// </summary>
    void load() {
        auto filecontent = readfile();
        log_value = parse_value(filecontent);
        try
        {
            logs = value_to< std::vector<log>>(log_value);
        }
        catch (const std::exception&)
        {
            PLOGD << "error loading config";
            log_value = parse_value(std::vector<std::string>{"[]"});
        }
    }

    /// <summary>
    /// 保存到文件
    /// </summary>
    void save() {
        std::ofstream out{ logn::filename, std::ios::trunc };
        //date_facet* facet(new date_facet("%A %B %d, %Y"));
        //out.imbue(std::locale(std::cout.getloc()));
        out << json::serialize(json::value_from(logs));
    }

    /// <summary>
    /// 添加Log
    /// </summary>
    /// <param name="log"></param>
    void addlog(logn::log log) {
        logs.push_back(log);
        update();
        save();
    };


    void system_exit() {
        namespace pt = boost::posix_time;
        auto time = pt::to_iso_string(pt::second_clock::local_time());
        logn::log log;
        log.message = "system exit";
        log.level = 1;
        log.time = time;
        logn::addlog(log);
        logn::save();
    }

    void system_start() {
        namespace pt = boost::posix_time;
        auto time = pt::to_iso_string(pt::second_clock::local_time());
        logn::log log;
        log.message = "system startup";
        log.level = 1;
        log.time = time;
        logn::addlog(log);
        logn::save();
    }

} // namespace log
