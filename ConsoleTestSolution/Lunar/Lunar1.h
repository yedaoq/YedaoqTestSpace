#ifndef _lunar_H_
#define _lunar_H_

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

struct SolarDate
{
	uint Year;
	uchar Month;
	uchar Date;
};
struct LunarDate
{
	uint Year;
	uchar Month;
	uchar Date;
	uchar GAN;  //¸É
	uchar ZHI;  //Ö§
};

uint Lunar(struct SolarDate *pSS,struct LunarDate *pLL);

#endif