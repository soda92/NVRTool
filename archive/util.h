#pragma once
#include <string>
#include <array>

template <typename T>
std::string HexToString(T uval);

template <typename T>
std::string HexArrayToString(T* valArray, size_t length);



namespace util {
    template <typename T>
    std::string tostring(T* valArray, size_t length);

}

