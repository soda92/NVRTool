#pragma once

#include <string>
// ��Ҫ��localhost������2s���ӳ�
// �ο�: https://stackoverflow.com/a/49052765/12291425
std::string http_get(std::string path, int port = 5000, std::string ip = "127.0.0.1");

