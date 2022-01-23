#pragma once
#include "D:/src/vcpkg/installed/x86-windows/include/httplib.h"
#include <cstdio>
#include <string>
#include <fmt/core.h>
#include <boost/json/src.hpp>
using namespace boost::json;
using namespace std;

/// <summary>
/// 获取磁盘使用情况
/// </summary>
/// <param name="ip">IP地址</param>
/// <param name="total">[out] 总容量</param>
/// <param name="used">[out] 已使用</param>
/// <param name="free">[out] 剩余</param>
/// <returns>是否成功</returns>
bool get_hdd_state(std::string ip, double &total, double& used, double& free) {
    // HTTP
    auto address = fmt::format("http://{}:8080", ip);
    httplib::Client cli(address);
    auto res = cli.Get("/size");
    if (res && res->status == 200) {
        printf("%d %s\n", res->status, res->body.c_str());
        value jv = parse(res->body);
        total = jv.at("total").as_double();
        used = jv.at("used").as_double();
        free = jv.at("free").as_double();
        printf("total: %lf, used: %lf, free: %lf\n", total, used, free);

        if (total == 0) {
            return false;
        }

        return true;
    }
    else {
        return false;
    }
    return false;
}
