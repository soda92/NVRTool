cmake_minimum_required(VERSION "3.12")

project(python_ext)

set(CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED True)

include_directories("../../HCNetSDK/include")

# https://stackoverflow.com/questions/9160335/os-specific-instructions-in-cmake-how-to
if (WIN32)
    link_directories("../../HCNetSDK/lib_win64")
endif (WIN32)

if (UNIX)
    link_directories("../../HCNetSDK/lib_linux64")
endif (UNIX)

find_package(Python3 COMPONENTS Development REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem json python310)
find_package(fmt CONFIG REQUIRED)
include_directories(${Boost_INCLUDE_DIRS} ${Python3_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_library(stream_lib SHARED stream_lib.cpp)
target_link_libraries(stream_lib PRIVATE HCNetSDK)
target_link_libraries(stream_lib PRIVATE Boost::boost Boost::python310)

add_library(serial_lib SHARED serial_lib.cpp)
target_link_libraries(serial_lib PRIVATE fmt::fmt Boost::boost Boost::json Boost::python310)

add_library(tax_lib SHARED tax_lib.cpp)
target_link_libraries(tax_lib PRIVATE Boost::boost Boost::json Boost::python310)
