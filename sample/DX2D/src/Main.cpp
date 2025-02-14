/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "../res/resource.h"

//===========================================================================
#include "Main.hpp"

//===========================================================================
#include "Window/View.hpp"
#include "Window/MainFrame.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Application : public cx::runtime::WindowApplication
{
public:
	Application() = default;

public:
	virtual ~Application() = default;

	//-----------------------------------------------------------------------
	// cx::runtime::WindowApplication
public:
	virtual bool initialize(void) override;
	virtual void terminate(void) override;

	//-----------------------------------------------------------------------
	// Application
public:
	void run(void);
	void launch(void);
};

//===========================================================================
bool Application::initialize(void)
{
	bool rv;


	rv = cx::runtime::WindowApplication::initialize();
	if (false == rv)
	{
		terminate();
		return false;
	}

	return true;
}

void Application::terminate(void)
{
	cx::runtime::WindowApplication::terminate();
}

void Application::run(void)
{
	MainFrame mainFrame;

	cx::wui::WindowMessageLoop windowMessageLoop;
	windowMessageLoop.addIdleHandler(std::bind(&MainFrame::onIdle, &mainFrame));
	windowMessageLoop.run();
}

void Application::launch(void)
{
	if (initialize())
	{
		run();
		_DebugMemory.testMemoryLeak();
		terminate();
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Application* getApplication(void)
{
	static Application instance;


	return &instance;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int APIENTRY wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{
	cx::wui::getAppModule()->setInstanceHandle(hInstance);

	
	getApplication()->launch();

	return 0;
}
