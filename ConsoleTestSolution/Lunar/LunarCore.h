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
	static BOOL IsLeapYear(WORD iYear) {return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	//����iYear,iMonth,iDay��Ӧ�����ڼ� 1��1��1�� --- 65535��12��31��
	static WORD WeekNumOfDate(WORD iYear, WORD iMonth, WORD iDay);

	//�����ָ���·ݵ�����
	static WORD WeekCountInMonth(WORD iYear, WORD iMonth);

	//����ָ�����Ǹ��µĵڼ���
	static WORD WeekCountFromMonthBegin(WORD iYear, WORD iMonth, WORD iDay);

	//����iYear��iMonth�µ����� 1��1�� --- 65535��12��
	static WORD DayCountInMonth(WORD iYear, WORD iMonth);

	//��������iLunarYer������iLunarMonth�µ����������iLunarMonthΪ���£�
	//����Ϊ�ڶ���iLunarMonth�µ��������������Ϊ0 
	// 1901��1��---2050��12��
	static LONG DayCountInLunarMonth(WORD iLunarYear, WORD iLunarMonth);

	//��������iLunarYear���������  // 1901��1��---2050��12��
	static WORD DayCountInLunarYear(WORD iLunarYear);

	//��������iLunarYear��������·ݣ���û�з���0 // 1901��1��---2050��12��
	static WORD GetLeapMonth(WORD iLunarYear);

	// ��ȡũ�������Ф/��֧��Ԫ
	static WCHAR GetLunarYearAnimal(WORD iLunarYear);
 	static WCHAR GetLunarYearEraGan(WORD iLunarYear);
 	static WCHAR GetLunarYearEraZhi(WORD iLunarYear);

	//��iMonth��ʽ���������ַ���
	static DWORD GetMonthNameZh(WORD iMonth, BOOL bLunar);

	//��iDay��ʽ���������ַ���
	static DWORD GetDayNameZh(WORD iDay);

	//���㹫���������ڼ���������  1��1��1�� --- 65535��12��31��
	static LONG DayCountBetween(WORD iEndYear, WORD iEndMonth, WORD iEndDay, WORD iStartYear = LUNAR_START_YEAR, WORD iStartMonth =1, WORD iStartDay =1);

	//���㹫��iYear��iMonth��iDay�ն�Ӧ���������� //1901��1��1��---2050��12��31��
	static WORD DateToLunar(WORD iYear, WORD iMonth, WORD iDay, WORD &iLunarYear, WORD &iLunarMonth, WORD &iLunarDay);

protected:
	//�����1901��1��1�չ�iSpanDays������������
	static void   l_CalcLunarDate(WORD &iYear, WORD &iMonth ,WORD &iDay, LONG iSpanDays);
	
	//���㹫��iYear��iMonth��iDay�ն�Ӧ�Ľ��� 0-24��0���ǽ���
	//static WORD   l_GetLunarHolDay(WORD iYear, WORD iMonth, WORD iDay, std::string& strHolidayInfoDesc);
};