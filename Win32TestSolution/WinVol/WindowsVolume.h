#pragma once

class IWindowsVolume
{
public:
	virtual ~WindowsVolume(void) = 0 {}

	virtual int		GetVolume() = 0;
	virtual void	SetVolume(int vol) = 0;
};

class WindowsVolume_XP : IWindowsVolume
{
public:
	virtual int		GetVolume();
	virtual void	SetVolume(int vol);

};

class WindowsVolume_Win7 : IWindowsVolume
{
public:
	virtual int		GetVolume();
	virtual void	SetVolume(int vol);

protected:
	IMMDevice		GetAudioEndpoint();

};