/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "runtime.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool getLastErrorString(std::wstring& lastErrorString, DWORD lastErrorCode)
{
	DWORD dwLastErrorCode;
	DWORD dwSystemLocale;
	DWORD dwFlags;
	HLOCAL hLocal;
	DWORD dwLength;


	dwLastErrorCode = lastErrorCode;
	dwSystemLocale  = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
	dwFlags         = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	hLocal          = NULL;
	dwLength        = FormatMessageW(dwFlags, NULL, dwLastErrorCode, dwSystemLocale, (LPWSTR)&hLocal, 0, NULL);
	if (dwLength && hLocal)
	{
		lastErrorString = (LPWSTR)LocalLock(hLocal);
		LocalFree(hLocal);
		return true;
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




