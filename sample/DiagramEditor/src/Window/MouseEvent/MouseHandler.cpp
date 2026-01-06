/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "MouseHandler.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MouseHandler::MouseHandler(HWND hwnd) :
	_Hwnd(hwnd)
{
}

//===========================================================================
void MouseHandler::getMouseDbClickTime(std::uint64_t& time)
{
	time = _MouseDbClickTime;
}

void MouseHandler::setMouseDbClickTime(std::uint64_t time)
{
	_MouseDbClickTime = time;
}

//===========================================================================
bool MouseHandler::getMouseCaptureEnabled(void)
{
	return _MouseCaptureEnabled;
}

void MouseHandler::setMouseCaptureEnabled(bool enabled)
{
	_MouseCaptureEnabled = enabled;
	if (false == enabled)
	{
		releaseMouseCapture();
	}
}

void MouseHandler::setMouseCapture(HWND hwnd)
{
	if (getMouseCaptureEnabled())
	{
		_MouseCaptured = true;
		::SetCapture(hwnd);
	}
}

void MouseHandler::releaseMouseCapture(void)
{
	if (_MouseCaptured)
	{
		::ReleaseCapture();
		_MouseCaptured = false;
	}
}

//===========================================================================
bool MouseHandler::getMouseTrackEnabled(void)
{
	return _MouseTrackEnabled;
}

void MouseHandler::setMouseTrackEnabled(bool enabled)
{
	_MouseTrackEnabled = enabled;
	if (false == enabled)
	{
		releaseMouseTrack();
	}
}

void MouseHandler::setMouseTrack(HWND hwnd)
{
	if (getMouseTrackEnabled())
	{
		if (false == _MouseTracked)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.hwndTrack = hwnd;
			tme.dwHoverTime = 1000;

			if (TrackMouseEvent(&tme))
			{
				_MouseTracked = true;
			}
		}
	}
}

void MouseHandler::releaseMouseTrack(void)
{
	if (_MouseTracked)
	{
		_MouseTracked = false;
	}
}

//===========================================================================
bool MouseHandler::isWindowMouseMessage(cx::wui::WindowMessage& windowMessage)
{
	switch (windowMessage.uMsg)
	{
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
	case WM_MOUSEHOVER:
	case WM_MOUSELEAVE:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		return true;

	default:
		break;
	}

	return false;
}

//===========================================================================
bool MouseHandler::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	bool handled = false;
	onWindowMouseMessage(windowMessage, handled);
	return handled;
}

void MouseHandler::onWindowMouseMessage(cx::wui::WindowMessage& windowMessage, bool& handled)
{
	switch (windowMessage.uMsg)
	{
	//-----------------------------------------------------------------------
	case WM_MOUSEWHEEL:
		onMouseWheel(windowMessage, handled);
		break;

	//-----------------------------------------------------------------------
	case WM_MOUSEMOVE:
		onMouseMove(windowMessage, handled);
		break;

	//-----------------------------------------------------------------------
	case WM_MOUSEHOVER:
		onMouseHOver(windowMessage, handled);
		break;

	case WM_MOUSELEAVE:
		onMouseLeave(windowMessage, handled);
		break;

	//-----------------------------------------------------------------------
	case WM_LBUTTONDBLCLK:
		onMouseLButtonDbClk(windowMessage, handled);
		break;

	case WM_LBUTTONDOWN:
		onMouseLButtonDown(windowMessage, handled);
		break;

	case WM_LBUTTONUP:
		onMouseLButtonUp(windowMessage, handled);
		break;

	//-----------------------------------------------------------------------
	case WM_RBUTTONDBLCLK:
		onMouseLButtonDbClk(windowMessage, handled);
		break;

	case WM_RBUTTONDOWN:
		onMouseLButtonDown(windowMessage, handled);
		break;

	case WM_RBUTTONUP:
		onMouseLButtonUp(windowMessage, handled);
		break;

	default:
		break;
	}
}

//===========================================================================
void MouseHandler::onMouseWheel(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

//===========================================================================
void MouseHandler::onMouseMove(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

//===========================================================================
void MouseHandler::onMouseHOver(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void MouseHandler::onMouseLeave(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

//===========================================================================
void MouseHandler::onMouseLButtonDbClk(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void MouseHandler::onMouseLButtonDown(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void MouseHandler::onMouseLButtonUp(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

//===========================================================================
void MouseHandler::onMouseRButtonDbClk(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void MouseHandler::onMouseRButtonDown(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void MouseHandler::onMouseRButtonUp(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}
