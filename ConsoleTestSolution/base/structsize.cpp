// base.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

struct test_struct
{
	char	z;
	int		x;
	char	y;
};

#pragma pack(push, 2)

struct test_struct_a2
{
	char	z;
	int		x;
	char	y;
};

#pragma pack(pop)

#pragma pack(push, 2)

struct msg
{
	void* buf;
	short size;
	short asize;
};



#pragma pack(pop)


int _tmain_structsize(int argc, _TCHAR* argv[])
{
	printf("size of struct(char,int,char) : %d\n", sizeof(test_struct));
	printf("size of struct(char,int,char) align 2bytes : %d\n", sizeof(test_struct_a2));
	getchar();
	return 0;
}

