#pragma once
#include <string>

extern char UPath[20]; // u��ת��·��
extern char TrainNum[50]; // ���ͳ���
extern char RecordFlag[20]; //¼�񱣴��־
extern char IPCName[12][50]; //����ͨ������

int WINAPI Thread_URecord(LPVOID lpPara);
int WINAPI Thread_DownLoad(LPVOID lpPara);


namespace ManageView {
    void init();
    std::string get_version();
}
