/*
*  Gregorian Solar Calendar and Chinese Lunar Calendar
*  农历; 阳历; 阴历; 节日; 时区; 节气; 干支; 生肖
*  1900-2100 超酷万年历
*  本类作者：webmote 2006-09 参考自互联网流行的 1900-2100 超酷万年历,并修改了一部分BUG
*  有任何问题，请email： luo31@yahoo.com.cn
*  ！！！！引用本类，请不要删除此段注释！！！！
*/
#pragma once

#include <atlbase.h>
#include <atlstr.h>

class CLunar
{
	struct Date
	{
		unsigned short	year;
		unsigned char	month;
		unsigned char	day;
		unsigned char   day_in_week;
	};

public:
	CLunar(void);
	~CLunar(void);

public:


	//农历
	int     m_lYear;    //公元年4位数字
	int     m_lMonth;   //农历月数字
	int     m_lDay;     //农历日数字
	CString m_cYearName; //年号纪年表示
	CString m_cWordDay; //文字表示的日期
	CString m_cMonthSB; //月大 小？
	BOOL    m_isLeap;  //是否为农历闰月?
	//八字
	CString m_cYear;   //年柱, 2个中文
	CString m_cMonth;  //月柱, 2个中文
	CString m_cDay;    //日柱, 2个中文
	CString m_cAnimal; //动物属相 

	//节日
	CString m_lunarFestival; //农历节日
	CString m_solarFestival; //公历节日
	CString m_solarTerms;    //节气

	BOOL    m_bStrong;       //强调的节日 放假日

	/************************************************************************/
	/* 计算农历日期                                                         */
	/************************************************************************/
	//计算农历
	void Lunar(Date tmMeas);

	static bool IsLeapYear(int iYear) {return !(iYear%4)&&(iYear%100) || !(iYear%400);}
	static int	DaysBetween(WORD iEndYear, WORD iEndMonth, WORD iEndDay, WORD iStartYear = 1900, WORD iStartMonth = 1, WORD iStartDay = 1);
	static bool DateToLunar(const Date &date, Date& lunar);
private:
	/************************************************************************/
	/* 农[阴]历所使用的全局常量                                                 */
	/************************************************************************/
	// 日期资料
	static const int m_lunarInfo[]; //农历日期
	static const CString m_GAN[]; //天干
	static const CString m_ZHI[]; //地支
	static const CString m_Animals[]; //生肖
	static const CString m_nStr1[]; //数字的中文描述
	static const CString m_nStr2[]; //数字的中文描述

	//农历节日
	static const CString m_lFtv[];

	/************************************************************************/
	/* 公[阳]历所使用的全局常量                                                 */
	/************************************************************************/ 
	static const int m_solarMonth[];    //月的天数
	static const CString m_solarTerm[];   //节气的中文描述
	static const int     m_sTermInfo[]; //节气的日期信息
	static const CString m_monthName[]; //月的英文描述

	//公历节日 *表示放假日
	static const CString m_sFtv[];
	//某月的第几个星期几。 5,6,7,8 表示到数第 1,2,3,4 个星期几
	static const CString m_wFtv[];



	//检测时间
	Date  m_tmMeas; //检测日期


private:
	
	int getDayCountInYear(int y);					//返回农历某年的总天数
	int getDayCountInMonth(int y,int m);			//返回农历某年某月的总天数

	int getLeapMonthInYear(int y);					//返回农历某年闰哪个月 1-12 , 没闰返回 0
	int getDayCountInLeapMonth(int y);				//返回农历某年闰月的天数
	
	int getSTerm(int y,int n);						//某年的第n个节气为几日(从0小寒起算)
	
	CString getWordDay(int d);						//得到农历天的描述
	
	CString getYearName(int y);						//得到年号纪年 描述

	//得到干支纪年
	CString getGZ(LONGLONG num);
	//从节日字符里解析出数字，描述，*
	void parseFtv(CString strInfo,int& nFirst,int& nLast,CString& strDesc,BOOL& bStrong);

	//得到当日为本月的第几周
	int getWeekOfMonth(int y,int m,int d);

	//得到当月有几周
	int getWeeksOfMonth(int y,int m);

	//计算指定年代与 UTC时间相差的 100ns 数
	LONGLONG getUTC(int nYear,
		int nMonth,
		int nDay,
		int nHour=0,
		int nMin=0,
		int nSec=0);

};

