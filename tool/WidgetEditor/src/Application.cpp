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
#include "Window/ToolBox.hpp"
#include "Window/View.hpp"
#include "Window/MainFrame.hpp"

//===========================================================================
#include "Application.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool Application::initialize(void)
{
	bool rv;


	rv = runtime::WindowApplication::initialize();
	if (false == rv)
	{
		terminate();
		return false;
	}

	return true;
}

void Application::terminate(void)
{
	runtime::WindowApplication::terminate();
}

void Application::run(void)
{
	MainFrame mainFrame;

	wui::WindowMessageLoop windowMessageLoop;
	windowMessageLoop.addIdleHandler(std::bind(&MainFrame::onIdle, &mainFrame));
	windowMessageLoop.run();
}

void Application::launch(void)
{
	if (initialize())
	{
		run();
		//_DebugMemory.testMemoryLeak();
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
