/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>
#include <runtime/log_facility/log_facility.hpp>

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
class Application
{
public:
	Application() = default;

public:
	virtual ~Application() = default;

	//-----------------------------------------------------------------------
public:
	bool initialize(void);
	void terminate(void);

	//-----------------------------------------------------------------------
public:
	void run(void);
	void launch(void);
};

//===========================================================================
bool Application::initialize(void)
{
	//-----------------------------------------------------------------------
	HRESULT hr;
	hr = OleInitialize(nullptr);
	if (FAILED(hr))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	bool rv;
	rv = cx::runtime::log_facility_initialize();
	if (false == rv)
	{
		printf("Failed to initialize log item memory.\n");
		return false;
	}
	CX_RUNTIME_LOG(cxLInfo)
		<< L"----------------------------------------------------------------------------" << std::endl
		<< L"START" << std::endl
		<< L"----------------------------------------------------------------------------"
		;


	//-----------------------------------------------------------------------
	rv = cx::gw::DirectX2dGraphic::createFactory();
	if (false == rv)
	{
		terminate();
		return false;
	}


	return true;
}

void Application::terminate(void)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"----------------------------------------------------------------------------" << std::endl
		<< L"END" << std::endl
		<< L"----------------------------------------------------------------------------"
		;


	//-----------------------------------------------------------------------
	cx::gw::DirectX2dGraphic::destroyFactory();


	//-----------------------------------------------------------------------
	cx::runtime::log_facility_cleanup();


	//-----------------------------------------------------------------------
	::OleUninitialize();
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
	cx::runtime::memory_debug _memory_debug;
	_memory_debug.enable();
	_memory_debug.start_leak_check();

	cx::wui::getAppModule()->setInstanceHandle(hInstance);
	getApplication()->launch();

	_memory_debug.test_leak_check();
	_memory_debug.end_leak_check();

	return 0;
}
