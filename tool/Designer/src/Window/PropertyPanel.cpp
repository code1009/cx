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
	_Designer->_ShowItemPropertytHandler = 
		[this]()
		{
			showItemProperty();
		}
	;

	showItemProperty();
}

PropertyPanel::~PropertyPanel()
{
	//-----------------------------------------------------------------------
	_Designer->_ShowItemPropertytHandler = nullptr;
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
#if 0

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


	//-----------------------------------------------------------------------
	for (auto item : _UIController->_View->model().items())
	{
		item->registerEventHandler(_UIController->_View->eventHandlerRegistry());
	}
}

#endif

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

void PropertyPanel::showItemProperty(void)
{
	_ItemProperty_valueChanged_Flag = false;
	setupUIControlls();
	_ItemProperty_valueChanged_Flag = true;
}

void PropertyPanel::setupUIControlls(void)
{
	//-----------------------------------------------------------------------
	_UILayoutManager.reset();
	_UIController->_View->model().clear();


	//-----------------------------------------------------------------------
	_UILayoutManager = std::make_unique<UILayoutManager>();


	//-----------------------------------------------------------------------
	if (_Designer->_PropertiesManipulator->getProperties())
	{
		loadItemPropertyUI();
	}
	else
	{
		LoadEmptyPropertyUI();
	}


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
	}


	//-----------------------------------------------------------------------
	for (auto item : _UIController->_View->model().items())
	{
		item->registerEventHandler(_UIController->_View->eventHandlerRegistry());
	}
}

void PropertyPanel::LoadEmptyPropertyUI(void)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLTrace)
		<< L"LoadEmptyPropertyUI"
		;


	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto layoutChangedHandler = [this](UILayout* layout)
		{
			using namespace cx::Widget;
			using Control = UIControl::Button;

			auto item = reinterpret_cast<Control*>(layout->_Item._Data);

			item->setPoint(0, Point(layout->_Item._L + 5, layout->_Item._T + 5));
			item->setPoint(1, Point(layout->_Item._R - 10, layout->_Item._B - 1));
		};

	UILayoutStyle uiLayoutStyle0{
		static_cast<Coord>(100), static_cast<Coord>(35),
		UILayoutAlignment::Fill, UILayoutAlignment::Fixed
	};
	UILayoutStyle uiLayoutStyle1{
		static_cast<Coord>(100), static_cast<Coord>(35),
		UILayoutAlignment::Fixed, UILayoutAlignment::Fixed
	};
	UILayoutStyle uiLayoutStyle2{
		static_cast<Coord>(100), static_cast<Coord>(35),
		UILayoutAlignment::Fill, UILayoutAlignment::Fixed
	};


	//-----------------------------------------------------------------------
	using Control = UIControl::Label;
	auto item = std::make_shared<Control>();
	item->text(L"대상을 선택 하세요.");
	_UIController->_View->model().add(item);


	_UILayoutManager->add(
		true,
		uiLayoutStyle0,
		layoutChangedHandler,
		reinterpret_cast<void*>(item.get())
	);
}

void PropertyPanel::loadItemPropertyUI(void)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLTrace)
		<< L"loadItemPropertyUI"
		;


	//-----------------------------------------------------------------------
	std::shared_ptr<cx::Widget::Properties> properties =
		_Designer->_PropertiesManipulator->getProperties();


	//-----------------------------------------------------------------------
	std::int32_t index = 0;
	for (auto const& property : properties->items())
	{
		if (!property->visible())
		{
			continue;
		}

		if      (property->type() == cx::Widget::PropertyTypes::UInt8    ) { addItemPropertyUI_UInt8    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt16   ) { addItemPropertyUI_UInt16   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt32   ) { addItemPropertyUI_UInt32   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt64   ) { addItemPropertyUI_UInt64   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int8     ) { addItemPropertyUI_Int8     (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int16    ) { addItemPropertyUI_Int16    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int32    ) { addItemPropertyUI_Int32    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int64    ) { addItemPropertyUI_Int64    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Float    ) { addItemPropertyUI_Float    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Double   ) { addItemPropertyUI_Double   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Bool     ) { addItemPropertyUI_Bool     (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::String   ) { addItemPropertyUI_String   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Points   ) { addItemPropertyUI_Points   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::FillStyle) { addItemPropertyUI_FillStyle(index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::LineStyle) { addItemPropertyUI_LineStyle(index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::TextStyle) { addItemPropertyUI_TextStyle(index, property); }
	}
}

void PropertyPanel::addItemPropertyUI_UInt8(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_UInt16(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_UInt32(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_UInt64(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_Int8(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_Int16(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_Int32(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_Int64(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_Float(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_Double(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_Bool(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_String(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_Points(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_FillStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_LineStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::addItemPropertyUI_TextStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}
