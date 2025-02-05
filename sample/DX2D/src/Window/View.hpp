#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class My_gw_Window : public gw::Window
{
private:
	ID2D1SolidColorBrush* _Brush{ nullptr };

public:
	My_gw_Window(HWND hwnd);

	//-----------------------------------------------------------------------
	// gw::window
public:
	virtual bool createDeviceResources(void) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(void) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View : public wui::MessageMapWindowT<View, wui::BaseWindow>
{
public:
	std::unique_ptr<My_gw_Window> _gwWindow;

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
