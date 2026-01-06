#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowScrollHandler
{
private:
	HWND _Hwnd{ nullptr };

public:
	explicit WindowScrollHandler(HWND hwnd);

public:
	WindowScrollHandler(const WindowScrollHandler& other) = delete;
	WindowScrollHandler& operator=(const WindowScrollHandler& other) = delete;
	WindowScrollHandler(WindowScrollHandler&& other) noexcept = delete;
	WindowScrollHandler& operator=(WindowScrollHandler&& other) noexcept = delete;

public:
	virtual bool onWindowMessage(cx::wui::WindowMessage& windowMessage);
	virtual void onWindowScrollMessage(cx::wui::WindowMessage& windowMessage, bool& handled);
};

