#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CommandPanel : 
	public cx::wui::MessageMapWindowT<CommandPanel, cx::wui::BaseWindow>
{
	//-----------------------------------------------------------------------
private:
	Designer* _Designer{ nullptr };

private:
	struct CommandInfo
	{
		std::wstring type;
		std::wstring label;
		std::wstring name;
	};

	std::vector<CommandInfo> _CommandInfos;

	std::wstring _NewItemName;

	//-----------------------------------------------------------------------
public:
	UILayoutManager _UILayoutManager;
	std::unique_ptr<UIController> _UIController;
	std::unique_ptr<cx::wui::dragdrop::WindowDropSourceNotifier> _DropSourceNotifier;

	//-----------------------------------------------------------------------
public:
	explicit CommandPanel(HWND parentWindowHandle, Designer* designer);

public:
	virtual HWND createCommandPanel(HWND parentWindowHandle);

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
	void loadCommand(void);
	void addCommand_Catalog(Catalog* catalog);

public:
	void setupUIControlls(void);
	void recalcUIControllsLayout(std::uint32_t cx, std::uint32_t cy);

public:
	void doDragDrop(void);
};
