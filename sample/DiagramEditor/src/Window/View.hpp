#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class d2dDiagram;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class d2dDiagramMouseHandler : public MouseHandler
{
public:
	HWND _Hwnd{ nullptr };
	d2dDiagram* _d2dDiagram{ nullptr };

public:
	explicit d2dDiagramMouseHandler(HWND hwnd, d2dDiagram* d2ddiagram);

public:
	//virtual bool onWindowMessage(cx::wui::WindowMessage& windowMessage) override;
	//virtual void onWindowMouseMessage(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	virtual void onMouseWheel(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	virtual void onMouseMove(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	//virtual void onMouseHOver(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	//virtual void onMouseLeave(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	//virtual void onMouseLButtonDbClk(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	virtual void onMouseLButtonDown(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	virtual void onMouseLButtonUp(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	//virtual void onMouseRButtonDbClk(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	//virtual void onMouseRButtonDown(cx::wui::WindowMessage& windowMessage, bool& handled) override;
	//virtual void onMouseRButtonUp(cx::wui::WindowMessage& windowMessage, bool& handled) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class d2dDiagram
{
public:
	HWND _Hwnd{ nullptr };
	std::unique_ptr<cx::d2d::Canvas> _Canvas;
	std::unique_ptr<cx::Diagram::PropertiesManipulator> _Diagram_PropertiesManipulator;
	std::unique_ptr<cx::Diagram::Edit> _Diagram_Edit;
	std::unique_ptr<d2dDiagramMouseHandler> _MouseHandler;

public:
	explicit d2dDiagram(HWND hwnd);
	virtual ~d2dDiagram();

public:
	d2dDiagram(const d2dDiagram& other);
	d2dDiagram& operator=(const d2dDiagram& other);
	d2dDiagram(d2dDiagram&& other) noexcept;
	d2dDiagram& operator=(d2dDiagram&& other) noexcept;

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
