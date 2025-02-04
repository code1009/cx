#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ToolBox : 
	public wui::MessageMapWindowT<ToolBox, wui::BaseWindow>
{
private:
	HWND _parentWindowHandle{ nullptr };

public:
	explicit ToolBox(HWND parentWindowHandle);

public:
	virtual HWND createToolBox(HWND parentWindowHandle);

public:
	void registerWindowMessageMap(void);
	void onCreate(wui::WindowMessage& windowMessage);
	void onClose(wui::WindowMessage& windowMessage);
	void onSize(wui::WindowMessage& windowMessage);
	void onEraseBkgnd(wui::WindowMessage& windowMessage);
	void onKeyDown(wui::WindowMessage& windowMessage);
	void onCommand(wui::WindowMessage& windowMessage);
	void onMenuCommand(wui::WindowMessage& windowMessage);
	void onCtlCommand(wui::WindowMessage& windowMessage);

public:
	void onIdle(void);
};
