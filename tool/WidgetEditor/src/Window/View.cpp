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
#include "View.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR View_WindowClassName = L"View";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
View::View(HWND parentWindowHandle)
{
	//-----------------------------------------------------------------------
	wui::WindowClass windowClass;


	windowClass.registerWindowClass(
		View_WindowClassName
	);


	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createView(parentWindowHandle);
	if (nullptr == hwnd)
	{
		throw std::runtime_error("View::View(): createView() failed");
	}


	//-----------------------------------------------------------------------
	_gwWindow = std::make_unique<gw::BaseEditWindow>(*this);
	_gwWindow->getStatusOverayPanel()->setVisible(true);
	_gwWindow->getDocumentGrid()->setVisible(true);


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);
}

//===========================================================================
HWND View::createView(HWND parentWindowHandle)
{
	LPCWSTR   lpszClassName = View_WindowClassName;
	HWND      hWndParent    = parentWindowHandle;
	LPCWSTR   lpWindowName  = L"Window";
	DWORD     dwStyle       = wui::ChildWindowStyle;
	DWORD     dwExStyle     = wui::ChildWindowStyleEx;


	//dwStyle |= WS_VSCROLL;
	//dwStyle |= WS_HSCROLL;
	//dwExStyle |= WS_EX_CLIENTEDGE;


	HWND hwnd;
	hwnd = createWindow(
		lpszClassName,
		hWndParent,
		lpWindowName,
		dwStyle,
		dwExStyle
	);

	return hwnd;
}

//===========================================================================
void View::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_CREATE    ) = &View::onCreate;
	_WindowMessageMap.handle(WM_DESTROY   ) = &View::onDestroy;
	_WindowMessageMap.handle(WM_CLOSE     ) = &View::onClose;

	_WindowMessageMap.handle(WM_SIZE      ) = &View::onSize;
	_WindowMessageMap.handle(WM_HSCROLL   ) = &View::onHScroll;
	_WindowMessageMap.handle(WM_VSCROLL   ) = &View::onVScroll;

	_WindowMessageMap.handle(WM_MOUSEWHEEL) = &View::onMouseWheel;
	_WindowMessageMap.handle(WM_MOUSEMOVE ) = &View::onMouseMove;

	_WindowMessageMap.handle(WM_KEYDOWN   ) = &View::onKeyDown;

	_WindowMessageMap.handle(WM_ERASEBKGND) = &View::onEraseBkgnd;
	_WindowMessageMap.handle(WM_PAINT     ) = &View::onPaint;

	_WindowMessageMap.handle(WM_COMMAND   ) = &View::onCommand;
}

void View::onCreate(wui::WindowMessage& windowMessage)
{
}

void View::onDestroy(wui::WindowMessage& windowMessage)
{
}

void View::onClose(wui::WindowMessage& windowMessage)
{
}

void View::onSize(wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	RECT rect;

	GetClientRect(*this, &rect);


	//-----------------------------------------------------------------------
	UINT cx;
	UINT cy;


	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);


	//-----------------------------------------------------------------------
	if (_gwWindow)
	{
		_gwWindow->resize(cx, cy);
	}
}

void View::onHScroll(wui::WindowMessage& windowMessage)
{
	wui::WM_HSCROLL_WindowMessageCrack wm{ windowMessage };


	_gwWindow->getViewport()->handleHScrollbar(wm.nSBCode());
}

void View::onVScroll(wui::WindowMessage& windowMessage)
{
	wui::WM_VSCROLL_WindowMessageCrack wm{ windowMessage };


	_gwWindow->getViewport()->handleVScrollbar(wm.nSBCode());
}

void View::onMouseWheel(wui::WindowMessage& windowMessage)
{
	wui::WM_MOUSEWHEEL_WindowMessageCrack wm{ windowMessage };

	UINT fwKeys = GET_KEYSTATE_WPARAM(windowMessage.wParam);
	bool scale  = false;


	switch (fwKeys)
	{
	case MK_LBUTTON:
		break;

	case MK_RBUTTON:
		break;

	case MK_MBUTTON:
		break;

	case MK_XBUTTON1:
		break;

	case MK_XBUTTON2:
		break;

	case MK_CONTROL:
		scale = true;
		break;

	case MK_SHIFT:
		break;

	default:
		break;
	}


	if (scale)
	{
		if (wm.zDelta() > 0)
		{
			_gwWindow->getViewport()->zoom(true);
		}
		else
		{

			_gwWindow->getViewport()->zoom(false);
		}
	}
	else
	{
		if (wm.zDelta() > 0)
		{
			_gwWindow->getViewport()->handleVScrollbar(SB_LINEUP);
		}
		else
		{
			_gwWindow->getViewport()->handleVScrollbar(SB_LINEDOWN);
		}
	}
}

void View::onMouseMove(wui::WindowMessage& windowMessage)
{
	wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };
	
	POINT pt = wm.point();


	_gwWindow->getStatusOverayPanel()->setMousePosition(pt.x, pt.y);
}

void View::onKeyDown(wui::WindowMessage& windowMessage)
{
	wui::WM_KEYDOWN_WindowMessageCrack wm{ windowMessage };


	switch (wm.nChar())
	{
	case VK_F7:
		_gwWindow->getStatusOverayPanel()->setVisible(!_gwWindow->getStatusOverayPanel()->getVisible());
		break;

	case VK_F8:
		_gwWindow->getDocumentGrid()->setVisible(!_gwWindow->getDocumentGrid()->getVisible());
		break;

	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void View::onEraseBkgnd(wui::WindowMessage& windowMessage)
{
	wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };


	wm.Result(TRUE);
}

void View::onPaint(wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	_gwWindow->render();


	//-----------------------------------------------------------------------
	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	::ValidateRect(*this, nullptr);
}

void View::onCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	if (wm.wndCtl() == nullptr)
	{
		onMenuCommand(windowMessage);
	}
	else
	{
		onCtlCommand(windowMessage);
	}
}

void View::onMenuCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void View::onCtlCommand(wui::WindowMessage& windowMessage)
{
	wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

//===========================================================================
void View::onIdle(void)
{
	_gwWindow->render();
}




