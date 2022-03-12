cmake_minimum_required(VERSION "3.12")
project(app)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../../HCNetSDK/include")
# https://stackoverflow.com/a/9160449/12291425
if (WIN32)
    link_directories("../../HCNetSDK/lib_win64")
endif (WIN32)

if (UNIX)
    link_directories("../../HCNetSDK/lib_linux64")
endif (UNIX)

find_package(Boost REQUIRED COMPONENTS filesystem json)
find_package(fmt CONFIG REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#  https://stackoverflow.com/a/49816516/12291425
find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)

if(DEBUG)
add_executable(app main.cpp)
else()
add_executable(app WIN32 main.cpp)
endif(DEBUG)

target_link_libraries(app PRIVATE Qt6::Widgets)

# https://stackoverflow.com/a/40572207/12291425

add_custom_target(
    deploy-dll ALL
    DEPENDS app
    COMMAND ${Qt6_DIR}/../../../bin/windeployqt.exe $<TARGET_FILE:app>
)
