#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowScrollHandler
{
public:
	using Handler = std::function<void(bool,std::int64_t, std::int64_t)>;

public:
	Handler scrollChangedHandler;

private:
	HWND _Hwnd{ nullptr };
	bool _XScrollEnabled{ true };
	bool _YScrollEnabled{ true };

private:
	std::int64_t _XSize{  0 };
	std::int64_t _XPage{  0 };
	std::int64_t _XPos {  0 };
	std::int64_t _XLine{ 20 };

private:
	std::int64_t _YSize{  0 };
	std::int64_t _YPage{  0 };
	std::int64_t _YPos {  0 };	
	std::int64_t _YLine{ 20 };

public:
	explicit WindowScrollHandler(HWND hwnd, bool xScrollEnabled=true, bool yScrollEnabled=true);

public:
	WindowScrollHandler(const WindowScrollHandler& other) = delete;
	WindowScrollHandler& operator=(const WindowScrollHandler& other) = delete;
	WindowScrollHandler(WindowScrollHandler&& other) noexcept = delete;
	WindowScrollHandler& operator=(WindowScrollHandler&& other) noexcept = delete;

protected:
	bool setXSize(std::int64_t size);
	bool setXPage(std::int64_t page);
	bool setXPos(std::int64_t pos);
	bool setXLine(std::int64_t line);

protected:
	bool setYSize(std::int64_t size);
	bool setYPage(std::int64_t page);
	bool setYPos(std::int64_t pos);
	bool setYLine(std::int64_t Line);

public:
	void setXYScrollLine(std::int64_t xline, std::int64_t yline);

public:
	void setXScroll(std::int64_t size, std::int64_t page, std::int64_t pos);
	void setXScroll(std::int64_t size, std::int64_t page);
	void setYScroll(std::int64_t size, std::int64_t page, std::int64_t pos);
	void setYScroll(std::int64_t size, std::int64_t page);
	void setXYScroll(
		std::int64_t xsize, std::int64_t xpage, std::int64_t xpos,
		std::int64_t ysize, std::int64_t ypage, std::int64_t ypos
	);
	void setXYScroll(
		std::int64_t xsize, std::int64_t xpage,
		std::int64_t ysize, std::int64_t ypage
	); 

public:
	void updateScrollBars(void);
	void updateXScrollBar(void);
	void updateYScrollBar(void);

public:
	std::int64_t getXScrollTrackPos(void);
	std::int64_t getYScrollTrackPos(void);
	std::int64_t getXScrollPos(void);
	std::int64_t getYScrollPos(void);

public:
	virtual bool onWindowMessage(cx::wui::WindowMessage& windowMessage);

public:
	virtual bool onHScroll(cx::wui::WindowMessage& windowMessage);
	virtual bool onVScroll(cx::wui::WindowMessage& windowMessage);

public:
	virtual void XScroll_Top          (void);
	virtual void XScroll_Bottom       (void);
	virtual void XScroll_LineUp       (void);
	virtual void XScroll_LineDown     (void);
	virtual void XScroll_PageUp       (void);
	virtual void XScroll_PageDown     (void);
	virtual void XScroll_ThumbTrack   (void);
	virtual void XScroll_ThumbPosition(void);
	virtual void XScroll_EndScroll    (void);

public:
	virtual void YScroll_Top          (void);
	virtual void YScroll_Bottom       (void);
	virtual void YScroll_LineUp       (void);
	virtual void YScroll_LineDown     (void);
	virtual void YScroll_PageUp       (void);
	virtual void YScroll_PageDown     (void);
	virtual void YScroll_ThumbTrack   (void);
	virtual void YScroll_ThumbPosition(void);
	virtual void YScroll_EndScroll    (void);

public:
	virtual void notifyScrollChanged(bool byScrollBar=true);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




