#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class PropertyPanel : 
	public cx::wui::MessageMapWindowT<PropertyPanel, cx::wui::BaseWindow>
{
	//-----------------------------------------------------------------------
private:
	Designer* _Designer{ nullptr };

private:
	struct PropertyInfo
	{
		std::wstring type;
		std::wstring label;
		std::wstring name;
	};

	std::vector<PropertyInfo> _PropertyInfos;

	std::wstring _NewItemName;

	//-----------------------------------------------------------------------
public:
	std::unique_ptr<UILayoutManager> _UILayoutManager;
	std::unique_ptr<UIController> _UIController;
	std::unique_ptr<cx::wui::dragdrop::WindowDropSourceNotifier> _DropSourceNotifier;

	//-----------------------------------------------------------------------
public:
	explicit PropertyPanel(HWND parentWindowHandle, Designer* designer);

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
	void onProperty(cx::wui::WindowMessage& windowMessage);
	void onMenuProperty(cx::wui::WindowMessage& windowMessage);
	void onCtlProperty(cx::wui::WindowMessage& windowMessage);

	//-----------------------------------------------------------------------
public:
	void onIdle(void);

	//-----------------------------------------------------------------------
public:
	void loadProperty(void);
	void addProperty_Catalog(Catalog* catalog);

public:
	void setupUIControlls(void);
	void recalcUIControllsLayout(std::uint32_t cx, std::uint32_t cy);

public:
	void doDragDrop(void);
};
