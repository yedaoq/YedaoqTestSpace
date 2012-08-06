#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#define BUFSIZE 80
typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
int __cdecl main_GetSystemVer()
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	BOOL bOsVersionInfoEx;
	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}
	// Call GetNativeSystemInfo if supported
	// or GetSystemInfo otherwise.
	pGNSI = (PGNSI) GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")), 
		"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);
	switch (osvi.dwPlatformId)
	{
		// Test for the Windows NT product family.
	case VER_PLATFORM_WIN32_NT:
		// Test for the specific product.
		if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 )
		{
			if( osvi.wProductType == VER_NT_WORKSTATION )
				printf ("Windows Vista ");
			else printf ("Windows Server \"Longhorn\" " );
		}
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			if( GetSystemMetrics(SM_SERVERR2) )
				printf( "Microsoft Windows Server 2003 \"R2\" ");
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
			{
				printf( "Microsoft Windows XP Professional x64 Edition ");
			}
			else printf ("Microsoft Windows Server 2003, ");
		}
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
			printf ("Microsoft Windows XP ");
		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			printf ("Microsoft Windows 2000 ");
		if ( osvi.dwMajorVersion <= 4 )
			printf ("Microsoft Windows NT ");
		// Test for specific product on Windows NT 4.0 SP6 and later.
		if( bOsVersionInfoEx )
		{
			// Test for the workstation type.
			if ( osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture!=PROCESSOR_ARCHITECTURE_AMD64)
			{
				if( osvi.dwMajorVersion == 4 )
					printf ( "Workstation 4.0 " );
				else if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
					printf ( "Home Edition " );
				else printf ( "Professional " );
			}

			// Test for the server type.
			else if ( osvi.wProductType == VER_NT_SERVER || 
				osvi.wProductType == VER_NT_DOMAIN_CONTROLLER )
			{
				if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2)
				{
					if ( si.wProcessorArchitecture ==
						PROCESSOR_ARCHITECTURE_IA64 )
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							printf ( "Datacenter Edition "
							"for Itanium-based Systems" );
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							printf ( "Enterprise Edition "
							"for Itanium-based Systems" );
					}
					else if ( si.wProcessorArchitecture ==
						PROCESSOR_ARCHITECTURE_AMD64 )
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							printf ( "Datacenter x64 Edition " );
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							printf ( "Enterprise x64 Edition " );
						else printf( "Standard x64 Edition " );
					}
					else
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
							printf ( "Datacenter Edition " );
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
							printf ( "Enterprise Edition " );
						else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
							printf ( "Web Edition " );
						else printf ( "Standard Edition " );
					}
				}
				else if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0)
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						printf ( "Datacenter Server " );
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						printf ( "Advanced Server " );
					else printf ( "Server " );
				}
				else // Windows NT 4.0 
				{
					if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						printf ("Server 4.0, Enterprise Edition " );
					else printf ( "Server 4.0 " );
				}
			}
		}
		// Test for specific product on Windows NT 4.0 SP5 and earlier
		else 
		{
			HKEY hKey;
			TCHAR szProductType[BUFSIZE];
			DWORD dwBufLen=BUFSIZE*sizeof(TCHAR);
			LONG lRet;
			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				TEXT("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey );
			if( lRet != ERROR_SUCCESS )
				return FALSE;
			lRet = RegQueryValueEx( hKey, TEXT("ProductType"),
				NULL, NULL, (LPBYTE) szProductType, &dwBufLen);
			RegCloseKey( hKey );
			if( (lRet != ERROR_SUCCESS) ||
				(dwBufLen > BUFSIZE*sizeof(TCHAR)) )
				return FALSE;
			if ( lstrcmpi( TEXT("WINNT"), szProductType) == 0 )
				printf( "Workstation " );
			if ( lstrcmpi( TEXT("LANMANNT"), szProductType) == 0 )
				printf( "Server " );
			if ( lstrcmpi( TEXT("SERVERNT"), szProductType) == 0 )
				printf( "Advanced Server " );
			printf( "%d.%d ", osvi.dwMajorVersion, osvi.dwMinorVersion );
		}
		// Display service pack (if any) and build number.
		if( osvi.dwMajorVersion == 4 && 
			lstrcmpi( osvi.szCSDVersion, TEXT("Service Pack 6") ) == 0 )
		{ 
			HKEY hKey;
			LONG lRet;
			// Test for SP6 versus SP6a.
			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
				TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"), 0, KEY_QUERY_VALUE, &hKey );
			if( lRet == ERROR_SUCCESS )
				printf( "Service Pack 6a (Build %d)\n", 
				osvi.dwBuildNumber & 0xFFFF );         
			else // Windows NT 4.0 prior to SP6a
			{
				_tprintf( TEXT("%s (Build %d)\n"),
					osvi.szCSDVersion,
					osvi.dwBuildNumber & 0xFFFF);
			}
			RegCloseKey( hKey );
		}
		else // not Windows NT 4.0 
		{
			_tprintf( TEXT("%s (Build %d)\n"),
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}
		break;
		// Test for the Windows Me/98/95.
	case VER_PLATFORM_WIN32_WINDOWS:
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			printf ("Microsoft Windows 95 ");
			if (osvi.szCSDVersion[1]=='C' || osvi.szCSDVersion[1]=='B')
				printf("OSR2 " );
		}
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10)
		{
			printf ("Microsoft Windows 98 ");
			if ( osvi.szCSDVersion[1]=='A' || osvi.szCSDVersion[1]=='B')
				printf("SE " );
		}
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90)
		{
			printf ("Microsoft Windows Millennium Edition\n");
		} 
		break;
	case VER_PLATFORM_WIN32s:
		printf ("Microsoft Win32s\n");
		break;
	}

	getchar();
	return TRUE; 
}