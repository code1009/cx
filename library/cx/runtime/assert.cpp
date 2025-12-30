/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "assert.hpp"
#include "log.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void assert_condition(
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


		DWORD thread_id = GetCurrentThreadId();


		std::wstringstream ss;


		ss << std::endl;
		ss << L" # ASSERTION FAILED" << std::endl;
		ss << std::endl;

		ss << L" - DATETIME: ";
		write_log_datetime_YYYYMMDD(ss, st);
		ss << L" ";
		write_log_datetime_hhmmssms(ss, st);
		ss << std::endl;

		ss << L" - THREAD: "    ; write_log_thread_id (ss, thread_id);   ss << std::endl;
		ss << L" - FILE: "      ; write_log_source_file_line(ss, file, line); ss << std::endl;
		ss << L" - FUNCTION: "  ; write_log_source_func (ss, func);       ss << std::endl;

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




