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
void assertCondition(
	bool condition,
	const wchar_t* expression,
	const wchar_t* file,
	int            line,
	const wchar_t* func
)
{
	if (!condition)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);


		DWORD threadId = GetCurrentThreadId();


		std::wstringstream ss;


		ss << std::endl;
		ss << L" # ASSERTION FAILED" << std::endl;
		ss << std::endl;

		ss << L" - DATETIME: ";
		writeLogDateTime_YYYYMMDD_String(ss, &st);
		ss << L" ";
		writeLogDateTime_hhmmssms_String(ss, &st);
		ss << std::endl;

		ss << L" - THREAD: "    ; writeLogThreadIdString      (ss, threadId);   ss << std::endl;
		ss << L" - FILE: "      ; writeLogSourceFileLineString(ss, file, line); ss << std::endl;
		ss << L" - FUNCTION: "  ; writeLogSourceFuncString    (ss, func);       ss << std::endl;

		ss << L" - EXPRESSION:" ; ss << std::endl;
		ss << L"   "; ss << expression; ss << std::endl;
		ss << std::endl;

		OutputDebugStringW(ss.str().c_str());

		::MessageBoxW(nullptr, ss.str().c_str(), L"CRITICAL ERROR", MB_ICONERROR | MB_OK);

		exit(-1);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




