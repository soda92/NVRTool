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
#include "progress_bar.h"
#include <fmt/core.h>
#include <boost/filesystem.hpp>
#include <D:/src/vcpkg/installed/x86-windows/include/httplib.h>
namespace fs = boost::filesystem;
extern char Global_IPCName[12][50];

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
        progress_bar::show();

#ifdef DEBUG
        //PLOGD << "fWarVoice.GetLength()" << warVoice.GetLength();
        //PLOGD << "fWarVoice" << warVoice;
        //PLOGD << "fWarVoice.GetAt(0)" << warVoice.GetAt(0);
        //PLOGD << "fWarVoice.GetAt(7)" << warVoice.GetAt(7);
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
        httplib::Client cli{ "localhost:5000" };
        auto res = cli.Get(fmt::format("/conf/{}", detector).c_str());
        std::string ret;
        if (res && res->status == 200) {
            ret = res->body;
        }
        strcpy_s(video_num_cstr, ret.c_str());

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

    while (true)
    {
        for (int i = 0; i < 6; i++) {
            auto user_id_index = 0;
            if (theApp.Local == 'A') {
                user_id_index = i;
            }
            else {
                user_id_index = i + 8;
            }
            if (dlg->lUserID[user_id_index] >= 0)
            {
                auto info_speed_mileage = fmt::format(
                    "{}km/h {:.3lf}", dlg->TaxData.Speed, dlg->TaxData.Signpost / 1000.0
                );
                auto info_trainNum_EngineNo = fmt::format(
                    "车次:{} 车号:{}", dlg->TaxData.TrainNum, dlg->TaxData.EngineNo
                );

                int ipcnum = 0;
                if (i >= 0 && i < 8) {
                    ipcnum = i;
                }
                if (i >= 8 && i < 16) {
                    ipcnum = i - 2;
                }
                auto info_position_driver = fmt::format(
                    "{} 司机:{}", Global_IPCName[ipcnum], dlg->TaxData.DriverNo
                );

                auto record_id_index = 0;
                if (theApp.Local == 'A') {
                    record_id_index = i;
                }
                else {
                    record_id_index = i + 6;
                }
                auto record_str = dlg->m_ManageDlg.RecordFlag[record_id_index] ? "REC" : "";
                auto usb_record_str = dlg->m_ManageDlg.URecordStatus[i] ? "USB" : "";
                auto info_record_status = fmt::format(
                    "{} {}", record_str, usb_record_str
                );
                dlg->OSD_impl(
                    dlg->lUserID[user_id_index],
                    info_record_status,
                    info_speed_mileage,
                    info_trainNum_EngineNo,
                    info_position_driver);
            }
        }
        Sleep(2 * 1000);
    }

    return 0;
}


int WINAPI Thread_Play(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

    while (true)
    {
        for (int i = 0; i < 32; i++)
        {
            if (dlg->lUserID[i] == -1)
            {
                string ip;
                if (i <= 5 && i >= 0) {
                    ip = fmt::format("192.168.104.7{}", i);
                }
                else if (i <= 13 && i >= 8) {
                    ip = fmt::format("192.168.104.8{}", i-8);
                }
                else {
                    continue;
                }

                int res = dlg->VideoPlay(
                    (char*)ip.c_str(),
                    dlg->lUserID[i],
                    dlg->lRealPlayHandle[i],
                    dlg->m_VideoDlg.m_videoPlayWnd[i]->GetSafeHwnd());

                if (res >= 0)
                {
                    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_SetOsd, lpPara, 0, NULL); //osd叠加线程
                }
                else {
                    dlg->lUserID[i] = -1;
                    dlg->lRealPlayHandle[i] = -1;
                }
            }
            Sleep(50);
        }

        Sleep(5 * 1000);
    }

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
//数据分类：防火数据，TAX校时，报警中断数据
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

                if (Remote[1] == 'A') //A节
                {
                    dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 0);
                }
                if (Remote[1] == 'B') //B节
                {
                    dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 1);
                }
            }
            else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x03) //校时
            {
                char Remote[10] = "";
                Remote[0] = RecBuf[2];
                Remote[1] = RecBuf[3];

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
            else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x04) //报警中断
            {
                dlg->m_FireMsgDlg.StopWarFun();
            }
        }
        memset(RecBuf, 0, sizeof(RecBuf));
    }
    return 0;
}


int WINAPI Thread_Index(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;
    //创建文件夹
    CString Path;
    CString File;

    char hddPath[20] = "";
    httplib::Client cli{ "localhost:5000" };
    auto res = cli.Get("/conf/HDD");
    std::string ret;
    if (res && res->status == 200) {
        ret = res->body;
    }
    strcpy_s(hddPath, ret.c_str());

    if (!dlg->m_ManageDlg.IsHDD(hddPath))
    {
        if (dlg->m_ManageDlg.IsHDD("e://"))
        {
            strcpy(hddPath, "e://");
        }
        else if (dlg->m_ManageDlg.IsHDD("f://"))
        {
            strcpy(hddPath, "f://");
        }
    }

    Path.Format("%slost+found", hddPath);
    int rest = 0;
    if (CreateDirectory(Path, NULL) == 0)
    {
        rest = GetLastError();
        if (rest == 3)
        {
            //AfxMessageBox("录像存储路径不正确，请修改配置文件中的路径。");
            return -1;
        }
    }

    char TrainNum[50] = "";

    res = cli.Get("/conf/TrainNum");
    if (res && res->status == 200) {
        ret = res->body;
    }
    strcpy_s(TrainNum, ret.c_str());

    Path.Format("%s/LT-VIDEO-%s-北京蓝天多维/",hddPath,TrainNum);	

    CreateDirectory(Path, NULL);

    SYSTEMTIME Time, TimeBuf;
    GetLocalTime(&TimeBuf);
    std::string FilePath;

    FilePath = fmt::format("{}/{}-{:02d}-{:02d}/", Path, TimeBuf.wYear, TimeBuf.wMonth, TimeBuf.wDay);

    if (!fs::exists(FilePath)) {
        fs::create_directory(FilePath);
    }

    GetLocalTime(&Time);
    std::string FileName;

    TAXDATA TaxData{};

    while (true)
    {
        Sleep(1000);
        TaxData = dlg->TaxData;
        GetLocalTime(&Time);
        if (Time.wYear != TimeBuf.wYear || Time.wMonth != TimeBuf.wMonth || Time.wDay != TimeBuf.wDay)
        {
            GetLocalTime(&TimeBuf);
            FilePath = fmt::format("{}/{}-{:02d}-{:02d}/", Path, TimeBuf.wYear, TimeBuf.wMonth, TimeBuf.wDay);
            if (!fs::exists(FilePath)) {
                fs::create_directory(FilePath);
            }

        }

        FileName = fmt::format("{}/{}_北京蓝天多维_{}{:02d}{:02d}.idx", FilePath, TrainNum, Time.wYear, Time.wMonth, Time.wDay);

        if (TaxData.EngineNo != 0)
        {

            //////////////////////////////////////////////////////////////////////////
            unsigned char IdxBuf[100] = "";
            // 参考文档： 6A系统视频深化统型方案 V1.2 （2014年4月）
            // 2.2	公共信息报文2（统型后使用）

            IdxBuf[0] = 0xAA;
            IdxBuf[1] = 0xAA;
            // 报文长度：0x60
            IdxBuf[2] = 0x60;
            // 报文类型：0x07
            IdxBuf[4] = 0x07;
            /*
            中央处理平台时间
            （由低到高依次为：年、月、日、时、分、秒，2000年计为0，余类推）
            */
            IdxBuf[5] = (unsigned char)(Time.wYear - 2000);
            IdxBuf[6] = (unsigned char)Time.wMonth;
            IdxBuf[7] = (unsigned char)Time.wDay;
            IdxBuf[8] = (unsigned char)Time.wHour;
            IdxBuf[9] = (unsigned char)Time.wMinute;
            IdxBuf[10] = (unsigned char)Time.wSecond;

            /*
            中央处理平台车号
            （共16个字符，由低到高依次存放，不足时补充空格）
            （例如：HXD3C0001[空格][空格][空格][空格][空格][空格][空格]）
            */
            char CheHao[16+1]{};
            // https://stackoverflow.com/a/276869/12291425
            sprintf_s(CheHao, "%-16s", TrainNum);

            memcpy_s(&IdxBuf[11], 16, CheHao, 16);
            // TAX报文有效
            IdxBuf[27] = 1;
            /*
            预留字节
            (TAX_TIME)
            */
            memset(&IdxBuf[28], 0, 4 * sizeof(IdxBuf[0]));
            /*
            车次字母部分
            （例如：LY501次的字母存放顺序为[空格][空格][L][Y]）
            */
            memcpy_s(&IdxBuf[32], 4 * sizeof(IdxBuf[0]),
                &TaxData.TrainTypeId[0], 4*sizeof(TaxData.TrainTypeId[0]));

            /*
            车次数字部分(按数值型)巩付伟标注
            （共3个字节，低位在前，高位在后）
            */
            IdxBuf[36] = TaxData.TrainNum;
            IdxBuf[37] = TaxData.TrainNum >> 8;
            IdxBuf[38] = TaxData.TrainNum >> 16;

            /*
            车站号
            */
            IdxBuf[39] = TaxData.StationNo;
            /*
            车站号扩充字节
            */
            IdxBuf[40] = 0;
            /*
            司机号
            */
            IdxBuf[41] = TaxData.DriverNo;
            IdxBuf[42] = TaxData.DriverNo >> 8;
            /*
            司机号扩充字节
            */
            IdxBuf[43] = 0;
            /*
            副司机号
            */
            IdxBuf[44] = TaxData.CopilotNo;
            IdxBuf[45] = TaxData.CopilotNo >> 8;
            /*
            副司机号扩充字节
            */
            IdxBuf[46] = 0;
            /*
            预留字节
            (CAR_NO)
            */
            memset(&IdxBuf[47], 0, 2);

            /*
            预留字节
            (CAR_TYPE)
            */
            IdxBuf[49] = 0;
            /*
            预留字节
            (CAR_TYPE_EX)
            */
            IdxBuf[50] = 0;
            /*实际交路号*/
            IdxBuf[51] = TaxData.FactRoute;
            /*
            客/货、本/补
            （b0：0/1=货/客，b1：0/1=本务/补机）
            */
            IdxBuf[52] = TaxData.TrainFlag;
            /*速度*/
            IdxBuf[53] = (unsigned char)TaxData.Speed;
            /*
            预留字节
            (SPEED)
            */
            memset(&IdxBuf[54], 0, 2);
            /*
            机车信号
            （b3～b0: 00-无灯,01-绿,02-黄,03-双黄,04-红黄,05-红,06-白,07-绿黄,08-黄2；
            b4：0/1=单灯/多灯）
            */
            IdxBuf[56] = TaxData.TrainSign;
            /*
            机车工况
            （b0：零位,b1：向后[即二端向前],b2：向前[即一端向前],b3：制动,b4：牵引）
            */
            IdxBuf[57] = TaxData.TrainState;
            /*信号机编号*/
            IdxBuf[58] = TaxData.SignNo;
            IdxBuf[59] = TaxData.SignNo >> 8;
            /*
            信号机种类
            （b2～b0: 02-出站,03-进站,04-通过,05-预告,06-容许）
            */
            IdxBuf[60] = TaxData.SignType;
            /*
            公里标
            （单位：米）
            */
            IdxBuf[61] = TaxData.Signpost;
            IdxBuf[62] = TaxData.Signpost >> 8;
            IdxBuf[63] = TaxData.Signpost >> 16;
            /*总重
            （单位：吨）
            */
            IdxBuf[64] = (unsigned char)TaxData.CarWeight;
            IdxBuf[65] = (unsigned char)(TaxData.CarWeight >> 8);
            /*
            计长
            （单位：0.1米）
            */
            IdxBuf[66] = (unsigned char)TaxData.CarLong;
            IdxBuf[67] = (unsigned char)(TaxData.CarLong >> 8);
            /*辆数*/
            IdxBuf[68] = TaxData.CarCount;
            /*列车管压力
            （b9～b0:管压[kPa],b15～b10:预留）
            */
            IdxBuf[69] = (unsigned char)TaxData.PipePressure;
            IdxBuf[70] = (unsigned char)(TaxData.PipePressure >> 8);

            /*装置状态
            （b0:1/0-降级/监控，b2:1/0-调车/非调车）
            */
            IdxBuf[71] = 0;
            /*TCMS报文有效*/
            IdxBuf[72] = 0;
            /*司机室状态
            （0-没占用，1-一端占用，2-二端占用，0xff-无效)
            */
            IdxBuf[73] = 0;
            /*预留字节
            （JCGK）
            */
            IdxBuf[74] = 0;
            /*
            受电弓状态
            （b1～b0:一端受电弓，b3～b2:二端受电弓，0-无效，1-升弓，2-降弓，3-隔离）
            */
            IdxBuf[75] = 0;
            /*主断状态
            （1-断开，2-闭合，0xFF-无效）
            */
            IdxBuf[76] = 0;
            /*
            手柄级位
            （×0.1级,0xFFFF无效）
            */
            IdxBuf[77] = 0;
            IdxBuf[78] = 0;
            /*重联信息
            （1-重联，2-不重联，0xFF-无效）
            */
            IdxBuf[79] = 0;
            /*大闸指令
            （b0-运转位，b1-初制动，b2-常用制动区，b3-全制动，b4-抑制位，b5-重联位，b6-紧急制动位，0xFF-无效）
            */
            IdxBuf[80] = 0;
            /*小闸指令
            （b0-运转位，b1-制动区，b2-全制动，0xFF-无效）
            */
            IdxBuf[81] = 0;
            /*其他指令
            （b0-LKJ制动，b1-紧急制动，b2-惩罚制动，b3-电制动，b4-停放制动施加，b5-停放制动缓解，b6-停放制动切除，0xFF-无效）
            */
            IdxBuf[82] = 0;
            /*其他指令屏蔽字节
            （位定义：1表示有效，0表示无效）
            */
            IdxBuf[83] = 0;
            /*预留字节（ZDJ）*/
            IdxBuf[84] = 0;
            IdxBuf[85] = 0;
            /*预留字节（ZDG）*/
            IdxBuf[86] = 0;
            IdxBuf[87] = 0;
            /*预留字节 (ZFG)*/
            IdxBuf[88] = 0;
            IdxBuf[89] = 0;
            /*
            特殊区域	过分相点	进入调车	人为紧急
            侧线运行  监控动作	停车事件	开车事件
            */
            IdxBuf[90] = 0;
            /*
                            减压制动	黄灯信号
            继乘交接	监控解锁	总风低压	红黄信号
            */
            IdxBuf[91] = 0;
            /*未定义*/
            IdxBuf[92] = 0;
            IdxBuf[93] = 0;
            IdxBuf[94] = 0;
            /*未定义 End*/

            /* SC */
            for (int i = 0; i < 95; i++)
            {
                IdxBuf[95] += IdxBuf[i];
            }
            //////////////////////////////////////////////////////////////////////////

            FILE* fd = fopen(FileName.c_str(), "ab+");
            if (fd)
            {
                fwrite(IdxBuf, 1, 96, fd);
                fclose(fd);
                fd = nullptr;
            }

            if (dlg->m_ManageDlg.URecordFlag)
            {
                FileName[0] = dlg->m_ManageDlg.udisk_path[0];

                fd = fopen(FileName.c_str(), "ab+");
                if (fd)
                {
                    fwrite(IdxBuf, 1, 96, fd);
                    fclose(fd);
                    fd = nullptr;
                }

            }
        }
    }


    return 0;
}
