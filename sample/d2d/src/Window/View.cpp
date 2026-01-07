/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include <cx/d2d/cx-d2d.hpp>

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

	/*
	//-----------------------------------------------------------------------
	RECT rect;
	UINT cx;
	UINT cy;
	::GetClientRect(*this, &rect);
	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);

	CX_RUNTIME_LOG(cxLInfo)
		<< L"cx=" << cx
		<< L" "
		<< L"cy=" << cy
		;


	//-----------------------------------------------------------------------
	::ShowWindow(*this, SW_SHOW);
	::UpdateWindow(*this);


	//-----------------------------------------------------------------------
	::GetClientRect(*this, &rect);
	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);

	CX_RUNTIME_LOG(cxLInfo)
		<< L"cx=" << cx
		<< L" "
		<< L"cy=" << cy
		;
	*/


	//-----------------------------------------------------------------------
	cx::d2d::Factory factory;
	_Canvas = std::make_unique<cx::d2d::Canvas>(&factory, hwnd);
	_Canvas->drawingHandler =
		[this](cx::d2d::DrawingSession* drawingSession)
		{
			auto renderTarget = drawingSession->getContext()->getD2dHwndRenderTarget();

			renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Yellow));


			wil::com_ptr_nothrow<ID2D1SolidColorBrush> solidColorBrush;

			renderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				solidColorBrush.put()
			);

			renderTarget->DrawEllipse(
				D2D1::Ellipse(
					D2D1::Point2F(100.0f, 100.0f),
					50.0f,
					50.0f
				),
				solidColorBrush.get(),
				5.0f
			);

			drawingSession->FillRoundedRectangle(
				200.0f,
				200.0f,
				150.0f,
				100.0f,
				15.0f,
				15.0f,
				cx::d2d::Color{ 255, 255, 0, 255 }
			);
			drawingSession->DrawRoundedRectangle(
				200.0f,
				200.0f,
				150.0f,
				100.0f,
				15.0f,
				15.0f,
				cx::d2d::Color{ 255, 0, 0, 255 },
				3.0f
			);

			cx::d2d::TextFormat textFormat;
			textFormat.FontFamily(L"맑은 고딕");
			textFormat.FontSize(24.0f);
			textFormat.VerticalAlignment(cx::d2d::TextVAlignment::Bottom);
			drawingSession->DrawRectangle(
				400.0f,
				200.0f,
				300.0f,
				50.0f,
				cx::d2d::Color{ 255, 0, 0, 255 },
				1.0f
			);
			drawingSession->DrawText(
				L"Hello, cx::d2d::Canvas!",
				400.0f,
				200.0f,
				300.0f,
				50.0f,
				cx::d2d::Color{ 255, 0, 0, 255 },
				textFormat
			);
		}
	;
}

//===========================================================================
HWND View::createView(HWND parentWindowHandle)
{
	LPCWSTR   lpszClassName = View_WindowClassName;
	HWND      hWndParent    = parentWindowHandle;
	LPCWSTR   lpWindowName  = L"Window";
	DWORD     dwStyle       = cx::wui::ChildWindowStyle;
	DWORD     dwExStyle     = cx::wui::ChildWindowStyleEx;


	dwStyle |= WS_VSCROLL;
	dwStyle |= WS_HSCROLL;
	dwExStyle |= WS_EX_CLIENTEDGE;

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
#if 0
	CX_RUNTIME_LOG(cxLInfo)
		<< L"onSize()"
		;
#endif


	//-----------------------------------------------------------------------
	RECT rect;
	::GetClientRect(*this, &rect);


	//-----------------------------------------------------------------------
	UINT cx;
	UINT cy;
	cx = static_cast<UINT>(rect.right - rect.left);
	cy = static_cast<UINT>(rect.bottom - rect.top);


	//-----------------------------------------------------------------------
	if (_Canvas)
	{
		_Canvas->resize(cx, cy);
	}
}

void View::onEraseBkgnd(cx::wui::WindowMessage& windowMessage)
{
#if 0
	CX_RUNTIME_LOG(cxLInfo)
		<< L"onEraseBkgnd()"
		;
#endif

	cx::wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };
	wm.Result(TRUE);
}

void View::onPaint(cx::wui::WindowMessage& windowMessage)
{
#if 0
	CX_RUNTIME_LOG(cxLInfo)
		<< L"onPaint()"
		;
#endif

	_Canvas->draw();

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
	if (_Canvas)
	{
		_Canvas->draw();
	}
}




