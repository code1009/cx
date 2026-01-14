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
};
