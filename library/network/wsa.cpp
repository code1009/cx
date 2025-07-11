/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

//===========================================================================
#include "../runtime/runtime.hpp"
#include "../common/charset_system.hpp"

//===========================================================================
#include "wsa.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment(lib, "ws2_32")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void wsa_dump(WSADATA& _wsa_data)
{
	CX_RUNTIME_LOG(cxLDebug) << L"WSA Version:             0x" << std::hex << _wsa_data.wVersion;
	CX_RUNTIME_LOG(cxLDebug) << L"WSA High version:        0x" << std::hex << _wsa_data.wHighVersion;
	CX_RUNTIME_LOG(cxLDebug) << L"WSA Max sockets:         " << _wsa_data.iMaxSockets;
	CX_RUNTIME_LOG(cxLDebug) << L"WSA Max UDP packet size: " << _wsa_data.iMaxUdpDg;
	CX_RUNTIME_LOG(cxLDebug) << L"WSA System status:       " << mbcs_to_wcs(_wsa_data.szSystemStatus);
	CX_RUNTIME_LOG(cxLDebug) << L"WSA Descritpion:         " << mbcs_to_wcs(_wsa_data.szDescription);
}

bool wsa_startup(int uHigh, int uLow)
{
	WSADATA _wsa_data;

	WORD dwVersion;
	int  nError;


	dwVersion = MAKEWORD(uHigh, uLow);
	nError = WSAStartup(dwVersion, &_wsa_data);
	if (nError != 0)
	{
		return false;
	}

	if (LOBYTE(_wsa_data.wVersion) != uHigh ||
		HIBYTE(_wsa_data.wVersion) != uLow)
	{
		WSACleanup();
		return false;
	}

	wsa_dump(_wsa_data);

	return true;
}

void wsa_cleanup(void)
{
	WSACleanup();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}
