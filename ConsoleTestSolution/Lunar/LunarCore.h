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
	//�ж�iYear�ǲ�������
	static BOOL IsLeapYear(year_t iYear) {return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//����iYear,iMonth,iDay��Ӧ�����ڼ� 1��1��1�� --- 65535��12��31��
	static WORD WeekNumOfDate(year_t iYear, month_t iMonth, WORD iDay);

	//�����ָ���·ݵ�����
	static WORD WeekCountInMonth(year_t iYear, month_t iMonth);

	//����ָ�����Ǹ��µĵڼ���
	static WORD WeekCountFromMonthBegin(year_t iYear, month_t iMonth, WORD iDay);

	//����iYear��iMonth�µ����� 1��1�� --- 65535��12��
	static WORD DayCountInMonth(year_t iYear, month_t iMonth);

	//��������iLunarYer������iLunarMonth�µ����������iLunarMonthΪ���£�
	//����Ϊ�ڶ���iLunarMonth�µ��������������Ϊ0 
	// 1901��1��---2050��12��
	static LONG DayCountInLunarMonth(year_t iLunarYear, month_t iLunarMonth);

	//��������iLunarYear���������  // 1901��1��---2050��12��
	static WORD DayCountInLunarYear(year_t iLunarYear);

	//��������iLunarYear��������·ݣ���û�з���0 // 1901��1��---2050��12��
	static WORD GetLeapMonth(year_t iLunarYear);

	// ��ȡũ�������Ф/��֧��Ԫ
	static WCHAR GetLunarYearAnimal(year_t iLunarYear);
 	static WCHAR GetLunarYearEraGan(year_t iLunarYear);
 	static WCHAR GetLunarYearEraZhi(year_t iLunarYear);

	//��iMonth��ʽ���������ַ���
	static DWORD GetMonthNameZh(month_t iMonth, BOOL bLunar);

	//��iDay��ʽ���������ַ���
	static DWORD GetDayNameZh(WORD iDay);

	//���㹫���������ڼ���������  1��1��1�� --- 65535��12��31��
	static LONG DayCountBetween(WORD iEndYear, month_t iEndMonth, WORD iEndDay, WORD iStartYear = LUNAR_START_YEAR, WORD iStartMonth =1, WORD iStartDay =1);

	//���㹫��iYear��iMonth��iDay�ն�Ӧ���������� //1901��1��1��---2050��12��31��
	static WORD DateToLunar(year_t iYear, month_t iMonth, WORD iDay, year_t &iLunarYear, month_t &iLunarMonth, WORD &iLunarDay);

protected:
	//�����1901��1��1�չ�iSpanDays������������
	static void   l_CalcLunarDate(year_t &iYear, month_t &iMonth ,WORD &iDay, LONG iSpanDays);
	
	//���㹫��iYear��iMonth��iDay�ն�Ӧ�Ľ��� 0-24��0���ǽ���
	//static WORD   l_GetLunarHolDay(year_t iYear, month_t iMonth, WORD iDay, std::string& strHolidayInfoDesc);
};