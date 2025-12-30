/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring braces (std::wstring v)
{
	std::wstring s;

	s = L"{'";
	s+= v;
	s+= L"}'";

	return s;
}

std::wstring brackets (std::wstring v) // square brackets
{
	std::wstring s;

	s = L"['";
	s+= v;
	s+= L"]'";

	return s;
}

std::wstring parentheses (std::wstring v) // round brackets
{
	std::wstring s;

	s = L"(";
	s+= v;
	s+= L")'";

	return s;
}

std::wstring squot(std::wstring v)  // add single quotation_marks
{
	std::wstring s;

	s = L"\'";
	s+= v;
	s+= L"\'";

	return s;
}

std::wstring dquot(std::wstring v) // add double quotation_marks
{
	std::wstring s;

	s = L"\"";
	s+= v;
	s+= L"\"";

	return s;
}

std::wstring eline(void) 
{
	std::wstring end_line = L"\r\n";


	return end_line;
}

std::wstring itab (std::size_t n) // indent tab
{
	std::wstring s;
	std::size_t  i;


	for(i=0u; i<n; i++)
	{
		s += L"\t";
	}

	return s;
}

std::wstring ispace (std::size_t n)
{
	std::wstring s;
	std::size_t  i;


	for(i=0u; i<n; i++)
	{
		s += L" ";
	}

	return s;
}

std::wstring ispace2 (std::size_t n)
{
	std::wstring s;
	std::size_t  i;


	for(i=0u; i<n; i++)
	{
		s += L"  ";
	}

	return s;
}








/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
