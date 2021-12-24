#include "stdafx.h"


#include "LT_LCWB_1A.h"
#include "LT_LCWB_1ADlg.h"

#include "VideoPlay.h"
#include "ManageDlg.h"

#include "ManageView.h"

#include "util.h"

char UPath[20] = { 0 }; // u盘转存路径
char TrainNum[50] = { 0 }; // 车型车号
char RecordFlag[20] = { 0 }; //录像保存标志
char IPCName[12][50] = { 0 }; //保存通道名称
char IPCToCh[12] = { 0 }; //保存IPC与通道的对应关系

namespace ManageView {
    void init() {
        GetPrivateProfileString("LT_WXCLCFG", "TrainNum", "No0000", TrainNum, 50, ".//LT_WXCLCFG.ini");

        for (int i = 0; i < theApp.IPCNum; i++)
        {
            char ipc[60] = "";
            char temp[20] = "";
            sprintf_s(temp, "IPC%d", i + 1);
            GetPrivateProfileString("LT_WXCLCFG", temp, "IPC", ipc, 60, ".//LT_WXCLCFG.ini");

            IPCToCh[i] = GetPrivateProfileInt("CHSet", temp, 0, ".//LT_WXCLCFG.ini");

            //IPCName.Add(ipc);
            strcpy_s(IPCName[i], ipc);
        }
    }
}

std::array<bool, 6> UCFlag{ false };

int WINAPI Thread_URecord(LPVOID lpPara)
{
    CManageDlg* dlg = (CManageDlg*)lpPara;

    //log
    PLOGD << "U盘外挂";

    char sysLog[256] = "";
    //创建8个文件夹
    CString Path;
    CString File;

    Path.Format("%slost+found", UPath);
    int res = 0;
    if (CreateDirectory(Path, NULL) == 0)
    {
        res = GetLastError();
        if (res == 3)
        {
            AfxMessageBox("录像存储路径不正确，请修改配置文件中的路径。");
            return -1;
        }
    }

    Path.Format("%s/LT-VIDEO-%s-北京蓝天多维/", UPath, TrainNum);
    CreateDirectory(Path, NULL);

    while (dlg->URecordFlag)
    {
        for (int i = 0; i < 6; i++)
        {

            if (UCFlag[i] == true)
            {
                /*if (((CLT_LCWB_1ADlg*)theApp.m_pMainWnd)->lUserID[i] == -1) {
                    continue;
                }*/

                if (dlg->URecordStatus[i] == false)
                {
                    //Path.Format("%s%d/",UPath,i+1);
                    //rtsp://admin:hk123456@192.168.101.77:554/mpeg4/ch1/sub
                    if (theApp.Local[1] == 'A') {
                        File.Format("rtsp://admin:hk123456@192.168.104.7%d:554/Streaming/Channels/101", i);
                    }
                    else {
                        File.Format("rtsp://admin:hk123456@192.168.104.8%d:554/Streaming/Channels/101", i);
                    }

                    if (Video_StartRecord(i + 1, File.GetBuffer(File.GetLength()),
                        Path.GetBuffer(Path.GetLength()),
                        TrainNum, IPCName[(theApp.Local[1] == 'A' ? i : i + 6)],
                        // ID，用于停止录像
                        (theApp.Local[1] == 'A' ? i : i + 6) + 1) == -1)
                    {
                        dlg->URecordStatus[i] = false;
                        PLOGD << IPCName[(theApp.Local[1] == 'A' ? i : i + 6)] << " 通道录像连接错误！";
                    }
                    else
                    {
                        dlg->URecordStatus[i] = true;
                        PLOGD << IPCName[(theApp.Local[1] == 'A' ? i : i + 6)] << " 通道开始录像...";
                    }

                    Sleep(50); // 必须等待一会, 否则会出现录像文件存到下一个文件夹的问题。
                }
            }

        }
        Sleep(5 * 1000);
    }
    for (int i = 0; i < 6; i++)
    {
        Video_StopRecord(i + 10 + 1);
        dlg->URecordStatus[i] = false;
    }
    return 0;
}


int CManageDlg::StartURecord(char* uPath)
{
    if (URecordFlag)
    {
        URecordFlag = FALSE;
        Sleep(12 * 1000);//wait 12s
        for (int i = 0; i < 6; i++)
        {
            Video_StopRecord(i + 10 + 1);
            URecordStatus[i] = false;
        }
        Sleep(1000);
    }

    char FilePath[60] = "";
    strcpy_s(FilePath, uPath);
    strcat_s(FilePath, "/license.txt");

    if (util::URecordConfigAnalyse(FilePath, UCFlag) == -1)
    {
        URecordFlag = FALSE;
        return -1;
    }
    else
        URecordFlag = TRUE;

    strcpy_s(UPath, uPath);

    //////////////////////////////////////////////////////////////////////////
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_URecord, this, 0, NULL);//开启录像线程
    return 0;
}

int DownConfigAnalyse(char* path, unsigned short* cflag, char* year, char* month, char* day, char* hour, char* minute, char* span)
{
    FILE* fd = NULL;
    fopen_s(&fd, path, "rb");
    if (!fd)
        return -2;
    char buf[2 * 1024] = "";
    char* FindTemp = nullptr;
    fread(buf, 1, sizeof(buf), fd);
    char usr[20] = "";
    char psw[20] = "";

    //////////////////////////////////////////////////////////////////////////
    //用户密码验证
    FindTemp = strstr(buf, "Username");
    if (FindTemp)
    {
        FindTemp = strchr(FindTemp, '[');
        if (FindTemp)
        {
            int len = strchr(FindTemp, ']') - FindTemp - 1;
            memcpy(usr, FindTemp + 1, len);
        }
    }
    FindTemp = strstr(buf, "Password");
    if (FindTemp)
    {
        FindTemp = strchr(FindTemp, '[');
        if (FindTemp)
        {
            int len = strchr(FindTemp, ']') - FindTemp - 1;
            memcpy(psw, FindTemp + 1, len);
        }
    }
    if (strcmp(usr, "6A") || strcmp(psw, "A6"))
    {
        fclose(fd);
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////
    //通道解析
    char Channel[60] = "";
    FindTemp = strstr(buf, "Channel");
    if (FindTemp)
    {
        FindTemp = strchr(FindTemp, '[');
        if (FindTemp)
        {
            int len = strchr(FindTemp, ']') - FindTemp - 1;
            memcpy(Channel, FindTemp + 1, len);
        }
    }
    if (!strcmp(Channel, ""))
    {
        fclose(fd);
        return -1;
    }
    //解析要下载的通道
    if (strcmp(Channel, "00"))
    {
        char* temp = Channel;
        int pos = 0;
        for (int i = 0; i < 8; i++)
        {
            pos = atoi(temp);
            // TODO
            *cflag |= 1 << pos - 1;
            temp = strchr(temp, ',');
            if (temp == nullptr)
            {
                break;
            }
            temp++;
        }

    }
    else
        *cflag = 0xFFFF;

    //////////////////////////////////////////////////////////////////////////
    //时间解析
    char timeTemp[60] = "";

    FindTemp = strstr(buf, "TimeFrom");
    if (FindTemp)
    {
        FindTemp = strchr(FindTemp, '[');
        if (FindTemp)
        {
            int len = strchr(FindTemp, ']') - FindTemp - 1;
            memcpy(timeTemp, FindTemp + 1, len);
        }
    }
    if (!strcmp(timeTemp, ""))
    {
        fclose(fd);
        return -1;
    }

    memcpy(year, timeTemp, 4);
    memcpy(month, timeTemp + 5, 2);
    memcpy(day, timeTemp + 8, 2);
    memcpy(hour, timeTemp + 11, 2);
    memcpy(minute, timeTemp + 14, 2);

    //char timeSpan[10] = "";

    FindTemp = strstr(buf, "TimeSpan");
    if (FindTemp)
    {
        FindTemp = strchr(FindTemp, '[');
        if (FindTemp)
        {
            int len = strchr(FindTemp, ']') - FindTemp - 1;
            memcpy(span, FindTemp + 1, len);
        }
    }
    //////////////////////////////////////////////////////////////////////////

    fclose(fd);
    return 0;
}

int WINAPI Thread_DownLoad(LPVOID lpPara)
{
    CLT_LCWB_1ADlg* dlg = (CLT_LCWB_1ADlg*)lpPara;

    char FilePath[60] = "";
    char UPath[60] = "";
    strcpy_s(FilePath, dlg->m_ManageDlg.szRootPathName);
    strcat_s(FilePath, "\\license.txt");
    strcpy_s(UPath, dlg->m_ManageDlg.szRootPathName);
    unsigned short CFlag = 0;//通道标记，8位代表8个通道，10011代表下载1，2，5通道
    char TimeBe[30] = "";//开始时间
    char TimeEn[30] = "";//结束时间

    char year[10] = "";
    char month[10] = "";
    char day[10] = "";
    char hour[10] = "";
    char minute[10] = "";
    char span[10] = "";

    int Res = DownConfigAnalyse(FilePath, &CFlag, year, month, day, hour, minute, span);
    if (Res == -1)
    {
        dlg->m_ManageDlg.StartURecord(UPath);
        return -1;
    }
    else if (Res == -2)
    {
        memset(UPath, 0, sizeof(UPath));
        memset(dlg->m_ManageDlg.szRootPathName, 0, sizeof(dlg->m_ManageDlg.szRootPathName));
        return -1;
    }

    //log
    PLOGD << "U盘下载";

    sprintf_s(TimeBe, "%s%s%s_%s%s", year, month, day, hour, minute);
    /*char Dhour = 0;
    Dhour = strtod(hour,nullptr)+strtod(span,nullptr);
    sprintf_s(TimeEn,"%s%s%s_%d%s",year,month,day,Dhour,minute);*/
    char Dhour = 0;
    CTime time(atoi(year), atoi(month), atoi(day), atoi(hour), atoi(minute), 0);
    CTimeSpan TimeSpan(0, atoi(span), 0, 0);
    time += TimeSpan;
    sprintf_s(TimeEn, "%d%02d%02d_%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());

    //IOCtrl(LED_DOWNLOAD, TRUE);
    //dlg->SetDlgItemText(IDC_BUTTON_URE, "下载");
    //dlg->GetDlgItem(IDC_BUTTON_URE)->ShowWindow(TRUE);
    //////////////////////////////////////////////////////////////////////////
    //创建路径
    strcat_s(dlg->m_ManageDlg.szRootPathName, "\\LT_视频下载\\");
    CreateDirectory(dlg->m_ManageDlg.szRootPathName, NULL);
    char trainNum[50] = "";
    GetPrivateProfileString("LT_WXCLCFG", "TrainNum", "No0000", trainNum, 50, ".//LT_WXCLCFG.ini");
    strcat_s(dlg->m_ManageDlg.szRootPathName, trainNum);
    CreateDirectory(dlg->m_ManageDlg.szRootPathName, NULL);
    sprintf_s(dlg->m_ManageDlg.szRootPathName, "%s\\%s-%s\\", dlg->m_ManageDlg.szRootPathName, TimeBe, TimeEn);
    CreateDirectory(dlg->m_ManageDlg.szRootPathName, NULL);


    //////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////////////

    CString SourPath;
    CString SourPath2;
    //CString DestPath;

    CString FileBe;
    CString FileEn;

    char RePath[20] = "";
    GetPrivateProfileString("LT_WXCLCFG", "HDD", "D://", RePath, 20, ".//LT_WXCLCFG.ini");

    //////////////////////////////////////////////////////////////////////////
    if (!dlg->m_ManageDlg.IsHDD(RePath))
    {
        if (dlg->m_ManageDlg.IsHDD("e://"))
        {
            strcpy_s(RePath, "e://");
        }
        else if (dlg->m_ManageDlg.IsHDD("f://"))
        {
            strcpy_s(RePath, "f://");
        }
    }
    //////////////////////////////////////////////////////////////////////////
    //SourPath.Format("%s/LT-VIDEO-%s-北京蓝天多维/%s-%s-%s/",RePath,trainNum,year,month,day);
    SourPath.Format("%s/LT-VIDEO-%s-北京蓝天多维/%s-%s-%s/", RePath, trainNum, year, month, day);
    /*if (atoi(day) != time.GetDay())
    {
        SourPath2.Format("%s/LT-VIDEO-%s-北京蓝天多维/%02d-%02d-%02d/",RePath,trainNum,time.GetYear(),time.GetMonth(),time.GetDay());
    }*/

    CFileFind ff;
    CString FPath;
    CString FileName;

    FPath.Format("%s/*.mp4", SourPath);
    BOOL res = ff.FindFile(FPath);
    while (res)
    {
        res = ff.FindNextFile();
        if (res >= 0)
        {
            FileName = ff.GetFileName();
            if (!ff.IsDirectory() && !ff.IsDots())
            {
                int pos = FileName.Find("蓝天多维");
                if (pos <= 0)
                {
                    continue;
                }
                CString temp = FileName;
                temp.Delete(0, pos + 9);
                // TODO
                int ch = strtod(temp, NULL);

                if (CFlag & 1 << (ch - 1))
                {
                    temp.Delete(0, 3);
                    pos = temp.Find("_");
                    if (pos <= 0)
                    {
                        continue;
                    }
                    temp.Delete(0, pos + 1);
                    temp.Delete(13, 4);
                    if (strcmp(temp.GetBuffer(), TimeBe) >= 0 && strcmp(temp.GetBuffer(), TimeEn) <= 0)
                    {

                        CString SourFile;
                        CString DestFile;
                        SourFile = SourPath + FileName;
                        DestFile = dlg->m_ManageDlg.szRootPathName + FileName;
                        TRACE("sourFile = %s,desFile = %s\n", SourFile, DestFile);
                        CopyFile(SourFile, DestFile, FALSE);
                    }

                }

            }
        }
    }
    ff.Close();
    //////////////////////////////////////////////////////////////////////////
    if (atoi(day) != time.GetDay())
    {
        //SourPath2.Format("%s/LT-VIDEO-%s-北京蓝天多维/%02d-%02d-%02d/",RePath,trainNum,time.GetYear(),time.GetMonth(),time.GetDay());
        SourPath2.Format("%s/6A-VIDEO-%s-北京蓝天多维/%02d-%02d-%02d/", RePath, trainNum, time.GetYear(), time.GetMonth(), time.GetDay());
        FPath.Format("%s/*.mp4", SourPath2);
        res = ff.FindFile(FPath);
        while (res)
        {
            res = ff.FindNextFile();
            if (res >= 0)
            {
                FileName = ff.GetFileName();
                if (!ff.IsDirectory() && !ff.IsDots())
                {
                    int pos = FileName.Find("蓝天多维");
                    if (pos <= 0)
                    {
                        continue;
                    }
                    CString temp = FileName;
                    temp.Delete(0, pos + 9);
                    // TODO
                    int ch = strtod(temp, NULL);

                    if (CFlag & 1 << (ch - 1))
                    {
                        temp.Delete(0, 3);
                        pos = temp.Find("_");
                        if (pos <= 0)
                        {
                            continue;
                        }
                        temp.Delete(0, pos + 1);
                        temp.Delete(13, 4);
                        if (strcmp(temp.GetBuffer(), TimeBe) >= 0 && strcmp(temp.GetBuffer(), TimeEn) <= 0)
                        {

                            CString SourFile;
                            CString DestFile;
                            SourFile = SourPath2 + FileName;
                            DestFile = dlg->m_ManageDlg.szRootPathName + FileName;
                            TRACE("sourFile = %s,desFile = %s\n", SourFile, DestFile);
                            CopyFile(SourFile, DestFile, FALSE);
                        }

                    }

                }
            }
        }
        ff.Close();
    }

    //IOCtrl(LED_DOWNLOAD, FALSE);
    //dlg->GetDlgItem(IDC_BUTTON_URE)->ShowWindow(FALSE);

    //U盘插入时优先下载录像，然后在检测是否转存	

    //dlg->m_ManagerDlg.StartURecord(UPath);


    //////////////////////////////////////////////////////////////////////////	

    return 0;
}
