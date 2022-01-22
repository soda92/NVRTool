#pragma once
#include <string>

extern char Global_UPath[20]; // u盘转存路径
extern char Global_TrainNum[50]; // 车型车号
extern char Global_IPCName[12][50]; //保存通道名称

int WINAPI Thread_URecord(LPVOID lpPara);
int WINAPI Thread_UDisk_Process(LPVOID lpPara);


namespace ManageView {
    void init();
    std::string get_version();
}
