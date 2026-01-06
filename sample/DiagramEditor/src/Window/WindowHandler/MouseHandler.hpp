#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class MouseHandler
{
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
	explicit MouseHandler(HWND hwnd);

public:
	MouseHandler(const MouseHandler& other) = delete;
	MouseHandler& operator=(const MouseHandler& other) = delete;
	MouseHandler(MouseHandler&& other) noexcept = delete;
	MouseHandler& operator=(MouseHandler&& other) noexcept = delete;

public:
	void getMouseDbClickTime(std::uint64_t& time);
	void setMouseDbClickTime(std::uint64_t time);

public:
	bool getMouseCaptureEnabled(void);
	void setMouseCaptureEnabled(bool enabled);
	void setMouseCapture(void);
	void releaseMouseCapture(void);

public:
	bool getMouseTrackEnabled(void);
	void setMouseTrackEnabled(bool enabled);
	void setMouseTrack(void);
	void releaseMouseTrack(void);

public:
	bool isWindowMouseMessage(cx::wui::WindowMessage& windowMessage);

public:
	virtual bool onWindowMessage(cx::wui::WindowMessage& windowMessage);
	virtual void onWindowMouseMessage(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseWheel(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseMove(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseHOver(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseLeave(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseLButtonDbClk(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseLButtonDown(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseLButtonUp(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseRButtonDbClk(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseRButtonDown(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onMouseRButtonUp(cx::wui::WindowMessage& windowMessage, bool& handled);
};

