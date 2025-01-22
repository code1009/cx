#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View : 
	public wui::MessageMapWindowT<View, wui::BaseWindow>
{
private:
	std::unique_ptr<gw::basic_window> _gw_window;

public:
	explicit View(HWND parentWindowHandle);

public:
	virtual HWND createView(HWND parentWindowHandle);

public:
	void registerWindowMessageMap(void);
	void onCreate(wui::WindowMessage& windowMessage);
	void onDestroy(wui::WindowMessage& windowMessage);
	void onClose(wui::WindowMessage& windowMessage);
	void onSize(wui::WindowMessage& windowMessage);
	void onHScroll(wui::WindowMessage& windowMessage);
	void onVScroll(wui::WindowMessage& windowMessage);	
	void onMouseWheel(wui::WindowMessage& windowMessage);
	void onMouseMove(wui::WindowMessage& windowMessage);
	void onKeyDown(wui::WindowMessage& windowMessage);
	void onEraseBkgnd(wui::WindowMessage& windowMessage);
	void onPaint(wui::WindowMessage& windowMessage);	
	void onCommand(wui::WindowMessage& windowMessage);
	void onMenuCommand(wui::WindowMessage& windowMessage);
	void onCtlCommand(wui::WindowMessage& windowMessage);

public:
	void onIdle(void);
};
