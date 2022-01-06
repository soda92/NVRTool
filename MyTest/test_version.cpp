#include "stdafx.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace MYTEST {
	TEST(TestVersion, get_version) {
		boost::filesystem::path full_path(boost::filesystem::current_path());
		std::cout << full_path << "\n";
		std::vector<std::string> lines;
		std::ifstream in{ "CHANGELOG.md" };
		std::string line;
		while (std::getline(in, line)) {
			if (line.find('v') != std::string::npos) {
				lines.push_back(line);
			}
		}
		std::string version_str = lines[lines.size() - 1];
		std::string version = version_str.substr(version_str.find('v'),
			version_str.length() - version_str.find('v'));
	}
}