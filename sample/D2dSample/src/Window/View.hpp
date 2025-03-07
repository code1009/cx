#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MyWindow : public cx::gw::Window
{
private:
	ID2D1SolidColorBrush* _Brush{ nullptr };

public:
	MyWindow(HWND hwnd);

	//-----------------------------------------------------------------------
	// cx::gw::window
public:
	virtual bool createDeviceResources(void) override;
	virtual void destroyDeviceResources(void) override;
	virtual void draw(void) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View : public cx::wui::MessageMapWindowT<View, cx::wui::BaseWindow>
{
public:
	std::unique_ptr<MyWindow> _Window;

public:
	explicit View(HWND parentWindowHandle);

public:
	virtual HWND createView(HWND parentWindowHandle);

public:
	void registerWindowMessageMap(void);
	void onCreate(cx::wui::WindowMessage& windowMessage);
	void onDestroy(cx::wui::WindowMessage& windowMessage);
	void onClose(cx::wui::WindowMessage& windowMessage);
	void onSize(cx::wui::WindowMessage& windowMessage);
	void onHScroll(cx::wui::WindowMessage& windowMessage);
	void onVScroll(cx::wui::WindowMessage& windowMessage);
	void onMouseWheel(cx::wui::WindowMessage& windowMessage);
	void onEraseBkgnd(cx::wui::WindowMessage& windowMessage);
	void onPaint(cx::wui::WindowMessage& windowMessage);

public:
	void onIdle(void);
};
