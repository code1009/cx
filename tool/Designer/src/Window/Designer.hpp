#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Designer
{
public:
	HWND _Hwnd{ nullptr };
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::Widget::PropertiesManipulator> _PropertiesManipulator;
	std::unique_ptr<cx::Widget::Edit> _Edit;
	std::unique_ptr<cx::wui::WindowMouseHandler> _MouseHandler;
	std::unique_ptr<cx::wui::WindowScrollHandler> _ScrollHandler;
	std::unique_ptr<cx::wui::dragdrop::WindowDropTargetHandler> _DropTargetHandler;

public:
	explicit Designer(HWND hwnd);
	virtual ~Designer();

public:
	Designer(const Designer& other) = delete;
	Designer& operator=(const Designer& other) = delete;
	Designer(Designer&& other) noexcept = delete;
	Designer& operator=(Designer&& other) noexcept = delete;

public:
	void resize(std::uint32_t cx, std::uint32_t cy);
	void invalidate(void);

public:
	void zoomIn(float px, float py);
	void zoomOut(float px, float py);
};
