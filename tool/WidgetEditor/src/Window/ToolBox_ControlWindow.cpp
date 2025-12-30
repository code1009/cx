/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/gw/gw.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "ToolBox.hpp"
#include "ToolBox_Item.hpp"
#include "ToolBox_Drawing.hpp"
#include "ToolBox_WindowMessageHandler.hpp"
#include "ToolBox_ItemView.hpp"
#include "ToolBox_ControlWindow.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::ControlWindow::ControlWindow(ToolBox* toolBox) :
	cx::gw::Window(toolBox->getWindowHandle(), false),
	_ToolBox(toolBox)
{
	_ItemView = std::make_unique<ToolBox::ItemView>(this);
	_WindowMessageHandler.setItemView(_ItemView.get());

	createDeviceIndependentResources();
}

//===========================================================================
bool ToolBox::ControlWindow::createDeviceIndependentResources(void)
{
	bool rv;


	rv = cx::gw::Window::createDeviceIndependentResources();
	if (!rv)
	{
		return false;
	}


	rv = getItemView()->createDeviceIndependentResources(getContext());
	if (!rv)
	{
		return false;
	}

	return true;
}

void ToolBox::ControlWindow::destroyDeviceIndependentResources(void)
{
	getItemView()->destroyDeviceIndependentResources();


	cx::gw::Window::destroyDeviceIndependentResources();
}

//===========================================================================
bool ToolBox::ControlWindow::createDeviceResources(void)
{
	bool rv;


	rv = cx::gw::Window::createDeviceResources();
	if (!rv)
	{
		return false;
	}


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
	if (getItemView())
	{
		getItemView()->destroyDeviceResources();
	}


	cx::gw::Window::destroyDeviceResources();
}

void ToolBox::ControlWindow::draw(void)
{
	cx::gw::Window::draw();


	if (getItemView())
	{
		getItemView()->draw(getContext());
	}
}

