/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DebugMemory
{
public:
#ifdef _DEBUG
	typedef struct _MemoryLeakCheck
	{
		_CrtMemState start;
		_CrtMemState end  ;
		_CrtMemState diff ;
	}
	MemoryLeakCheck;

	MemoryLeakCheck _MemoryLeakCheck{};
#endif

public:
	DebugMemory();

public:
	virtual ~DebugMemory() = default;

public:
	DebugMemory(const DebugMemory&) = delete;
	DebugMemory& operator=(const DebugMemory&) = delete;

	DebugMemory(DebugMemory&&) = delete;
	DebugMemory& operator=(DebugMemory&&) = delete;

public:
	void enableMemoryCheck(void);

public:
	void startMemoryLeakCheck(void);
	void endMemoryLeakCheck(void);

public:
	void testMemoryLeak(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





