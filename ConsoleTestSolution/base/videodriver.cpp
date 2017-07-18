#include   "stdafx.h"  
#include   <stdlib.h>  
#include   <windows.h>  
#include   <setupapi.h>  

#pragma comment(lib, "SetupApi.lib")

int main_videodriver(int   argc,   char*   argv[])  
{  
	DWORD dwIdx   =   0;  
	SP_DEVINFO_DATA adapterData;  
	DWORD dwType   =   0;  
	DWORD dwBuflen   =   0;  
	PUCHAR pBuf   =   NULL;  
	HDEVINFO hScsi   =   INVALID_HANDLE_VALUE;  
//	{4d36e96c-e325-11ce-bfc1-08002be10318}

// 	GUID guidScsiRaid   =   {  
// 		0x4D36E97B,  
// 		0xE325,  
// 		0x11CE,  
// 		{0xBF,   0xC1,   0x08,   0x00,   0x2B,   0xE1,   0x03,   0x18}  
// 	};  

	GUID guidScsiRaid   =   {  
		0x4D36E968,  
		0xE325,  
		0x11CE,  
		{0xBF,   0xC1,   0x08,   0x00,   0x2B,   0xE1,   0x03,   0x18}  
	};  

	BOOL   bret   =   FALSE;  

	hScsi   =   SetupDiGetClassDevs(&guidScsiRaid  
		,   NULL,   NULL,   DIGCF_PRESENT);  

	adapterData.cbSize   =   sizeof(SP_DEVINFO_DATA);  

	for(dwIdx   =   0;   SetupDiEnumDeviceInfo(hScsi,   dwIdx,   &adapterData);   dwIdx++)  
	{  
		SetupDiGetDeviceRegistryProperty(  
			hScsi,    
			&adapterData,    
			SPDRP_DEVICEDESC,  
			&dwType,  
			NULL,  
			0,  
			&dwBuflen  
			);  

		if(ERROR_INSUFFICIENT_BUFFER   ==   GetLastError())  
		{  
			pBuf   =   (PUCHAR)malloc(dwBuflen);  
			if(SetupDiGetDeviceRegistryProperty(  
				hScsi,    
				&adapterData,    
				SPDRP_DEVICEDESC,  
				&dwType,  
				pBuf,  
				dwBuflen,  
				NULL  
				))  
			{  
				_tprintf(TEXT("%s\n"),   pBuf);  
			}  

			free(pBuf);  
			pBuf   =   NULL;  
		}  
	}  

	SetupDiDestroyDeviceInfoList(hScsi);  

	getchar();

	return   0;  
}