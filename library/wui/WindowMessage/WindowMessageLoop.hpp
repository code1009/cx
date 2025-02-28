/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowMessageLoop
{
public:
	using IdleHandler = std::function<void(void)>;

private:
	std::vector< IdleHandler > _idleHandlers;

public:
	WindowMessageLoop() = default;

public:
	virtual ~WindowMessageLoop() = default;

public:
	WindowMessageLoop(const WindowMessageLoop&) = delete;
	WindowMessageLoop& operator=(const WindowMessageLoop&) = delete;

	WindowMessageLoop(WindowMessageLoop&&) = delete;
	WindowMessageLoop& operator=(WindowMessageLoop&&) = delete;

public:
	virtual void addIdleHandler(IdleHandler handler);

public:
	virtual void run(void);
	virtual void onIdle(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




