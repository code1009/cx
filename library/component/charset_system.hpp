#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring mbcs_to_wcs  (std::string  input, UINT codepage=CP_ACP);
std::string  wcs_to_mbcs  (std::wstring input, UINT codepage=CP_ACP);

std::string  utf8_to_mbcs (std::string  input, UINT codepage=CP_ACP);
std::string  mbcs_to_utf8 (std::string  input, UINT codepage=CP_ACP);

std::string  wcs_to_utf8 (std::wstring input);
std::wstring utf8_to_wcs (std::string input);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




