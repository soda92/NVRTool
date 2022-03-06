cmake_minimum_required(VERSION "3.12")
project("TaiYuan")

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2)

set(SRCS
"config.h"
"FireData.cpp"
"FireData.h"
"FireMsgDlg.cpp"
"FireMsgDlg.h"
"FireMsgView.cpp"
"FireMsgView.h"
"hdd_state.h"
"HeaderCtrlCl.cpp"
"HeaderCtrlCl.h"
"http_get.cpp"
"http_get.h"
"ListCtrlCl.cpp"
"ListCtrlCl.h"
"log.cpp"
"log.h"
"LogDlg.cpp"
"LogDlg.h"
"LogView.cpp"
"LogView.h"
"FireVideo.cpp"
"FireVideo.h"
"FireVideoDlg.cpp"
"FireVideoDlg.h"
"FireVideoView.cpp"
"FireVideoView.h"
"FireVideo.rc"
"ManageDlg.cpp"
"ManageDlg.h"
"ManageView.cpp"
"ManageView.h"
"MyButton.cpp"
"MyButton.h"
"MyTabCtrl.cpp"
"MyTabCtrl.h"
"progress_bar.cpp"
"progress_bar.h"
"resource.h"
"stdafx.cpp"
"stdafx.h"
"targetver.h"
"TaxData.cpp"
"TaxData.h"
"util.cpp"
"util.h"
"VideoDlg.cpp"
"VideoDlg.h"
"VideoPlayWnd.cpp"
"VideoPlayWnd.h"
"WaitDlg.cpp"
"WaitDlg.h"
)

find_package(Boost REQUIRED COMPONENTS filesystem json python38)
if (Boost_FOUND)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
endif()
find_package(fmt CONFIG REQUIRED)

include_directories("HCNetSDK/include" "include")
link_directories("HCNetSDK/lib")

add_executable(FireVideo WIN32 ${SRCS})
set_target_properties(FireVideo PROPERTIES COMPILE_FLAGS "/EHsc /wd4566")

target_link_libraries(FireVideo PRIVATE fmt::fmt HCNetSDK)
target_link_libraries(FireVideo PRIVATE Boost::boost Boost::json Boost::filesystem)

# include_directories("C:\\Users\\18463\\AppData\\Local\\Programs\\Python\\Python38\\include")
# link_directories("C:\\Users\\18463\\AppData\\Local\\Programs\\Python\\Python38\\libs")
add_library(stream_lib SHARED stream_lib.cpp)
target_link_libraries(stream_lib PRIVATE HCNetSDK Boost::boost Boost::python38)
