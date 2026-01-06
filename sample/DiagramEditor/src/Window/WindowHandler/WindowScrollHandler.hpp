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
	

public:
	virtual bool onWindowMessage(cx::wui::WindowMessage& windowMessage);

public:
	virtual void onWindowScrollMessage(cx::wui::WindowMessage& windowMessage, bool& handled);

public:
	virtual void onHScroll(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onVScroll(cx::wui::WindowMessage& windowMessage, bool& handled);

public:
	virtual void onHScroll_Top(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onHScroll_Bottom(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onHScroll_LineUp(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onHScroll_LineDown(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onHScroll_PageUp(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onHScroll_PageDown(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onHScroll_ThumbTrack(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onHScroll_ThumbPosition(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onHScroll_EndScroll(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);

public:
	virtual void onVScroll_Top(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onVScroll_Bottom(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onVScroll_LineUp(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onVScroll_LineDown(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onVScroll_PageUp(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onVScroll_PageDown(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onVScroll_ThumbTrack(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onVScroll_ThumbPosition(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
	virtual void onVScroll_EndScroll(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled);
};

