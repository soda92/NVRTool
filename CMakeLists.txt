cmake_minimum_required(VERSION "3.12")
project("TaiYuan")

find_package(fmt CONFIG REQUIRED)

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

find_package(Boost COMPONENTS filesystem json REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

include_directories("HCNetSDK\\include" "include")
link_directories("HCNetSDK\\lib")

find_package(fmt CONFIG REQUIRED)
add_executable(FireVideo WIN32 ${SRCS})
target_link_libraries(FireVideo PRIVATE fmt::fmt HCNetSDK)

target_link_libraries(FireVideo PRIVATE Boost::boost Boost::json Boost::filesystem)
set_target_properties(FireVideo PROPERTIES COMPILE_FLAGS "/EHsc /wd4566")

add_custom_command(TARGET FireVideo POST_BUILD
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/cmake_afterbuild.ps1
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "copy dlls"
)
