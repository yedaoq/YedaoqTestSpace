#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include "Lunar.h"
#include <Strsafe.h>
#include "LunarCore.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL,"Chinese-simplified");

	TCHAR buf[128];
	while(true)
	{
		std::cout << "input date (example : 20140305)\n";
		std::wcin >> buf;
		int num = _ttoi(buf);
		if(num > 99999999 || num < 10000000)
		{
			std::cout << "error date string!\n";
			continue;
		}
		
		int Year = num / 10000;
		int Month = (num % 10000) / 100;
		int Day = (num % 100);

		CCalendarCore::LunarDate date;
		CCalendarCore::DateToLunar(Year, Month, Day, date);

		WCHAR buf[128];
		DWORD zhMonth = CCalendarCore::GetMonthNameZh(date.month(), TRUE);
		DWORD zhDay = CCalendarCore::GetDayNameZh(date.day());
		
		StringCbPrintf(buf, sizeof(buf), TEXT("%c%cÄê %c%cÔÂ %c%c"), CCalendarCore::GetLunarYearEraGan(date.year()), CCalendarCore::GetLunarYearEraZhi(date.year()), HIWORD(zhMonth), LOWORD(zhMonth), HIWORD(zhDay), LOWORD(zhDay));

		std::wcout << buf << std::endl;
	}

	getchar();
	return 0;
}

