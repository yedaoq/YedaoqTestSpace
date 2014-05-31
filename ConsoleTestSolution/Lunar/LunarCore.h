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
	//�ж�iYear�ǲ�������
	static bool IsLeapYear(year_t iYear) {return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//����iYear,iMonth,iDay��Ӧ�����ڼ� 1��1��1�� --- 65535��12��31��
	static int  WeekNumOfDate(year_t iYear, month_t iMonth, day_t iDay);

	// ����ָ�������Ǳ��µĵڼ���
	static int  WeekCountFromMonthBegin( year_t iYear, month_t iMonth, day_t iDay );

	// ����һ�����ж�����
	static int  CCalendarCore::WeekCountInMonth( year_t iYear, month_t iMonth );

	//��������iLunarYear���������  // 1901��1��---2050��12��
	static int  DayCountInLunarYear(year_t iLunarYear);

	// ��������ĳ�µ���������������£���λ�ְ������µ�����
	static DWORD  DayCountInLunarMonth(year_t iLunarYear, month_t iLunarMonth);

	//��������iLunarYear��������·ݣ���û�з���0 // 1901��1��---2050��12��
	static int  GetLunarLeapMonth(year_t iLunarYear);

	// ��ȡũ�������Ф/��֧��Ԫ
	static WCHAR GetLunarYearAnimal(year_t iLunarYear);
 	static WCHAR GetLunarYearEraGan(year_t iLunarYear);
 	static WCHAR GetLunarYearEraZhi(year_t iLunarYear);

	//��iMonth��ʽ���������ַ���
	//static void GetMonthNameZh(month_t iMonth, WCHAR *pBuffer, BOOL bLunar);
	static DWORD GetMonthNameZh(month_t iMonth, bool bLunar);

	//��iDay��ʽ���������ַ���
	//static void GetDayNameZh(day_t iDay, WCHAR *pBuffer);
	static DWORD GetDayNameZh(day_t iDay);

	//���㹫���������ڼ���������  1��1��1�� --- 65535��12��31��
	static int  DayCountBetween(year_t iEndYear, month_t iEndMonth, day_t iEndDay, year_t iStartYear = LUNAR_START_YEAR, month_t iStartMonth =1, day_t iStartDay =1);

	// ���㹫��ĳ�µ�����
	static int  CCalendarCore::DayCountInMonth(year_t iYear, month_t iMonth);

	//���㹫��iYear��iMonth��iDay�ն�Ӧ���������� //1901��1��1��---2050��12��31��
	static bool DateToLunar(year_t iYear, month_t iMonth, day_t iDay, LunarDate& lunar_date);

protected:
	//�����1901��1��1�չ�iSpanDays������������
	static void   l_CalcLunarDate(LunarDate& lunar_date, int iSpanDays);
	
	//���㹫��iYear��iMonth��iDay�ն�Ӧ�Ľ��� 0-24��0���ǽ���
	//static WORD   l_GetLunarHolDay(year_t iYear, month_t iMonth, day_t iDay, std::string& strHolidayInfoDesc);
};