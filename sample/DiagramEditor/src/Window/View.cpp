/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include <cx/d2d/cx-d2d.hpp>
#include <cx/d2dDiagram/cx-Diagram.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "WindowHandler/WindowHandler.hpp"
#include "View.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
d2dDiagram::d2dDiagram(HWND hwnd) :
	_Hwnd(hwnd)
{
	//-----------------------------------------------------------------------
	_Diagram_Edit = std::make_unique<cx::Diagram::Edit>(cx::Diagram::DefaultViewWidth, cx::Diagram::DefaultViewHeight);

	_Diagram_Edit->viewGrid().showGridLine(true);
	_Diagram_Edit->viewGrid().showCenterLine(true);
	_Diagram_Edit->viewGrid().showOutline(true);
	_Diagram_Edit->viewGrid().showCoordinate(true);
	_Diagram_Edit->viewStatus().show(true);


	//-----------------------------------------------------------------------
	cx::d2d::Factory factory;
	_Canvas = std::make_unique<cx::d2d::Canvas>(&factory, hwnd);
	_Canvas->onDraw =
		[this](cx::d2d::DrawingSession* drawingSession)
		{
			drawingSession->getContext()->getD2dHwndRenderTarget()->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);


			drawingSession->Clear(cx::d2d::Colors::AliceBlue());

			_Diagram_Edit->draw(*drawingSession);

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
		}
	;


	//-----------------------------------------------------------------------
	_Diagram_Edit->invalidatedEventListener.attach(
		reinterpret_cast<std::uintptr_t>(this),
		[this](cx::ev::Event& /*event*/)
		{
			_Canvas->invalidate();
		}
	);


	//-----------------------------------------------------------------------
	_MouseHandler = std::make_unique<WindowMouseHandler>(hwnd);
	_MouseHandler->mouseWheelHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEWHEEL_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.pt();
			short delta = wm.zDelta();
			UINT flag = wm.nFlags();

			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			//bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			//bool lButtonPressed = (flag & MK_LBUTTON) != 0;
			//bool rButtonPressed = (flag & MK_RBUTTON) != 0;
			//bool mButtonPressed = (flag & MK_MBUTTON) != 0;
			//bool x1ButtonPressed = (flag & MK_XBUTTON1) != 0;
			//bool x2ButtonPressed = (flag & MK_XBUTTON2) != 0;

			if (controlKeyPressed)
			{
				if (delta > 0)
				{
					zoomIn(
						static_cast<float>(pt.x),
						static_cast<float>(pt.y)
					);
				}
				else
				{
					zoomOut(
						static_cast<float>(pt.x),
						static_cast<float>(pt.y)
					);
				}
			}
			return true;
		}
	;
	_MouseHandler->mouseMoveHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			_Diagram_Edit->eventGenerator().pointerMoved(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);
			return true;
		}
	;
	_MouseHandler->mouseLButtonDownHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			_Diagram_Edit->eventGenerator().pointerPressed(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);

			_MouseHandler->setMouseCapture();
			return true;
		}
	;
	_MouseHandler->mouseLButtonUpHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			_Diagram_Edit->eventGenerator().pointerReleased(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);

			_MouseHandler->releaseMouseCapture();
			return true;
		}
	;


	//-----------------------------------------------------------------------
	_ScrollHandler = std::make_unique<WindowScrollHandler>(hwnd);
	_ScrollHandler->scrollChangedHandler =
		[this](bool byScrollBar, std::int64_t x, std::int64_t y)
		{
			if (byScrollBar)
			{
				cx::Diagram::Point scrollOffset;
				scrollOffset.X = static_cast<float>(x);
				scrollOffset.Y = static_cast<float>(y);
				_Diagram_Edit->viewContext().setWindowScrollOffset(scrollOffset);

				invalidate();
			}
		}
	;
}

d2dDiagram::~d2dDiagram()
{

}

//===========================================================================
void d2dDiagram::resize(std::uint32_t cx, std::uint32_t cy)
{
	_Canvas->resize(cx, cy);


	bool rv;
	rv = _Diagram_Edit->viewContext().setWindowSize(static_cast<float>(cx), static_cast<float>(cy));
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLTrace) << L"not changed.";
	}


	std::int64_t scaledWidth  = static_cast<std::int64_t>(_Diagram_Edit->viewContext().scaledWidth());
	std::int64_t scaledHeight = static_cast<std::int64_t>(_Diagram_Edit->viewContext().scaledHeight());
	std::int64_t windowWidth  = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowWidth());
	std::int64_t windowHeight = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowHeight());
	std::int64_t xScrollPos = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowScrollOffset().X);
	std::int64_t yScrollPos = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowScrollOffset().Y);

	float maxXScrollOffset = _Diagram_Edit->viewContext().scaledWidth() - _Diagram_Edit->viewContext().windowWidth();
	float maxYScrollOffset = _Diagram_Edit->viewContext().scaledHeight() - _Diagram_Edit->viewContext().windowHeight();

	_ScrollHandler->setXYScroll(
		scaledWidth, windowWidth, xScrollPos,
		scaledHeight, windowHeight, yScrollPos
		);

	_Canvas->invalidate();
}

void d2dDiagram::invalidate(void)
{
	_Canvas->invalidate();
}

//===========================================================================
void d2dDiagram::zoomIn(float px, float py)
{
	//-------------------------------------------------------------------
	cx::Diagram::Point window0{ px, py };
	cx::Diagram::Point view0;
	view0 = _Diagram_Edit->viewContext().fromWindow(window0);
	CX_RUNTIME_LOG(cxLTrace)
		<< L"zoomIn(): "
		<< std::format(L"window0=({:.2f}, {:.2f}) view0=({:.2f}, {:.2f})",
			window0.X, window0.Y,
			view0.X, view0.Y
		);


	//-------------------------------------------------------------------
	if (!_Diagram_Edit->viewContext().zoomIn())
	{
		//return;
	}

	std::int64_t scaledWidth = static_cast<std::int64_t>(_Diagram_Edit->viewContext().scaledWidth());
	std::int64_t scaledHeight = static_cast<std::int64_t>(_Diagram_Edit->viewContext().scaledHeight());
	std::int64_t windowWidth = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowWidth());
	std::int64_t windowHeight = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowHeight());
	std::int64_t xScrollPos = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowScrollOffset().X);
	std::int64_t yScrollPos = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowScrollOffset().Y);

	float maxXScrollOffset = _Diagram_Edit->viewContext().scaledWidth() - _Diagram_Edit->viewContext().windowWidth();
	float maxYScrollOffset = _Diagram_Edit->viewContext().scaledHeight() - _Diagram_Edit->viewContext().windowHeight();

	_ScrollHandler->setXYScroll(
		scaledWidth, windowWidth, xScrollPos,
		scaledHeight, windowHeight, yScrollPos
	);
}

void d2dDiagram::zoomOut(float px, float py)
{
	//-------------------------------------------------------------------
	cx::Diagram::Point window0{ px, py };
	cx::Diagram::Point view0;
	view0 = _Diagram_Edit->viewContext().fromWindow(window0);
	CX_RUNTIME_LOG(cxLTrace)
		<< L"zoomOut(): "
		<< std::format(L"window0=({:.2f}, {:.2f}) view0=({:.2f}, {:.2f})",
			window0.X, window0.Y,
			view0.X, view0.Y
		);


	//-------------------------------------------------------------------
	if (!_Diagram_Edit->viewContext().zoomOut())
	{
		//return;
	}

	std::int64_t scaledWidth = static_cast<std::int64_t>(_Diagram_Edit->viewContext().scaledWidth());
	std::int64_t scaledHeight = static_cast<std::int64_t>(_Diagram_Edit->viewContext().scaledHeight());
	std::int64_t windowWidth = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowWidth());
	std::int64_t windowHeight = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowHeight());
	std::int64_t xScrollPos = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowScrollOffset().X);
	std::int64_t yScrollPos = static_cast<std::int64_t>(_Diagram_Edit->viewContext().windowScrollOffset().Y);

	float maxXScrollOffset = _Diagram_Edit->viewContext().scaledWidth() - _Diagram_Edit->viewContext().windowWidth();
	float maxYScrollOffset = _Diagram_Edit->viewContext().scaledHeight() - _Diagram_Edit->viewContext().windowHeight();

	_ScrollHandler->setXYScroll(
		scaledWidth, windowWidth, xScrollPos,
		scaledHeight, windowHeight, yScrollPos
	);
}






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
	_d2dDiagram = std::make_unique<d2dDiagram>(*this);
}

//===========================================================================
LRESULT View::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	auto result = cx::wui::MessageMapWindowT<View, cx::wui::BaseWindow>::onWindowMessage(windowMessage);

	if (_d2dDiagram)
	{
		bool handled; 
		
		handled = _d2dDiagram->_MouseHandler->onWindowMessage(windowMessage);
		if (handled)
		{
			return windowMessage.lResult;
		}

		handled = _d2dDiagram->_ScrollHandler->onWindowMessage(windowMessage);
		if (handled)
		{
			return windowMessage.lResult;
		}
	}

	return result;
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
	if (_d2dDiagram)
	{
		_d2dDiagram->resize(cx, cy);
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

	if (_d2dDiagram)
	{
		_d2dDiagram->invalidate();
	}

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
	if (_d2dDiagram)
	{
		_d2dDiagram->invalidate();
	}
}

//===========================================================================
void View::zoomIn(float px, float py)
{
	if (_d2dDiagram)
	{
		_d2dDiagram->zoomIn(px, py);
	}
}

void View::zoomOut(float px, float py)
{
	if (_d2dDiagram)
	{
		_d2dDiagram->zoomOut(px, py);
	}
}



