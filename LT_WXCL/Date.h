#pragma once

#include <string>
using std::string;

class CDate {

public:
	CDate();
	CDate(int y, int m, int d);
	CDate(string strDate);
	int getYear();
	int getMonth();
	int getDay();
	bool setDate(int y, int m, int d);
	bool setDate(string strDate);
	void print();
	void addOneDay();
	void addDay(int n);
	string tostring();//20190101 格式
	CDate stringtodate(string strDate); //20190101 格式

private:
	int year;
	int month;
	int day;

};