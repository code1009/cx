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
	void addIdleHandler(IdleHandler handler);

public:
	void run(HACCEL hAccelTable=nullptr);

private:
	void runStandard(void);
	void runWithAccel(HACCEL hAccelTable);
	void runWithIdle(void);
	void runWithIdleAndAccel(HACCEL hAccelTable);

private:
	void processIdleHandlers(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




