
#include "stdafx.h"

// ��Ҫע������˳�򣬷���ᵼ�±������
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/json.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include <boost/date_time/time_facet.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost;
using boost::json::value_to_tag;
using boost::json::value_from_tag;
using boost::json::value;
using boost::json::value_to;
using boost::json::object;

#include "log.h"


namespace logn {
    
    value log_value;

    // ���л�
    void tag_invoke(value_from_tag, json::value& jv, log const& c)
    {
        jv = {
            { "time" , c.time },
            { "level", c.level },
            { "message", c.message }
        };
    }

    // �����л�
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

    /// <summary>
    /// ����vector��json
    /// </summary>
    /// <param name="values">�ļ�����</param>
    /// <returns>json::value</returns>
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
    /// ��ȡ�ļ�
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
    /// ����Log
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
            logs = value_to< std::vector<log>>(log_value);
        }
    }

    /// <summary>
    /// ���浽�ļ�
    /// </summary>
    void save() {
        std::ofstream out{ logn::filename, std::ios::trunc };
        //date_facet* facet(new date_facet("%A %B %d, %Y"));
        //out.imbue(std::locale(std::cout.getloc()));
        out << json::serialize(json::value_from(logs));
    }

    /// <summary>
    /// ���Log
    /// </summary>
    /// <param name="log"></param>
    void addlog(logn::log log) {
        logs.push_back(log);
        //update();
        save();
    };

    std::string get_date() {
        namespace pt = boost::posix_time;
        auto time = pt::second_clock::local_time();
        using pt::time_facet;
        time_facet* facet(new time_facet("%Y-%m-%d %H:%M:%S%F"));
        std::stringstream ss;
        ss.imbue(std::locale(std::cout.getloc(), facet));
        ss << time;
        return ss.str();
    }


    void system_exit() {
        logn::log log;
        log.message = "system exit";
        log.level = 1;
        log.time = get_date();
        logn::addlog(log);
        logn::save();
    }

    void system_start() {
        logn::log log;
        log.message = "system startup";
        log.level = 1;
        log.time = get_date();
        logn::addlog(log);
        logn::save();
    }

} // namespace log
