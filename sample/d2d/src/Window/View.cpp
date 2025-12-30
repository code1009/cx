/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "../d2d/cx-d2d.hpp"

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
	cx::wui::WindowClass windowClass;


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
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);


	//-----------------------------------------------------------------------
	cx::d2d::Factory factory;
	_Context = std::make_unique<cx::d2d::Context>(hwnd, &factory);
	_Renderer = std::make_unique<cx::d2d::Renderer>(_Context.get());


	//-----------------------------------------------------------------------
	RECT rect;
	::GetClientRect(*this, &rect);


	//-----------------------------------------------------------------------
	UINT cx;
	UINT cy;
	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);
	_Renderer->resize(cx, cy);
	_Renderer->render();
}

//===========================================================================
HWND View::createView(HWND parentWindowHandle)
{
	LPCWSTR   lpszClassName = View_WindowClassName;
	HWND      hWndParent    = parentWindowHandle;
	LPCWSTR   lpWindowName  = L"Window";
	DWORD     dwStyle       = cx::wui::ChildWindowStyle;
	DWORD     dwExStyle     = cx::wui::ChildWindowStyleEx;


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
	_WindowMessageMap.handle(WM_SIZE) = &View::onSize;
	_WindowMessageMap.handle(WM_ERASEBKGND) = &View::onEraseBkgnd;
	_WindowMessageMap.handle(WM_PAINT) = &View::onPaint;
	_WindowMessageMap.handle(WM_COMMAND) = &View::onCommand;
}

void View::onSize(cx::wui::WindowMessage& windowMessage)
{
	//-----------------------------------------------------------------------
	RECT rect;
	::GetClientRect(*this, &rect);


	//-----------------------------------------------------------------------
	UINT cx;
	UINT cy;
	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);


	//-----------------------------------------------------------------------
	if (_Renderer)
	{
		_Renderer->resize(cx, cy);
		//_Renderer->render();
	}
}

void View::onEraseBkgnd(cx::wui::WindowMessage& windowMessage)
{
	///OutputDebugStringW(L"View::onEraseBkgnd()\r\n");
	cx::wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };
	wm.Result(TRUE);
}

void View::onPaint(cx::wui::WindowMessage& windowMessage)
{
	//OutputDebugStringW(L"View::onPaint()\r\n");
	_Renderer->render();

	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	::ValidateRect(*this, nullptr);
}

void View::onCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	if (wm.wndCtl() == nullptr)
	{
		onMenuCommand(windowMessage);
	}
	else
	{
		onCtlCommand(windowMessage);
	}
}

void View::onMenuCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


	switch (wm.nID())
	{
	case 0:
	default:
		defaultWindowProc(windowMessage);
		break;
	}
}

void View::onCtlCommand(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_COMMAND_WindowMessageCrack wm{ windowMessage };


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
	if (_Renderer)
	{
		_Renderer->render();
	}
}




