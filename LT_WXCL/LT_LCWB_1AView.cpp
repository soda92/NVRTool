#include "stdafx.h"

#include <sapi.h> // 导入语音头文件
#include "sphelper.h"
#pragma comment(lib, "sapi.lib")

#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"
#include "LT_LCWB_1AView.h"

#include "log.h"
#include "LogView.h"
#include <string>
#include <sstream>

namespace Interactive {
    void log(CString warnStr) {
        char AB = warnStr.GetAt(0);
        char num = warnStr.GetAt(7);

        std::stringstream ss;
        ss << "fire";
        ss << AB;
        ss << num;

        logn::addlog(1, ss.str());
    }

    void ShowFire(CString warVoice) {
        Interactive::log(warVoice);
        LogView::Update();

        CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)theApp.m_pMainWnd;
        // TEST
#ifdef TEST_DISABLED
    }
        else {
        fWarVoice = "B节探头1报警.....";
#endif
        // END TEST

        // 联动
        //
        // 设置当前选项
        dlg->m_TabCtrl.SetCurSel(0);
        // 切换窗口
        dlg->m_VideoDlg.ShowWindow(SW_SHOW);
        dlg->m_ManageDlg.ShowWindow(SW_HIDE);
        dlg->m_FireMsgDlg.ShowWindow(SW_HIDE);
        dlg->m_logDlg.ShowWindow(SW_HIDE);

#ifdef DEBUG
        PLOGD << "fWarVoice.GetLength()" << warVoice.GetLength();
        PLOGD << "fWarVoice" << warVoice;
        PLOGD << "fWarVoice.GetAt(0)" << warVoice.GetAt(0);
        PLOGD << "fWarVoice.GetAt(7)" << warVoice.GetAt(7);
#endif // DEBUG

        if (warVoice.GetAt(0) == 'A')
        {
            PLOGD << "switch to A...";
            if (dlg->m_VideoDlg.CurrentBox != 0) {
                dlg->m_VideoDlg.OnBnClickedButtonBa();
            }
        }
        if (warVoice.GetAt(0) == 'B')
        {
            PLOGD << "switch to B...";
            if (dlg->m_VideoDlg.CurrentBox != 1) {
                dlg->m_VideoDlg.OnBnClickedButtonBa();
            }
        }

       
        // 最大化
        auto detector_num = 0;
        if (warVoice.GetAt(0) == 'A')
        {
            auto c1 = warVoice.GetAt(7);
            detector_num = warVoice.GetAt(7) - '0';
        }
        else
        {
            detector_num = warVoice.GetAt(7) - '0' + 4;
        }
        PLOGD << "detector num: " << detector_num;
        auto detector = "detector" + std::to_string(detector_num);
        char video_num_cstr[21] = { '\0' };
        // 读取对应IPC
        GetPrivateProfileString("LT_WXCLCFG", detector.c_str(), "0", video_num_cstr, 20, ".//LT_WXCLCFG.ini");
        int video_num = std::stoi(std::string(video_num_cstr));
        PLOGD << "video num: " << video_num;
        // 切换窗口
        dlg->m_VideoDlg.ChangeWndRects(0, video_num - 1);
    }
}


int WINAPI Thread_Voice(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

    if (FAILED(CoInitialize(NULL)))
    {
        AfxMessageBox(_T("Error to intiliaze COM"));
        return -1;
    }

    Sleep(3 * 1000);

    HRESULT hr = E_FAIL;
    ISpVoice* pVoice = NULL; //初始化COM
    hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
    pVoice->SetRate(1);
    ISpObjectToken* p = NULL;

    char lan = -1;

    while (1)
    {
        if ((dlg==nullptr) || dlg->firemsgdlg_exited) {
            continue;
        }

        UCHAR f_add = 0;
        CString fWarVoice = "";
        for (int i = 0; i < 2; i++)
        {
            if (dlg->m_FireMsgDlg.f_fire_tex[i] != "")
            {
                fWarVoice += dlg->m_FireMsgDlg.f_fire_tex[i];
                f_add++;
            }
        }

        if (f_add > 0)
        {
            hr = SpFindBestToken(SPCAT_VOICES, L"language=804", NULL, &p);
            if (SUCCEEDED(hr))
            {
                pVoice->SetVoice(p);
            }
            pVoice->Speak(fWarVoice.AllocSysString(), 0, NULL);
            pVoice->WaitUntilDone(INFINITE);

            Interactive::ShowFire(fWarVoice);
        }

        Sleep(3 * 1000);
    }

    pVoice->Release();
    pVoice = NULL;

    CoUninitialize();
    return 0;
}


int WINAPI Thread_SetOsd(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;
    int i = dlg->OsdIndex;

    TRACE("osd i = %d\n", i);

    while (1)
    {
        char Speed[20] = "";
        char Mileage[20] = "";
        char CheCi[20] = "";
        char CheHao[20] = "";
        char SiJiHao[20] = "";

        double LiCheng = dlg->TaxData.Signpost / 1000.0;

        sprintf_s(CheCi, "%d", dlg->TaxData.TrainNum);

        sprintf_s(Speed, "%d", dlg->TaxData.Speed);
        sprintf_s(Mileage, "%.3lf", LiCheng);
        sprintf_s(CheHao, "%d", dlg->TaxData.EngineNo);
        sprintf_s(SiJiHao, "%d", dlg->TaxData.DriverNo);

        if (dlg->lUserID[i] >= 0)
        {
            dlg->VideoOSDSet(&dlg->lUserID[i], Speed, Mileage, CheCi, CheHao, (i > 7 ? i - 8 : i), SiJiHao);
        }

        Sleep(2 * 1000);
    }

    return 0;
}


int WINAPI Thread_Play(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;
    char ip[30] = "";
    //Sleep(5 * 1000);

    int tmp = (theApp.Local[1] == 'A' ? 0 : 8);

    for (int i = tmp; i < (8 + tmp); i++)
    {
        dlg->OsdIndex = i;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_SetOsd, lpPara, 0, NULL); //osd叠加线程
        Sleep(50);
    }

    /*while (1)
    {*/
    for (int i = 0; i < 32; i++)
    {

        if (dlg->lUserID[i] == -1)
        {

            if (i <= 5 && i >= 0)
                sprintf_s(ip, "192.168.10%d.7%d", atoi(&theApp.Local[0]), i);
            else if (i <= 13 && i >= 8)
                sprintf_s(ip, "192.168.10%d.8%d", atoi(&theApp.Local[0]), i - 8);
            else
                continue;

            TRACE("ipc ip = %s\n", ip);
            strcpy_s(dlg->ip[i], ip);

            int res = dlg->VideoPlay(dlg->ip[i], &(dlg->lUserID[i]), &(dlg->lRealPlayHandle[i]),
                dlg->m_VideoDlg.m_videoPlayWnd[i]->GetSafeHwnd());
            if (res < 0)
            {
                //TRACE("ipc %s error\n",ip);
                dlg->lUserID[i] = -1;
                dlg->lRealPlayHandle[i] = -1;
            }
        }
        Sleep(50);
    }

    Sleep(5 * 1000);
    //}

    return 0;
}

int WINAPI Thread_SetIpcTime(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

    while (1)
    {
        Sleep(10 * 60 * 1000);
        dlg->TimeCFG();
    }
    return 0;
}

//UDP广播接收函数
//
//作用：接收UDP广播，识别报头，区分数据来源，然后将数据交给对应的处理函数
//
//数据分类：防火数据，采集盒数据，报警中断数据，空转数据
//
int WINAPI Thread_UDPBroadcastRecv(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

    struct sockaddr_in addr = { 0 };
    int addrLen = sizeof(addr);
    unsigned char RecBuf[1024] = "";

    while (1)
    {
        int res = recvfrom(dlg->BRecUdp, (char*)RecBuf, sizeof(RecBuf), 0, (struct sockaddr*)&addr, &addrLen);
        if (res > 0)
        {
            if (RecBuf[0] == 0xFF && RecBuf[1] == 0x01) //防火
            {
                char Remote[10] = "";
                Remote[0] = RecBuf[2];
                Remote[1] = RecBuf[3];

                if (Remote[0] == theApp.Local[0] && Remote[1] == 'A') //本车A
                {
                    dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 0);
                }
                if (Remote[0] == theApp.Local[0] && Remote[1] == 'B') //本车B
                {
                    dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 1);
                }
                if (Remote[0] != theApp.Local[0] && Remote[1] == 'A') //他车A
                {
                    dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 2);
                }
                if (Remote[0] != theApp.Local[0] && Remote[1] == 'B') //他车B
                {
                    dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 3);
                }
            }
            else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x02) //采集盒
            {
                char Remote[10] = "";
                Remote[0] = RecBuf[2];
                Remote[1] = RecBuf[3];

            }
            else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x03) //校时
            {
                char Remote[10] = "";
                Remote[0] = RecBuf[2];
                Remote[1] = RecBuf[3];

                if (strcmp(Remote, theApp.Local) && Remote[0] == theApp.Local[0])
                {
                    memcpy(&dlg->TaxData, &RecBuf[4], sizeof(dlg->TaxData));

                    if (dlg->TaxData.TAXTime.Year != 0 && dlg->TaxData.TAXTime.Month != 0
                        && dlg->TaxData.TAXTime.Day != 0)
                    {
                        CTime curTime = CTime::GetCurrentTime();
                        CTime TaxTime(dlg->TaxData.TAXTime.Year, dlg->TaxData.TAXTime.Month,
                            dlg->TaxData.TAXTime.Day, dlg->TaxData.TAXTime.Hour,
                            dlg->TaxData.TAXTime.Minute, dlg->TaxData.TAXTime.Second);
                        CTimeSpan span = curTime - TaxTime;

                        if (span.GetTotalSeconds() > 60 || span.GetTotalSeconds() < -60)
                        {
                            //MainDlg->SetFireText("set time");
                            TRACE("tax set time\n");
                            SYSTEMTIME time;
                            GetLocalTime(&time);
                            time.wYear = dlg->TaxData.TAXTime.Year;
                            time.wMonth = dlg->TaxData.TAXTime.Month;
                            time.wDay = dlg->TaxData.TAXTime.Day;
                            time.wHour = dlg->TaxData.TAXTime.Hour;
                            time.wMinute = dlg->TaxData.TAXTime.Minute;
                            time.wSecond = dlg->TaxData.TAXTime.Second;

                            SetLocalTime(&time);
                        }
                    }
                }
            }
            else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x04) //报警中断
            {
                dlg->m_FireMsgDlg.StopWarFun();
            }
            else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x05) //空转
            {
                char Remote[10] = "";
                Remote[0] = RecBuf[2];
                Remote[1] = RecBuf[3];
            }
        }
        memset(RecBuf, 0, sizeof(RecBuf));
    }
    return 0;
}
