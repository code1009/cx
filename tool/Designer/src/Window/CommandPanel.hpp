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
	View* _View{ nullptr };

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
	explicit CommandPanel(HWND parentWindowHandle, View* view);

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
	void addCommands(void);
	void addCommand_Label(std::wstring Label);
	void addCommand_Spare();
	void addCommand_NewItem(std::shared_ptr<cx::Widget::Item> const& item, cx::Widget::ClassInfo::MakePropertiesFunction const& makeProperties, cx::Widget::StringView const& friendlyName);

public:
	void loadCommands(void);

public:
	void doDragDrop(void);
};
