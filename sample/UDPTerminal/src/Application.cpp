/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

#include <network/net_msg.hpp>
#include <network/net_msg_memory.hpp>
#include <network/net_msg_event_queue.hpp>
#include <network/net_addr_config.hpp>
#include <network/wsa.hpp>

#include <runtime/LogFacility/LogFacility.hpp>

//===========================================================================
#include "../res/resource.h"
	
//===========================================================================
#include "WebUI/WebUI.hpp"
#include "Window/MainBox.hpp"
#include "Model/Model.hpp"

//===========================================================================
#include "Application.hpp"





/////////////////////////////////////////////////////////////////////////////
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
	
	getModel()->setWebUIManager(nullptr);

	rv = cx::runtime::LogFacility_Initialize();
	if (false == rv)
	{
		terminate();
		return false;
	}

	CX_RUNTIME_LOG(cxLInfo) << L"START" << std::endl;

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

	rv = app::WebMessageMemory_Initialize();
	if (false == rv)
	{
		terminate();
		return false;
	}

	return true;
}

void Application::terminate(void)
{
	getModel()->disconnect();

	app::WebMessageMemory_Cleanup();
	cx::network::net_msg_memory_cleanup();

	CX_RUNTIME_LOG(cxLInfo) << L"END" << std::endl;	
	
	cx::runtime::LogFacility_Cleanup();

	cx::network::wsa_cleanup();
	cx::runtime::WindowApplication::terminate();
}

void Application::run(void)
{
	MainBox mainBox;
	mainBox.doModal(nullptr);
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
