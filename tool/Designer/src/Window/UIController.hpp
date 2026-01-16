#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UIController
{
	//-----------------------------------------------------------------------
private:
	HWND _Hwnd{ nullptr };
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::Widget::View> _View;
	std::unique_ptr<cx::wui::WindowMouseHandler> _MouseHandler;
	std::unique_ptr<cx::wui::WindowScrollHandler> _ScrollHandler;

	//-----------------------------------------------------------------------
public:
	explicit UIController(HWND hwnd);
	virtual ~UIController();

	//-----------------------------------------------------------------------
public:
	UIController(const UIController& other) = delete;
	UIController& operator=(const UIController& other) = delete;
	UIController(UIController&& other) noexcept = delete;
	UIController& operator=(UIController&& other) noexcept = delete;

	//-----------------------------------------------------------------------
public:
	cx::d2d::Canvas* canvas(void) { return _Canvas.get(); }
	cx::Widget::View* view(void) { return _View.get(); }
	cx::wui::WindowMouseHandler* mouseHandler(void) { return _MouseHandler.get(); }
	cx::wui::WindowScrollHandler* scrollHandler(void) { return _ScrollHandler.get(); }

	//-----------------------------------------------------------------------
private:
	void setupCanvasView(void);
	void setupMouseHandlerHandler(void);
	void setupScrollHandler(void);
	void loadUIControls(void);

	//-----------------------------------------------------------------------
public:
	void resize(std::uint32_t cx, std::uint32_t cy);
	void invalidate(void);

public:
	void updateScrollBar(void);

public:
	bool onWindowMessage(cx::wui::WindowMessage& windowMessage);
};
