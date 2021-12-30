#pragma once
#include "stdafx.h"

namespace config {
    constexpr auto start_dir = ".";
    constexpr auto py = "../../py-TaiYuan/core/python.exe";
    constexpr auto pyw = "../../py-TaiYuan/core/pythonw.exe";
#if defined(DEBUG)
    constexpr auto python_interpreter = py;
#else
    constexpr auto python_interpreter = pyw;
#endif

}
