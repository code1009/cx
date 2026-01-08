#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class d2dDiagram
{
public:
	HWND _Hwnd{ nullptr };
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::Widget::PropertiesManipulator> _Diagram_PropertiesManipulator;
	std::unique_ptr<cx::Widget::Edit> _Diagram_Edit;
	std::unique_ptr<cx::wui::WindowMouseHandler> _MouseHandler;
	std::unique_ptr<cx::wui::WindowScrollHandler> _ScrollHandler;
	std::unique_ptr<cx::wui::dragdrop::WindowDropTargetHandler> _DropTargetHandler;

public:
	explicit d2dDiagram(HWND hwnd);
	virtual ~d2dDiagram();

public:
	d2dDiagram(const d2dDiagram& other) = delete;
	d2dDiagram& operator=(const d2dDiagram& other) = delete;
	d2dDiagram(d2dDiagram&& other) noexcept = delete;
	d2dDiagram& operator=(d2dDiagram&& other) noexcept = delete;

public:
	void resize(std::uint32_t cx, std::uint32_t cy);
	void invalidate(void);

public:
	void zoomIn(float px, float py);
	void zoomOut(float px, float py);
};
