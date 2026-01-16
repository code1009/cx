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

#include "InputValueBox.hpp"





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
		static_cast<float>(0), static_cast<float>(25),
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
			using Control = UIControl::Label;

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
	for (auto const& property : properties->items())
	{
		if (!property->visible())
		{
			continue;
		}

		if      (property->type() == cx::Widget::PropertyTypes::UInt8    ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt16   ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt32   ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::UInt64   ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int8     ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int16    ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int32    ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::Int64    ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::Float    ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::Double   ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::Bool     ) { addItemPropertyBoolUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::String   ) { addItemPropertyTextUI     (property); }
		else if (property->type() == cx::Widget::PropertyTypes::Points   ) { addItemPropertyPointsUI   (property); }
		else if (property->type() == cx::Widget::PropertyTypes::FillStyle) { addItemPropertyFillStyleUI(property); }
		else if (property->type() == cx::Widget::PropertyTypes::LineStyle) { addItemPropertyLineStyleUI(property); }
		else if (property->type() == cx::Widget::PropertyTypes::TextStyle) { addItemPropertyTextStyleUI(property); }
	}
}

bool PropertyPanel::isPropertyEditable(std::shared_ptr<cx::Widget::Property> property) const
{
	return _ItemProperty_valueChanged_Flag;
}

std::wstring PropertyPanel::getTextHAlignmentString(cx::Widget::TextHAlignment textHAlignment)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	std::wstring textHAlignmentString;
	switch (textHAlignment)
	{
	case TextHAlignment::Left: textHAlignmentString = cx::Widget::PropertyFriendlyNames::textHAlignment_Left.data(); break;
	case TextHAlignment::Center: textHAlignmentString = cx::Widget::PropertyFriendlyNames::textHAlignment_Center.data(); break;
	case TextHAlignment::Right: textHAlignmentString = cx::Widget::PropertyFriendlyNames::textHAlignment_Right.data(); break;
	default:
		textHAlignmentString = cx::Widget::PropertyFriendlyNames::textHAlignment_Left.data(); break;
		break;
	}

	return textHAlignmentString;
}

std::wstring PropertyPanel::getTextVAlignmentString(cx::Widget::TextVAlignment textVAlignment)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	std::wstring textVAlignmentString;
	switch (textVAlignment)
	{
	case TextVAlignment::Top: textVAlignmentString = cx::Widget::PropertyFriendlyNames::textVAlignment_Top.data(); break;
	case TextVAlignment::Center: textVAlignmentString = cx::Widget::PropertyFriendlyNames::textVAlignment_Center.data(); break;
	case TextVAlignment::Bottom: textVAlignmentString = cx::Widget::PropertyFriendlyNames::textVAlignment_Bottom.data(); break;
	default:
		textVAlignmentString = cx::Widget::PropertyFriendlyNames::textVAlignment_Top.data(); break;
		break;
	}

	return textVAlignmentString;
}

void PropertyPanel::calcPropertyValueBoxRect(
	std::shared_ptr<cx::Widget::UIControl::Base> control,
	std::uint32_t& x, std::uint32_t& y,
	std::uint32_t& cx, std::uint32_t& cy
)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	auto lt = control->getPoint(0);
	auto rb = control->getPoint(1);
	
	Point wlt = _UIController->_View->viewContext().toWindow(lt);
	Point wrb = _UIController->_View->viewContext().toWindow(rb);


	//-----------------------------------------------------------------------
	POINT points[2];
	points[0].x = static_cast<LONG>(wlt.X);
	points[0].y = static_cast<LONG>(wlt.Y);
	points[1].x = static_cast<LONG>(wrb.X);
	points[1].y = static_cast<LONG>(wrb.Y);
	ClientToScreen(*this, &points[0]);
	ClientToScreen(*this, &points[1]);

	x = points[0].x;
	y = points[0].y;
	cx = points[1].x - points[0].x;
	cy = points[1].y - points[0].y;
}

void PropertyPanel::calcPropertyValueDropBoxRect(
	std::shared_ptr<cx::Widget::UIControl::Base> control,
	std::uint32_t& x, std::uint32_t& y,
	std::uint32_t& cx, std::uint32_t& cy
)
{
	std::uint32_t X;
	std::uint32_t Y;
	std::uint32_t CX;
	std::uint32_t CY;
	calcPropertyValueBoxRect(control, X, Y, CX, CY);

	x = X + 1;
	y = Y + CY + 1;
	if (CX > 2)
	{
		cx = CX - 2;
	}
}

void PropertyPanel::addPropertyNameUIControl(std::wstring const& name)
{
	//-----------------------------------------------------------------------
	/*
	LightGray = 0xD3D3D3,
	DarkGray = 0xA9A9A9,
	Gray = 0x808080,
	DimGray = 0x696969,
	*/


	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto nameItem = std::make_shared<UIControl::Label>();
	nameItem->text(name);
	_UIController->_View->model().add(nameItem);

	nameItem->uiControlStyle().text().textHAlignment(TextHAlignment::Left);
	nameItem->uiControlStyle().text().fontBold(true);
	nameItem->uiControlStyle().fill().fillColor(0xb8b8b8);

	_UILayoutManager->add(
		true,
		*_UILayoutStyle1.get(),
		_UILayoutChangedHandler1,
		reinterpret_cast<void*>(nameItem.get())
	);
}

void PropertyPanel::addPropertyGroupNameUIControl(std::wstring const& name)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto nameItem = std::make_shared<UIControl::Label>();
	nameItem->text(name);
	_UIController->_View->model().add(nameItem);

	nameItem->uiControlStyle().text().textHAlignment(TextHAlignment::Left);
	nameItem->uiControlStyle().text().fontBold(true);
	nameItem->uiControlStyle().fill().fillColor(0xb8b8b8);

	_UILayoutManager->add(
		true,
		*_UILayoutStyle3.get(),
		_UILayoutChangedHandler3,
		reinterpret_cast<void*>(nameItem.get())
	);
}

void PropertyPanel::addPropertySubNameUIControl(std::wstring const& name)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto nameItem = std::make_shared<UIControl::Label>();
	nameItem->text(name);
	_UIController->_View->model().add(nameItem);

	nameItem->uiControlStyle().text().fontBold(false);
	nameItem->uiControlStyle().text().textHAlignment(TextHAlignment::Right);
	nameItem->uiControlStyle().fill().fillColor(0xc8c8c8);

	_UILayoutManager->add(
		true,
		*_UILayoutStyle1.get(),
		_UILayoutChangedHandler1,
		reinterpret_cast<void*>(nameItem.get())
	);
}

std::shared_ptr<cx::Widget::UIControl::Button> PropertyPanel::addPropertyTextUIControl(std::wstring const& value, bool readOnly)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto valueItem = std::make_shared<UIControl::Button>();
	valueItem->text(value);
	_UIController->_View->model().add(valueItem);

	if (readOnly)
	{
		//valueItem->uiControlStyle().text().fontItalic(true);
		valueItem->uiControlStyle().fill().fillColor(Colors::LightGray());
	}
	else
	{
		//valueItem->uiControlStyle().text().fontItalic(false);
		valueItem->uiControlStyle().fill().fillColor(Colors::White());
	}

	_UILayoutManager->add(
		false,
		*_UILayoutStyle2.get(),
		_UILayoutChangedHandler2,
		reinterpret_cast<void*>(valueItem.get())
	);

	return valueItem;
}

void PropertyPanel::setPropertyColorValueUIControl(std::shared_ptr<cx::Widget::UIControl::Button> control, cx::Widget::Color color)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto r = getColorR8(color);
	auto g = getColorG8(color);
	auto b = getColorB8(color);


	// RGB를 HSV로 변환하여 채도와 명도 계산
	float rf = r / 255.0f;
	float gf = g / 255.0f;
	float bf = b / 255.0f;

	float max_val = std::max({ rf, gf, bf });
	float min_val = std::min({ rf, gf, bf });
	float delta = max_val - min_val;

	// 채도(Saturation) 계산
	float saturation = (max_val == 0.0f) ? 0.0f : (delta / max_val);
	// 명도(Value/Brightness) 계산
	float brightness = max_val;

	// 텍스트 색상 결정 로직
	bool useWhiteText = false;

	if (brightness < 0.5f)
	{
		// 어두운 색상 -> 흰색 텍스트
		useWhiteText = true;
	}
	else if (brightness > 0.8f && saturation < 0.3f) 
	{
		// 밝고 채도가 낮은 색상 (회색계열) -> 검은색 텍스트
		useWhiteText = false;
	}
	else if (saturation < 0.4f) 
	{
		// 채도가 낮은 경우 명도만으로 판단
		useWhiteText = (brightness < 0.6f);
	}
	else
	{
		// 채도가 높은 경우 더 세밀한 판단
		if (brightness < 0.4f) 
		{
			useWhiteText = true;
		}
		else if (brightness > 0.7f) 
		{
			useWhiteText = false;
		}
		else
		{
			// 중간 명도에서는 색상별 가중치 적용 (인간의 시각 특성 고려)
			float luminance = 0.299f * rf + 0.587f * gf + 0.114f * bf;
			useWhiteText = (luminance < 0.5f);
		}
	}


	//-----------------------------------------------------------------------
	auto rgb = getColorRGB32(color);

	Color fillColor(rgb);
	Color textColor(~rgb);
//	Color lineColor(Colors::Black());

	if (useWhiteText) 
	{
		textColor = Colors::White();
	}
	else 
	{
		textColor = Colors::Black();
	}


	control->uiControlStyle().fill().fillColor(fillColor);
//	control->uiControlStyle().line().lineColor(lineColor);
//	control->uiControlStyle().line().lineSize(1);
	control->uiControlStyle().text().textColor(textColor);
}

//===========================================================================
void PropertyPanel::addItemPropertyTextUI(std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	auto valueString = property->value();


	//-----------------------------------------------------------------------
	addPropertyNameUIControl(name);


	//-----------------------------------------------------------------------
	auto valueUIControl = addPropertyTextUIControl(valueString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		valueUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			InputTextBox::TextType textType;
			if      (property->type() == cx::Widget::PropertyTypes::UInt8 ) { textType = InputTextBox::TextType::UInt8 ; }
			else if (property->type() == cx::Widget::PropertyTypes::UInt16) { textType = InputTextBox::TextType::UInt16; }
			else if (property->type() == cx::Widget::PropertyTypes::UInt32) { textType = InputTextBox::TextType::UInt32; }
			else if (property->type() == cx::Widget::PropertyTypes::UInt64) { textType = InputTextBox::TextType::UInt64; }
			else if (property->type() == cx::Widget::PropertyTypes::Int8  ) { textType = InputTextBox::TextType::Int8  ; }
			else if (property->type() == cx::Widget::PropertyTypes::Int16 ) { textType = InputTextBox::TextType::Int16 ; }
			else if (property->type() == cx::Widget::PropertyTypes::Int32 ) { textType = InputTextBox::TextType::Int32 ; }
			else if (property->type() == cx::Widget::PropertyTypes::Int64 ) { textType = InputTextBox::TextType::Int64 ; }
			else if (property->type() == cx::Widget::PropertyTypes::Float ) { textType = InputTextBox::TextType::Float ; }
			else if (property->type() == cx::Widget::PropertyTypes::Double) { textType = InputTextBox::TextType::Double; }
			else if (property->type() == cx::Widget::PropertyTypes::String) { textType = InputTextBox::TextType::String; }
			else
			{
				return;
			}


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			std::wstring valueString = property->value();


			std::uint32_t x, y, cx, cy;
			calcPropertyValueBoxRect(item, x, y, cx, cy);
			if (showInputTextBox(*this, x, y, cx, cy, property->readOnly(), textType, valueString))
			{
				property->value(valueString);
				item->text(valueString);
			}
		}
	);
}

void PropertyPanel::addItemPropertyBoolUI(std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	auto valueString = property->value();
	auto value = cx::to_bool(valueString);


	//-----------------------------------------------------------------------
	addPropertyNameUIControl(name);


	//-----------------------------------------------------------------------
	auto valueUIControl = addPropertyTextUIControl(valueString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		valueUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			std::wstring valueString = property->value();
			
			
			std::vector<std::wstring> valueStringList;
			valueStringList.push_back(L"true");
			valueStringList.push_back(L"false");


			std::uint32_t x, y, cx, cy = 50;
			calcPropertyValueDropBoxRect(item, x, y, cx, cy);
			if (showInputTextListBox(*this, x, y, cx, cy, property->readOnly(), InputTextListBox::TextType::String, valueStringList, valueString))
			{
				property->value(valueString);
				item->text(valueString);
			}
		}
	);
}

void PropertyPanel::addItemPropertyPointsUI(std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	auto valueString = property->value();


	//-----------------------------------------------------------------------
	addPropertyNameUIControl(name);


	//-----------------------------------------------------------------------
	auto valueUIControl = addPropertyTextUIControl(valueString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		valueUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			std::wstring valueString = property->value();


			std::uint32_t x, y, cx, cy;
			calcPropertyValueBoxRect(item, x, y, cx, cy);
			showInputTextBox(*this, x, y, cx, cy, true, InputTextBox::TextType::String, valueString);
		}
	);
}

void PropertyPanel::addItemPropertyFillStyleUI(std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	auto valueString = property->value();
	FillStyle fillStyle = to_FillStyle(valueString);


	//-----------------------------------------------------------------------
	addPropertyGroupNameUIControl(name);


	//-----------------------------------------------------------------------
	auto fillColor = fillStyle.fillColor();
	std::wstring fillColorString = to_std_wstring(fillColor);

	auto fillColorA8 = getColorA8(fillColor);
	auto fillColorR8 = getColorR8(fillColor);
	auto fillColorG8 = getColorG8(fillColor);
	auto fillColorB8 = getColorB8(fillColor);

	auto fillColorRGBString = std::format(L"{},{},{}", fillColorR8, fillColorG8, fillColorB8);
	auto fillColorAString = std::format(L"{}", fillColorA8);


	//-----------------------------------------------------------------------
	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::fillStyle_fillColor));
	auto fillColorRGBUIControl = addPropertyTextUIControl(fillColorRGBString, property->readOnly());
	setPropertyColorValueUIControl(fillColorRGBUIControl, fillColor);
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		fillColorRGBUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			FillStyle fillStyle = to_FillStyle(valueString);


			auto fillColor = fillStyle.fillColor();
			auto fillColorA8 = getColorA8(fillColor);
			auto fillColorR8 = getColorR8(fillColor);
			auto fillColorG8 = getColorG8(fillColor);
			auto fillColorB8 = getColorB8(fillColor);
			auto fillColorRGBString = std::format(L"{},{},{}", fillColorR8, fillColorG8, fillColorB8);


			cx::wui::ColorPickerDialog box;
			if (IDOK == box.doModal(*this, fillColorR8, fillColorG8, fillColorB8))
			{
				fillColorRGBString = std::format(L"{},{},{}", fillColorR8, fillColorG8, fillColorB8);


				fillColor = Color(
					fillColorA8,
					fillColorR8,
					fillColorG8,
					fillColorB8
				);
				fillStyle.fillColor(fillColor);


				valueString = to_std_wstring(fillStyle);
				property->value(valueString);


				item->text(fillColorRGBString);
				setPropertyColorValueUIControl(item, fillColor);
			}
		}
	);


	//-----------------------------------------------------------------------
	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::fillStyle_fillColorA));
	auto fillColorAUIControl = addPropertyTextUIControl(fillColorAString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		fillColorAUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			FillStyle fillStyle = to_FillStyle(valueString);


			auto fillColor = fillStyle.fillColor();
			auto fillColorA8 = getColorA8(fillColor);
			auto fillColorR8 = getColorR8(fillColor);
			auto fillColorG8 = getColorG8(fillColor);
			auto fillColorB8 = getColorB8(fillColor);
			auto fillColorAString = std::format(L"{}", fillColorA8);


			std::uint32_t x, y, cx, cy;
			calcPropertyValueBoxRect(item, x, y, cx, cy);
			if (showInputTextBox(*this, x, y, cx, cy, property->readOnly(), InputTextBox::TextType::UInt8, fillColorAString))
			{
				fillColorA8 = cx::to_std_uint8_t(fillColorAString);
				fillColorAString = std::format(L"{}", fillColorA8);


				fillColor = Color(
					fillColorA8,
					fillColorR8,
					fillColorG8,
					fillColorB8
				);
				fillStyle.fillColor(fillColor);


				valueString = to_std_wstring(fillStyle);
				property->value(valueString);


				item->text(fillColorAString);
			}
		}
	);
}

void PropertyPanel::addItemPropertyLineStyleUI(std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	auto valueString = property->value();
	LineStyle lineStyle = to_LineStyle(valueString);


	//-----------------------------------------------------------------------
	addPropertyGroupNameUIControl(name);


	//-----------------------------------------------------------------------
	auto lineColor = lineStyle.lineColor();
	std::wstring lineColorString = to_std_wstring(lineColor);

	auto lineColorA8 = getColorA8(lineColor);
	auto lineColorR8 = getColorR8(lineColor);
	auto lineColorG8 = getColorG8(lineColor);
	auto lineColorB8 = getColorB8(lineColor);

	auto lineColorRGBString = std::format(L"{},{},{}", lineColorR8, lineColorG8, lineColorB8);
	auto lineColorAString = std::format(L"{}", lineColorA8);


	//-----------------------------------------------------------------------
	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::lineStyle_lineColor));
	auto lineColorRGBUIControl = addPropertyTextUIControl(lineColorRGBString, property->readOnly());
	setPropertyColorValueUIControl(lineColorRGBUIControl, lineColor);
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		lineColorRGBUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			LineStyle lineStyle = to_LineStyle(valueString);


			auto lineColor = lineStyle.lineColor();
			auto lineColorA8 = getColorA8(lineColor);
			auto lineColorR8 = getColorR8(lineColor);
			auto lineColorG8 = getColorG8(lineColor);
			auto lineColorB8 = getColorB8(lineColor);
			auto lineColorRGBString = std::format(L"{},{},{}", lineColorR8, lineColorG8, lineColorB8);


			cx::wui::ColorPickerDialog box;
			if (IDOK == box.doModal(*this, lineColorR8, lineColorG8, lineColorB8))
			{
				lineColorRGBString = std::format(L"{},{},{}", lineColorR8, lineColorG8, lineColorB8);


				lineColor = Color(
					lineColorA8,
					lineColorR8,
					lineColorG8,
					lineColorB8
				);
				lineStyle.lineColor(lineColor);


				valueString = to_std_wstring(lineStyle);
				property->value(valueString);


				item->text(lineColorRGBString);
				setPropertyColorValueUIControl(item, lineColor);
			}
		}
	);


	//-----------------------------------------------------------------------
	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::lineStyle_lineColorA));
	auto lineColorAUIControl = addPropertyTextUIControl(lineColorAString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		lineColorAUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			LineStyle lineStyle = to_LineStyle(valueString);


			auto lineColor = lineStyle.lineColor();
			auto lineColorA8 = getColorA8(lineColor);
			auto lineColorR8 = getColorR8(lineColor);
			auto lineColorG8 = getColorG8(lineColor);
			auto lineColorB8 = getColorB8(lineColor);
			auto lineColorAString = std::format(L"{}", lineColorA8);


			std::uint32_t x, y, cx, cy;
			calcPropertyValueBoxRect(item, x, y, cx, cy);
			if (showInputTextBox(*this, x, y, cx, cy, property->readOnly(), InputTextBox::TextType::UInt8, lineColorAString))
			{
				lineColorA8 = cx::to_std_uint8_t(lineColorAString);
				lineColorAString = std::format(L"{}", lineColorA8);


				lineColor = Color(
					lineColorA8,
					lineColorR8,
					lineColorG8,
					lineColorB8
				);
				lineStyle.lineColor(lineColor);


				valueString = to_std_wstring(lineStyle);
				property->value(valueString);


				item->text(lineColorAString);
			}
		}
	);


	//-----------------------------------------------------------------------
	auto lineSize = lineStyle.lineSize();
	std::wstring lineSizeString = cx::to_std_wstring(lineSize);

	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::lineStyle_lineSize));
	auto lineSizeUIControl = addPropertyTextUIControl(lineSizeString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		lineSizeUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			LineStyle lineStyle = to_LineStyle(valueString);


			auto lineSize = lineStyle.lineSize();
			std::wstring lineSizeString = cx::to_std_wstring(lineSize);


			std::uint32_t x, y, cx, cy;
			calcPropertyValueBoxRect(item, x, y, cx, cy);
			if (showInputTextBox(*this, x, y, cx, cy, property->readOnly(), InputTextBox::TextType::Float, lineSizeString))
			{
				lineSize = cx::to_float(lineSizeString);
				if (0.1f < lineSize && lineSize < 128.0f)
				{
					lineStyle.lineSize(lineSize);
					valueString = cx::Widget::to_std_wstring(lineStyle);
					property->value(valueString);


					item->text(lineSizeString);
				}
			}
		}
	);
}

void PropertyPanel::addItemPropertyTextStyleUI(std::shared_ptr<cx::Widget::Property> property)
{
	//-----------------------------------------------------------------------
	using namespace cx::Widget;


	//-----------------------------------------------------------------------
	auto name = property->friendlyName();
	auto valueString = property->value();
	TextStyle textStyle = to_TextStyle(valueString);


	//-----------------------------------------------------------------------
	addPropertyGroupNameUIControl(name);


	//-----------------------------------------------------------------------
	auto textColor = textStyle.textColor();
	std::wstring textColorString = to_std_wstring(textColor);

	auto textColorA8 = getColorA8(textColor);
	auto textColorR8 = getColorR8(textColor);
	auto textColorG8 = getColorG8(textColor);
	auto textColorB8 = getColorB8(textColor);

	auto textColorRGBString = std::format(L"{},{},{}", textColorR8, textColorG8, textColorB8);
	auto textColorAString = std::format(L"{}", textColorA8);


	//-----------------------------------------------------------------------
	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::textStyle_textColor));
	auto textColorRGBUIControl = addPropertyTextUIControl(textColorRGBString, property->readOnly());
	setPropertyColorValueUIControl(textColorRGBUIControl, textColor);
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		textColorRGBUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			TextStyle textStyle = to_TextStyle(valueString);


			auto textColor = textStyle.textColor();
			auto textColorA8 = getColorA8(textColor);
			auto textColorR8 = getColorR8(textColor);
			auto textColorG8 = getColorG8(textColor);
			auto textColorB8 = getColorB8(textColor);
			auto textColorRGBString = std::format(L"{},{},{}", textColorR8, textColorG8, textColorB8);


			cx::wui::ColorPickerDialog box;
			if (IDOK == box.doModal(*this, textColorR8, textColorG8, textColorB8))
			{
				textColorRGBString = std::format(L"{},{},{}", textColorR8, textColorG8, textColorB8);


				textColor = Color(
					textColorA8,
					textColorR8,
					textColorG8,
					textColorB8
				);
				textStyle.textColor(textColor);


				valueString = to_std_wstring(textStyle);
				property->value(valueString);


				item->text(textColorRGBString);
				setPropertyColorValueUIControl(item, textColor);
			}
		}
	);


	//-----------------------------------------------------------------------
	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::textStyle_textColorA));
	auto textColorAUIControl = addPropertyTextUIControl(textColorAString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		textColorAUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			TextStyle textStyle = to_TextStyle(valueString);


			auto textColor = textStyle.textColor();
			auto textColorA8 = getColorA8(textColor);
			auto textColorR8 = getColorR8(textColor);
			auto textColorG8 = getColorG8(textColor);
			auto textColorB8 = getColorB8(textColor);
			auto textColorAString = std::format(L"{}", textColorA8);


			std::uint32_t x, y, cx, cy;
			calcPropertyValueBoxRect(item, x, y, cx, cy);
			if (showInputTextBox(*this, x, y, cx, cy, property->readOnly(), InputTextBox::TextType::UInt8, textColorAString))
			{
				textColorA8 = cx::to_std_uint8_t(textColorAString);
				textColorAString = std::format(L"{}", textColorA8);


				textColor = Color(
					textColorA8,
					textColorR8,
					textColorG8,
					textColorB8
				);
				textStyle.textColor(textColor);


				valueString = to_std_wstring(textStyle);
				property->value(valueString);


				item->text(textColorAString);
			}
		}
	);


	//-----------------------------------------------------------------------
	std::wstring fontFamily = textStyle.fontFamily().data();
	std::wstring fontFamilyString = fontFamily;

	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::textStyle_fontFamily));
	addPropertyTextUIControl(fontFamilyString, property->readOnly());


	//-----------------------------------------------------------------------
	float fontSize = textStyle.fontSize();
	std::wstring fontSizeString = cx::to_std_wstring(fontSize);

	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::textStyle_fontSize));
	auto fontSizeUIControl = addPropertyTextUIControl(fontSizeString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		fontSizeUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			TextStyle textStyle = to_TextStyle(valueString);


			auto fontSize = textStyle.fontSize();
			std::wstring fontSizeString = cx::to_std_wstring(fontSize);


			std::uint32_t x, y, cx, cy;
			calcPropertyValueBoxRect(item, x, y, cx, cy);
			if (showInputTextBox(*this, x, y, cx, cy, property->readOnly(), InputTextBox::TextType::Float, fontSizeString))
			{
				fontSize = cx::to_float(fontSizeString);
				if (1.0f < fontSize && fontSize < 128.0f)
				{
					textStyle.fontSize(fontSize);
					valueString = cx::Widget::to_std_wstring(textStyle);
					property->value(valueString);


					item->text(fontSizeString);
				}
			}
		}
	);


	//-----------------------------------------------------------------------
	bool fontBold = textStyle.fontBold();
	std::wstring fontBoldString = cx::to_std_wstring(fontBold);

	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::textStyle_fontBold));
	auto fontBoldUIControl = addPropertyTextUIControl(fontBoldString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		fontBoldUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);

	
			auto valueString = property->value();
			TextStyle textStyle = to_TextStyle(valueString);

			bool fontBold = textStyle.fontBold();
			fontBold = !fontBold;
			textStyle.fontBold(fontBold);

			property->value(cx::Widget::to_std_wstring(textStyle));

			
			std::wstring fontBoldString = cx::to_std_wstring(fontBold);
			item->text(fontBoldString);
		}
	);


	//-----------------------------------------------------------------------
	bool fontItalic = textStyle.fontItalic();
	std::wstring fontItalicString = cx::to_std_wstring(fontItalic);

	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::textStyle_fontItalic));
	auto fontItalicUIControl = addPropertyTextUIControl(fontItalicString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		fontItalicUIControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return;  }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item =	std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);


			auto valueString = property->value();
			TextStyle textStyle = to_TextStyle(valueString);

			bool fontItalic = textStyle.fontItalic();
			fontItalic = !fontItalic;
			textStyle.fontItalic(fontItalic);

			property->value(cx::Widget::to_std_wstring(textStyle));


			std::wstring fontItalicString = cx::to_std_wstring(fontItalic);
			item->text(fontItalicString);
		}
	);


	//-----------------------------------------------------------------------
	TextHAlignment textHAlignment = textStyle.textHAlignment();
	std::wstring textHAlignmentString = getTextHAlignmentString(textHAlignment);

	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::textStyle_textHAlignment));
	auto textHAlignmenUItControl = addPropertyTextUIControl(textHAlignmentString, property->readOnly());
	_UIController->_View->eventHandlerRegistry().registerEventHandler(
		ItemPointerClickedEvent,
		textHAlignmenUItControl,
		[this, property](cx::ev::Event& event)
		{
			if (!isPropertyEditable(property)) { return; }


			using Control = UIControl::Button;
			auto eventType = event.eventType();
			auto itemPointerEventData = event.eventDataAs<ItemPointerEventData>();
			std::shared_ptr<Control> item = std::dynamic_pointer_cast<Control>(itemPointerEventData->_Item);



		}
	);


	//-----------------------------------------------------------------------
	TextVAlignment textVAlignment = textStyle.textVAlignment();
	std::wstring textVAlignmentString = getTextVAlignmentString(textVAlignment);

	addPropertySubNameUIControl(std::wstring(cx::Widget::PropertyFriendlyNames::textStyle_textVAlignment));
	addPropertyTextUIControl(textVAlignmentString, property->readOnly());
}
