﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

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
	bool rv;
	rv = cx::runtime::WindowApplication::initialize();
	if (false == rv)
	{
		terminate();
		return false;
	}
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
	cx::gw::DirectX2dGraphic::destroyFactory();
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
