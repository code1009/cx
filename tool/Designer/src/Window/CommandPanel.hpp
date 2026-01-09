#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CommandPanel : 
	public cx::wui::MessageMapWindowT<CommandPanel, cx::wui::BaseWindow>
{
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

public:
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::wui::WindowMouseHandler> _MouseHandler;
	std::unique_ptr<cx::wui::WindowScrollHandler> _ScrollHandler;
	std::unique_ptr<cx::wui::dragdrop::WindowDropSourceNotifier> _DropSourceNotifier;

public:
	explicit CommandPanel(HWND parentWindowHandle, Designer* designer);

public:
	virtual HWND createCommandPanel(HWND parentWindowHandle);

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

public:
	void onIdle(void);

public:
	void loadCommand(void);
	void addCommand_Catalog(Catalog* catalog);

public:
	void doDragDrop(void);
};
