#pragma once
#include <cstdio>
#include <string>
#include <fmt/core.h>
#include <boost/json.hpp>
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
