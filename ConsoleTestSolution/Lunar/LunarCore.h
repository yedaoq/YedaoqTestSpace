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

	struct LunarDate
	{
		static const month_t FLAG_MONTH_IS_LEAP = 0x80;

	public:
		LunarDate()
			: year_(1900), month_(1), day_(1)
		{}

		inline void	set(year_t year, month_t month, bool is_leap, day_t day)
		{
			year_ = year;
			month_ = is_leap ? (month | FLAG_MONTH_IS_LEAP) : month;
			day_ = day;
		}

		// 		inline void		set_year(year_t year) { year_ = year; }
		// 		inline void		set_month(month_t month, bool is_leap) { month_ = is_leap ? (month | FLAG_MONTH_IS_LEAP) : month; }
		// 		inline void		set_day(day_t day) { day_ = day; }

		inline year_t	year() const { return year_; }
		inline month_t	month() const { return month_ & ~FLAG_MONTH_IS_LEAP; }
		inline bool		month_is_leap() const { return (month_ & FLAG_MONTH_IS_LEAP) > 0; }
		inline day_t	day() const { return day_; }

	protected:

		year_t	year_;
		month_t month_;
		day_t	day_;
	};


public:
	//判断iYear是不是闰年
	static bool IsLeapYear(year_t iYear) {return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//计算iYear,iMonth,iDay对应是星期几 1年1月1日 --- 65535年12月31日
	static int  WeekNumOfDate(year_t iYear, month_t iMonth, day_t iDay);

	//计算出指定月份的周数
	static int  WeekCountInMonth(year_t iYear, month_t iMonth);

	//计算指定天是该月的第几周
	static int  WeekCountFromMonthBegin(year_t iYear, month_t iMonth, day_t iDay);

	//返回iYear年iMonth月的天数 1年1月 --- 65535年12月
	static int  DayCountInMonth(year_t iYear, month_t iMonth);

	//返回阴历iLunarYer年阴历iLunarMonth月的天数，如果iLunarMonth为闰月，
	//高字为第二个iLunarMonth月的天数，否则高字为0 
	// 1901年1月---2050年12月
	static long DayCountInLunarMonth(year_t iLunarYear, month_t iLunarMonth);

	//返回阴历iLunarYear年的总天数  // 1901年1月---2050年12月
	static WORD DayCountInLunarYear(year_t iLunarYear);

	//返回阴历iLunarYear年的闰月月份，如没有返回0 // 1901年1月---2050年12月
	static WORD GetLunarLeapMonth(year_t iLunarYear);

	// 获取农历年的生肖/干支纪元
	static WCHAR GetLunarYearAnimal(year_t iLunarYear);
 	static WCHAR GetLunarYearEraGan(year_t iLunarYear);
 	static WCHAR GetLunarYearEraZhi(year_t iLunarYear);

	//把iMonth格式化成中文字符串
	static DWORD GetMonthNameZh(month_t iMonth, bool bLunar);

	//把iDay格式化成中文字符串
	static DWORD GetDayNameZh(day_t iDay);

	//计算公历两个日期间相差的天数  1年1月1日 --- 65535年12月31日
	static long DayCountBetween(year_t iEndYear, month_t iEndMonth, day_t iEndDay, year_t iStartYear = LUNAR_START_YEAR, month_t iStartMonth =1, day_t iStartDay =1);

	//计算公历iYear年iMonth月iDay日对应的阴历日期 //1901年1月1日---2050年12月31日
	static WORD DateToLunar(year_t iYear, month_t iMonth, day_t iDay, LunarDate& lunar_date);

protected:
	//计算从1901年1月1日过iSpanDays天后的阴历日期
	static void   l_CalcLunarDate(LunarDate& lunar_date, long iSpanDays);
	
	//计算公历iYear年iMonth月iDay日对应的节气 0-24，0表不是节气
	//static WORD   l_GetLunarHolDay(year_t iYear, month_t iMonth, day_t iDay, std::string& strHolidayInfoDesc);
};