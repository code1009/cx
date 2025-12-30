/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/gw/gw.hpp>
#include <cx/runtime/runtime.hpp>

#include <cx/network/net_msg.hpp>
#include <cx/network/net_msg_memory.hpp>
#include <cx/network/net_msg_event_queue.hpp>
#include <cx/network/net_addr_config.hpp>
#include <cx/network/wsa.hpp>
#include <cx/runtime/log_facility/log_facility.hpp>

//===========================================================================
#include "../res/resource.h"

//===========================================================================
#include "Window/MainBox.hpp"

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
	rv = cx::gw::DirectX2dGraphic::createFactory();
	if (false == rv)
	{
		terminate();
		return false;
	}


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
	cx::gw::DirectX2dGraphic::destroyFactory();


	//-----------------------------------------------------------------------
	cx::runtime::log_facility_cleanup();


	//-----------------------------------------------------------------------
	::OleUninitialize();
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
