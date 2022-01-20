#include "stdafx.h"

#include "util.h"

#include <string>
#include <sstream>
#include <iostream>
#include <array>

template <typename T>
std::string HexToString(T uval)
{
    std::stringstream ss;
    ss << "0x" << std::setw(sizeof(uval) * 2) << std::setfill('0') << std::hex << +uval;
    return ss.str();
}

template <typename T>
std::string HexArrayToString(T* valArray, size_t length)
{
    std::stringstream ss;
    for (size_t i = 0; i < length; i++) {
        ss << std::setw(sizeof(valArray[0]) * 2) << std::setfill('0') << std::hex << valArray[i];
    }
    return ss.str();
}



namespace util {
    template <typename T>
    std::string tostring(T* valArray, size_t length) {
        return HexArrayToString<T>(valArray, length);
    }

    int URecordConfigAnalyse(char* path, std::array<bool, 6>& cflag)
    {
        FILE* fd = NULL;
        fopen_s(&fd, path, "rb");
        if (!fd)
            return -1;
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
            for (int i = 0; i < 6; i++)
            {
                pos = atoi(temp);
                //找到IPC和通道的对应
                cflag[pos-1] = true;

                temp = strchr(temp, ',');
                if (temp == nullptr)
                {
                    break;
                }
                temp++;
            }

        }
        else {
            for (auto& i : cflag) {
                i = true;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        FindTemp = strstr(buf, "TimeFrom");
        if (FindTemp)
        {
            fclose(fd);
            return -1;
        }
        //////////////////////////////////////////////////////////////////////////
        fclose(fd);
        return 0;
    }

}

