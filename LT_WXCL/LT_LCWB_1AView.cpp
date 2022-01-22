#include "stdafx.h"

#include <sapi.h> // ��������ͷ�ļ�
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
        fWarVoice = "B��̽ͷ1����.....";
#endif
        // END TEST

        // ����
        //
        // ���õ�ǰѡ��
        dlg->m_TabCtrl.SetCurSel(0);
        // �л�����
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

       
        // ���
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

        // ��ȡ��ӦIPC
        httplib::Client cli{ "localhost:5000" };
        auto res = cli.Get(fmt::format("/conf/{}", detector).c_str());
        std::string ret;
        if (res && res->status == 200) {
            ret = res->body;
        }
        strcpy_s(video_num_cstr, ret.c_str());

        int video_num = std::stoi(std::string(video_num_cstr));
        PLOGD << "video num: " << video_num;
        // �л�����
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
    ISpVoice* pVoice = NULL; //��ʼ��COM
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
                    "����:{} ����:{}", dlg->TaxData.TrainNum, dlg->TaxData.EngineNo
                );

                int ipcnum = 0;
                if (i >= 0 && i < 8) {
                    ipcnum = i;
                }
                if (i >= 8 && i < 16) {
                    ipcnum = i - 2;
                }
                auto info_position_driver = fmt::format(
                    "{} ˾��:{}", Global_IPCName[ipcnum], dlg->TaxData.DriverNo
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
                    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_SetOsd, lpPara, 0, NULL); //osd�����߳�
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

//UDP�㲥���պ���
//
//���ã�����UDP�㲥��ʶ��ͷ������������Դ��Ȼ�����ݽ�����Ӧ�Ĵ�����
//
//���ݷ��ࣺ�������ݣ�TAXУʱ�������ж�����
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
            if (RecBuf[0] == 0xFF && RecBuf[1] == 0x01) //����
            {
                char Remote[10] = "";
                Remote[0] = RecBuf[2];
                Remote[1] = RecBuf[3];

                if (Remote[1] == 'A') //A��
                {
                    dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 0);
                }
                if (Remote[1] == 'B') //B��
                {
                    dlg->m_FireMsgDlg.FireDataAnalyse(&RecBuf[4], 41, 1);
                }
            }
            else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x03) //Уʱ
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
            else if (RecBuf[0] == 0xFF && RecBuf[1] == 0x04) //�����ж�
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
    //�����ļ���
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
            //AfxMessageBox("¼��洢·������ȷ�����޸������ļ��е�·����");
            return -1;
        }
    }

    char TrainNum[50] = "";

    res = cli.Get("/conf/TrainNum");
    if (res && res->status == 200) {
        ret = res->body;
    }
    strcpy_s(TrainNum, ret.c_str());

    Path.Format("%s/LT-VIDEO-%s-���������ά/",hddPath,TrainNum);	

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

        FileName = fmt::format("{}/{}_���������ά_{}{:02d}{:02d}.idx", FilePath, TrainNum, Time.wYear, Time.wMonth, Time.wDay);

        if (TaxData.EngineNo != 0)
        {

            //////////////////////////////////////////////////////////////////////////
            unsigned char IdxBuf[100] = "";
            // �ο��ĵ��� 6Aϵͳ��Ƶ�ͳ�ͷ��� V1.2 ��2014��4�£�
            // 2.2	������Ϣ����2��ͳ�ͺ�ʹ�ã�

            IdxBuf[0] = 0xAA;
            IdxBuf[1] = 0xAA;
            // ���ĳ��ȣ�0x60
            IdxBuf[2] = 0x60;
            // �������ͣ�0x07
            IdxBuf[4] = 0x07;
            /*
            ���봦��ƽ̨ʱ��
            ���ɵ͵�������Ϊ���ꡢ�¡��ա�ʱ���֡��룬2000���Ϊ0�������ƣ�
            */
            IdxBuf[5] = (unsigned char)(Time.wYear - 2000);
            IdxBuf[6] = (unsigned char)Time.wMonth;
            IdxBuf[7] = (unsigned char)Time.wDay;
            IdxBuf[8] = (unsigned char)Time.wHour;
            IdxBuf[9] = (unsigned char)Time.wMinute;
            IdxBuf[10] = (unsigned char)Time.wSecond;

            /*
            ���봦��ƽ̨����
            ����16���ַ����ɵ͵������δ�ţ�����ʱ����ո�
            �����磺HXD3C0001[�ո�][�ո�][�ո�][�ո�][�ո�][�ո�][�ո�]��
            */
            char CheHao[16+1]{};
            // https://stackoverflow.com/a/276869/12291425
            sprintf_s(CheHao, "%-16s", TrainNum);

            memcpy_s(&IdxBuf[11], 16, CheHao, 16);
            // TAX������Ч
            IdxBuf[27] = 1;
            /*
            Ԥ���ֽ�
            (TAX_TIME)
            */
            memset(&IdxBuf[28], 0, 4 * sizeof(IdxBuf[0]));
            /*
            ������ĸ����
            �����磺LY501�ε���ĸ���˳��Ϊ[�ո�][�ո�][L][Y]��
            */
            memcpy_s(&IdxBuf[32], 4 * sizeof(IdxBuf[0]),
                &TaxData.TrainTypeId[0], 4*sizeof(TaxData.TrainTypeId[0]));

            /*
            �������ֲ���(����ֵ��)����ΰ��ע
            ����3���ֽڣ���λ��ǰ����λ�ں�
            */
            IdxBuf[36] = TaxData.TrainNum;
            IdxBuf[37] = TaxData.TrainNum >> 8;
            IdxBuf[38] = TaxData.TrainNum >> 16;

            /*
            ��վ��
            */
            IdxBuf[39] = TaxData.StationNo;
            /*
            ��վ�������ֽ�
            */
            IdxBuf[40] = 0;
            /*
            ˾����
            */
            IdxBuf[41] = TaxData.DriverNo;
            IdxBuf[42] = TaxData.DriverNo >> 8;
            /*
            ˾���������ֽ�
            */
            IdxBuf[43] = 0;
            /*
            ��˾����
            */
            IdxBuf[44] = TaxData.CopilotNo;
            IdxBuf[45] = TaxData.CopilotNo >> 8;
            /*
            ��˾���������ֽ�
            */
            IdxBuf[46] = 0;
            /*
            Ԥ���ֽ�
            (CAR_NO)
            */
            memset(&IdxBuf[47], 0, 2);

            /*
            Ԥ���ֽ�
            (CAR_TYPE)
            */
            IdxBuf[49] = 0;
            /*
            Ԥ���ֽ�
            (CAR_TYPE_EX)
            */
            IdxBuf[50] = 0;
            /*ʵ�ʽ�·��*/
            IdxBuf[51] = TaxData.FactRoute;
            /*
            ��/������/��
            ��b0��0/1=��/�ͣ�b1��0/1=����/������
            */
            IdxBuf[52] = TaxData.TrainFlag;
            /*�ٶ�*/
            IdxBuf[53] = (unsigned char)TaxData.Speed;
            /*
            Ԥ���ֽ�
            (SPEED)
            */
            memset(&IdxBuf[54], 0, 2);
            /*
            �����ź�
            ��b3��b0: 00-�޵�,01-��,02-��,03-˫��,04-���,05-��,06-��,07-�̻�,08-��2��
            b4��0/1=����/��ƣ�
            */
            IdxBuf[56] = TaxData.TrainSign;
            /*
            ��������
            ��b0����λ,b1�����[��������ǰ],b2����ǰ[��һ����ǰ],b3���ƶ�,b4��ǣ����
            */
            IdxBuf[57] = TaxData.TrainState;
            /*�źŻ����*/
            IdxBuf[58] = TaxData.SignNo;
            IdxBuf[59] = TaxData.SignNo >> 8;
            /*
            �źŻ�����
            ��b2��b0: 02-��վ,03-��վ,04-ͨ��,05-Ԥ��,06-����
            */
            IdxBuf[60] = TaxData.SignType;
            /*
            �����
            ����λ���ף�
            */
            IdxBuf[61] = TaxData.Signpost;
            IdxBuf[62] = TaxData.Signpost >> 8;
            IdxBuf[63] = TaxData.Signpost >> 16;
            /*����
            ����λ���֣�
            */
            IdxBuf[64] = (unsigned char)TaxData.CarWeight;
            IdxBuf[65] = (unsigned char)(TaxData.CarWeight >> 8);
            /*
            �Ƴ�
            ����λ��0.1�ף�
            */
            IdxBuf[66] = (unsigned char)TaxData.CarLong;
            IdxBuf[67] = (unsigned char)(TaxData.CarLong >> 8);
            /*����*/
            IdxBuf[68] = TaxData.CarCount;
            /*�г���ѹ��
            ��b9��b0:��ѹ[kPa],b15��b10:Ԥ����
            */
            IdxBuf[69] = (unsigned char)TaxData.PipePressure;
            IdxBuf[70] = (unsigned char)(TaxData.PipePressure >> 8);

            /*װ��״̬
            ��b0:1/0-����/��أ�b2:1/0-����/�ǵ�����
            */
            IdxBuf[71] = 0;
            /*TCMS������Ч*/
            IdxBuf[72] = 0;
            /*˾����״̬
            ��0-ûռ�ã�1-һ��ռ�ã�2-����ռ�ã�0xff-��Ч)
            */
            IdxBuf[73] = 0;
            /*Ԥ���ֽ�
            ��JCGK��
            */
            IdxBuf[74] = 0;
            /*
            �ܵ繭״̬
            ��b1��b0:һ���ܵ繭��b3��b2:�����ܵ繭��0-��Ч��1-������2-������3-���룩
            */
            IdxBuf[75] = 0;
            /*����״̬
            ��1-�Ͽ���2-�պϣ�0xFF-��Ч��
            */
            IdxBuf[76] = 0;
            /*
            �ֱ���λ
            ����0.1��,0xFFFF��Ч��
            */
            IdxBuf[77] = 0;
            IdxBuf[78] = 0;
            /*������Ϣ
            ��1-������2-��������0xFF-��Ч��
            */
            IdxBuf[79] = 0;
            /*��բָ��
            ��b0-��תλ��b1-���ƶ���b2-�����ƶ�����b3-ȫ�ƶ���b4-����λ��b5-����λ��b6-�����ƶ�λ��0xFF-��Ч��
            */
            IdxBuf[80] = 0;
            /*Сբָ��
            ��b0-��תλ��b1-�ƶ�����b2-ȫ�ƶ���0xFF-��Ч��
            */
            IdxBuf[81] = 0;
            /*����ָ��
            ��b0-LKJ�ƶ���b1-�����ƶ���b2-�ͷ��ƶ���b3-���ƶ���b4-ͣ���ƶ�ʩ�ӣ�b5-ͣ���ƶ����⣬b6-ͣ���ƶ��г���0xFF-��Ч��
            */
            IdxBuf[82] = 0;
            /*����ָ�������ֽ�
            ��λ���壺1��ʾ��Ч��0��ʾ��Ч��
            */
            IdxBuf[83] = 0;
            /*Ԥ���ֽڣ�ZDJ��*/
            IdxBuf[84] = 0;
            IdxBuf[85] = 0;
            /*Ԥ���ֽڣ�ZDG��*/
            IdxBuf[86] = 0;
            IdxBuf[87] = 0;
            /*Ԥ���ֽ� (ZFG)*/
            IdxBuf[88] = 0;
            IdxBuf[89] = 0;
            /*
            ��������	�������	�������	��Ϊ����
            ��������  ��ض���	ͣ���¼�	�����¼�
            */
            IdxBuf[90] = 0;
            /*
                            ��ѹ�ƶ�	�Ƶ��ź�
            �̳˽���	��ؽ���	�ܷ��ѹ	����ź�
            */
            IdxBuf[91] = 0;
            /*δ����*/
            IdxBuf[92] = 0;
            IdxBuf[93] = 0;
            IdxBuf[94] = 0;
            /*δ���� End*/

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
