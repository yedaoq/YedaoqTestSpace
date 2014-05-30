#include <Windows.h>
#include <tchar.h>
#include "Lunar.h"
#include <math.h>

//数组存入阴历1900年到2100年每年中的月天数信息，  
//阴历每月只能是29或30天，一年用12（或13）个二进制位表示，对应位为1表30天，否则为29天  
const int CLunar::m_lunarInfo[]={ //农历日期信息
	0x4bd8,0x4ae0,0xa570,0x54d5,0xd260,0xd950,0x5554,0x56af,0x9ad0,0x55d2,
	0x4ae0,0xa5b6,0xa4d0,0xd250,0xd255,0xb54f,0xd6a0,0xada2,0x95b0,0x4977,
	0x497f,0xa4b0,0xb4b5,0x6a50,0x6d40,0xab54,0x2b6f,0x9570,0x52f2,0x4970,
	0x6566,0xd4a0,0xea50,0x6a95,0x5adf,0x2b60,0x86e3,0x92ef,0xc8d7,0xc95f,
	0xd4a0,0xd8a6,0xb55f,0x56a0,0xa5b4,0x25df,0x92d0,0xd2b2,0xa950,0xb557,
	0x6ca0,0xb550,0x5355,0x4daf,0xa5b0,0x4573,0x52bf,0xa9a8,0xe950,0x6aa0,
	0xaea6,0xab50,0x4b60,0xaae4,0xa570,0x5260,0xf263,0xd950,0x5b57,0x56a0,
	0x96d0,0x4dd5,0x4ad0,0xa4d0,0xd4d4,0xd250,0xd558,0xb540,0xb6a0,0x95a6,
	0x95bf,0x49b0,0xa974,0xa4b0,0xb27a,0x6a50,0x6d40,0xaf46,0xab60,0x9570,
	0x4af5,0x4970,0x64b0,0x74a3,0xea50,0x6b58,0x5ac0,0xab60,0x96d5,0x92e0,
	0xc960,0xd954,0xd4a0,0xda50,0x7552,0x56a0,0xabb7,0x25d0,0x92d0,0xcab5,
	0xa950,0xb4a0,0xbaa4,0xad50,0x55d9,0x4ba0,0xa5b0,0x5176,0x52bf,0xa930,
	0x7954,0x6aa0,0xad50,0x5b52,0x4b60,0xa6e6,0xa4e0,0xd260,0xea65,0xd530,
	0x5aa0,0x76a3,0x96d0,0x4afb,0x4ad0,0xa4d0,0xd0b6,0xd25f,0xd520,0xdd45,
	0xb5a0,0x56d0,0x55b2,0x49b0,0xa577,0xa4b0,0xaa50,0xb255,0x6d2f,0xada0,
	0x4b63,0x937f,0x49f8,0x4970,0x64b0,0x68a6,0xea5f,0x6b20,0xa6c4,0xaaef,
	0x92e0,0xd2e3,0xc960,0xd557,0xd4a0,0xda50,0x5d55,0x56a0,0xa6d0,0x55d4,
	0x52d0,0xa9b8,0xa950,0xb4a0,0xb6a6,0xad50,0x55a0,0xaba4,0xa5b0,0x52b0,
	0xb273,0x6930,0x7337,0x6aa0,0xad50,0x4b55,0x4b6f,0xa570,0x54e4,0xd260,
	0xe968,0xd520,0xdaa0,0x6aa6,0x56df,0x4ae0,0xa9d4,0xa4d0,0xd150,0xf252,
	0xd520
};
const CString CLunar::m_GAN[]={ //天干
	_T("甲"),_T("乙"),_T("丙"),_T("丁"),_T("戊"),_T("己"),_T("庚"),_T("辛"),
	_T("壬"),_T("癸")
};
const CString CLunar::m_ZHI[]={ //地支
	_T("子"),_T("丑"),_T("寅"),_T("卯"),_T("辰"),_T("巳"),_T("午"),_T("未"),
	_T("申"),_T("酉"),_T("戌"),_T("亥")  
};
const CString CLunar::m_Animals[]={ //生肖
	_T("鼠"),_T("牛"),_T("虎"),_T("兔"),_T("龙"),_T("蛇"),_T("马"),_T("羊"),
	_T("猴"),_T("鸡"),_T("狗"),_T("猪")
};
const CString CLunar::m_nStr1[]={ //数字的中文描述
	_T("日"),_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六"),_T("七"),
	_T("八"),_T("九"),_T("十")
};
const CString CLunar::m_nStr2[]={ //数字的中文描述
	_T("初"),_T("十"),_T("廿"),_T("卅"),_T("□")
};
const int CLunar::m_solarMonth[]={    //月的天数
	31,28,31,30,31,30,31,31,30,31,30,31
};
const CString CLunar::m_solarTerm[]={   //节气的中文描述
	_T("小寒"),_T("大寒"),_T("立春"),_T("雨水"),_T("惊蛰"),_T("春分"),_T("清明"),
	_T("谷雨"),_T("立夏"),_T("小满"),_T("芒种"),_T("夏至"),_T("小暑"),_T("大暑"),
	_T("立秋"),_T("处暑"),_T("白露"),_T("秋分"),_T("寒露"),_T("霜降"),_T("立冬"),
	_T("小雪"),_T("大雪"),_T("冬至")
};
const int CLunar::m_sTermInfo[]={ //节气的日期信息
	0,21208,42467,63836,85337,107014,128867,150921,173149,195551,218072,240693,
	263343,285989,308563,331033,353350,375494,397447,419210,440795,462224,483532,
	504758
};
const CString CLunar::m_monthName[]={ //月的英文描述
	_T("JAN"),_T("FEB"),_T("MAR"),_T("APR"),_T("MAY"),_T("JUN"),_T("JUL"),_T("AUG"),
	_T("SEP"),_T("OCT"),_T("NOV"),_T("DEC")
};

const CString CLunar::m_sFtv[]={ //公历节日 *表示放假日
	_T("0101*新年元旦"),
	_T("0202 世界湿地日"),
	_T("0207 国际声援南非日"),
	_T("0210 国际气象节"),
	_T("0214 情人节"),
	_T("0301 国际海豹日"),
	_T("0303 全国爱耳日"),
	_T("0308 国际妇女节"),
	_T("0312 植树节 孙中山逝世纪念日"),
	_T("0314 国际警察日"),
	_T("0315 国际消费者权益日"),
	_T("0317 中国国医节 国际航海日"),
	_T("0321 世界森林日 消除种族歧视国际日"),
	_T("0321 世界儿歌日"),
	_T("0322 世界水日"),
	_T("0323 世界气象日"),
	_T("0324 世界防治结核病日"),
	_T("0325 全国中小学生安全教育日"),
	_T("0330 巴勒斯坦国土日"),
	_T("0401 愚人节 全国爱国卫生运动月(四月) 税收宣传月(四月)"),
	_T("0407 世界卫生日"),
	_T("0422 世界地球日"),
	_T("0423 世界图书和版权日"),
	_T("0424 亚非新闻工作者日"),
	_T("0501 国际劳动节"),
	_T("0504 中国五四青年节"),
	_T("0505 碘缺乏病防治日"),
	_T("0508 世界红十字日"),
	_T("0512 国际护士节"),
	_T("0515 国际家庭日"),
	_T("0517 世界电信日"),
	_T("0518 国际博物馆日"),
	_T("0520 全国学生营养日"),
	_T("0523 国际牛奶日"),
	_T("0531 世界无烟日"), 
	_T("0601 国际儿童节"),
	_T("0605 世界环境日"),
	_T("0606 全国爱眼日"),
	_T("0617 防治荒漠化和干旱日"),
	_T("0623 国际奥林匹克日"),
	_T("0625 全国土地日"),
	_T("0626 国际反毒品日"),
	_T("0701 中国共产党建党日 世界建筑日"),
	_T("0702 国际体育记者日"),
	_T("0707 中国人民抗日战争纪念日"),
	_T("0711 世界人口日"),
	_T("0730 非洲妇女日"),
	_T("0801 中国建军节"),
	_T("0808 中国男子节(爸爸节)"),
	_T("0815 日本正式宣布无条件投降日"),
	_T("0908 国际扫盲日 国际新闻工作者日"),
	_T("0910 教师节"),
	_T("0914 世界清洁地球日"),
	_T("0916 国际臭氧层保护日"),
	_T("0918 九·一八事变纪念日"),
	_T("0920 国际爱牙日"),
	_T("0927 世界旅游日"),
	_T("1001*国庆节 世界音乐日 国际老人节"),
	_T("1001 国际音乐日"),
	_T("1002 国际和平与民主自由斗争日"),
	_T("1004 世界动物日"),
	_T("1008 全国高血压日"),
	_T("1008 世界视觉日"),
	_T("1009 世界邮政日 万国邮联日"),
	_T("1010 辛亥革命纪念日 世界精神卫生日"),
	_T("1013 世界保健日 国际教师节"),
	_T("1014 世界标准日"),
	_T("1015 国际盲人节(白手杖节)"),
	_T("1016 世界粮食日"),
	_T("1017 世界消除贫困日"),
	_T("1022 世界传统医药日"),
	_T("1024 联合国日 世界发展信息日"),
	_T("1031 世界勤俭日"),
	_T("1107 十月社会主义革命纪念日"),
	_T("1108 中国记者日"),
	_T("1109 全国消防安全宣传教育日"),
	_T("1110 世界青年节"),
	_T("1111 国际科学与和平周(本日所属的一周)"),
	_T("1112 孙中山诞辰纪念日"),
	_T("1114 世界糖尿病日"),
	_T("1117 国际大学生节 世界学生节"),
	_T("1121 世界问候日 世界电视日"),
	_T("1129 国际声援巴勒斯坦人民国际日"),
	_T("1201 世界艾滋病日"),
	_T("1203 世界残疾人日"),
	_T("1205 国际经济和社会发展志愿人员日"),
	_T("1208 国际儿童电视日"),
	_T("1209 世界足球日"),
	_T("1210 世界人权日"),
	_T("1212 西安事变纪念日"),
	_T("1213 南京大屠杀(1937年)纪念日！紧记血泪史！"),
	_T("1221 国际篮球日"),
	_T("1224 平安夜"),
	_T("1225 圣诞节"),
	_T("1229 国际生物多样性日")
};
const CString CLunar::m_wFtv[]={//某月的第几个星期几。 5,6,7,8 表示到数第 1,2,3,4 个星期几
	_T("0110 黑人日"),
	_T("0150 世界麻风日"), //一月的最后一个星期日（月倒数第一个星期日）
	_T("0520 国际母亲节"),
	_T("0530 全国助残日"),
	_T("0630 父亲节"),
	_T("0911 劳动节"),
	_T("0932 国际和平日"),
	_T("0940 国际聋人节 世界儿童日"),
	_T("0950 世界海事日"),
	_T("1011 国际住房日"),
	_T("1013 国际减轻自然灾害日(减灾日)"),
	_T("1144 感恩节")
};
const CString CLunar::m_lFtv[]={//农历节日
	_T("0101*春节"),
	_T("0115 元宵节"),
	_T("0202 龙抬头节"),
	_T("0323 妈祖生辰 (天上圣母诞辰)"),
	_T("0505 端午节"),
	_T("0707 七七中国情人节"),
	_T("0815 中秋节"),
	_T("0909 重阳节"),
	_T("1208 腊八节"),
	_T("1224 小年"),
	_T("1200*除夕")
};

CLunar::CLunar(void)
{
	m_lYear=0;    //公元年4位数字
	m_lMonth=0;   //农历月数字
	m_lDay=0;     //农历日数字
	m_isLeap=FALSE;  //是否为农历闰月?
	//八字
	m_cYear=_T("");   //年柱, 2个中文
	m_cMonth=_T("");  //月柱, 2个中文
	m_cDay=_T("");    //日柱, 2个中文
	m_cAnimal=_T(""); //动物属相

	//节日
	m_lunarFestival=_T(""); //农历节日
	m_solarFestival=_T(""); //公历节日
	m_solarTerms=_T("");    //节气

	m_bStrong=FALSE;       //强调的节日

}

CLunar::~CLunar(void)
{
}

//返回农历 y年的总天数
int CLunar::getDayCountInYear(int y)
{
	int i=0, sum = 348;
	for(i=0x8000; i>0x8; i>>=1)
		sum += (m_lunarInfo[y-1900] & i)? 1: 0;
	return(sum+getDayCountInLeapMonth(y));

}
//返回农历 y年闰月的天数
int CLunar::getDayCountInLeapMonth(int y)
{
	if(getLeapMonthInYear(y))
		return((m_lunarInfo[y-1899]&0xf)==0xf? 30: 29);
	else
		return(0);

}
//返回农历 y年闰哪个月 1-12 , 没闰返回 0
int CLunar::getLeapMonthInYear(int y)
{
	int lm = m_lunarInfo[y-1900] & 0xf;
	return(lm==0xf?0:lm);

}
//返回农历 y年m月的总天数
int CLunar::getDayCountInMonth(int y,int m)
{
	return((m_lunarInfo[y-1900] & (0x10000>>m))? 30: 29 );

}

// 传入 offset 返回干支, 0=甲子
// 年柱 1900年立春后为庚子年(60进制36)
//cyclical(SY-1900+36) + '年 【'+Animals[(SY-4)%12]+'年】
CString CLunar::getGZ(LONGLONG num) {
	return m_GAN[num%10] + m_ZHI[num%12];
}
//某年的第n个节气为几日(从0小寒起算)
int CLunar::getSTerm(int y,int n)
{ 
	LONGLONG lUtc1=-22085493000000000; //1900,1,6,2,5,0  unit:100ns
	LONGLONG lUtc2=-116444736000000000; //1601,1,1,0,0,0  unit:100ns

	ULONGLONG lDate=315569259747000;
	lDate *= (y-1900);
	lDate += UInt32x32To64(600000000,m_sTermInfo[n]);
	lDate += lUtc1; //100ns--->ms
	//TRACE("ldate= %I64d/n",lDate);
	lDate -=lUtc2;
	SYSTEMTIME st;
	FileTimeToSystemTime((FILETIME*)&lDate,&st);
	//COleDateTime tmRet(st); 
	return st.wDay;
}
void CLunar::Lunar(Date tmMeas)
{
	m_tmMeas=tmMeas;
	int i, leap=0, temp=0;

	//COleDateTime tmConst(1900,1,31,m_tmMeas.GetHour(),m_tmMeas.GetMinute(),m_tmMeas.GetSecond()); 
	//COleDateTimeSpan tmSpan=m_tmMeas-tmConst;
	//int offset2=static_cast<int>(floor(tmSpan.GetTotalDays())); 

	int y = m_tmMeas.year;
	int m = m_tmMeas.month;
	int d = m_tmMeas.day;

	ATLASSERT(y>1899 && y<2101);
	m_cYearName=getYearName(y);
	int offset=static_cast<int>(getUTC(y,m,d)/864000000000 - getUTC(1900,1,31)/864000000000);
	for(i=1900; i<2100 && offset>0; i++) { temp=getDayCountInYear(i); offset-=temp; }

	if(offset<0) { offset+=temp; i--; }

	m_lYear = i;

	leap = getLeapMonthInYear(i); //闰哪个月
	m_isLeap = FALSE;

	for(i=1; i<13 && offset>0; i++) {
		//闰月
		if(leap>0 && i==(leap+1) && (!m_isLeap))
		{ --i; m_isLeap = TRUE; temp = getDayCountInLeapMonth(m_lYear); }
		else
		{ temp = getDayCountInMonth(m_lYear, i); }

		//解除闰月
		if(m_isLeap==TRUE && i==(leap+1)) m_isLeap = FALSE;

		offset -= temp;
	}

	if(offset==0 && leap>0 && i==leap+1)
	{
		if(m_isLeap)
		{ 
			m_isLeap = FALSE;
		}
		else
		{
			m_isLeap = TRUE;
			--i;
		}
	}

	if(offset<0){
		offset += temp;
		--i;
	}
	m_lMonth = i;
	m_lDay = offset + 1;
	m_cWordDay= getWordDay(m_lDay);
	m_cMonthSB=((m_isLeap?getDayCountInLeapMonth(m_lYear):getDayCountInMonth(m_lYear,m_lMonth))>29)?_T("大"):_T("小");


	//////////////////////////////////////////////////////////////////////////
	//初始化 八字

	//36 年一循环 
	////////年柱 1900年立春后为庚子年(60进制36)
	m_cAnimal=m_Animals[(m_lYear-4)%12];
	m_cYear=getGZ(m_lYear-1900+36);
	///////月柱 1900年1月小寒以前为 丙子月(60进制12)
	//m_cMonth= getGZ((y-1900)*12+m+12);
	m_cMonth= getGZ((m_lYear-1900)*12+m_lMonth+1+12);

	//当月一日与 1900/1/1 相差天数
	//1900/1/1与 1970/1/1 相差25567日, 1900/1/1 日柱为甲戌日(60进制10)
	LONGLONG dayCyclical = getUTC(y,m,d)/864000000000+25567+10;
	m_cDay= getGZ(dayCyclical); 

	//////////////////////////////////////////////////////////////////////////
	//初始化 节日
	//公历节气 
	int nMSterm=getSTerm(y,(m-1)*2);
	if(d==getSTerm(y,(m-1)*2))
		m_solarTerms=m_solarTerm[(m-1)*2];
	else if(d==getSTerm(y,(m-1)*2+1))
		m_solarTerms=m_solarTerm[(m-1)*2+1];
	else
		m_solarTerms=_T("");

	//公历节日
	m_solarFestival=_T("");
	m_bStrong=FALSE;
	int nFtvM=0,nFtvD=0;
	CString strDesc;
	BOOL bStrong;
	for(int i=0;i<sizeof(m_sFtv)/sizeof(m_sFtv[0]);i++)
	{
		parseFtv(m_sFtv[i],nFtvM,nFtvD,strDesc,bStrong);
		if(m==nFtvM && d==nFtvD)
		{
			if(!m_solarFestival.IsEmpty())
				m_solarFestival+=_T(" ");
			m_solarFestival+=strDesc;
			m_bStrong=bStrong;   
		}
	}

	//月周节日
	int nWeekNo=0,nWeekDay=0; //第几周，星期几
	for(int i=0;i<sizeof(m_wFtv)/sizeof(m_wFtv[0]);i++)
	{
		parseFtv(m_wFtv[i],nFtvM,nFtvD,strDesc,bStrong);
		nWeekNo=nFtvD/10;nWeekDay=nFtvD%10; //0--星期日
		if((m==nFtvM) && (m_tmMeas.day_in_week)==nWeekDay )   
		{
			if((nWeekNo<6 && (getWeekOfMonth(y,m,d)==nWeekNo)) ||
				(nWeekNo>=6 && ((getWeeksOfMonth(y,m)-getWeekOfMonth(y,m,d)+6)==nWeekNo)))
			{
				if(!m_solarFestival.IsEmpty())
					m_solarFestival+=_T(" ");
				m_solarFestival+=strDesc;
				m_bStrong=(m_bStrong?TRUE:bStrong); 
			}
		}
	}

	//黑色星期五
	Date tmFirst = {y, m, 13, 5 };  //13号是不是星期五, 请把6替换为真实的星期几序号
	if(5 == tmFirst.day_in_week)
	{
		if(!m_solarFestival.IsEmpty())
			m_solarFestival+=_T(" ");
		m_solarFestival+=_T(" 黑色星期五");;
	}

	//复活节只出现在3或4月 复活节日举行此盛会的日子，这个日子是3月21日或其后月满之后的第一个星期天
	//if(m==3 || m==4) {
	// var estDay = new easter(y);
	// if(m == estDay.m)
	//  ' 复活节 Easter Sunday';
	//}


	//农历节日
	m_lunarFestival=_T("");
	for(int i=0;i<sizeof(m_lFtv)/sizeof(m_lFtv[0]);i++) 
	{
		parseFtv(m_lFtv[i],nFtvM,nFtvD,strDesc,bStrong);
		if((m_lMonth==nFtvM && m_lDay==nFtvD) ||
			(m_lMonth==nFtvM && nFtvD==0 &&  (m_isLeap?getDayCountInLeapMonth(m_lYear):getDayCountInMonth(m_lYear,m_lMonth))==m_lDay)) //最后一天，用0标示
		{
			m_lunarFestival=strDesc;
			m_bStrong=(m_bStrong?TRUE:bStrong);
		}  
	}
}

LONGLONG CLunar::getUTC(int nYear,
						int nMonth,
						int nDay,
						int nHour,
						int nMin,
						int nSec )
{
	// 计算指定年代与 UTC时间相差的 数
	//此段代码 联系 webmote 
	LONGLONG lValue=10000;
		return lValue;
}

void CLunar::parseFtv(CString strInfo,int& nFirst,int& nLast,CString& strDesc,BOOL& bStrong)
{
	CString sInfo(strInfo);
	sInfo.TrimLeft();
	sInfo.TrimRight();
	if(sInfo.IsEmpty())
		return;

	CString sFirstNum,sLastNum;
	sFirstNum=sInfo.Left(2);
	sLastNum=sInfo.Mid(2,2);
	nFirst=_ttoi(sFirstNum.GetBuffer(2));
	nLast=_ttoi(sLastNum.GetBuffer(2));
	sFirstNum.ReleaseBuffer();
	sLastNum.ReleaseBuffer();

	CString sStrong=sInfo.Mid(4,1);
	if(sStrong==_T("*"))
		bStrong=TRUE;
	else
		bStrong=FALSE;

	strDesc=sInfo.Mid(5);
	strDesc.TrimLeft();
}
int CLunar::getWeekOfMonth(int y,int m,int d)
{
	Date tm = {y,m,1,0};	// 需要把0替换为真实的星期几
	Date tmCurr = {y,m,d,0}; // 需要把0替换为真实的星期几
	int nSpanDays=d-1;
	int nWeekSpan=nSpanDays/7+1;
	int nWeekDay=tm.day_in_week+nSpanDays%7;
	if(nWeekDay>6)
	{
		nWeekDay-=7;
		nWeekSpan++;
	}
	return nWeekSpan;
}

int CLunar::getWeeksOfMonth(int y,int m)
{
	//计算当月天数
	int nMonths=0;
	static int cDaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	nMonths=cDaysInMonth[m-1];
	if(y % 100 != 0 && y % 4 == 0)
	{
		if(m==2)
			nMonths += 1;
	}
	return getWeekOfMonth(y,m,nMonths); 
}
//中文日期
CString  CLunar::getWordDay(int d){
	CString s;
	switch (d) {
   case 10:
	   s = _T("初十");
	   break;
   case 20:
	   s = _T("二十");
	   break;
   case 30:
	   s = _T("三十");
	   break;
   default :
	   s = m_nStr2[d/10];
	   s += m_nStr1[d%10];
	}
	return s;
}
//年号
CString CLunar::getYearName(int y)
{
	CString cYear,cTmp;
	if(y>1874 && y<1909)  //光绪
	{
		(y-1874==1)?cTmp=_T("元"):cTmp.Format(_T("%d"),y-1874);
		cYear.Format(_T("光绪%s年"),cTmp);
	}
	else if(y>1908 && y<1912)  //宣统
	{
		(y-1908==1)?cTmp=_T("元"):cTmp.Format(_T("%d"),y-1908);
		cYear.Format(_T("宣统%s年"),cTmp);
	}
	else if(y>1911 && y<1950) //民国
	{
		(y-1911==1)?cTmp=_T("元"):cTmp.Format(_T("%d"),y-1911);
		cYear.Format(_T("民国%s年"),cTmp);
	}
	else if(y>1949)           //共和国
	{
		(y-1949==1)?cTmp=_T("元"):cTmp.Format(_T("%d"),y-1949);
		cYear.Format(_T("共和国%s年"),cTmp);
	}
	else
		cYear.Format(_T("公元%d年"),y);
	return cYear;
}

int CLunar::DaysBetween( WORD iEndYear, WORD iEndMonth, WORD iEndDay, WORD iStartYear, WORD iStartMonth, WORD iStartDay )
{
	WORD monthday[]={0, 31, 59 ,90, 120, 151, 181, 212, 243, 273, 304, 334}; 

	//计算两个年份1月1日之间相差的天数
	LONG iDiffDays =(iEndYear - iStartYear)*365;
	iDiffDays += (iEndYear-1)/4 - (iStartYear-1)/4;
	iDiffDays -= ((iEndYear-1)/100 - (iStartYear-1)/100);
	iDiffDays += (iEndYear-1)/400 - (iStartYear-1)/400;

	//加上iEndYear年1月1日到iEndMonth月iEndDay日之间的天数
	iDiffDays += monthday[iEndMonth-1] +
		(IsLeapYear(iEndYear)&&iEndMonth>2? 1: 0);
	iDiffDays += iEndDay;

	//减去iStartYear年1月1日到iStartMonth月iStartDay日之间的天数
	iDiffDays -= (monthday[iStartMonth-1] + 
		(IsLeapYear(iStartYear)&&iStartMonth>2 ? 1: 0));
	iDiffDays -= iStartDay;	
	return iDiffDays;
}

bool CLunar::DateToLunar( const Date &date, Date& lunar )
{
	 DaysBetween(date.year, date.month, date.day);
}

