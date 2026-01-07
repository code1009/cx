#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class d2dDiagram
{
public:
	HWND _Hwnd{ nullptr };
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::Diagram::PropertiesManipulator> _Diagram_PropertiesManipulator;
	std::unique_ptr<cx::Diagram::Edit> _Diagram_Edit;
	std::unique_ptr<cx::wui::WindowMouseHandler> _MouseHandler;
	std::unique_ptr<cx::wui::WindowScrollHandler> _ScrollHandler;

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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class View : 
	public cx::wui::MessageMapWindowT<View, cx::wui::BaseWindow>
{
public:
	std::unique_ptr<d2dDiagram> _d2dDiagram;

public:
	explicit View(HWND parentWindowHandle);

public:
	virtual LRESULT onWindowMessage(cx::wui::WindowMessage& windowMessage) override;

public:
	HWND createView(HWND parentWindowHandle);

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

public:
	void zoomIn(float px, float py);
	void zoomOut(float px, float py);
};
