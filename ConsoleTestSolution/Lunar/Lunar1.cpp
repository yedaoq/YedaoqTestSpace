#include "Lunar.h"
// -----------------------------------------------------------
//      农历计算参数表
// -----------------------------------------------------------
static const  ulong  LunarParas[] = 
{
        0x2E47752B, 0x5400952B, 0x3E012A5B, 0x2A21D55A, 0x4E02956A,     // 2005
        0x38733B55, 0x6003DBA4, 0x4A047B49, 0x32553A93, 0x5805DA95, 
        0x4206752D, 0x2C470AAD, 0x50004AAD, 0x3C90F5AA, 0x620195D2,     // 2015
        0x4C022DA5, 0x3662FD4A, 0x5C038D4A, 0x46042C95, 0x3044D52E, 
        0x54059556, 0x3E062AB5, 0x2A26D5B2, 0x500776D2, 0x3860AEA5,     // 2025 
        0x5E015725, 0x4801F64B, 0x32528C97, 0x56035CAB, 0x4203E55A,
        0x2C348AD6                                                       // 2031
};

/*********************************************************************
 农历计数参数位定义：
 31--25  7 ：（BaseDays）公历1月1日到农历正月初一的累积天数
 24--20  5：（Inter）闰月月份，0表示此年没有闰月
 19--13  7：（BaseGANZHI）此年公历1月1日的干支序号减1
 12--0   13：（MonthDays）此农历年每月之大小，0为小月29天1为大月30天
**********************************************************************/
#define Base_Days 0xfe000000
#define Inter  0x01f00000
#define Base_GZ 0x000fe000
#define Month_Days  0x00001fff
#define YBase_Days 25
#define YInter 20
#define YBase_GZ 13
#define YMonth_Days 0
#define GetBaseDays(i)  ((LunarParas[i]&Base_Days)>>YBase_Days)   
#define GetInter(i)     ((LunarParas[i]&Inter)>>YInter)
#define GetBaseGZ(i)    ((LunarParas[i]&Base_GZ)>>YBase_GZ)
#define GetMonthDays(Year,Month)  ((((LunarParas[Year]&Month_Days)>>YMonth_Days)>>Month)&0x01)
#define FirstYear  2001
#define YearNum  (sizeof(LunarParas)/sizeof(unsigned long))
#define LastYear (FirstYear+YearNum-1)
/************************************************************************
     公历年每月天数标记表
              由高位到低位为12-1月：1表示31天，0表示30天，二月除外
        月份： 12   11   10    9    8    7     6    5    4    3    2    1
               31,  30,  31,  30,  31,   31,  30,  31,   30,  31, 28,  31
               1    0    1    0    1     1     0    1    0    1    0    1
***************************************************************************/
static const uint SolarMDflag=0x0ad5; //公历月天数标志
// 公历年每月累积天数, 平年与闰年
// -----------------------------------------------------------
static  const  uint SolarDays[14] =
{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365, 396 }; //闰年2月29天，平年2月都是28天
/********************************************
求次年是否为闰年，返回0是平年1是闰年
******************************************/
uchar Lunar_Inter(uint Year)
{
   if(Year%400==0)return 1;
   else if(Year%100==0)return 0;
   else if(Year%4==0)return 1;
   else return 0; 
}
uint Lunar(struct SolarDate *pSS,struct LunarDate *pLL)
{
  unsigned int  i,IM,SolarMonth,Daysum,SDaysum,Temp1,Temp2,GZsum;
  uchar LeapFlag; 
  uchar MonthDays,YearIndex;
  if((pSS->Year<FirstYear)||(pSS->Year>LastYear)) return 1;
  SolarMonth=pSS->Month-1;
  if(SolarMonth>11)return 2;
  LeapFlag=Lunar_Inter(pSS->Year);
  if(LeapFlag==1)
  {
    MonthDays=LeapFlag+28;
  }
  else
  {
   MonthDays=((SolarMDflag>>SolarMonth)&0x01)+30;
  }
  if(pSS->Date<1||pSS->Date>MonthDays)return 3;
  YearIndex=pSS->Year-FirstYear;
  SDaysum=SolarDays[SolarMonth];
  if(SolarMonth>1)SDaysum+=LeapFlag;
  Daysum=SDaysum+pSS->Date;
  GZsum=Daysum+GetBaseGZ(YearIndex);
  pLL->GAN=GZsum%10;
  pLL->ZHI=GZsum%12;
  if(Daysum<=GetBaseDays(YearIndex))
  {
    YearIndex--;
 pLL->Year=pSS->Year-1;
 LeapFlag=Lunar_Inter(pLL->Year);
 SolarMonth+=12;                        //不明白
 SDaysum=SolarDays[SolarMonth];
 if(SolarMonth>1)SDaysum+=LeapFlag;
 Daysum=SDaysum+pSS->Date;
  }
  else
  {
    pLL->Year=pSS->Year;
  }
  Temp1=GetBaseDays(YearIndex);
  for(i=0;i<13;i++)
  {
    Temp2=Temp1+GetMonthDays(YearIndex,i)+29;
 if(Daysum<=Temp2)break;
 Temp1=Temp2;
  }
  pLL->Month=i+1;
  pLL->Date=Daysum-Temp1;
  IM=GetInter(YearIndex);
  if(IM!=0&&pLL->Month>IM)pLL->Month--;
  if(pLL->Month>12)pLL->Month=(pLL->Month)-12;
  return 0;
}
 