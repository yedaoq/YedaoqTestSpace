#include "StdAfx.h"
#include "WindowsVolume.h"


int WindowsVolume_XP::GetVolume()
{
	throw std::exception("The method or operation is not implemented.");
}

void WindowsVolume_XP::SetVolume()
{
	throw std::exception("The method or operation is not implemented.");
}

int WindowsVolume_Win7::GetVolume()
{
	ATL::CComPtr<IMMDevice>	audio_endpoint(GetAudioEndpoint());
	ATL::CComPtr<IAudioEndpointVolume>	audio_volume;
	if(audio_endpoint)
	{
		HRESULT hr = audio_endpoint->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, 0, &audio_volume);
		if(SUCCEEDED(hr))
		{
			audio_volume->GetMasterVolumeLevelScalar();
		}
	}
}

void WindowsVolume_Win7::SetVolume( int vol )
{
	
}

IMMDevice* WindowsVolume_Win7::GetAudioEndpoint()
{
	IMMDevice*							device = 0;
	ATL::CComPtr<IMMDeviceEnumerator>	device_enumerator;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device_enumerator));
	if (SUCCEEDED(hr))
	{
		hr = device_enumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eMultimedia, &device);
	}

	return device;
}
