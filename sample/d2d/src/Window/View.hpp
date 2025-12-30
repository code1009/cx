#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View : 
	public cx::wui::MessageMapWindowT<View, cx::wui::BaseWindow>
{
public:
	explicit View(HWND parentWindowHandle);

public:
	virtual HWND createView(HWND parentWindowHandle);

public:
	void registerWindowMessageMap(void);
	void onCommand(cx::wui::WindowMessage& windowMessage);
	void onMenuCommand(cx::wui::WindowMessage& windowMessage);
	void onCtlCommand(cx::wui::WindowMessage& windowMessage);

public:
	void onIdle(void);
};
