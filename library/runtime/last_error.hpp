﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool get_window_last_error_string(
	std::wstring& last_error_string, 
	DWORD last_error_code = ::GetLastError()
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





