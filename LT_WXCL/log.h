#pragma once
#include "stdafx.h"
#include <string>

namespace logn {

    struct log
    {
        std::string time;
        int level;
        std::string message;
    };

    //void update();
    void addlog(log log);
    void addlog(int level, std::string message);
    void load();
    void save();
    /// <summary>
    /// 程序退出
    /// </summary>
    void system_exit();
    /// <summary>
    /// 程序启动
    /// </summary>
    void system_start();
    /// <summary>
    /// 相机连接失败
    /// </summary>
    void camera_connect_failed(int IPCNum);

    std::string get_date();

    extern std::vector<logn::log> logs;
}
