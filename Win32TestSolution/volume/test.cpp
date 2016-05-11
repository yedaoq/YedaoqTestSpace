

bool GetVolumeControl(HMIXER hmixer ,long componentType,long ctrlType,MIXERCONTROL* mxc)
{
	MIXERLINECONTROLS mxlc;
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(mxl);
	mxl.dwComponentType = componentType;
	if(!mixerGetLineInfo((HMIXEROBJ)hmixer, &mxl, MIXER_GETLINEINFOF_COMPONENTTYPE))
	{
		mxlc.cbStruct = sizeof(mxlc);
		mxlc.dwLineID = mxl.dwLineID;
		mxlc.dwControlType = ctrlType;
		mxlc.cControls = 1;
		mxlc.cbmxctrl = sizeof(MIXERCONTROL);
		mxlc.pamxctrl = mxc;
		if(mixerGetLineControls((HMIXEROBJ)hmixer,&mxlc,MIXER_GETLINECONTROLSF_ONEBYTYPE))
			return 0;
		else
			return 1;
	}
	return 0;
}

unsigned GetVolumeValue(HMIXER hmixer ,MIXERCONTROL *mxc)
{
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED vol; vol.dwValue=0;
	mxcd.hwndOwner = 0;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbDetails = sizeof(vol);
	mxcd.paDetails = &vol;
	mxcd.cChannels = 1;
	if(mixerGetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE))
		return -1;
	return vol.dwValue;
}

bool SetVolumeValue(HMIXER hmixer ,MIXERCONTROL *mxc, long volume)
{
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED vol;vol.dwValue = volume;
	mxcd.hwndOwner = 0;
	mxcd.dwControlID = mxc->dwControlID;
	mxcd.cbStruct = sizeof(mxcd);
	mxcd.cbDetails = sizeof(vol);
	mxcd.paDetails = &vol;
	mxcd.cChannels = 1;
	if(mixerSetControlDetails((HMIXEROBJ)hmixer, &mxcd, MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))
		return 0;
	return 1;
}

//设置音量值
bool WINAPI SetVolume(LONG vol)
{
	bool Result=false;   //用于判断函数是否执行成功
	MIXERCONTROL volCtrl;
	HMIXER hmixer;
	if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0;
	if(GetVolumeControl(hmixer,MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,MIXERCONTROL_CONTROLTYPE_VOLUME,&volCtrl))
	{ 
		int ChangeValue=volCtrl.Bounds.lMaximum/INTERVAL;
		int TotalValue=ChangeValue*vol;
		if(SetVolumeValue(hmixer,&volCtrl,TotalValue))
			Result=true;
	}   
	mixerClose(hmixer);
	return Result;
}
//返回音量值
unsigned WINAPI GetVolume()
{
	unsigned rt=0; 
	MIXERCONTROL volCtrl;
	HMIXER hmixer; 
	if(mixerOpen(&hmixer, 0, 0, 0, 0)) return 0;
	if(!GetVolumeControl(hmixer, MIXERLINE_COMPONENTTYPE_DST_SPEAKERS,MIXERCONTROL_CONTROLTYPE_VOLUME,&volCtrl))
		return 0;
	int ChangeValue=volCtrl.Bounds.lMaximum/INTERVAL; //每一次调用函数改变的音量值
	rt=GetVolumeValue(hmixer,&volCtrl)/ChangeValue;   //GetVolumeValue得到的是具体值
	mixerClose(hmixer);
	return rt;
}

