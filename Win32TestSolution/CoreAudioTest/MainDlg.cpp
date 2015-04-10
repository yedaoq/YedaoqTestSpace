// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include <atlcomcli.h>
#include <mmsystem.h>
#include <Endpointvolume.h>
#include <mmdeviceapi.h>


#define TIMER_ID_VOLINFO 1001

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	ctl_info_.Attach(GetDlgItem(IDC_LST_INFO));

	SetDlgItemInt(IDC_TXT_LEVEL, -100, TRUE);
	SetDlgItemInt(IDC_TXT_SCALER, -100, TRUE);

	SetTimer(TIMER_ID_VOLINFO, 2000, 0);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

float GetDlgItemFloat(HWND hWnd, WORD ID)
{
	TCHAR buf[256];
	float val = 0;
	GetDlgItemText(hWnd, ID, buf, ARRAYSIZE(buf) - 1);
	_stscanf(buf, TEXT("%f"), &val);
	return val;
}

IMMDevice* GetAudioEndpoint()
{
	IMMDevice*							device = 0;
	ATL::CComPtr<IMMDeviceEnumerator>	device_enumerator;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device_enumerator));
	if (SUCCEEDED(hr))
	{
		hr = device_enumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
	}

	return device;
}

HRESULT GetDeviceVolume(IMMDevice* device, IAudioEndpointVolume** volume)
{
	HRESULT hr = E_FAIL;
	if(device && volume)
	{
		hr = device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, 0, (void**)volume);
	}

	return hr;
}

void CMainDlg::SetVolume(int channel, float level, float level_scaler)
{
	ATL::CComPtr<IMMDevice>				audio_device(GetAudioEndpoint());
	ATL::CComPtr<IAudioEndpointVolume>	audio_volume;
	HRESULT hr = GetDeviceVolume(audio_device, &audio_volume);
	if(FAILED(hr))
		return;

	if(channel)
	{
		if((int)level != -100)
		{
			hr = audio_volume->SetChannelVolumeLevel(channel - 1, level, 0);
			if(FAILED(hr))
			{
				ctl_info_.InsertString(0, TEXT("fail to set channel level"));
			}
		}

		if((int)level_scaler != -100)
		{
			hr = audio_volume->SetChannelVolumeLevelScalar(channel - 1, level_scaler, 0);
			if(FAILED(hr))
			{
				ctl_info_.InsertString(0, TEXT("fail to set channel level scaler"));
			}
		}
	}
	else
	{
		if((int)level != -100)
		{
			hr = audio_volume->SetMasterVolumeLevel(level, 0);
			if(FAILED(hr))
			{
				ctl_info_.InsertString(0, TEXT("fail to set master level"));
			}
		}

		if((int)level_scaler != -100)
		{
			hr = audio_volume->SetMasterVolumeLevel(level_scaler, 0);
			if(FAILED(hr))
			{
				ctl_info_.InsertString(0, TEXT("fail to set master level scaler"));
			}
		}
	}
}

void CMainDlg::ShowVolumeInfo()
{
	ATL::CComPtr<IMMDevice>				audio_device(GetAudioEndpoint());
	ATL::CComPtr<IAudioEndpointVolume>	audio_volume;
	HRESULT hr = GetDeviceVolume(audio_device, &audio_volume);

	ctl_info_.InsertString(0, TEXT(""));
	
	if(SUCCEEDED(hr))
	{
		UINT	uValue;
		float	fValue;
		TCHAR	buf[1024];

		float vMin, vMax, VInc;
		if(SUCCEEDED(audio_volume->GetVolumeRange(&vMin, &vMax, &VInc)))
		{
			_stprintf(buf, TEXT("Volume Range : %f - %f   %f(inc)\r\n"), vMin, vMax, VInc);
			ctl_info_.InsertString(0, buf);
		}

		if(SUCCEEDED(audio_volume->GetMasterVolumeLevelScalar(&fValue)))
		{
			_stprintf(buf, TEXT("Master Volume Level Scaler : %f\r\n"), fValue);
			ctl_info_.InsertString(0, buf);
		}

		if(SUCCEEDED(audio_volume->GetMasterVolumeLevel(&fValue)))
		{
			_stprintf(buf, TEXT("Master Volume Level : %f\r\n"), fValue);
			ctl_info_.InsertString(0, buf);
		}

		if(SUCCEEDED(audio_volume->GetChannelCount(&uValue)))
		{
// 			_stprintf(buf, TEXT("Channel Count : %d\r\n"), uValue);
// 			ctl_info_.InsertString(0, buf);

			for (int i = 0; i < uValue; ++i)
			{
				if(SUCCEEDED(audio_volume->GetChannelVolumeLevel(i, &fValue)))
				{
					_stprintf(buf, TEXT("Channel Volume Level (%d) : %f\r\n"), i, fValue);
					ctl_info_.InsertString(0, buf);
				}

				if(SUCCEEDED(audio_volume->GetChannelVolumeLevelScalar(i, &fValue)))
				{
					_stprintf(buf, TEXT("Channel Volume Level Scaler (%d) : %f\r\n"), i, fValue);
					ctl_info_.InsertString(0, buf);
				}
			}
		}
	}

}

LRESULT CMainDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
{
	return 0;
}

LRESULT CMainDlg::OnTimer( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	ShowVolumeInfo();
	return 0;
}

LRESULT CMainDlg::OnBnClickedSet(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	volume_level_to_set_ = GetDlgItemFloat(m_hWnd, IDC_TXT_LEVEL);
	volume_channel_to_set_ = GetDlgItemInt( IDC_TXT_CHANNEL, 0, FALSE);
	volume_level_scaler_to_set_ = GetDlgItemFloat(m_hWnd, IDC_TXT_SCALER);

	SetVolume(volume_channel_to_set_, volume_level_to_set_, volume_level_scaler_to_set_);

	return 0;
}

LRESULT CMainDlg::OnBnClickedBtnUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	ATL::CComPtr<IMMDevice>				audio_device(GetAudioEndpoint());
	ATL::CComPtr<IAudioEndpointVolume>	audio_volume;
	HRESULT hr = GetDeviceVolume(audio_device, &audio_volume);
	if(SUCCEEDED(hr))
	{
		audio_volume->VolumeStepUp(0);
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	ATL::CComPtr<IMMDevice>				audio_device(GetAudioEndpoint());
	ATL::CComPtr<IAudioEndpointVolume>	audio_volume;
	HRESULT hr = GetDeviceVolume(audio_device, &audio_volume);
	if(SUCCEEDED(hr))
	{
		audio_volume->VolumeStepDown(0);
	}

	return 0;
}
