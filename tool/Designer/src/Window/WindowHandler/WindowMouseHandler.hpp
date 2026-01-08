#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowMouseHandler
{
public:
	using Handler = std::function<bool(cx::wui::WindowMessage&)>;

public:
	Handler mouseWheelHandler;
	Handler mouseMoveHandler;
	Handler mouseHoverHandler;
	Handler mouseLeaveHandler;
	Handler mouseLButtonDbClkHandler;
	Handler mouseLButtonDownHandler;
	Handler mouseLButtonUpHandler;
	Handler mouseRButtonDbClkHandler;
	Handler mouseRButtonDownHandler;
	Handler mouseRButtonUpHandler;

private:
	HWND _Hwnd{ nullptr };

private:
	std::uint64_t _MouseClickedTime{ 0 };
	std::uint64_t _MouseDbClickTime{ 300 };

private:
	bool _MouseCaptureEnabled{ true };
	bool _MouseCaptured{ false };

private:
	bool _MouseTrackEnabled{ true };
	bool _MouseTracked{ false };

public:
	explicit WindowMouseHandler(HWND hwnd);

public:
	WindowMouseHandler(const WindowMouseHandler& other) = delete;
	WindowMouseHandler& operator=(const WindowMouseHandler& other) = delete;
	WindowMouseHandler(WindowMouseHandler&& other) noexcept = delete;
	WindowMouseHandler& operator=(WindowMouseHandler&& other) noexcept = delete;

public:
	void getMouseDbClickTime(std::uint64_t& time);
	void setMouseDbClickTime(std::uint64_t time);

public:
	bool getMouseCaptureEnabled(void);
	void setMouseCaptureEnabled(bool enabled);
	void setMouseCapture(void);
	void releaseMouseCapture(void);
	void resetMouseCaptureFlag(void);

public:
	bool getMouseTrackEnabled(void);
	void setMouseTrackEnabled(bool enabled);
	void setMouseTrack(void);
	void releaseMouseTrack(void);

public:
	virtual bool onWindowMessage(cx::wui::WindowMessage& windowMessage);

public:
	virtual bool onMouseWheel       (cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseMove        (cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseHover       (cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseLeave       (cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseLButtonDbClk(cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseLButtonDown (cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseLButtonUp   (cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseRButtonDbClk(cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseRButtonDown (cx::wui::WindowMessage& windowMessage);
	virtual bool onMouseRButtonUp   (cx::wui::WindowMessage& windowMessage);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



