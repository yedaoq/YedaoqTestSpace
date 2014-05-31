#pragma once

#define LUNAR_START_YEAR	1901
#define LUNAR_END_YEAR		2050

#include <Windows.h>
#include <tchar.h>
#include <string>

class CCalendarCore{
public:
	typedef unsigned char	month_t;
	typedef unsigned char	day_t;
	typedef unsigned short	year_t;

	static const month_t FLAG_MONTH_IS_LEAP = 0x80;

public:
	//判断iYear是不是闰年
	static BOOL IsLeapYear(WORD iYear) {return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//计算iYear,iMonth,iDay对应是星期几 1年1月1日 --- 65535年12月31日
	static WORD WeekNumOfDate(WORD iYear, WORD iMonth, WORD iDay);

	//计算出指定月份的周数
	static WORD WeekCountInMonth(WORD iYear, WORD iMonth);

	//计算指定天是该月的第几周
	static WORD WeekCountFromMonthBegin(WORD iYear, WORD iMonth, WORD iDay);

	//返回iYear年iMonth月的天数 1年1月 --- 65535年12月
	static WORD DayCountInMonth(WORD iYear, WORD iMonth);

	//返回阴历iLunarYer年阴历iLunarMonth月的天数，如果iLunarMonth为闰月，
	//高字为第二个iLunarMonth月的天数，否则高字为0 
	// 1901年1月---2050年12月
	static LONG DayCountInLunarMonth(WORD iLunarYear, WORD iLunarMonth);

	//返回阴历iLunarYear年的总天数  // 1901年1月---2050年12月
	static WORD DayCountInLunarYear(WORD iLunarYear);

	//返回阴历iLunarYear年的闰月月份，如没有返回0 // 1901年1月---2050年12月
	static WORD GetLeapMonth(WORD iLunarYear);

	// 获取农历年的生肖/干支纪元
	static WCHAR GetLunarYearAnimal(WORD iLunarYear);
 	static WCHAR GetLunarYearEraGan(WORD iLunarYear);
 	static WCHAR GetLunarYearEraZhi(WORD iLunarYear);

	//把iMonth格式化成中文字符串
	static DWORD GetMonthNameZh(WORD iMonth, BOOL bLunar);

	//把iDay格式化成中文字符串
	static DWORD GetDayNameZh(WORD iDay);

	//计算公历两个日期间相差的天数  1年1月1日 --- 65535年12月31日
	static LONG DayCountBetween(WORD iEndYear, WORD iEndMonth, WORD iEndDay, WORD iStartYear = LUNAR_START_YEAR, WORD iStartMonth =1, WORD iStartDay =1);

	//计算公历iYear年iMonth月iDay日对应的阴历日期 //1901年1月1日---2050年12月31日
	static WORD DateToLunar(WORD iYear, WORD iMonth, WORD iDay, WORD &iLunarYear, WORD &iLunarMonth, WORD &iLunarDay);

protected:
	//计算从1901年1月1日过iSpanDays天后的阴历日期
	static void   l_CalcLunarDate(WORD &iYear, WORD &iMonth ,WORD &iDay, LONG iSpanDays);
	
	//计算公历iYear年iMonth月iDay日对应的节气 0-24，0表不是节气
	//static WORD   l_GetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay, std::string& strHolidayInfoDesc);
};