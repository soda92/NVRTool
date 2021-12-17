#include "pch.h"
#include "log.h"
#include <iostream>

namespace MyTest {
	TEST(TestCaseName, TestName) {
		EXPECT_EQ(1, 1);
		EXPECT_TRUE(true);
	}

	TEST(TestReadLog, open) {
		logn::load();

		logn::addlog({ "time", 2, "111" });
		auto content = logn::logs;

		auto date = logn::get_date();

		auto i = 1;
	}
}
