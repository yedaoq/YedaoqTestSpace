/*
*  Gregorian Solar Calendar and Chinese Lunar Calendar
*  ũ��; ����; ����; ����; ʱ��; ����; ��֧; ��Ф
*  1900-2100 ����������
*  �������ߣ�webmote 2006-09 �ο��Ի��������е� 1900-2100 ����������,���޸���һ����BUG
*  ���κ����⣬��email�� luo31@yahoo.com.cn
*  �����������ñ��࣬�벻Ҫɾ���˶�ע�ͣ�������
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


	//ũ��
	int     m_lYear;    //��Ԫ��4λ����
	int     m_lMonth;   //ũ��������
	int     m_lDay;     //ũ��������
	CString m_cYearName; //��ż����ʾ
	CString m_cWordDay; //���ֱ�ʾ������
	CString m_cMonthSB; //�´� С��
	BOOL    m_isLeap;  //�Ƿ�Ϊũ������?
	//����
	CString m_cYear;   //����, 2������
	CString m_cMonth;  //����, 2������
	CString m_cDay;    //����, 2������
	CString m_cAnimal; //�������� 

	//����
	CString m_lunarFestival; //ũ������
	CString m_solarFestival; //��������
	CString m_solarTerms;    //����

	BOOL    m_bStrong;       //ǿ���Ľ��� �ż���

	/************************************************************************/
	/* ����ũ������                                                         */
	/************************************************************************/
	//����ũ��
	void Lunar(Date tmMeas);

	static bool IsLeapYear(int iYear) {return !(iYear%4)&&(iYear%100) || !(iYear%400);}
	static int	DaysBetween(WORD iEndYear, WORD iEndMonth, WORD iEndDay, WORD iStartYear = 1900, WORD iStartMonth = 1, WORD iStartDay = 1);
	static bool DateToLunar(const Date &date, Date& lunar);
private:
	/************************************************************************/
	/* ũ[��]����ʹ�õ�ȫ�ֳ���                                                 */
	/************************************************************************/
	// ��������
	static const int m_lunarInfo[]; //ũ������
	static const CString m_GAN[]; //���
	static const CString m_ZHI[]; //��֧
	static const CString m_Animals[]; //��Ф
	static const CString m_nStr1[]; //���ֵ���������
	static const CString m_nStr2[]; //���ֵ���������

	//ũ������
	static const CString m_lFtv[];

	/************************************************************************/
	/* ��[��]����ʹ�õ�ȫ�ֳ���                                                 */
	/************************************************************************/ 
	static const int m_solarMonth[];    //�µ�����
	static const CString m_solarTerm[];   //��������������
	static const int     m_sTermInfo[]; //������������Ϣ
	static const CString m_monthName[]; //�µ�Ӣ������

	//�������� *��ʾ�ż���
	static const CString m_sFtv[];
	//ĳ�µĵڼ������ڼ��� 5,6,7,8 ��ʾ������ 1,2,3,4 �����ڼ�
	static const CString m_wFtv[];



	//���ʱ��
	Date  m_tmMeas; //�������


private:
	
	int getDayCountInYear(int y);					//����ũ��ĳ���������
	int getDayCountInMonth(int y,int m);			//����ũ��ĳ��ĳ�µ�������

	int getLeapMonthInYear(int y);					//����ũ��ĳ�����ĸ��� 1-12 , û�򷵻� 0
	int getDayCountInLeapMonth(int y);				//����ũ��ĳ�����µ�����
	
	int getSTerm(int y,int n);						//ĳ��ĵ�n������Ϊ����(��0С������)
	
	CString getWordDay(int d);						//�õ�ũ���������
	
	CString getYearName(int y);						//�õ���ż��� ����

	//�õ���֧����
	CString getGZ(LONGLONG num);
	//�ӽ����ַ�����������֣�������*
	void parseFtv(CString strInfo,int& nFirst,int& nLast,CString& strDesc,BOOL& bStrong);

	//�õ�����Ϊ���µĵڼ���
	int getWeekOfMonth(int y,int m,int d);

	//�õ������м���
	int getWeeksOfMonth(int y,int m);

	//����ָ������� UTCʱ������ 100ns ��
	LONGLONG getUTC(int nYear,
		int nMonth,
		int nDay,
		int nHour=0,
		int nMin=0,
		int nSec=0);

};

