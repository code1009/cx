#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring parentheses (std::wstring v);
std::wstring brackets (std::wstring v);
std::wstring braces (std::wstring v);

std::wstring dquot(std::wstring v);
std::wstring squot(std::wstring v);

std::wstring eline(void);

std::wstring itab (std::size_t n);
std::wstring ispace (std::size_t n);
std::wstring ispace2 (std::size_t n);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
