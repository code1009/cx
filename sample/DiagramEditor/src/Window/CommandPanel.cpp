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
#include "CommandPanel.hpp"
#include "View.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static std::vector<std::uint8_t> std_wstring_to_std_uint8_t_vector(std::wstring const& str)
{
	std::size_t size = (str.size()) * sizeof(wchar_t);
	std::vector<std::uint8_t> data(size);
	std::memcpy(data.data(), str.c_str(), size);
	return data;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR CommandPanel_WindowClassName = L"CommandPanel";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CommandPanel::CommandPanel(HWND parentWindowHandle, View* view) :
	_View{ view }
{
	//-----------------------------------------------------------------------
	cx::wui::WindowClass windowClass;


	windowClass.registerWindowClass(
		CommandPanel_WindowClassName
	);


	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createCommandPanel(parentWindowHandle);
	if (nullptr == hwnd)
	{
		throw std::runtime_error("CommandPanel::CommandPanel(): createCommandPanel() failed");
	}


	//-----------------------------------------------------------------------
	cx::d2d::Factory factory;
	_Canvas = std::make_unique<cx::d2d::Canvas>(&factory, hwnd);
	_Canvas->onDraw =
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


	//-----------------------------------------------------------------------
	_MouseHandler = std::make_unique<cx::wui::WindowMouseHandler>(hwnd);
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
				}
				else
				{
				}
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

			/*
			_Diagram_Edit->eventGenerator().pointerMoved(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);
			*/
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

			/*
			_Diagram_Edit->eventGenerator().pointerPressed(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);
			*/

			doDragDrop();
			
			//_MouseHandler->setMouseCapture();
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

			/*
			_Diagram_Edit->eventGenerator().pointerReleased(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);
			*/

			//_MouseHandler->releaseMouseCapture();
			return true;
		}
	;


	//-----------------------------------------------------------------------
	_ScrollHandler = std::make_unique<cx::wui::WindowScrollHandler>(hwnd);
	_ScrollHandler->scrollChangedHandler =
		[this](bool byScrollBar, std::int64_t x, std::int64_t y)
		{
			if (byScrollBar)
			{
				/*
				cx::Diagram::Point scrollOffset;
				scrollOffset.X = static_cast<float>(x);
				scrollOffset.Y = static_cast<float>(y);
				_Diagram_Edit->viewContext().setWindowScrollOffset(scrollOffset);

				invalidate();
				*/
			}
		}
	;


	//-----------------------------------------------------------------------
	_DropSourceNotifier = std::make_unique<cx::wui::dragdrop::WindowDropSourceNotifier>();


	//-----------------------------------------------------------------------
	addCommands();
}

//===========================================================================
LRESULT CommandPanel::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	auto result = cx::wui::MessageMapWindowT<CommandPanel, cx::wui::BaseWindow>::onWindowMessage(windowMessage);

	if (_MouseHandler)
	{
		bool handled;

		handled = _MouseHandler->onWindowMessage(windowMessage);
		if (handled)
		{
			return windowMessage.lResult;
		}
	}
	if (_ScrollHandler)
	{
		bool handled;

		handled = _ScrollHandler->onWindowMessage(windowMessage);
		if (handled)
		{
			return windowMessage.lResult;
		}
	}

	return result;
}

//===========================================================================
HWND CommandPanel::createCommandPanel(HWND parentWindowHandle)
{
	LPCWSTR   lpszClassName = CommandPanel_WindowClassName;
	HWND      hWndParent    = parentWindowHandle;
	LPCWSTR   lpWindowName  = L"Window";
	DWORD     dwStyle       = cx::wui::ChildWindowStyle;
	DWORD     dwExStyle     = cx::wui::ChildWindowStyleEx;


	dwStyle |= WS_VSCROLL;
	//dwStyle |= WS_HSCROLL;
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
void CommandPanel::registerWindowMessageMap(void)
{
	_WindowMessageMap.handle(WM_SIZE) = &CommandPanel::onSize;
	_WindowMessageMap.handle(WM_ERASEBKGND) = &CommandPanel::onEraseBkgnd;
	_WindowMessageMap.handle(WM_PAINT) = &CommandPanel::onPaint;
	_WindowMessageMap.handle(WM_COMMAND) = &CommandPanel::onCommand;
}

void CommandPanel::onSize(cx::wui::WindowMessage& windowMessage)
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

void CommandPanel::onEraseBkgnd(cx::wui::WindowMessage& windowMessage)
{
#if 0
	CX_RUNTIME_LOG(cxLInfo)
		<< L"onEraseBkgnd()"
		;
#endif

	cx::wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };
	wm.Result(TRUE);
}

void CommandPanel::onPaint(cx::wui::WindowMessage& windowMessage)
{
#if 0
	CX_RUNTIME_LOG(cxLInfo)
		<< L"onPaint()"
		;
#endif

	_Canvas->invalidate();

	// The ValidateRect function validates the client area within a rectangle by
	// removing the rectangle from the update region of the window.
	::ValidateRect(*this, nullptr);
}

void CommandPanel::onCommand(cx::wui::WindowMessage& windowMessage)
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

void CommandPanel::onMenuCommand(cx::wui::WindowMessage& windowMessage)
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

void CommandPanel::onCtlCommand(cx::wui::WindowMessage& windowMessage)
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
void CommandPanel::onIdle(void)
{
	if (_Canvas)
	{
		_Canvas->invalidate();
	}
}

//===========================================================================
void CommandPanel::addCommands(void)
{
	//-------------------------------------------------------------------
	using namespace cx::Diagram;
	using namespace cx::Diagram::Shape;
//	using namespace rs::Diagram;


	//-------------------------------------------------------------------
	_View->_d2dDiagram->_Diagram_Edit->factory().clear();
	_CommandInfos.clear();


	//-------------------------------------------------------------------
	addCommand_Label(L"도형");
	//-------------------------------------------------------------------
	addCommand_NewItem(std::make_shared<TextDesign         >(), makeProperties_TextDesign, L"글");
	addCommand_NewItem(std::make_shared<LineDesign         >(), makeProperties_LineDesign, L"선");
	//AddCommand_NewItem(std::make_shared<ArrowDesign        >(), makeProperties_ArrowDesign, L"화살표");
	//AddCommand_NewItem(std::make_shared<HalfArrowDesign    >(), makeProperties_HalfArrowDesign, L"반쪽화살표");
	addCommand_NewItem(std::make_shared<RectangleDesign    >(), makeProperties_RectangleDesign, L"사각형");
	addCommand_NewItem(std::make_shared<EllipseDesign      >(), makeProperties_BaseDesign, L"타원");
	addCommand_Spare();
}

void CommandPanel::addCommand_Label(std::wstring Label)
{
	CommandInfo commandInfo{ L"Label", Label, L"" };
	_CommandInfos.push_back(commandInfo);
}

void CommandPanel::addCommand_Spare()
{
	CommandInfo commandInfo{ L"Spare", L"", L"" };
	_CommandInfos.push_back(commandInfo);
}

void CommandPanel::addCommand_NewItem(std::shared_ptr<cx::Diagram::Item> const& item, cx::Diagram::ClassInfo::MakePropertiesFunction const& makeProperties, cx::Diagram::StringView const& friendlyName)
{
	if (!item)
	{
		return;
	}
	if (!makeProperties)
	{
		return;
	}


	_View->_d2dDiagram->_Diagram_Edit->factory().registerItem(item, makeProperties, friendlyName);

	CommandInfo commandInfo{ L"NewItem", friendlyName.data(), L"" };
	_CommandInfos.push_back(commandInfo);
}

void CommandPanel::loadCommands(void)
{
	for (auto item : _CommandInfos)
	{
		if (item.type == L"NewItem")
		{
			//loadCommand_NewItem(item);
		}
		else if (item.type == L"Label")
		{
			//loadCommand_Label(item);
		}
		else if (item.type == L"Spare")
		{
			//loadCommand_Spare(item);
		}
	}
}

void CommandPanel::doDragDrop(void)
{
	CommandInfo const& commandInfo = _CommandInfos[4];


	auto itemClassInfo = _View->_d2dDiagram->_Diagram_Edit->factory().findByFriendlyName(commandInfo.label);
	if (!itemClassInfo)
	{
		CX_RUNTIME_LOG(cxLError) << L"itemClassInfo is nullptr";
		return;
	}


	auto const& name = itemClassInfo->friendlyName();



	cx::wui::dragdrop::WindowDropSourceData data(
		cx::wui::dragdrop::getWindowDragDropClipboardFormat()->getClipboardFormat()
	);
	data._DataBuffer = std_wstring_to_std_uint8_t_vector(name);



	_DropSourceNotifier->doDragDrop(data);



	_View->_d2dDiagram->_Diagram_Edit->setNewItem(nullptr);
}
