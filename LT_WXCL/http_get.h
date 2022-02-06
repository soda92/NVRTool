#pragma once

#include <string>
// 不要用localhost，会有2s的延迟
// 参考: https://stackoverflow.com/a/49052765/12291425
std::string http_get(std::string path, int port = 5000, std::string ip = "127.0.0.1");

