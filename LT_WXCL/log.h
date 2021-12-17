#pragma once
#include "stdafx.h"
#include <string>

namespace logn {

    struct log
    {
        std::wstring time;
        int level;
        std::wstring message;
    };

    void update();
    void addlog(log log);
    void load();
    void save();
    void system_exit();
    void system_start();
}
