#pragma once

int WINAPI Thread_Voice(LPVOID lpPara);
int WINAPI Thread_SetIpcTime(LPVOID lpPara);

/// <summary>
/// UDP广播接收函数
/// </summary>
/// <param name="lpPara"></param>
/// <returns></returns>
int WINAPI Thread_UDPBroadcastRecv(LPVOID lpPara);


int WINAPI Thread_Play(LPVOID lpPara);

namespace Interactive {
    void ShowFire(CString warVoice);
}

int WINAPI Thread_Index(LPVOID lpPara);
