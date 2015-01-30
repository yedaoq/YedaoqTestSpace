#include "stdafx.h"
#include <Windows.h>
#include <wininet.h>

#pragma comment(lib, "Wininet.lib")

bool InternetSetProxy(LPCTSTR rsa_conn_name, LPCTSTR proxy_full_addr)

{
	//conn_name: rsa connection name, or set null for lan. 
	//proxy_full_addr : eg "210.78.22.87:8000", or set null for disable proxy.
	INTERNET_PER_CONN_OPTION_LIST    list;
	INTERNET_PER_CONN_OPTION        options[3];


	list.dwSize = sizeof(list);    // Fill out list struct.    
	list.pszConnection = const_cast<LPTSTR>(rsa_conn_name);    // NULL == LAN, otherwise connectoid name.  
	list.dwOptionCount = (proxy_full_addr) ? 3 : 1;
	list.dwOptionError = 0;
	list.pOptions = options;


	// Set flags.    
	options[0].dwOption = INTERNET_PER_CONN_FLAGS;
	options[0].Value.dwValue = PROXY_TYPE_DIRECT;
	if (proxy_full_addr)
	{
		options[0].Value.dwValue |= PROXY_TYPE_PROXY;


		// Set proxy server.    
		options[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
		options[1].Value.pszValue = const_cast<LPTSTR>(proxy_full_addr);//"http://proxy:80"; // Set proxy override.    
		options[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
		options[2].Value.pszValue = TEXT("<local>");    // some implemention use "local"
	}


	// Set the options on the connection.
	BOOL bReturn = InternetSetOption(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &list, sizeof(list));


	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL,0);
	InternetSetOption(NULL, INTERNET_OPTION_REFRESH , NULL,0);


	return FALSE != bReturn;
}
