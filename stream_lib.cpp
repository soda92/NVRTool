extern "C"
{
#include "HCNetSDK.h"
}
#include <string>
#include <thread>
#include <chrono>
using namespace std;

bool init_ = false;
class Client
{
public:
    int user_id = -1;
    int realplay_handle = -1;
    long hwnd = -1;
    bool recording = false;
    bool main = false;
    string ip;
    Client(long hwnd, string ip) : hwnd(hwnd), ip(ip) {}

    void set_main(){
        main = true;
    }

    int realplay()
    {
        if (!init_)
        {
            NET_DVR_Init();
            NET_DVR_SetConnectTime(2000, 1);
            NET_DVR_SetReconnect(6000, true);
        }
        NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
        struLoginInfo.bUseAsynLogin = 0;                    //同步登录方式
        strcpy_s(struLoginInfo.sDeviceAddress, ip.c_str()); //设备IP地址
        struLoginInfo.wPort = 8000;                         //设备服务端口
        strcpy_s(struLoginInfo.sUserName, "admin");         //设备登录用户名
        strcpy_s(struLoginInfo.sPassword, "hk123456");      //设备登录密码

        NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

        if (user_id < 0)
        {
            user_id = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
            if (user_id < 0)
            {
                return NET_DVR_GetLastError();
            }
        }

        NET_DVR_PREVIEWINFO struPlayInfo = {0};
        struPlayInfo.hPlayWnd = reinterpret_cast<HWND>(hwnd); //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo.lChannel = 1;                            //预览通道号
        if(this->main){
            struPlayInfo.dwStreamType = 0; // 0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        }else{
            struPlayInfo.dwStreamType = 1;
        }
        struPlayInfo.dwLinkMode = 0;   // 0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP

        realplay_handle = NET_DVR_RealPlay_V40(user_id, &struPlayInfo, NULL, NULL);

        if (realplay_handle < 0)
        {
            return NET_DVR_GetLastError();
        }

        return NET_DVR_NOERROR;
    }

    void stream(){
        std::thread([this]{
                auto ret = realplay();
                while(ret!=NET_DVR_NOERROR){
                    std::this_thread::sleep_for(3s);
                    ret = realplay();
                }
            }).detach();
    }

    bool is_streaming(){
        return realplay_handle>=0;
    }
};

#include <boost/python.hpp>
using namespace boost::python;

BOOST_PYTHON_MODULE(stream_lib)
{
    class_<Client>("Client", init<long, std::string>())
        .def(init<long, std::string>())
        .def("set_main", &Client::set_main)
        .def("stream", &Client::stream)
        .def("is_streaming", &Client::is_streaming);
}
