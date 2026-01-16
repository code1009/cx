#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MainFrame : 
	public cx::wui::MessageMapWindowT<MainFrame, cx::wui::BaseWindow>
{
private:
	std::unique_ptr<CommandPanel> _CommandPanel;
	std::unique_ptr<PropertyPanel> _PropertyPanel;
	std::unique_ptr<View> _View;

private:
	class MenuCheck
	{
	public:
		bool VIEW_COMMAND{ true };
		bool VIEW_PROPERTY{ true };
		bool SNAP_TO_GRID{ true };
		bool SHOW_GRID{ true };
		bool SHOW_GRID_COORD{ false };
		bool SHOW_STATUS{ false };

	public:
		MenuCheck() = default;
	};

	MenuCheck _MenuCheck;

public:
	MainFrame();

public:
	void registerWindowMessageMap(void);
	void onCreate(cx::wui::WindowMessage& windowMessage);
	void onDestroy(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);	
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onEraseBkgnd(cx::wui::WindowMessage& windowMessage);
	void onKeyDown(cx::wui::WindowMessage& windowMessage);
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onMenuCommand(cx::wui::WindowMessage& windowMessage);
	void onAppAbout(cx::wui::WindowMessage& windowMessage);
	void onCtlCommand(cx::wui::WindowMessage& windowMessage);

public:
	void toggleMenuItemChecked(UINT itemID, bool& checked);

public:
	void onIdle(void);

public:
	void updateLayout(std::uint32_t cx, std::uint32_t cy);
};
