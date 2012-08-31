class A
{
public:
	int fun1(int p1)
	{
		mem = p1 = 1;
		return 0;
	}

	int __stdcall fun2(int p1)
	{
		mem = p1 = 1;
		return 0;
	}

	int __cdecl fun3(int p1, ...)
	{
		mem = p1 = 1;
		return 0;
	}

	void test()
	{
		fun1(0);
		fun2(1);
		fun3(2);
		fun3(3,1,2);
	}

	int mem;
};

int _tmain(int argc, wchar_t* argv[])
{
	A a;
	a.test();
	return 0;
}