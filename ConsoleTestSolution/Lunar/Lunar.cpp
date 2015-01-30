#include <Windows.h>
#include <tchar.h>
#include "Lunar.h"
#include <math.h>

//�����������1900�굽2100��ÿ���е���������Ϣ��  
//����ÿ��ֻ����29��30�죬һ����12����13����������λ��ʾ����ӦλΪ1��30�죬����Ϊ29��  
const int CLunar::m_lunarInfo[]={ //ũ��������Ϣ
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
const CString CLunar::m_GAN[]={ //���
	_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),
	_T("��"),_T("��")
};
const CString CLunar::m_ZHI[]={ //��֧
	_T("��"),_T("��"),_T("��"),_T("î"),_T("��"),_T("��"),_T("��"),_T("δ"),
	_T("��"),_T("��"),_T("��"),_T("��")  
};
const CString CLunar::m_Animals[]={ //��Ф
	_T("��"),_T("ţ"),_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),
	_T("��"),_T("��"),_T("��"),_T("��")
};
const CString CLunar::m_nStr1[]={ //���ֵ���������
	_T("��"),_T("һ"),_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),_T("��"),
	_T("��"),_T("��"),_T("ʮ")
};
const CString CLunar::m_nStr2[]={ //���ֵ���������
	_T("��"),_T("ʮ"),_T("إ"),_T("ئ"),_T("��")
};
const int CLunar::m_solarMonth[]={    //�µ�����
	31,28,31,30,31,30,31,31,30,31,30,31
};
const CString CLunar::m_solarTerm[]={   //��������������
	_T("С��"),_T("��"),_T("����"),_T("��ˮ"),_T("����"),_T("����"),_T("����"),
	_T("����"),_T("����"),_T("С��"),_T("â��"),_T("����"),_T("С��"),_T("����"),
	_T("����"),_T("����"),_T("��¶"),_T("���"),_T("��¶"),_T("˪��"),_T("����"),
	_T("Сѩ"),_T("��ѩ"),_T("����")
};
const int CLunar::m_sTermInfo[]={ //������������Ϣ
	0,21208,42467,63836,85337,107014,128867,150921,173149,195551,218072,240693,
	263343,285989,308563,331033,353350,375494,397447,419210,440795,462224,483532,
	504758
};
const CString CLunar::m_monthName[]={ //�µ�Ӣ������
	_T("JAN"),_T("FEB"),_T("MAR"),_T("APR"),_T("MAY"),_T("JUN"),_T("JUL"),_T("AUG"),
	_T("SEP"),_T("OCT"),_T("NOV"),_T("DEC")
};

const CString CLunar::m_sFtv[]={ //�������� *��ʾ�ż���
	_T("0101*����Ԫ��"),
	_T("0202 ����ʪ����"),
	_T("0207 ������Ԯ�Ϸ���"),
	_T("0210 ���������"),
	_T("0214 ���˽�"),
	_T("0301 ���ʺ�����"),
	_T("0303 ȫ��������"),
	_T("0308 ���ʸ�Ů��"),
	_T("0312 ֲ���� ����ɽ����������"),
	_T("0314 ���ʾ�����"),
	_T("0315 ����������Ȩ����"),
	_T("0317 �й���ҽ�� ���ʺ�����"),
	_T("0321 ����ɭ���� �����������ӹ�����"),
	_T("0321 ���������"),
	_T("0322 ����ˮ��"),
	_T("0323 ����������"),
	_T("0324 ������ν�˲���"),
	_T("0325 ȫ����Сѧ����ȫ������"),
	_T("0330 ����˹̹������"),
	_T("0401 ���˽� ȫ�����������˶���(����) ˰��������(����)"),
	_T("0407 ����������"),
	_T("0422 ���������"),
	_T("0423 ����ͼ��Ͱ�Ȩ��"),
	_T("0424 �Ƿ����Ź�������"),
	_T("0501 �����Ͷ���"),
	_T("0504 �й����������"),
	_T("0505 ��ȱ����������"),
	_T("0508 �����ʮ����"),
	_T("0512 ���ʻ�ʿ��"),
	_T("0515 ���ʼ�ͥ��"),
	_T("0517 ���������"),
	_T("0518 ���ʲ������"),
	_T("0520 ȫ��ѧ��Ӫ����"),
	_T("0523 ����ţ����"),
	_T("0531 ����������"), 
	_T("0601 ���ʶ�ͯ��"),
	_T("0605 ���绷����"),
	_T("0606 ȫ��������"),
	_T("0617 ���λ�Į���͸ɺ���"),
	_T("0623 ���ʰ���ƥ����"),
	_T("0625 ȫ��������"),
	_T("0626 ���ʷ���Ʒ��"),
	_T("0701 �й������������� ���罨����"),
	_T("0702 ��������������"),
	_T("0707 �й�������ս��������"),
	_T("0711 �����˿���"),
	_T("0730 ���޸�Ů��"),
	_T("0801 �й�������"),
	_T("0808 �й����ӽ�(�ְֽ�)"),
	_T("0815 �ձ���ʽ����������Ͷ����"),
	_T("0908 ����ɨä�� �������Ź�������"),
	_T("0910 ��ʦ��"),
	_T("0914 ������������"),
	_T("0916 ���ʳ����㱣����"),
	_T("0918 �š�һ���±������"),
	_T("0920 ���ʰ�����"),
	_T("0927 ����������"),
	_T("1001*����� ���������� �������˽�"),
	_T("1001 ����������"),
	_T("1002 ���ʺ�ƽ���������ɶ�����"),
	_T("1004 ���綯����"),
	_T("1008 ȫ����Ѫѹ��"),
	_T("1008 �����Ӿ���"),
	_T("1009 ���������� ���������"),
	_T("1010 �������������� ���羫��������"),
	_T("1013 ���籣���� ���ʽ�ʦ��"),
	_T("1014 �����׼��"),
	_T("1015 ����ä�˽�(�����Ƚ�)"),
	_T("1016 ������ʳ��"),
	_T("1017 ��������ƶ����"),
	_T("1022 ���紫ͳҽҩ��"),
	_T("1024 ���Ϲ��� ���緢չ��Ϣ��"),
	_T("1031 �����ڼ���"),
	_T("1107 ʮ������������������"),
	_T("1108 �й�������"),
	_T("1109 ȫ��������ȫ����������"),
	_T("1110 ���������"),
	_T("1111 ���ʿ�ѧ���ƽ��(����������һ��)"),
	_T("1112 ����ɽ����������"),
	_T("1114 ����������"),
	_T("1117 ���ʴ�ѧ���� ����ѧ����"),
	_T("1121 �����ʺ��� ���������"),
	_T("1129 ������Ԯ����˹̹���������"),
	_T("1201 ���簬�̲���"),
	_T("1203 ����м�����"),
	_T("1205 ���ʾ��ú���ᷢչ־Ը��Ա��"),
	_T("1208 ���ʶ�ͯ������"),
	_T("1209 ����������"),
	_T("1210 ������Ȩ��"),
	_T("1212 �����±������"),
	_T("1213 �Ͼ�����ɱ(1937��)�����գ�����Ѫ��ʷ��"),
	_T("1221 ����������"),
	_T("1224 ƽ��ҹ"),
	_T("1225 ʥ����"),
	_T("1229 ���������������")
};
const CString CLunar::m_wFtv[]={//ĳ�µĵڼ������ڼ��� 5,6,7,8 ��ʾ������ 1,2,3,4 �����ڼ�
	_T("0110 ������"),
	_T("0150 ���������"), //һ�µ����һ�������գ��µ�����һ�������գ�
	_T("0520 ����ĸ�׽�"),
	_T("0530 ȫ��������"),
	_T("0630 ���׽�"),
	_T("0911 �Ͷ���"),
	_T("0932 ���ʺ�ƽ��"),
	_T("0940 �������˽� �����ͯ��"),
	_T("0950 ���纣����"),
	_T("1011 ����ס����"),
	_T("1013 ���ʼ�����Ȼ�ֺ���(������)"),
	_T("1144 �ж���")
};
const CString CLunar::m_lFtv[]={//ũ������
	_T("0101*����"),
	_T("0115 Ԫ����"),
	_T("0202 ��̧ͷ��"),
	_T("0323 �������� (����ʥĸ����)"),
	_T("0505 �����"),
	_T("0707 �����й����˽�"),
	_T("0815 �����"),
	_T("0909 ������"),
	_T("1208 ���˽�"),
	_T("1224 С��"),
	_T("1200*��Ϧ")
};

CLunar::CLunar(void)
{
	m_lYear=0;    //��Ԫ��4λ����
	m_lMonth=0;   //ũ��������
	m_lDay=0;     //ũ��������
	m_isLeap=FALSE;  //�Ƿ�Ϊũ������?
	//����
	m_cYear=_T("");   //����, 2������
	m_cMonth=_T("");  //����, 2������
	m_cDay=_T("");    //����, 2������
	m_cAnimal=_T(""); //��������

	//����
	m_lunarFestival=_T(""); //ũ������
	m_solarFestival=_T(""); //��������
	m_solarTerms=_T("");    //����

	m_bStrong=FALSE;       //ǿ���Ľ���

}

CLunar::~CLunar(void)
{
}

//����ũ�� y���������
int CLunar::getDayCountInYear(int y)
{
	int i=0, sum = 348;
	for(i=0x8000; i>0x8; i>>=1)
		sum += (m_lunarInfo[y-1900] & i)? 1: 0;
	return(sum+getDayCountInLeapMonth(y));

}
//����ũ�� y�����µ�����
int CLunar::getDayCountInLeapMonth(int y)
{
	if(getLeapMonthInYear(y))
		return((m_lunarInfo[y-1899]&0xf)==0xf? 30: 29);
	else
		return(0);

}
//����ũ�� y�����ĸ��� 1-12 , û�򷵻� 0
int CLunar::getLeapMonthInYear(int y)
{
	int lm = m_lunarInfo[y-1900] & 0xf;
	return(lm==0xf?0:lm);

}
//����ũ�� y��m�µ�������
int CLunar::getDayCountInMonth(int y,int m)
{
	return((m_lunarInfo[y-1900] & (0x10000>>m))? 30: 29 );

}

// ���� offset ���ظ�֧, 0=����
// ���� 1900��������Ϊ������(60����36)
//cyclical(SY-1900+36) + '�� ��'+Animals[(SY-4)%12]+'�꡿
CString CLunar::getGZ(LONGLONG num) {
	return m_GAN[num%10] + m_ZHI[num%12];
}
//ĳ��ĵ�n������Ϊ����(��0С������)
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

	leap = getLeapMonthInYear(i); //���ĸ���
	m_isLeap = FALSE;

	for(i=1; i<13 && offset>0; i++) {
		//����
		if(leap>0 && i==(leap+1) && (!m_isLeap))
		{ --i; m_isLeap = TRUE; temp = getDayCountInLeapMonth(m_lYear); }
		else
		{ temp = getDayCountInMonth(m_lYear, i); }

		//�������
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
	m_cMonthSB=((m_isLeap?getDayCountInLeapMonth(m_lYear):getDayCountInMonth(m_lYear,m_lMonth))>29)?_T("��"):_T("С");


	//////////////////////////////////////////////////////////////////////////
	//��ʼ�� ����

	//36 ��һѭ�� 
	////////���� 1900��������Ϊ������(60����36)
	m_cAnimal=m_Animals[(m_lYear-4)%12];
	m_cYear=getGZ(m_lYear-1900+36);
	///////���� 1900��1��С����ǰΪ ������(60����12)
	//m_cMonth= getGZ((y-1900)*12+m+12);
	m_cMonth= getGZ((m_lYear-1900)*12+m_lMonth+1+12);

	//����һ���� 1900/1/1 �������
	//1900/1/1�� 1970/1/1 ���25567��, 1900/1/1 ����Ϊ������(60����10)
	LONGLONG dayCyclical = getUTC(y,m,d)/864000000000+25567+10;
	m_cDay= getGZ(dayCyclical); 

	//////////////////////////////////////////////////////////////////////////
	//��ʼ�� ����
	//�������� 
	int nMSterm=getSTerm(y,(m-1)*2);
	if(d==getSTerm(y,(m-1)*2))
		m_solarTerms=m_solarTerm[(m-1)*2];
	else if(d==getSTerm(y,(m-1)*2+1))
		m_solarTerms=m_solarTerm[(m-1)*2+1];
	else
		m_solarTerms=_T("");

	//��������
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

	//���ܽ���
	int nWeekNo=0,nWeekDay=0; //�ڼ��ܣ����ڼ�
	for(int i=0;i<sizeof(m_wFtv)/sizeof(m_wFtv[0]);i++)
	{
		parseFtv(m_wFtv[i],nFtvM,nFtvD,strDesc,bStrong);
		nWeekNo=nFtvD/10;nWeekDay=nFtvD%10; //0--������
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

	//��ɫ������
	Date tmFirst = {y, m, 13, 5 };  //13���ǲ���������, ���6�滻Ϊ��ʵ�����ڼ����
	if(5 == tmFirst.day_in_week)
	{
		if(!m_solarFestival.IsEmpty())
			m_solarFestival+=_T(" ");
		m_solarFestival+=_T(" ��ɫ������");;
	}

	//�����ֻ������3��4�� ������վ��д�ʢ������ӣ����������3��21�ջ��������֮��ĵ�һ��������
	//if(m==3 || m==4) {
	// var estDay = new easter(y);
	// if(m == estDay.m)
	//  ' ����� Easter Sunday';
	//}


	//ũ������
	m_lunarFestival=_T("");
	for(int i=0;i<sizeof(m_lFtv)/sizeof(m_lFtv[0]);i++) 
	{
		parseFtv(m_lFtv[i],nFtvM,nFtvD,strDesc,bStrong);
		if((m_lMonth==nFtvM && m_lDay==nFtvD) ||
			(m_lMonth==nFtvM && nFtvD==0 &&  (m_isLeap?getDayCountInLeapMonth(m_lYear):getDayCountInMonth(m_lYear,m_lMonth))==m_lDay)) //���һ�죬��0��ʾ
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
	// ����ָ������� UTCʱ������ ��
	//�˶δ��� ��ϵ webmote 
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
	Date tm = {y,m,1,0};	// ��Ҫ��0�滻Ϊ��ʵ�����ڼ�
	Date tmCurr = {y,m,d,0}; // ��Ҫ��0�滻Ϊ��ʵ�����ڼ�
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
	//���㵱������
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
//��������
CString  CLunar::getWordDay(int d){
	CString s;
	switch (d) {
   case 10:
	   s = _T("��ʮ");
	   break;
   case 20:
	   s = _T("��ʮ");
	   break;
   case 30:
	   s = _T("��ʮ");
	   break;
   default :
	   s = m_nStr2[d/10];
	   s += m_nStr1[d%10];
	}
	return s;
}
//���
CString CLunar::getYearName(int y)
{
	CString cYear,cTmp;
	if(y>1874 && y<1909)  //����
	{
		(y-1874==1)?cTmp=_T("Ԫ"):cTmp.Format(_T("%d"),y-1874);
		cYear.Format(_T("����%s��"),cTmp);
	}
	else if(y>1908 && y<1912)  //��ͳ
	{
		(y-1908==1)?cTmp=_T("Ԫ"):cTmp.Format(_T("%d"),y-1908);
		cYear.Format(_T("��ͳ%s��"),cTmp);
	}
	else if(y>1911 && y<1950) //���
	{
		(y-1911==1)?cTmp=_T("Ԫ"):cTmp.Format(_T("%d"),y-1911);
		cYear.Format(_T("���%s��"),cTmp);
	}
	else if(y>1949)           //���͹�
	{
		(y-1949==1)?cTmp=_T("Ԫ"):cTmp.Format(_T("%d"),y-1949);
		cYear.Format(_T("���͹�%s��"),cTmp);
	}
	else
		cYear.Format(_T("��Ԫ%d��"),y);
	return cYear;
}

int CLunar::DaysBetween( WORD iEndYear, WORD iEndMonth, WORD iEndDay, WORD iStartYear, WORD iStartMonth, WORD iStartDay )
{
	WORD monthday[]={0, 31, 59 ,90, 120, 151, 181, 212, 243, 273, 304, 334}; 

	//�����������1��1��֮����������
	LONG iDiffDays =(iEndYear - iStartYear)*365;
	iDiffDays += (iEndYear-1)/4 - (iStartYear-1)/4;
	iDiffDays -= ((iEndYear-1)/100 - (iStartYear-1)/100);
	iDiffDays += (iEndYear-1)/400 - (iStartYear-1)/400;

	//����iEndYear��1��1�յ�iEndMonth��iEndDay��֮�������
	iDiffDays += monthday[iEndMonth-1] +
		(IsLeapYear(iEndYear)&&iEndMonth>2? 1: 0);
	iDiffDays += iEndDay;

	//��ȥiStartYear��1��1�յ�iStartMonth��iStartDay��֮�������
	iDiffDays -= (monthday[iStartMonth-1] + 
		(IsLeapYear(iStartYear)&&iStartMonth>2 ? 1: 0));
	iDiffDays -= iStartDay;	
	return iDiffDays;
}

bool CLunar::DateToLunar( const Date &date, Date& lunar )
{
	 DaysBetween(date.year, date.month, date.day);
}

