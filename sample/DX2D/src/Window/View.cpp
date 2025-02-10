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
My_gw_Window::My_gw_Window(HWND hwnd) :
	gw::Window{ hwnd, false }
{
	//-----------------------------------------------------------------------
	getViewport()->setWindowSize(0, 0);
	getViewport()->setDocumentSize(1024, 768);
	getViewport()->enableScrollbar(false);
}

//===========================================================================
bool My_gw_Window::createDeviceResources(void)
{
	//-----------------------------------------------------------------------
	bool rv;

	
	rv = gw::Window::createDeviceResources();
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	HRESULT hr;


	if (!_Brush)
	{
		hr = getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 1.0f, 0.5f),
			&_Brush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void My_gw_Window::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_Brush)
	{
		_Brush->Release();
		_Brush = nullptr;
	}


	//-----------------------------------------------------------------------
	gw::Window::destroyDeviceResources();
}

void My_gw_Window::draw(void)
{
	//-----------------------------------------------------------------------
	gw::Window::draw();


	//-----------------------------------------------------------------------
	D2D1_ROUNDED_RECT rrect;


	rrect.rect.left = 100.0f;
	rrect.rect.top = 100.0f;
	rrect.rect.right = 200.0f;
	rrect.rect.bottom = 200.0f;
	rrect.radiusX = 10.0f;
	rrect.radiusY = 10.0f;

	getDRenderTarget()->FillRoundedRectangle(&rrect, _Brush);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR View_WindowClassName = L"View";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
View::View(HWND parentWindowHandle)
{
	//-----------------------------------------------------------------------
	wui::WindowClass windowClass(View_WindowClassName);


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
	_gwWindow = std::make_unique<My_gw_Window>(*this);


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
	int       X             = CW_USEDEFAULT;
	int       Y             = CW_USEDEFAULT;
	int       nWidth        = CW_USEDEFAULT;
	int       nHeight       = CW_USEDEFAULT;
	HMENU     hMenu         = nullptr;
	HINSTANCE hInstance     = nullptr;


	//dwStyle |= WS_VSCROLL;
	//dwStyle |= WS_HSCROLL;
	dwExStyle |= WS_EX_CLIENTEDGE;


	HWND hwnd;
	hwnd = createWindow(
		lpszClassName,
		hWndParent,
		lpWindowName,
		dwStyle,
		dwExStyle
		/*
		X,
		Y,
		nWidth,
		nHeight,
		hMenu,
		hInstance,
		this
		*/
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
	_WindowMessageMap.handle(WM_ERASEBKGND) = &View::onEraseBkgnd;
	_WindowMessageMap.handle(WM_PAINT     ) = &View::onPaint;
}

void View::onCreate(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onCreate()\r\n");
}

void View::onDestroy(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onDestroy()\r\n");

	::PostQuitMessage(0);
}

void View::onClose(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onClose()\r\n");

	destroyWindow();
}

void View::onSize(wui::WindowMessage& windowMessage)
{
	RECT rect;
	GetClientRect(*this, &rect);

	UINT cx = static_cast<UINT>(rect.right - rect.left);
	UINT cy = static_cast<UINT>(rect.bottom - rect.top);

	if (_gwWindow.get())
	{	
		_gwWindow->getViewport()->setWindowSize(cx, cy);
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
	bool scale = false;


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

void View::onEraseBkgnd(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onEraseBkgnd()\r\n");

	wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };


	wm.Result(TRUE);
}

void View::onPaint(wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onPaint()\r\n");

	_gwWindow->render();

	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	::ValidateRect(*this, nullptr);
}

//===========================================================================
void View::onIdle(void)
{
	_gwWindow->render();
}




