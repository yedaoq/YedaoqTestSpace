#pragma once

#include <Windows.h>

extern HINSTANCE hInst;

struct INavBarCallback
{
	virtual ~INavBarCallback() = 0 {}

	virtual void OnNavBarMouseHover(int pos) = 0;
};

class NavBar
{
public:
	NavBar(void);
	~NavBar(void);

	bool Create(HWND hParent, INavBarCallback* callback);

public:

	HWND				hwnd_;
	INavBarCallback*	callback_;
};
