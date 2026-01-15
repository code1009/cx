#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class PropertyPanel : 
	public cx::wui::MessageMapWindowT<PropertyPanel, cx::wui::BaseWindow>
{
	//-----------------------------------------------------------------------
private:
	Designer* _Designer{ nullptr };

	//-----------------------------------------------------------------------
private:
	bool _ItemProperty_valueChanged_Flag{ false };

	//-----------------------------------------------------------------------
public:
	std::unique_ptr<UIController> _UIController;

	std::unique_ptr<UILayoutManager> _UILayoutManager;

	std::unique_ptr<UILayoutStyle> _UILayoutStyle0;
	std::unique_ptr<UILayoutStyle> _UILayoutStyle1;
	std::unique_ptr<UILayoutStyle> _UILayoutStyle2;
	std::unique_ptr<UILayoutStyle> _UILayoutStyle3;

	std::function<void(UILayout*)> _UILayoutChangedHandler0 = { nullptr };
	std::function<void(UILayout*)> _UILayoutChangedHandler1 = { nullptr };
	std::function<void(UILayout*)> _UILayoutChangedHandler2 = { nullptr };
	std::function<void(UILayout*)> _UILayoutChangedHandler3 = { nullptr };

	//-----------------------------------------------------------------------
public:
	explicit PropertyPanel(HWND parentWindowHandle, Designer* designer);
	virtual ~PropertyPanel();

public:
	virtual HWND createPropertyPanel(HWND parentWindowHandle);

	//-----------------------------------------------------------------------
public:
	virtual LRESULT onWindowMessage(cx::wui::WindowMessage& windowMessage) override;

public:
	void registerWindowMessageMap(void);
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onEraseBkgnd(cx::wui::WindowMessage& windowMessage);
	void onPaint(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onMenuCommand(cx::wui::WindowMessage& windowMessage);
	void onCtlCommand(cx::wui::WindowMessage& windowMessage);

	//-----------------------------------------------------------------------
public:
	void onIdle(void);

	//-----------------------------------------------------------------------
public:
	void setupUILayoutStyles(void);
	void setupUIControlls(void);
	void recalcUIControllsLayout(std::uint32_t cx, std::uint32_t cy);

public:
	void showItemProperty(void);

public:
	void LoadEmptyPropertyUI(void);
	void loadItemPropertyUI(void);

	bool isPropertyEditable(std::shared_ptr<cx::Widget::Property> property) const;
	std::wstring getTextHAlignmentString(cx::Widget::TextHAlignment textHAlignment);
	std::wstring getTextVAlignmentString(cx::Widget::TextVAlignment textVAlignment);
	void calcPropertyValueBoxRect(
		std::shared_ptr<cx::Widget::UIControl::Base> control,
		std::uint32_t& x, std::uint32_t& y,
		std::uint32_t& cx, std::uint32_t& cy
	);
	void calcPropertyValueDropBoxRect(
		std::shared_ptr<cx::Widget::UIControl::Base> control,
		std::uint32_t& x, std::uint32_t& y,
		std::uint32_t& cx, std::uint32_t& cy
		);

	void addUIControl_PropertyName(std::wstring const& name);
	void addUIControl_PropertyGroupName(std::wstring const& name);
	void addUIControl_PropertySubName(std::wstring const& name);	
	std::shared_ptr<cx::Widget::UIControl::Button> addUIControl_PropertyStringValue(std::wstring const& value, bool readOnly = false);

	void loadItemPropertyUI_UInt8    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_UInt16   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_UInt32   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_UInt64   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_Int8     (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_Int16    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_Int32    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_Int64    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_Float    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_Double   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_Bool     (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_String   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_Points   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
 	void loadItemPropertyUI_FillStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_LineStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void loadItemPropertyUI_TextStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
};
