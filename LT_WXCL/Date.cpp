#include "StdAfx.h"
#include "Date.h"

CDate::CDate() :year(1900), month(1), day(1) {}

CDate::CDate(int y, int m, int d) : year(y), month(m), day(d) {}

CDate::CDate(string strDate)
{
	if(strDate.length()!=8)
	{
		return ; 
	}
	year=(strDate[0]-48)*1000 + (strDate[1]-48)*100 + (strDate[2]-48)*10 + (strDate[3]-48);
	
	month = (strDate[4]-48)*10 + (strDate[5]-48);

	day = (strDate[6]-48)*10 + (strDate[7]-48);
	
	return ;
}


int CDate::getYear() 
{
	return year;
}

int CDate::getMonth() 
{
	return month;
}

int CDate::getDay() 
{
	return day;
}

bool CDate::setDate(int y, int m, int d) 
{
	year = y; month = m; day = d;
	return 1;
}

bool CDate::setDate( string strDate )
{
	if(strDate.length()!=8)
	{
		return 0; 
	}
	year=(strDate[0]-48)*1000 + (strDate[1]-48)*100 + (strDate[2]-48)*10 + (strDate[3]-48);

	month = (strDate[4]-48)*10 + (strDate[5]-48);

	day = (strDate[6]-48)*10 + (strDate[7]-48);

	return 1;
}

void CDate::print() 
{
	printf("Today is %d/%d/%d\n", year, month, day);
	addOneDay();
	printf("Tomorrow is %d/%d/%d\n", year, month, day);
}

void CDate::addOneDay() {
	int daysOfMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if(year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
		daysOfMonth[1]++;
	day++;
	if(day > daysOfMonth[month - 1]) {
		day = 1;
		month++;
		if(month > 12) {
			month = 1;
			year++;
		}
	}
}

void CDate::addDay( int n )
{
	for(int i=0;i<n;i++)
	{
		addOneDay();
	}
}

string CDate::tostring()
{
	string date;
	
	int num=year*10000+month*100+day;
	int temp=10000000;
	for(int i=0;i<8;i++)
	{
		date += ('0'+ num/temp);
		num = num%temp;
		temp=temp/10;
	}
	return date;
}

