/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "WindowScrollHandler.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowScrollHandler::WindowScrollHandler(HWND hwnd) :
	_Hwnd(hwnd)
{
}

//===========================================================================
bool WindowScrollHandler::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	bool handled = false;
	onWindowScrollMessage(windowMessage, handled);
	return handled;
}

void WindowScrollHandler::onWindowScrollMessage(cx::wui::WindowMessage& windowMessage, bool& handled)
{
	switch (windowMessage.uMsg)
	{
	//-----------------------------------------------------------------------
	case WM_MOUSEWHEEL:

	default:
		break;
	}
}
