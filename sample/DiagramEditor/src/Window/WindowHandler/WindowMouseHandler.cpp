/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "WindowMouseHandler.hpp"





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
bool WindowMouseHandler::isWindowMouseMessage(cx::wui::WindowMessage& windowMessage)
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
bool WindowMouseHandler::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	bool handled = false;
	onWindowMouseMessage(windowMessage, handled);
	return handled;
}

void WindowMouseHandler::onWindowMouseMessage(cx::wui::WindowMessage& windowMessage, bool& handled)
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
void WindowMouseHandler::onMouseWheel(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

//===========================================================================
void WindowMouseHandler::onMouseMove(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

//===========================================================================
void WindowMouseHandler::onMouseHOver(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void WindowMouseHandler::onMouseLeave(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

//===========================================================================
void WindowMouseHandler::onMouseLButtonDbClk(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void WindowMouseHandler::onMouseLButtonDown(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void WindowMouseHandler::onMouseLButtonUp(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

//===========================================================================
void WindowMouseHandler::onMouseRButtonDbClk(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void WindowMouseHandler::onMouseRButtonDown(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}

void WindowMouseHandler::onMouseRButtonUp(cx::wui::WindowMessage& windowMessage, bool& handled)
{
}
