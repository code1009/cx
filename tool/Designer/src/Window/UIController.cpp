/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include <cx/d2d/cx-d2d.hpp>
#include <cx/d2dw/cx-Widget.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "../WindowHandler/WindowHandler.hpp"

//===========================================================================
#include "Catalog.hpp"
#include "UIController.hpp"
#include "View.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static std::wstring std_uint8_t_vector_to_std_wstring(std::vector<std::uint8_t> const& dataBuffer)
{
	std::wstring result;

	if (dataBuffer.size() < 2 || dataBuffer.size() % 2 != 0)
	{
		return result;
	}

	result.resize(dataBuffer.size() / 2);

	for (std::size_t i = 0; i < result.size(); ++i)
	{
		std::size_t byteIndex = i * 2;

		wchar_t ch = 
			static_cast<wchar_t>(dataBuffer[byteIndex]) |
			(static_cast<wchar_t>(dataBuffer[byteIndex + 1]) << 8);

		result[i] = ch;
	}

	return result;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
UIController::UIController(HWND hwnd) :
	_Hwnd(hwnd)
{
	//-----------------------------------------------------------------------
	setupCanvasView();
	setupMouseHandlerHandler();
	setupScrollHandler();


	//-----------------------------------------------------------------------
	loadUIControls();
}

UIController::~UIController()
{
}

//===========================================================================
void UIController::setupCanvasView(void)
{
	//-----------------------------------------------------------------------
	_View = std::make_unique<cx::Widget::View>(cx::Widget::DefaultViewWidth, cx::Widget::DefaultViewHeight, false);


	//-----------------------------------------------------------------------
	cx::d2d::Factory factory;
	_Canvas = std::make_unique<cx::d2d::Canvas>(&factory, _Hwnd);
	_Canvas->drawingHandler =
		[this](cx::d2d::DrawingSession* drawingSession)
		{
			drawingSession->Clear(cx::d2d::Colors::LightGray());
			_View->draw(*drawingSession);
		}
	;


	//-----------------------------------------------------------------------
	_View->invalidatedEventListener.attach(
		reinterpret_cast<std::uintptr_t>(this),
		[this](cx::ev::Event& /*event*/)
		{
			invalidate();
		}
	);
}

void UIController::setupMouseHandlerHandler(void)
{
	//-------------------------------------------------------------------
	_MouseHandler = std::make_unique<cx::wui::WindowMouseHandler>(_Hwnd);


	//-------------------------------------------------------------------
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
#if 0
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
#endif
			}
			else
			{
				if (delta > 0)
				{
					_ScrollHandler->YScroll_LineUp();
				}
				else
				{
					_ScrollHandler->YScroll_LineDown();
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

			_View->eventGenerator().pointerMoved(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);

			if (_View->eventGenerator().isPointerCaptured())
			{
				invalidate();
			}

			return true;
		}
	;
	_MouseHandler->mouseLButtonDownHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_LBUTTONDOWN_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			_View->eventGenerator().pointerPressed(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);

			_MouseHandler->setMouseCapture();
			invalidate();
			return true;
		}
	;
	_MouseHandler->mouseLButtonUpHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_LBUTTONUP_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			_View->eventGenerator().pointerReleased(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);

			_MouseHandler->releaseMouseCapture();
			invalidate();
			return true;
		}
	;
}

void UIController::setupScrollHandler(void)
{
	//-------------------------------------------------------------------
	_ScrollHandler = std::make_unique<cx::wui::WindowScrollHandler>(_Hwnd, false, true);


	//-------------------------------------------------------------------
	_ScrollHandler->scrollChangedHandler =
		[this](bool byScrollBar, std::int64_t x, std::int64_t y)
		{
			if (byScrollBar)
			{
				cx::Widget::Point scrollOffset;
				scrollOffset.X = static_cast<float>(x);
				scrollOffset.Y = static_cast<float>(y);
				_View->viewContext().setWindowScrollOffset(scrollOffset);

				invalidate();
			}
		}
	;
}

void UIController::loadUIControls(void)
{
	//-------------------------------------------------------------------
	using namespace cx::Widget;
	using namespace cx::Widget::UIControl;
	//	using namespace rs::Diagram;


	//-------------------------------------------------------------------
	_View->factory().clear();


	//-------------------------------------------------------------------
	_View->factory().registerItem(std::make_shared<Text>(), makeProperties_Text, L"글");
}

//===========================================================================
void UIController::resize(std::uint32_t cx, std::uint32_t cy)
{
	_Canvas->resize(cx, cy);


	bool rv;
	rv = _View->viewContext().setWindowSize(static_cast<float>(cx), static_cast<float>(cy));
	if (!rv)
	{
		CX_RUNTIME_LOG(cxLTrace) << L"no changed.";
		return;
	}
//	_View->viewContext().setSize(static_cast<float>(cx), static_cast<float>(cy));

	updateScrollBar();
	invalidate();
}

//===========================================================================
void UIController::invalidate(void)
{
	InvalidateRect(_Hwnd, nullptr, FALSE);
}

void UIController::updateScrollBar(void)
{
	//-------------------------------------------------------------------
	std::int64_t windowWidth = static_cast<std::int64_t>(_View->viewContext().windowWidth());
	std::int64_t windowHeight = static_cast<std::int64_t>(_View->viewContext().windowHeight());

	std::int64_t scaledWidth = static_cast<std::int64_t>(_View->viewContext().scaledWidth());
	std::int64_t scaledHeight = static_cast<std::int64_t>(_View->viewContext().scaledHeight());

	std::int64_t xScrollPos = static_cast<std::int64_t>(_View->viewContext().windowScrollOffset().X);
	std::int64_t yScrollPos = static_cast<std::int64_t>(_View->viewContext().windowScrollOffset().Y);

	//std::int64_t maxXScrollOffset = scaledWidth - windowWidth;
	//std::int64_t maxYScrollOffset = scaledHeight - windowHeight;

	_ScrollHandler->setXYScroll(
		scaledWidth, windowWidth, xScrollPos,
		scaledHeight, windowHeight, yScrollPos
	);
}

//===========================================================================
void UIController::zoomIn(float px, float py)
{
	//-------------------------------------------------------------------
#if 0
	cx::Widget::Point window0{ px, py };
	cx::Widget::Point view0;
	view0 = _View->viewContext().fromWindow(window0);
	CX_RUNTIME_LOG(cxLTrace)
		<< L"zoomIn(): "
		<< std::format(L"window0=({:.2f}, {:.2f}) view0=({:.2f}, {:.2f})",
			window0.X, window0.Y,
			view0.X, view0.Y
		);
#endif


	//-------------------------------------------------------------------
	if (!_View->viewContext().zoomIn())
	{
		CX_RUNTIME_LOG(cxLTrace) << L"no changed.";
		return;
	}
	updateScrollBar();
	invalidate();
}

void UIController::zoomOut(float px, float py)
{
	//-------------------------------------------------------------------
#if 0
	cx::Widget::Point window0{ px, py };
	cx::Widget::Point view0;
	view0 = _View->viewContext().fromWindow(window0);
	CX_RUNTIME_LOG(cxLTrace)
		<< L"zoomOut(): "
		<< std::format(L"window0=({:.2f}, {:.2f}) view0=({:.2f}, {:.2f})",
			window0.X, window0.Y,
			view0.X, view0.Y
		);
#endif


	//-------------------------------------------------------------------
	if (!_View->viewContext().zoomOut())
	{
		CX_RUNTIME_LOG(cxLTrace) << L"no changed.";
		return;
	}
	updateScrollBar();
	invalidate();
}
