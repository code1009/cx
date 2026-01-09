#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class UIController
{
	//-----------------------------------------------------------------------
public:
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
	void zoomIn(float px, float py);
	void zoomOut(float px, float py);
};
