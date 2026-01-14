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
	std::unique_ptr<UILayoutManager> _UILayoutManager;
	std::unique_ptr<UIController> _UIController;

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
	void setupUIControlls(void);
	void recalcUIControllsLayout(std::uint32_t cx, std::uint32_t cy);

public:
	void showItemProperty(void);

public:
	void LoadEmptyPropertyUI(void);
	void loadItemPropertyUI(void);
	void addItemPropertyUI_UInt8    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_UInt16   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_UInt32   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_UInt64   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_Int8     (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_Int16    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_Int32    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_Int64    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_Float    (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_Double   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_Bool     (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_String   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_Points   (std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
 	void addItemPropertyUI_FillStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_LineStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);
	void addItemPropertyUI_TextStyle(std::int32_t& index, std::shared_ptr<cx::Widget::Property> property);

};
