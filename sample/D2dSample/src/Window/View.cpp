/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/gw/gw.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "View.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
MyWindow::MyWindow(HWND hwnd) :
	cx::gw::Window{ hwnd, false }
{
	//-----------------------------------------------------------------------
	getViewport()->setWindowSize(0, 0);
	getViewport()->setDocumentSize(1024, 768);
	getViewport()->enableScrollbar(false);
}

//===========================================================================
bool MyWindow::createDeviceResources(void)
{
	//-----------------------------------------------------------------------
	bool rv;

	
	rv = cx::gw::Window::createDeviceResources();
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	HRESULT hr;


	if (!_Brush)
	{
		hr = getContext()->getD2dRenderTarget()->CreateSolidColorBrush(
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

void MyWindow::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_Brush)
	{
		_Brush->Release();
		_Brush = nullptr;
	}


	//-----------------------------------------------------------------------
	cx::gw::Window::destroyDeviceResources();
}

void MyWindow::draw(void)
{
	//-----------------------------------------------------------------------
	cx::gw::Window::draw();


	//-----------------------------------------------------------------------
	D2D1_ROUNDED_RECT rrect;


	rrect.rect.left = 100.0f;
	rrect.rect.top = 100.0f;
	rrect.rect.right = 200.0f;
	rrect.rect.bottom = 200.0f;
	rrect.radiusX = 10.0f;
	rrect.radiusY = 10.0f;

	getContext()->getD2dRenderTarget()->FillRoundedRectangle(&rrect, _Brush);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR View_WindowClassName = L"View";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
View::View(HWND parentWindowHandle)
{
	//-----------------------------------------------------------------------
	cx::wui::WindowClass windowClass(View_WindowClassName);


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
	_Window = std::make_unique<MyWindow>(*this);


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
	DWORD     dwStyle       = cx::wui::ChildWindowStyle;
	DWORD     dwExStyle     = cx::wui::ChildWindowStyleEx;
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
	_WindowMessageMap[WM_CREATE    ] = &View::onCreate;
	_WindowMessageMap[WM_DESTROY   ] = &View::onDestroy;
	_WindowMessageMap[WM_CLOSE     ] = &View::onClose;
	_WindowMessageMap[WM_SIZE      ] = &View::onSize;
	_WindowMessageMap[WM_HSCROLL   ] = &View::onHScroll;
	_WindowMessageMap[WM_VSCROLL   ] = &View::onVScroll;
	_WindowMessageMap[WM_MOUSEWHEEL] = &View::onMouseWheel;
	_WindowMessageMap[WM_ERASEBKGND] = &View::onEraseBkgnd;
	_WindowMessageMap[WM_PAINT     ] = &View::onPaint;
}

void View::onCreate(cx::wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onCreate()\r\n");
}

void View::onDestroy(cx::wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onDestroy()\r\n");

	::PostQuitMessage(0);
}

void View::onClose(cx::wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onClose()\r\n");

	destroyWindow();
}

void View::onSize(cx::wui::WindowMessage& windowMessage)
{
	RECT rect;
	GetClientRect(*this, &rect);

	UINT cx = static_cast<UINT>(rect.right - rect.left);
	UINT cy = static_cast<UINT>(rect.bottom - rect.top);

	if (_Window.get())
	{	
		_Window->getViewport()->setWindowSize(cx, cy);
	}
}

void View::onHScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_HSCROLL_WindowMessageCrack wm{ windowMessage };


	_Window->getViewport()->handleHScrollbar(wm.nSBCode());
}

void View::onVScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_VSCROLL_WindowMessageCrack wm{ windowMessage };


	_Window->getViewport()->handleVScrollbar(wm.nSBCode());
}

void View::onMouseWheel(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_MOUSEWHEEL_WindowMessageCrack wm{ windowMessage };

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
			_Window->getViewport()->zoom(true);
		}
		else
		{

			_Window->getViewport()->zoom(false);
		}
	}
	else
	{
		if (wm.zDelta() > 0)
		{
			_Window->getViewport()->handleVScrollbar(SB_LINEUP);
		}
		else
		{
			_Window->getViewport()->handleVScrollbar(SB_LINEDOWN);
		}
	}
}

void View::onEraseBkgnd(cx::wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onEraseBkgnd()\r\n");

	cx::wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };


	wm.Result(TRUE);
}

void View::onPaint(cx::wui::WindowMessage& windowMessage)
{
	OutputDebugStringW(L"View::onPaint()\r\n");

	_Window->render();

	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	::ValidateRect(*this, nullptr);
}

//===========================================================================
void View::onIdle(void)
{
	_Window->render();
}




