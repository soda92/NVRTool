#pragma once
#include <string>

extern char Global_UPath[20]; // u��ת��·��
extern char Global_TrainNum[50]; // ���ͳ���
extern char Global_IPCName[12][50]; //����ͨ������

int WINAPI Thread_URecord(LPVOID lpPara);
int WINAPI Thread_UDisk_Process(LPVOID lpPara);


namespace ManageView {
    void init();
    std::string get_version();
}
