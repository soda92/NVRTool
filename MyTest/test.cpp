#include "stdafx.h"
#include "log.h"
#include "util.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>

namespace MyTest {
	TEST(TestCaseName, TestName) {
		EXPECT_EQ(1, 1);
		EXPECT_TRUE(true);
	}

	TEST(TestReadLog, open) {
		logn::load();

		logn::addlog({ "time", 2, "111" });
		std::vector<logn::log> content = logn::logs;

		auto date = logn::get_date();

		auto i = 1;
	}

}

namespace TestUDisk {
	struct cert_urecord {
		std::string username {""};
		std::string password {""};
		std::array<bool, 6> channels {false};
	};

	std::string get_str_channel(std::array<bool, 6> channels) {
		std::string str_channel {""};
		int true_cnt = 0;
		for (const auto& i : channels) {
			if (i) {
				true_cnt += 1;
			}
		}
		if (true_cnt == 6) {
			str_channel = "00";
		}
		else {
			for (size_t i = 0; i < channels.size(); i++) {
				if (channels[i]) {
					str_channel = str_channel + "0" + std::to_string(i) + ",";
				}
			}
		}

		if (str_channel[str_channel.size() - 1] == ',') {
			str_channel = str_channel.substr(0, str_channel.size() - 1);
		}

		return str_channel;
	}

	void write_config(std::string path, cert_urecord ur) {
		std::ofstream f{ path };

		std::string str_channel = get_str_channel(ur.channels);

		f << "Username = [" << ur.username
			<< "] //**xx\n\tPassword = [" << ur.password
			<< "] !!\n  Channel = [" << str_channel << "]";

		f.close();
	}
	TEST(testURecord, parseConfig) {
		std::ofstream f{ "D:/file.txt" };
		cert_urecord ur;
		ur.username = "6A";
		ur.password = "A6";
		ur.channels = {
			true, true, true,
			false, false, false
		};

		write_config("D:/file.txt", ur);

		std::array<bool, 6> cflag{ false };
		int ret = util::URecordConfigAnalyse("D:/file.txt", cflag);
		EXPECT_TRUE(ret != -1);
		
		EXPECT_TRUE(cflag[0]);
		EXPECT_TRUE(cflag[1]);
		EXPECT_TRUE(cflag[2]);
		EXPECT_TRUE(!cflag[3]);
		EXPECT_TRUE(!cflag[4]);
		EXPECT_TRUE(!cflag[5]);
	}
}