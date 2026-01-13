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

#include "PropertyPanel.hpp"
#include "View.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr LPCWSTR PropertyPanel_WindowClassName = L"PropertyPanel";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
PropertyPanel::PropertyPanel(HWND parentWindowHandle, Designer* designer) :
	_Designer(designer)
{
	//-----------------------------------------------------------------------
	cx::wui::WindowClass windowClass;
	windowClass.registerWindowClass(
		PropertyPanel_WindowClassName
	);
	//-----------------------------------------------------------------------
	registerWindowMessageMap();


	//-----------------------------------------------------------------------
	HWND hwnd;
	hwnd = createPropertyPanel(parentWindowHandle);
	if (nullptr == hwnd)
	{
		throw std::runtime_error("PropertyPanel::PropertyPanel(): createPropertyPanel() failed");
	}


	//-----------------------------------------------------------------------
	_UIController = std::make_unique<UIController>(hwnd);


	//-----------------------------------------------------------------------
	loadProperty();
}

//===========================================================================
LRESULT PropertyPanel::onWindowMessage(cx::wui::WindowMessage& windowMessage)
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

	return cx::wui::MessageMapWindowT<PropertyPanel, cx::wui::BaseWindow>::onWindowMessage(windowMessage);;
}

//===========================================================================
HWND PropertyPanel::createPropertyPanel(HWND parentWindowHandle)
{
	LPCWSTR   lpszClassName = PropertyPanel_WindowClassName;
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
void PropertyPanel::registerWindowMessageMap(void)
{
	_WindowMessageMap[WM_SIZE] = &PropertyPanel::onSize;
	_WindowMessageMap[WM_ERASEBKGND] = &PropertyPanel::onEraseBkgnd;
	_WindowMessageMap[WM_PAINT] = &PropertyPanel::onPaint;
	_WindowMessageMap[WM_COMMAND] = &PropertyPanel::onCommand;
}

void PropertyPanel::onSize(cx::wui::WindowMessage& windowMessage)
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

void PropertyPanel::onEraseBkgnd(cx::wui::WindowMessage& windowMessage)
{
#if 0
	CX_RUNTIME_LOG(cxLInfo)
		<< L"onEraseBkgnd()"
		;
#endif

	cx::wui::WM_ERASEBKGND_WindowMessageCrack wm{ windowMessage };
	wm.Result(TRUE);
}

void PropertyPanel::onPaint(cx::wui::WindowMessage& windowMessage)
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

void PropertyPanel::onCommand(cx::wui::WindowMessage& windowMessage)
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

void PropertyPanel::onMenuCommand(cx::wui::WindowMessage& windowMessage)
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

void PropertyPanel::onCtlCommand(cx::wui::WindowMessage& windowMessage)
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
void PropertyPanel::onIdle(void)
{
	if (_UIController)
	{
		_UIController->_Canvas->draw();
	}
}

//===========================================================================
void PropertyPanel::loadProperty(void)
{
	addProperty_Catalog(_Designer->catalog());

	setupUIControlls();
}

void PropertyPanel::addProperty_Catalog(Catalog* catalog)
{
	for (auto item : catalog->_Items)
	{
		if (item.type == L"Item")
		{
			PropertyInfo commandInfo{ L"NewItem", item.label, L""};
			_PropertyInfos.push_back(commandInfo);
		}
		else if (item.type == L"Label")
		{
			PropertyInfo commandInfo{ L"Label", item.label, L"" };
			_PropertyInfos.push_back(commandInfo);
		}
		else if (item.type == L"Spare")
		{
			PropertyInfo commandInfo{ L"Spare", L"", L"" };
			_PropertyInfos.push_back(commandInfo);
		}
	}
}

void PropertyPanel::setupUIControlls(void)
{
	//-------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	using Control = UIControl::Button;


	//-----------------------------------------------------------------------
	auto layoutChangedHandler = [this](UILayout* layout)
		{
			using namespace cx::Widget;
			using Control = UIControl::Button;

			auto item = reinterpret_cast<Control*>(layout->_Item._Data);

			item->setPoint(0, Point(layout->_Item._L + 5, layout->_Item._T + 5));
			item->setPoint(1, Point(layout->_Item._R - 5, layout->_Item._B - 1));
		};

	UILayoutStyle uiLayoutStyle {
		static_cast<Coord>(100), static_cast<Coord>(35),
		UILayoutAlignment::Fill, UILayoutAlignment::Fixed
	};
	_UILayoutManager = std::make_unique<UILayoutManager>();


	//-----------------------------------------------------------------------
	for (auto info : _PropertyInfos)
	{
		if (info.type == L"NewItem")
		{
			auto item = std::make_shared<Control>();
			item->text(info.label);
			item->name(info.label);
			_UIController->_View->model().add(item);
			item->registerEventHandler(_UIController->_View->eventHandlerRegistry());


			_UIController->_View->eventHandlerRegistry().registerEventHandler(
				ItemPointerPressedEvent,
				item,
				[this](cx::ev::Event& event)
				{
					using Control = UIControl::Button;
					auto eventType = event.eventType();
					auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
					std::shared_ptr<Control> item =
						std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);
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
			_UIController->_View->model().add(item);
			item->registerEventHandler(_UIController->_View->eventHandlerRegistry());


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

void PropertyPanel::recalcUIControllsLayout(std::uint32_t cx, std::uint32_t cy)
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
