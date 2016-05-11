#include "StdAfx.h"
#include "MyVolumeCtrl.h"
#pragma comment(lib, "Winmm.lib")
CMyVolumeCtrl::CMyVolumeCtrl(void)
:m_pEndptVolCtrl(NULL),
m_pEnumerator(NULL),
m_pDevice(NULL),
m_guidMyContext(GUID_NULL)
{
	m_eWinType = vGetWindowsType();
	TRACE("Now windows is %d", m_eWinType);
	if(WindowsVista == m_eWinType|| Windows7 == m_eWinType)
	{
		vInitEndPoint();
	}
}
CMyVolumeCtrl::~CMyVolumeCtrl(void)
{
	if(WindowsVista == m_eWinType|| Windows7 == m_eWinType)
	{
		vUninitEndPoint();
	}

	m_eWinType = UnknownOS;
}
int CMyVolumeCtrl::GetMaxVol()
{
	return MAX_VOL;
}
int CMyVolumeCtrl::GetMinVol()
{
	return 0;
}
int CMyVolumeCtrl::GetVolume()
{
	int nVol = 0;
	DWORD dwNowMaxVol = GetMaxVol();
	switch (m_eWinType)
	{
	case WindowsXP:
		{

			unsigned int nXPVol = vGetVolume(0);
			TRACE("XP vol = %d, nowmax = %d", nXPVol, dwNowMaxVol);
			nVol = nXPVol;
		}
		break;
	case WindowsVista:
	case Windows7:
		{
			DWORD dwNewVol = 0;
			vVolumeGet(&dwNewVol);
			nVol = dwNewVol;
		}

		break;
	default:
		nVol = -1;
	}
	return nVol;
}
BOOL CMyVolumeCtrl::SetVolume(int nVol)
{
	BOOL bSet = FALSE;
	DWORD dwNowMaxVol = GetMaxVol();
	switch (m_eWinType)
	{
	case WindowsXP:
		{
			long nXPSetVol = nVol;
			bSet = vSetVolume(0, nXPSetVol);
		}
		break;
	case WindowsVista:
	case Windows7:
		{
			int nWin7SetVol = nVol ;
			bSet = vVolumeSet(nWin7SetVol);
		}
		break;
	default:
		bSet = FALSE;
	}
	return bSet;
}
BOOL CMyVolumeCtrl::SetMute(BOOL bMute)
{
	BOOL bSet = FALSE;
	switch (m_eWinType)
	{
	case WindowsXP:
		bSet = vSetMute(0, bMute);
		break;
	case WindowsVista:
	case Windows7:
		bSet = vMuteSet(bMute);
		break;
	default:
		bSet = FALSE;
	}
	return bSet;
}
BOOL CMyVolumeCtrl::GetMute()
{
	BOOL bMute = FALSE;
	switch (m_eWinType)
	{
	case WindowsXP:
		bMute = vGetMute(0);
		break;
	case WindowsVista:
	case Windows7:
		bMute  = vMuteGet();
		break;
	default:
		bMute = FALSE;
	}
	return bMute;
}



unsigned int CMyVolumeCtrl::vGetVolume(int   dev)//�õ��豸������dev=0��������1WAVE   ,2MIDI   ,3   LINE   IN
{
	long   device;
	unsigned   int rt=0;
	MIXERCONTROL   volCtrl;
	HMIXER   hmixer;
	switch   (dev)
	{
	case   1:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;
	case   2:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;
	case   3:
		//     device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;     //   cd   ����
		//     device=MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE;   break;       //��˷�����
		//     device=MIXERLINE_COMPONENTTYPE_SRC_LINE;   break;                   //PC   ����������
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;
	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}
	if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;
	if(!vGetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_VOLUME,&volCtrl))
		return   0;
	rt=vGetVolumeValue(hmixer,&volCtrl)*MAX_VOL/volCtrl.Bounds.lMaximum;
	mixerClose(hmixer);
	return   rt;
}
//---------------------------------------------------------------------------
BOOL CMyVolumeCtrl::vSetVolume(long   dev,long   vol)//�����豸������
{
	//   dev   =0,1,2       �ֱ��ʾ������,����,MIDI   ,LINE   IN
	//   vol=0-MAX_VOL           ��ʾ�����Ĵ�С   ,   �����뷵��������ֵ�õ��ǰٷֱȣ���������0   -   MAX_VOL���������豸�ľ���ֵ
	//   retrun   false   ��ʾ���������Ĵ�С�Ĳ������ɹ�
	//   retrun   true     ��ʾ���������Ĵ�С�Ĳ����ɹ�
	long   device;
	BOOL   rc = FALSE;
	MIXERCONTROL   volCtrl;
	HMIXER   hmixer;
	switch   (dev)
	{
	case   1:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;
	case   2:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;
	case   3:
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;
	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}
	if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;
	if(vGetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_VOLUME,&volCtrl))
	{
		vol=vol*volCtrl.Bounds.lMaximum/MAX_VOL;
		if(vSetVolumeValue(hmixer,&volCtrl,vol))
			rc=true;
	}
	mixerClose(hmixer);
	return   rc;
}
//---------------------------------------------------------------------------
BOOL CMyVolumeCtrl::vSetMute(long   dev,long   vol)//�����豸����
{
	//   dev   =0,1,2       �ֱ��ʾ������,����,MIDI   ,LINE   IN
	//   vol=0,1             �ֱ��ʾȡ������,���þ���
	//   retrun   false   ��ʾȡ�������þ����������ɹ�
	//   retrun   true     ��ʾȡ�������þ��������ɹ�
	long   device;
	BOOL   rc = FALSE;
	MIXERCONTROL   volCtrl;
	HMIXER   hmixer;
	switch   (dev)
	{
	case   1:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;
	case   2:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;
	case   3:
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;
	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}
	if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;
	if(vGetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_MUTE,&volCtrl))
		if(vSetMuteValue(hmixer,&volCtrl,(BOOL)vol))
			rc=TRUE;
	mixerClose(hmixer);
	return   rc;
}
//---------------------------------------------------------------------------
BOOL CMyVolumeCtrl::vGetMute(long   dev)//����豸�Ƿ���
{
	//dev   =0,1,2     �ֱ��ʾ������������,MIDI   ,LINE   IN
	//   retrun   false   ��ʾû�о���
	//   retrun   true     ��ʾ����
	long   device;
	BOOL   rc = FALSE;
	MIXERCONTROL   volCtrl;
	HMIXER   hmixer;
	switch   (dev)
	{
	case   1:
		device=MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;   break;
	case   2:
		device=MIXERLINE_COMPONENTTYPE_SRC_SYNTHESIZER;   break;
	case   3:
		device=MIXERLINE_COMPONENTTYPE_SRC_COMPACTDISC;   break;
	default:
		device=MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	}
	if(mixerOpen(&hmixer,   0,   0,   0,   0))   return   0;
	if(vGetVolumeControl(hmixer,device,MIXERCONTROL_CONTROLTYPE_MUTE,&volCtrl))
		rc = vGetMuteValue(hmixer,&volCtrl);
	mixerClose(hmixer);
	return   rc;
}


BOOL  CMyVolumeCtrl::vGetVolumeControl(HMIXER   hmixer   ,long   componentType,long   ctrlType,MIXERCONTROL*   mxc)
{
	MIXERLINECONTROLS   mxlc;
	MIXERLINE   mxl;
	mxl.cbStruct   =   sizeof(mxl);
	mxl.dwComponentType   =   componentType;
	if(!mixerGetLineInfo((HMIXEROBJ)hmixer,   &mxl,   MIXER_GETLINEINFOF_COMPONENTTYPE))
	{
		mxlc.cbStruct   =   sizeof(mxlc);
		mxlc.dwLineID   =   mxl.dwLineID;
		mxlc.dwControlType   =   ctrlType;
		mxlc.cControls   =   1;
		mxlc.cbmxctrl   =   sizeof(MIXERCONTROL);
		mxlc.pamxctrl   =   mxc;

		if(mixerGetLineControls((HMIXEROBJ)hmixer,&mxlc,MIXER_GETLINECONTROLSF_ONEBYTYPE))
			return   FALSE;
		else
		{
			return   TRUE;
		}
	}
	return   FALSE;
}
//---------------------------------------------------------------------------
long CMyVolumeCtrl::vGetMuteValue(HMIXER   hmixer   ,MIXERCONTROL   *mxc)
{
	MIXERCONTROLDETAILS   mxcd;
	MIXERCONTROLDETAILS_BOOLEAN   mxcdMute;
	mxcd.hwndOwner   =   0;
	mxcd.cbStruct   =   sizeof(mxcd);
	mxcd.dwControlID   =   mxc-> dwControlID;
	mxcd.cbDetails   =   sizeof(mxcdMute);
	mxcd.paDetails   =   &mxcdMute;
	mxcd.cChannels   =   1;
	mxcd.cMultipleItems   =   0;
	if   (mixerGetControlDetails((HMIXEROBJ)hmixer,   &mxcd,MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE))
		return   -1;
	return   mxcdMute.fValue;
}
//---------------------------------------------------------------------------
unsigned int CMyVolumeCtrl::vGetVolumeValue(HMIXER   hmixer   ,MIXERCONTROL   *mxc)
{
	MIXERCONTROLDETAILS   mxcd;
	MIXERCONTROLDETAILS_UNSIGNED   vol;   vol.dwValue=0;
	mxcd.hwndOwner   =   0;
	mxcd.cbStruct   =   sizeof(mxcd);
	mxcd.dwControlID   =   mxc-> dwControlID;
	mxcd.cbDetails   =   sizeof(vol);
	mxcd.paDetails   =   &vol;
	mxcd.cChannels   =   1;
	if(mixerGetControlDetails((HMIXEROBJ)hmixer,   &mxcd,   MIXER_OBJECTF_HMIXER|MIXER_GETCONTROLDETAILSF_VALUE))
		return   -1;
	return   vol.dwValue;
}
//---------------------------------------------------------------------------
BOOL CMyVolumeCtrl::vSetMuteValue(HMIXER   hmixer   ,MIXERCONTROL   *mxc,   BOOL   mute)
{
	MIXERCONTROLDETAILS   mxcd;
	MIXERCONTROLDETAILS_BOOLEAN   mxcdMute;mxcdMute.fValue=mute;
	mxcd.hwndOwner   =   0;
	mxcd.dwControlID   =   mxc-> dwControlID;
	mxcd.cbStruct   =   sizeof(mxcd);
	mxcd.cbDetails   =   sizeof(mxcdMute);
	mxcd.paDetails   =   &mxcdMute;
	mxcd.cChannels   =   1;
	mxcd.cMultipleItems   =   0;
	if   (mixerSetControlDetails((HMIXEROBJ)hmixer,   &mxcd,   MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))
		return   FALSE;
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL CMyVolumeCtrl::vSetVolumeValue(HMIXER   hmixer   ,MIXERCONTROL   *mxc,   long   volume)
{
	MIXERCONTROLDETAILS   mxcd;
	MIXERCONTROLDETAILS_UNSIGNED   vol;vol.dwValue   =   volume;
	mxcd.hwndOwner   =   0;
	mxcd.dwControlID   =   mxc-> dwControlID;
	mxcd.cbStruct   =   sizeof(mxcd);
	mxcd.cbDetails   =   sizeof(vol);
	mxcd.paDetails   =   &vol;
	mxcd.cChannels   =   1;
	if(mixerSetControlDetails((HMIXEROBJ)hmixer,   &mxcd,   MIXER_OBJECTF_HMIXER|MIXER_SETCONTROLDETAILSF_VALUE))
		return   FALSE;
	return   TRUE;
}




BOOL CMyVolumeCtrl::vInitEndPoint()
{
	if(m_pEndptVolCtrl != NULL)
		return FALSE;
	//g_EPVolEvents.SetCallback(fCallback); //set callback
	HRESULT hr = S_OK;
	CoInitialize(NULL);
	hr = CoCreateGuid(&m_guidMyContext);
	if(FAILED(hr))
		return FALSE;
	// Get enumerator for audio endpoint devices.
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&m_pEnumerator);
	if(FAILED(hr))
		return FALSE;
	// Get default audio-rendering device.
	hr = m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
	if(FAILED(hr))
		return FALSE;
	hr = m_pDevice->Activate(__uuidof(IAudioEndpointVolume),
		CLSCTX_ALL, NULL, (void**)&m_pEndptVolCtrl);
	if(FAILED(hr))
		return FALSE;

	if(FAILED(hr))
		return FALSE;
	return TRUE;
}

BOOL CMyVolumeCtrl::vMuteSet(BOOL bMute)
{
	if(m_pEndptVolCtrl)
		m_pEndptVolCtrl->SetMute(bMute, &m_guidMyContext);
	return TRUE;
}
BOOL CMyVolumeCtrl::vMuteGet()
{
	BOOL bMute = FALSE;
	if(m_pEndptVolCtrl)
		m_pEndptVolCtrl->GetMute(&bMute);
	return bMute;
}
BOOL CMyVolumeCtrl::vVolumeSet(DWORD dwVolume)
{
	if((int)dwVolume < 0)
		dwVolume = 0;
	if((int)dwVolume > MAX_VOL)
		dwVolume = MAX_VOL;
	if(m_pEndptVolCtrl)
	{
		HRESULT hr = m_pEndptVolCtrl->SetMasterVolumeLevelScalar((float)dwVolume / MAX_VOL, &m_guidMyContext);
		return SUCCEEDED(hr) ? TRUE : FALSE;
	}
	return FALSE;
}
BOOL CMyVolumeCtrl::vVolumeGet(DWORD* pdwVolume)
{
	float fVolume;
	if(m_pEndptVolCtrl)
	{
		m_pEndptVolCtrl->GetMasterVolumeLevelScalar(&fVolume);
		if(pdwVolume != NULL)
			*pdwVolume =(DWORD)( MAX_VOL * fVolume + 0.5);
	}
	return TRUE;
}

BOOL CMyVolumeCtrl::vUninitEndPoint()
{
	SAFE_RELEASE(m_pEnumerator);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pEndptVolCtrl);
	CoUninitialize();
	return TRUE;
}
WindowsType CMyVolumeCtrl::vGetWindowsType()
{
	OSVERSIONINFO info;
	ZeroMemory(&info, sizeof(OSVERSIONINFO));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	WindowsType eType = UnknownOS;
	if(GetVersionEx(&info))
	{
		TRACE("GetVersionEx: MajorVer = %d, MinVer = %d, BuildNo = %d", info.dwMajorVersion, info.dwMinorVersion, info.dwBuildNumber);
		if(info.dwMajorVersion <= 5)//XP or early
		{
			eType = WindowsXP;
		}else
		{
			if(info.dwMinorVersion == 0)
			{
				eType = WindowsVista;
			}else
			{
				eType = Windows7;
			}
		}
	}
	return eType;
}