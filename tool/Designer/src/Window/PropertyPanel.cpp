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
	setupUILayoutStyles();


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

void PropertyPanel::setupUILayoutStyles(void)
{
	//-----------------------------------------------------------------------
	_UILayoutStyle0 = std::make_unique<UILayoutStyle>(
		static_cast<float>(100), static_cast<float>(25),
		UILayoutAlignment::Fill, UILayoutAlignment::Fixed
	);
	_UILayoutChangedHandler0 = [this](UILayout* layout)
		{
			using namespace cx::Widget;
			using Control = UIControl::Button;

			auto item = reinterpret_cast<Control*>(layout->_Item._Data);

			item->setPoint(0, Point(layout->_Item._L, layout->_Item._T));
			item->setPoint(1, Point(layout->_Item._R, layout->_Item._B));
		};


	//-----------------------------------------------------------------------
	_UILayoutStyle1 = std::make_unique<UILayoutStyle>(
		static_cast<float>(100), static_cast<float>(25),
		UILayoutAlignment::Fixed, UILayoutAlignment::Fixed
	);

	_UILayoutStyle2 = std::make_unique<UILayoutStyle>(
		static_cast<float>(0), static_cast<float>(25),
		UILayoutAlignment::Fill, UILayoutAlignment::Fixed
	);

	_UILayoutStyle3 = std::make_unique<UILayoutStyle>(
		static_cast<float>(0), static_cast<float>(25),
		UILayoutAlignment::Fill, UILayoutAlignment::Fixed
	);

	//-----------------------------------------------------------------------
	_UILayoutChangedHandler1 = [this](UILayout* layout)
		{
			using namespace cx::Widget;
			using Control = UIControl::Label;

			auto item = reinterpret_cast<Control*>(layout->_Item._Data);

			item->setPoint(0, Point(layout->_Item._L, layout->_Item._T));
			item->setPoint(1, Point(layout->_Item._R, layout->_Item._B));
		};

	_UILayoutChangedHandler2 = [this](UILayout* layout)
		{
			using namespace cx::Widget;
			using Control = UIControl::Button;

			auto item = reinterpret_cast<Control*>(layout->_Item._Data);

			item->setPoint(0, Point(layout->_Item._L, layout->_Item._T));
			item->setPoint(1, Point(layout->_Item._R, layout->_Item._B));
		};

	_UILayoutChangedHandler3 = [this](UILayout* layout)
		{
			using namespace cx::Widget;
			using Control = UIControl::Button;

			auto item = reinterpret_cast<Control*>(layout->_Item._Data);

			item->setPoint(0, Point(layout->_Item._L, layout->_Item._T));
			item->setPoint(1, Point(layout->_Item._R, layout->_Item._B));
		};
}

void PropertyPanel::setupUIControlls(void)
{
	//-----------------------------------------------------------------------
	_UILayoutManager.reset();
	_UIController->_View->model().clear();

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
	auto item = std::make_shared<UIControl::Label>();
	item->text(L"대상을 선택 하세요.");
	_UIController->_View->model().add(item);


	_UILayoutManager->add(
		true,
		*_UILayoutStyle0.get(),
		_UILayoutChangedHandler0,
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

		if      (property->type() == cx::Widget::PropertyTypes::UInt8    ) { loadItemPropertyUI_UInt8    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt16   ) { loadItemPropertyUI_UInt16   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt32   ) { loadItemPropertyUI_UInt32   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt64   ) { loadItemPropertyUI_UInt64   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int8     ) { loadItemPropertyUI_Int8     (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int16    ) { loadItemPropertyUI_Int16    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int32    ) { loadItemPropertyUI_Int32    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int64    ) { loadItemPropertyUI_Int64    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Float    ) { loadItemPropertyUI_Float    (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Double   ) { loadItemPropertyUI_Double   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Bool     ) { loadItemPropertyUI_Bool     (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::String   ) { loadItemPropertyUI_String   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::Points   ) { loadItemPropertyUI_Points   (index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::FillStyle) { loadItemPropertyUI_FillStyle(index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::LineStyle) { loadItemPropertyUI_LineStyle(index, property); }
		else if (property->type() == cx::Widget::PropertyTypes::TextStyle) { loadItemPropertyUI_TextStyle(index, property); }
	}
}

void PropertyPanel::addUIControl_Label(std::wstring const& name)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto nameItem = std::make_shared<UIControl::Label>();
	nameItem->text(name);
	_UIController->_View->model().add(nameItem);


	_UILayoutManager->add(
		true,
		*_UILayoutStyle1.get(),
		_UILayoutChangedHandler1,
		reinterpret_cast<void*>(nameItem.get())
	);
}

void PropertyPanel::addUIControl_Text(std::wstring const& value)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto valueItem = std::make_shared<UIControl::Button>();
	valueItem->text(value);
	_UIController->_View->model().add(valueItem);


	_UILayoutManager->add(
		false,
		*_UILayoutStyle2.get(),
		_UILayoutChangedHandler2,
		reinterpret_cast<void*>(valueItem.get())
	);
}

void PropertyPanel::loadItemPropertyUI_UInt8(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_UInt16(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_UInt32(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_UInt64(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_Int8(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_Int16(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_Int32(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_Int64(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_Float(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_Double(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_Bool(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_String(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	//auto value = cx::to_std_uint8_t(property->value());
	auto value = property->value();


	//-----------------------------------------------------------------------
	addUIControl_Label(name);


	//-----------------------------------------------------------------------
	addUIControl_Text(value);


	//-------------------------------------------------------------------
	if (property->readOnly())
	{
		//input.IsEnabled(false);
	}


	//-------------------------------------------------------------------
	index++;
}

void PropertyPanel::loadItemPropertyUI_Points(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::loadItemPropertyUI_FillStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::loadItemPropertyUI_LineStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}

void PropertyPanel::loadItemPropertyUI_TextStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property)
{
}
