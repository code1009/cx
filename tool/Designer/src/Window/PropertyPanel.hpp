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

	std::vector<std::wstring> getTextHAlignmentStringList(void);
	std::vector<std::wstring> getTextVAlignmentStringList(void);
	std::wstring getTextHAlignmentString(cx::Widget::TextHAlignment textHAlignment);
	std::wstring getTextVAlignmentString(cx::Widget::TextVAlignment textVAlignment);
	cx::Widget::TextHAlignment toTextHAlignment(std::wstring const& textHAlignmentString);
	cx::Widget::TextVAlignment toTextVAlignment(std::wstring const& textVAlignmentString);

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

	void addPropertyNameUIControl(std::wstring const& name);
	void addPropertyGroupNameUIControl(std::wstring const& name);
	void addPropertySubNameUIControl(std::wstring const& name);
	std::shared_ptr<cx::Widget::UIControl::Button> addPropertyTextUIControl(std::wstring const& value, bool readOnly = false);
	void setPropertyColorValueUIControl(std::shared_ptr<cx::Widget::UIControl::Button>, cx::Widget::Color color);

	void addItemPropertyTextUI     (std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyBoolUI     (std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyPointsUI   (std::shared_ptr<cx::Widget::Property> property);
 	void addItemPropertyFillStyleUI(std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyLineStyleUI(std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyTextStyleUI(std::shared_ptr<cx::Widget::Property> property);
};
