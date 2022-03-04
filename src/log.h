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
    /// �����˳�
    /// </summary>
    void system_exit();
    /// <summary>
    /// ��������
    /// </summary>
    void system_start();
    /// <summary>
    /// �������ʧ��
    /// </summary>
    void camera_connect_failed(int IPCNum);

    std::string get_date();

    extern std::vector<logn::log> logs;
}
