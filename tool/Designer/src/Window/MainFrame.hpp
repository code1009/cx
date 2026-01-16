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
	class MenuFlags
	{
	public:
		bool design_viewCommand{ true };
		bool design_viewProperty{ true };
		bool design_snapToGrid{ true };
		bool design_showGrid{ true };
		bool design_showGridCoord{ false };
		bool design_showStatus{ false };

	public:
		MenuFlags() = default;
	};

	MenuFlags _MenuFlags;

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
	void onMenuCommand_DesignViewCommand();
	void onMenuCommand_DesignViewProperty();

public:
	void onIdle(void);

public:
	void updateTitle(void);
	void updateLayout(std::uint32_t cx, std::uint32_t cy);
};
