#pragma once

extern char UPath[20]; // u��ת��·��
extern char TrainNum[50]; // ���ͳ���
extern char RecordFlag[20]; //¼�񱣴��־
extern char IPCName[12][50]; //����ͨ������
extern char IPCToCh[12]; //����IPC��ͨ���Ķ�Ӧ��ϵ

int WINAPI Thread_URecord(LPVOID lpPara);
int WINAPI Thread_DownLoad(LPVOID lpPara);


namespace ManageView {
    void init();
}
