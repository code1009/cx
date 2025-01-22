#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class my_gw_window : public gw::window
{
private:
	ID2D1SolidColorBrush* _Brush{ nullptr };

public:
	my_gw_window(HWND hwnd);

	//-----------------------------------------------------------------------
	// gw::window
public:
	virtual bool create_device_resources(void) override;
	virtual void destroy_device_resources(void) override;
	virtual void draw(void) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View : public wui::MessageMapWindowT<View, wui::BaseWindow>
{
public:
	std::unique_ptr<my_gw_window> _gw_window;

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
	void onEraseBkgnd(wui::WindowMessage& windowMessage);
	void onPaint(wui::WindowMessage& windowMessage);

public:
	void onIdle(void);
};
