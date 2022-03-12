cmake_minimum_required(VERSION "3.12")
project(app)

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
find_package(Qt5 COMPONENTS Widgets Network Gui REQUIRED)
set(CMAKE_AUTOMOC ON)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE Qt5::Widgets)
# https://stackoverflow.com/a/40572207/12291425
add_custom_command(
    TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt5::Widgets>
        $<TARGET_FILE:Qt5::Core>
        $<TARGET_FILE:Qt5::Gui>
        $<TARGET_FILE_DIR:app>
)
