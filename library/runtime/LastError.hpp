﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool getLastErrorString(
	std::wstring& lastErrorString, 
	DWORD lastErrorCode=::GetLastError()
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





