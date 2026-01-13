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
#include "UILayout.hpp"
#include "UIController.hpp"

#include "Catalog.hpp"
#include "Designer.hpp"

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
CommandPanel::CommandPanel(HWND parentWindowHandle, Designer* designer) :
	_Designer(designer)
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
	_UIController = std::make_unique<UIController>(hwnd);
	_UIController->_MouseHandler->mouseMoveHandler =
		[this](cx::wui::WindowMessage& windowMessage) -> bool
		{
			cx::wui::WM_MOUSEMOVE_WindowMessageCrack wm{ windowMessage };

			POINT pt = wm.point();
			UINT flag = wm.nFlags();
			bool controlKeyPressed = (flag & MK_CONTROL) != 0;
			bool shiftKeyPressed = (flag & MK_SHIFT) != 0;

			//-----------------------------------------------------------------------
			RECT rect;
			::GetClientRect(*this, &rect);
			//-----------------------------------------------------------------------
			UINT cx;
			UINT cy;
			cx = static_cast<UINT>(rect.right - rect.left);
			cy = static_cast<UINT>(rect.bottom - rect.top);
			//-----------------------------------------------------------------------
			if ((pt.x < 0) || (pt.x >= static_cast<LONG>(cx))
				||
				(pt.y < 0) || (pt.y >= static_cast<LONG>(cy))
				)
			{
				_UIController->_MouseHandler->mouseLButtonUpHandler(windowMessage);
				//_MouseHandler->releaseMouseCapture();
				doDragDrop();
				return true;
			}


			//-----------------------------------------------------------------------
			_UIController->_View->eventGenerator().pointerMoved(
				static_cast<float>(pt.x),
				static_cast<float>(pt.y),
				controlKeyPressed,
				shiftKeyPressed
			);


			//-----------------------------------------------------------------------
			return true;
		}
	;


	//-----------------------------------------------------------------------
	_DropSourceNotifier = std::make_unique<cx::wui::dragdrop::WindowDropSourceNotifier>();


	//-----------------------------------------------------------------------
	loadCommand();
}

//===========================================================================
LRESULT CommandPanel::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	if (_UIController)
	{
		bool handled;
		handled = _UIController->_MouseHandler->onWindowMessage(windowMessage);
		if (handled)
		{
			return windowMessage.lResult;
		}

		handled = _UIController->_ScrollHandler->onWindowMessage(windowMessage);
		if (handled)
		{
			return windowMessage.lResult;
		}
	}

	return cx::wui::MessageMapWindowT<CommandPanel, cx::wui::BaseWindow>::onWindowMessage(windowMessage);;
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
	_WindowMessageMap[WM_SIZE] = &CommandPanel::onSize;
	_WindowMessageMap[WM_ERASEBKGND] = &CommandPanel::onEraseBkgnd;
	_WindowMessageMap[WM_PAINT] = &CommandPanel::onPaint;
	_WindowMessageMap[WM_COMMAND] = &CommandPanel::onCommand;
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
	if (_UIController)
	{
		recalcUIControllsLayout(cx, cy);
		_UIController->resize(cx, cy);
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

	_UIController->_Canvas->draw();

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
	if (_UIController)
	{
		_UIController->_Canvas->draw();
	}
}

//===========================================================================
void CommandPanel::loadCommand(void)
{
	addCommand_Catalog(_Designer->catalog());

	setupUIControlls();
}

void CommandPanel::addCommand_Catalog(Catalog* catalog)
{
	for (auto item : catalog->_Items)
	{
		if (item.type == L"Item")
		{
			CommandInfo commandInfo{ L"NewItem", item.label, L""};
			_CommandInfos.push_back(commandInfo);
		}
		else if (item.type == L"Label")
		{
			CommandInfo commandInfo{ L"Label", item.label, L"" };
			_CommandInfos.push_back(commandInfo);
		}
		else if (item.type == L"Spare")
		{
			CommandInfo commandInfo{ L"Spare", L"", L"" };
			_CommandInfos.push_back(commandInfo);
		}
	}
}

void CommandPanel::setupUIControlls(void)
{
	//-------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	//using Control = UIControl::Text;
	using Control = Shape::Rectangle;


	//-----------------------------------------------------------------------
	auto layoutChangedHandler = [this](UILayout* layout)
		{
			using namespace cx::Widget;
			using Control = Shape::Rectangle;

			auto item = reinterpret_cast<Control*>(layout->_Item._Data);

			item->setPoint(0, Point(layout->_Item._L + 5, layout->_Item._T + 5));
			item->setPoint(1, Point(layout->_Item._R - 5, layout->_Item._B - 1));
		};

	UILayoutStyle uiLayoutStyle {
		static_cast<Coord>(0), static_cast<Coord>(35),
		UILayoutAlignment::Fill, UILayoutAlignment::Fixed
	};
	_UILayoutManager = std::make_unique<UILayoutManager>();


	//-----------------------------------------------------------------------
	for (auto info : _CommandInfos)
	{
		if (info.type == L"NewItem")
		{
			auto item = std::make_shared<Control>();
			item->text(info.label);
			item->name(info.label);
			item->shapeStyle().fill().fillColor(Colors::LightGray());
			item->shapeStyle().line().lineSize(0.0f);
			item->shapeStyle().text().textColor(Colors::Black());
			_UIController->_View->model().add(item);


			_UIController->_View->eventHandlerRegistry().registerEventHandler(
				ItemPointerPressedEvent,
				item,
				[this](cx::ev::Event& event)
				{
					using Control = Shape::Rectangle;
					auto eventType = event.eventType();
					auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
					std::shared_ptr<Control> item =
						std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);

					item->shapeStyle().fill().fillColor(Colors::LightSkyBlue());

					_NewItemName = item->name();
				}
			);

			_UIController->_View->eventHandlerRegistry().registerEventHandler(
				ItemPointerReleasedEvent,
				item,
				[this](cx::ev::Event& event)
				{
					using Control = Shape::Rectangle;
					auto eventType = event.eventType();
					auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
					std::shared_ptr<Control> item =
						std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);

					item->shapeStyle().fill().fillColor(Colors::LightGray());
				}
			);

			_UIController->_View->eventHandlerRegistry().registerEventHandler(
				ItemPointerOverEvent,
				item,
				[this](cx::ev::Event& event)
				{
					using Control = Shape::Rectangle;
					auto eventType = event.eventType();
					auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
					std::shared_ptr<Control> item =
						std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);

					item->shapeStyle().fill().fillColor(Colors::LightSkyBlue());
				}
			);
			
			_UIController->_View->eventHandlerRegistry().registerEventHandler(
				ItemPointerLeaveEvent,
				item,
				[this](cx::ev::Event& event)
				{
					using Control = Shape::Rectangle;
					auto eventType = event.eventType();
					auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
					std::shared_ptr<Control> item =
						std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);

					item->shapeStyle().fill().fillColor(Colors::LightGray());
				}
			);

			_UILayoutManager->add(
				true,
				uiLayoutStyle,
				layoutChangedHandler,
				reinterpret_cast<void*>(item.get())
			);
		}
		else if (info.type == L"Label")
		{
			auto item = std::make_shared<Control>();
			item->text(info.label);
			item->shapeStyle().fill().fillColor(Colors::DarkBlue());
			item->shapeStyle().line().lineSize(0.0f);
			item->shapeStyle().text().textColor(Colors::White());
			_UIController->_View->model().add(item);


			_UIController->_View->eventHandlerRegistry().registerEventHandler(
				ItemPointerPressedEvent,
				item,
				[this](cx::ev::Event& event)
				{
					using Control = Shape::Rectangle;
					auto eventType = event.eventType();
					auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
					std::shared_ptr<Control> item =
						std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);

					item->text(L"Pressed");
				}
			);

			_UIController->_View->eventHandlerRegistry().registerEventHandler(
				ItemPointerReleasedEvent,
				item,
				[this](cx::ev::Event& event)
				{
					using Control = Shape::Rectangle;
					auto eventType = event.eventType();
					auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
					std::shared_ptr<Control> item =
						std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);

					item->text(L"Release");
				}
				/*
				// onItemPointerPressed(cx::ev::Event& event)
				std::bind(
					&CommandPanel::onItemPointerPressed,
					this,
					std::placeholders::_1
				)
				*/
			);

			_UILayoutManager->add(
				true,
				uiLayoutStyle,
				layoutChangedHandler,
				reinterpret_cast<void*>(item.get())
			);
		}
		else if (info.type == L"Spare")
		{
			_UILayoutManager->add(
				true,
				uiLayoutStyle
			);
		}
	}
}

void CommandPanel::recalcUIControllsLayout(std::uint32_t cx, std::uint32_t cy)
{
	using namespace cx::Widget;


	if (_UILayoutManager)
	{
		_UILayoutManager->recalcLayout(static_cast<Coord>(cx), static_cast<Coord>(cy));

		auto viewCx = static_cast<Coord>(_UILayoutManager->getCX());
		auto viewCy = static_cast<Coord>(_UILayoutManager->getCY());
		_UIController->_View->viewContext().setSize(viewCx, viewCy);
	}
}

//===========================================================================
void CommandPanel::doDragDrop(void)
{
	//-----------------------------------------------------------------------
	if (_NewItemName.empty())
	{
		return;
	}


	//-----------------------------------------------------------------------
	std::wstring name = _NewItemName;
	cx::wui::dragdrop::WindowDropSourceData data(
		cx::wui::dragdrop::getWindowDragDropClipboardFormat()->getClipboardFormat()
	);
	data._DataBuffer = std_wstring_to_std_uint8_t_vector(name);
	_DropSourceNotifier->doDragDrop(data);


	//-----------------------------------------------------------------------
	_NewItemName.clear();


	_Designer->_Edit->setNewItem(nullptr);
}
