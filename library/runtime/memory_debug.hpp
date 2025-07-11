/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#ifdef _DEBUG
struct memory_check_point
{
	_CrtMemState start;
	_CrtMemState end;
	_CrtMemState diff;
};
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class memory_debug
{
public:
#ifdef _DEBUG
	memory_check_point _leak_check_point{};
#endif

public:
	memory_debug();

public:
	virtual ~memory_debug() = default;

public:
	memory_debug(const memory_debug&) = delete;
	memory_debug& operator=(const memory_debug&) = delete;

	memory_debug(memory_debug&&) = delete;
	memory_debug& operator=(memory_debug&&) = delete;

public:
	void enable(void);

public:
	void start_leak_check(void);
	void end_leak_check(void);

public:
	void test_leak_check(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





