/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "runtime.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// https://learn.microsoft.com/ko-kr/cpp/c-runtime-library/debug-routines?view=msvc-170
// https://learn.microsoft.com/ko-kr/cpp/sanitizers/asan?view=msvc-170





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
DebugMemory::DebugMemory()
{
#ifdef _DEBUG
	memset(&_MemoryLeakCheck, 0, sizeof(_MemoryLeakCheck));
#endif
}

void DebugMemory::enableMemoryCheck(void)
{
#ifdef _DEBUG
	// Enable run-time memory check for debug builds.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//testMemoryLeak();
#endif
}

void DebugMemory::startMemoryLeakCheck(void)
{
#ifdef _DEBUG
	_CrtMemCheckpoint(&_MemoryLeakCheck.start);

	//testMemoryLeak();
#endif
}

void DebugMemory::endMemoryLeakCheck(void)
{
#ifdef _DEBUG
	_CrtMemCheckpoint(&_MemoryLeakCheck.end);


	if (_CrtMemDifference(&_MemoryLeakCheck.diff, &_MemoryLeakCheck.start, &_MemoryLeakCheck.end))
	{
		_CrtMemDumpStatistics(&_MemoryLeakCheck.diff);
	}
#endif
}

void DebugMemory::testMemoryLeak(void)
{
#ifdef _DEBUG
	char* memory_leack_p0;
	char* memory_leack_p1;
	void* memory_leack_p2;
	void* memory_leack_p3;


	memory_leack_p0 = new char[16];
	memory_leack_p1 = cpp_new char[16];
	memory_leack_p2 = malloc(16);
	memory_leack_p3 = calloc(16, 1);
#endif
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




