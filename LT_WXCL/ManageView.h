#pragma once
#include <string>

extern char UPath[20]; // u盘转存路径
extern char TrainNum[50]; // 车型车号
extern char RecordFlag[20]; //录像保存标志
extern char IPCName[12][50]; //保存通道名称

int WINAPI Thread_URecord(LPVOID lpPara);
int WINAPI Thread_DownLoad(LPVOID lpPara);


namespace ManageView {
    void init();
    std::string get_version();
}
