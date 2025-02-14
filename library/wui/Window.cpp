/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
HWND Window::getWindowHandle(void) const
{
	return _WindowHandle;
}

HWND Window::setWindowHandle(HWND handle)
{
	HWND old;


	old = _WindowHandle;
	_WindowHandle = handle;

	return old;
}

LRESULT Window::onMessage(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return defaultWindowProc(hwnd, umsg, wparam, lparam);
}

LRESULT Window::defaultWindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return ::DefWindowProcW(hwnd, umsg, wparam, lparam);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




