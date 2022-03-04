cmake_minimum_required(VERSION "3.12")
project("TaiYuan")

find_package(fmt CONFIG REQUIRED)

add_definitions(-D_AFXDLL)
set(CMAKE_MFC_FLAG 2)

add_subdirectory(src)
