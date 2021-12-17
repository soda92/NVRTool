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

    void update();
    void addlog(log log);
    void load();
    void save();
    void system_exit();
    void system_start();
}