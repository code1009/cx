/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "memory_debug.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
Reference:
https://learn.microsoft.com/ko-kr/cpp/sanitizers/asan?view=msvc-170
https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/debug-routines?view=msvc-170
https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/find-memory-leaks-using-the-crt-library?view=msvc-170
https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/recommendations-for-choosing-between-functions-and-macros?view=msvc-170
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
memory_debug::memory_debug()
{
#ifdef _DEBUG
	memset(&_leak_check_point, 0, sizeof(_leak_check_point));
#endif
}

void memory_debug::enable(void)
{
#ifdef _DEBUG
	// Enable run-time memory check for debug builds.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//testMemoryLeak();
#endif
}

void memory_debug::start_leak_check(void)
{
#ifdef _DEBUG
	_CrtMemCheckpoint(&_leak_check_point.start);

	//testMemoryLeak();
#endif
}

void memory_debug::end_leak_check(void)
{
#ifdef _DEBUG
	_CrtMemCheckpoint(&_leak_check_point.end);


	if (_CrtMemDifference(&_leak_check_point.diff, &_leak_check_point.start, &_leak_check_point.end))
	{
		_CrtMemDumpStatistics(&_leak_check_point.diff);
	}
#endif
}

void memory_debug::test_leak_check(void)
{
#ifdef _DEBUG
	char* memory_leak_p0;
	char* memory_leak_p1;
	void* memory_leak_p2;
	void* memory_leak_p3;


	memory_leak_p0 = new char[16];
	memory_leak_p1 = cpp_new char[16];
	memory_leak_p2 = malloc(16);
	memory_leak_p3 = calloc(16, 1);

#if 0
\Library\cx\library\runtime\memory_debug.cpp(88) : {3401} normal block at 0x00000170A1E74A70, 16 bytes long. <-- memory_leak_p3 = calloc(16, 1);
Data : < > 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
\Library\cx\library\runtime\memory_debug.cpp(87) : {3400} normal block at 0x00000170A1E743E0, 16 bytes long. <-- memory_leak_p2 = malloc(16);
Data : <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
\Library\cx\library\runtime\memory_debug.cpp(86) : {3399} normal block at 0x00000170A1E74610, 16 bytes long. <-- memory_leak_p1 = cpp_new char[16];
Data : <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
{ 3398 } normal block at 0x00000170A1E73DF0, 16 bytes long.                                                  < -- memory_leak_p0 = new char[16];
Data : <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
Object dump complete.
#endif

#endif
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




