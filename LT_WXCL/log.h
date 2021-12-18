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
    void system_exit();
    void system_start();

    std::string get_date();

    extern std::vector<logn::log> logs;
}
