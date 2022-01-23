#pragma once
#include "D:/src/vcpkg/installed/x86-windows/include/httplib.h"
#include <cstdio>
#include <string>
#include <fmt/core.h>
#include <boost/json/src.hpp>
using namespace boost::json;
using namespace std;

/// <summary>
/// ��ȡ����ʹ�����
/// </summary>
/// <param name="ip">IP��ַ</param>
/// <param name="total">[out] ������</param>
/// <param name="used">[out] ��ʹ��</param>
/// <param name="free">[out] ʣ��</param>
/// <returns>�Ƿ�ɹ�</returns>
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
