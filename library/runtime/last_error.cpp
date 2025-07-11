/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "last_error.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool get_window_last_error_string(std::wstring& last_error_string, DWORD last_error_code)
{
	DWORD dwLastErrorCode;
	DWORD dwSystemLocale;
	DWORD dwFlags;
	HLOCAL hLocal;
	DWORD dwLength;


	dwLastErrorCode = last_error_code;
	dwSystemLocale  = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
	dwFlags         = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
	hLocal          = NULL;
	dwLength        = FormatMessageW(dwFlags, NULL, dwLastErrorCode, dwSystemLocale, (LPWSTR)&hLocal, 0, NULL);
	if (dwLength && hLocal)
	{
		last_error_string = (LPWSTR)LocalLock(hLocal);
		LocalFree(hLocal);
		return true;
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




