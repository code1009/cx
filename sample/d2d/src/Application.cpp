/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>
#include <cx/runtime/log_facility/log_facility.hpp>
#include <cx/wui/wui.hpp>
#include <cx/network/network.hpp>

//===========================================================================
#include "../res/resource.h"

//===========================================================================
#include "Window/View.hpp"
#include "Window/MainFrame.hpp"

//===========================================================================
#include "Application.hpp"





/////////////////////////////////////////////////////////////////////////////
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
	rv = cx::network::wsa_startup();
	if (false == rv)
	{
		terminate();
		return false;
	}
	rv = cx::network::net_msg_memory_initialize();
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
	cx::network::net_msg_memory_cleanup();
	cx::network::wsa_cleanup();


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo)
		<< L"----------------------------------------------------------------------------" << std::endl
		<< L"END" << std::endl
		<< L"----------------------------------------------------------------------------"
		;


	//-----------------------------------------------------------------------
	cx::runtime::log_facility_cleanup();


	//-----------------------------------------------------------------------
	::OleUninitialize();
}

void Application::run(void)
{
	MainFrame mainFrame;
	cx::wui::WindowMessageLoop windowMessageLoop;
	//windowMessageLoop.addIdleHandler(std::bind(&MainFrame::onIdle, &mainFrame));
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


