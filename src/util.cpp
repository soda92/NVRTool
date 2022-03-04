#include "stdafx.h"

#include "util.h"

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
}

