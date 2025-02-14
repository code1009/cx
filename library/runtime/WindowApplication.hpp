/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowApplication
{
public:
	DebugMemory _DebugMemory;

public:
	WindowApplication() = default;

public:
	virtual ~WindowApplication() = default;

public:
	WindowApplication(const WindowApplication&) = delete;
	WindowApplication& operator=(const WindowApplication&) = delete;

	WindowApplication(WindowApplication&&) = delete;
	WindowApplication& operator=(WindowApplication&&) = delete;

public:
	virtual bool initialize(void);
	virtual void terminate(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




