#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowScrollHandler
{
private:
	HWND _Hwnd{ nullptr };

private:
	std::int64_t _xSize{ 0 };
	std::int64_t _xPage{ 0 };
	std::int64_t _xPos{ 0 };
	std::int64_t _xLine{ 20 };

	std::int64_t _ySize{ 0 };
	std::int64_t _yPage{ 0 };
	std::int64_t _yPos { 0 };	
	std::int64_t _yLine{ 20 };

public:
	explicit WindowScrollHandler(HWND hwnd);

public:
	WindowScrollHandler(const WindowScrollHandler& other) = delete;
	WindowScrollHandler& operator=(const WindowScrollHandler& other) = delete;
	WindowScrollHandler(WindowScrollHandler&& other) noexcept = delete;
	WindowScrollHandler& operator=(WindowScrollHandler&& other) noexcept = delete;

public:
	bool setXSize(std::int64_t size);
	bool setXPage(std::int64_t page);
	bool setXPos(std::int64_t pos);
	bool setXLine(std::int64_t line);

public:
	bool setYSize(std::int64_t size);
	bool setYPage(std::int64_t page);
	bool setYPos(std::int64_t pos);
	bool setYLine(std::int64_t Line);

public:
	void updateScrollBars(void);
	void updateXScrollBar(void);
	void updateYScrollBar(void);

public:
	virtual bool onWindowMessage(cx::wui::WindowMessage& windowMessage);

public:
	virtual void onWindowScrollMessage(cx::wui::WindowMessage& windowMessage, bool& handled);

public:
	virtual void onHScroll(cx::wui::WindowMessage& windowMessage, bool& handled);
	virtual void onVScroll(cx::wui::WindowMessage& windowMessage, bool& handled);

public:
	virtual void onHScroll_Top(void);
	virtual void onHScroll_Bottom(void);
	virtual void onHScroll_LineUp(void);
	virtual void onHScroll_LineDown(void);
	virtual void onHScroll_PageUp(void);
	virtual void onHScroll_PageDown(void);
	virtual void onHScroll_ThumbTrack(void);
	virtual void onHScroll_ThumbPosition(void);
	virtual void onHScroll_EndScroll(void);

public:
	virtual void onVScroll_Top(void);
	virtual void onVScroll_Bottom(void);
	virtual void onVScroll_LineUp(void);
	virtual void onVScroll_LineDown(void);
	virtual void onVScroll_PageUp(void);
	virtual void onVScroll_PageDown(void);
	virtual void onVScroll_ThumbTrack(void);
	virtual void onVScroll_ThumbPosition(void);
	virtual void onVScroll_EndScroll(void);
};

