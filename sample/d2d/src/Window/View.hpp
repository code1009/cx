#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View : 
	public cx::wui::MessageMapWindowT<View, cx::wui::BaseWindow>
{
public:
	std::unique_ptr<cx::d2d::Context> _Context;
	std::unique_ptr<cx::d2d::Renderer> _Renderer;
	
public:
	explicit View(HWND parentWindowHandle);

public:
	virtual HWND createView(HWND parentWindowHandle);

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
};
