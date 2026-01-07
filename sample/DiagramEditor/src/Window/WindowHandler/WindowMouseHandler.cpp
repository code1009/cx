/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>
#include <cx/wui/wui.hpp>

//===========================================================================
#include "WindowMouseHandler.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowMouseHandler::WindowMouseHandler(HWND hwnd) :
	_Hwnd(hwnd)
{
}

//===========================================================================
void WindowMouseHandler::getMouseDbClickTime(std::uint64_t& time)
{
	time = _MouseDbClickTime;
}

void WindowMouseHandler::setMouseDbClickTime(std::uint64_t time)
{
	_MouseDbClickTime = time;
}

//===========================================================================
bool WindowMouseHandler::getMouseCaptureEnabled(void)
{
	return _MouseCaptureEnabled;
}

void WindowMouseHandler::setMouseCaptureEnabled(bool enabled)
{
	_MouseCaptureEnabled = enabled;
	if (false == enabled)
	{
		releaseMouseCapture();
	}
}

void WindowMouseHandler::setMouseCapture(void)
{
	if (getMouseCaptureEnabled())
	{
		_MouseCaptured = true;
		::SetCapture(_Hwnd);
	}
}

void WindowMouseHandler::releaseMouseCapture(void)
{
	if (_MouseCaptured)
	{
		::ReleaseCapture();
		_MouseCaptured = false;
	}
}

//===========================================================================
bool WindowMouseHandler::getMouseTrackEnabled(void)
{
	return _MouseTrackEnabled;
}

void WindowMouseHandler::setMouseTrackEnabled(bool enabled)
{
	_MouseTrackEnabled = enabled;
	if (false == enabled)
	{
		releaseMouseTrack();
	}
}

void WindowMouseHandler::setMouseTrack(void)
{
	if (getMouseTrackEnabled())
	{
		if (false == _MouseTracked)
		{
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER | TME_LEAVE;
			tme.hwndTrack = _Hwnd;
			tme.dwHoverTime = 1000;

			if (TrackMouseEvent(&tme))
			{
				_MouseTracked = true;
			}
		}
	}
}

void WindowMouseHandler::releaseMouseTrack(void)
{
	if (_MouseTracked)
	{
		_MouseTracked = false;
	}
}

//===========================================================================
bool WindowMouseHandler::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	switch (windowMessage.uMsg)
	{
	case WM_MOUSEWHEEL: return onMouseWheel(windowMessage);
	case WM_MOUSEMOVE: return onMouseMove(windowMessage);
	case WM_MOUSEHOVER: return onMouseHover(windowMessage);
	case WM_MOUSELEAVE: return onMouseLeave(windowMessage);
	case WM_LBUTTONDBLCLK: return onMouseLButtonDbClk(windowMessage);
	case WM_LBUTTONDOWN: return onMouseLButtonDown(windowMessage);
	case WM_LBUTTONUP: return onMouseLButtonUp(windowMessage);
	case WM_RBUTTONDBLCLK: return onMouseLButtonDbClk(windowMessage);
	case WM_RBUTTONDOWN: return onMouseLButtonDown(windowMessage);
	case WM_RBUTTONUP: return onMouseLButtonUp(windowMessage);
	default:
		break;
	}
	return false;
}

//===========================================================================
bool WindowMouseHandler::onMouseWheel(cx::wui::WindowMessage& windowMessage)
{
	if (mouseWheelHandler)
	{
		return mouseWheelHandler(windowMessage);
	}
	return false;
}

//===========================================================================
bool WindowMouseHandler::onMouseMove(cx::wui::WindowMessage& windowMessage)
{
	if (mouseMoveHandler)
	{
		return mouseMoveHandler(windowMessage);
	}
	return false;
}

//===========================================================================
bool WindowMouseHandler::onMouseHover(cx::wui::WindowMessage& windowMessage)
{
	if (mouseHoverHandler)
	{
		return mouseHoverHandler(windowMessage);
	}
	return false;
}

bool WindowMouseHandler::onMouseLeave(cx::wui::WindowMessage& windowMessage)
{
	if (mouseLeaveHandler)
	{
		return mouseLeaveHandler(windowMessage);
	}
	return false;
}

//===========================================================================
bool WindowMouseHandler::onMouseLButtonDbClk(cx::wui::WindowMessage& windowMessage)
{
	if (mouseLButtonDbClkHandler)
	{
		return mouseLButtonDbClkHandler(windowMessage);
	}
	return false;
}

bool WindowMouseHandler::onMouseLButtonDown(cx::wui::WindowMessage& windowMessage)
{
	if (mouseLButtonDownHandler)
	{
		return mouseLButtonDownHandler(windowMessage);
	}
	return false;
}

bool WindowMouseHandler::onMouseLButtonUp(cx::wui::WindowMessage& windowMessage)
{
	if (mouseLButtonUpHandler)
	{
		return mouseLButtonUpHandler(windowMessage);
	}
	return false;
}

//===========================================================================
bool WindowMouseHandler::onMouseRButtonDbClk(cx::wui::WindowMessage& windowMessage)
{
	if (mouseRButtonDbClkHandler)
	{
		return mouseRButtonDbClkHandler(windowMessage);
	}
	return false;
}

bool WindowMouseHandler::onMouseRButtonDown(cx::wui::WindowMessage& windowMessage)
{
	if (mouseRButtonDownHandler)
	{
		return mouseRButtonDownHandler(windowMessage);
	}
	return false;
}

bool WindowMouseHandler::onMouseRButtonUp(cx::wui::WindowMessage& windowMessage)
{
	if (mouseRButtonUpHandler)
	{
		return mouseRButtonUpHandler(windowMessage);
	}
	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}

