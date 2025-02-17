/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "ToolBox.hpp"

#include "ToolBox_Item.hpp"
#include "ToolBox_Drawing.hpp"
#include "ToolBox_ItemView.hpp"
#include "ToolBox_ControlWindow.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::ControlWindow::ControlWindow(HWND hwnd) :
	cx::gw::Window(hwnd, false),
	_WindowHandle(hwnd)
{
	_ItemView = std::make_unique<ToolBox::ItemView>(this);
}

//===========================================================================
bool ToolBox::ControlWindow::createDeviceResources(void)
{
	//-----------------------------------------------------------------------
	bool rv;


	rv = cx::gw::Window::createDeviceResources();
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	if (getItemView())
	{
		rv = getItemView()->createDeviceResources(getContext());
		if (!rv)
		{
			return false;
		}
	}


	return true;
}

void ToolBox::ControlWindow::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (getItemView())
	{
		getItemView()->destroyDeviceResources();
	}


	//-----------------------------------------------------------------------
	cx::gw::Window::destroyDeviceResources();
}

void ToolBox::ControlWindow::draw(void)
{
	//-----------------------------------------------------------------------
	cx::gw::Window::draw();


	//-----------------------------------------------------------------------
	if (getItemView())
	{
		getItemView()->draw(getContext());
	}
}

//===========================================================================
ToolBox::ItemView* ToolBox::ControlWindow::getItemView(void)
{
	return _ItemView.get();
}
