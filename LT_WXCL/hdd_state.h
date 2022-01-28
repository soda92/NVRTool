#pragma once
#include <cstdio>
#include <string>
#include <fmt/core.h>
#include <boost/json.hpp>
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
    auto ret = http_get("/size");
    if (ret == "") {
        return false;
    }
    value jv = parse(ret);
    total = jv.at("total").as_double();
    used = jv.at("used").as_double();
    free = jv.at("free").as_double();
    return true;
}
