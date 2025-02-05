/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Viewport
{
public:
	using RepaintHandler = std::function<void(void)>;
	using ResizeHandler = std::function<void(std::int64_t width, std::int64_t height)>;

protected:
	HWND _WindowHandle{ nullptr };

protected:
	RepaintHandler _RepaintHandler{};
	ResizeHandler  _ResizeHandler{};

protected:
	std::int64_t _Window_CX{ 0 };
	std::int64_t _Window_CY{ 0 };

	double _Document_CX{ 0 };
	double _Document_CY{ 0 };

	double _Scale{ 1.0 };

	std::int64_t _Image_CX{ 0 };
	std::int64_t _Image_CY{ 0 };

protected:
	double _DocumentViewport_X{ 0 };
	double _DocumentViewport_Y{ 0 };

	std::int64_t _ImageViewport_X{ 0 };
	std::int64_t _ImageViewport_Y{ 0 };

protected:
	std::int64_t _View_X_Scroll_Min{ 0 };
	std::int64_t _View_X_Scroll_Max{ 0 };
	std::int64_t _View_X_Scroll_Page{ 0 };
	std::int64_t _View_X_Scroll_Line{ 0 };

	std::int64_t _View_Y_Scroll_Min{ 0 };
	std::int64_t _View_Y_Scroll_Max{ 0 };
	std::int64_t _View_Y_Scroll_Page{ 0 };
	std::int64_t _View_Y_Scroll_Line{ 0 };

	bool _ScrollbarEnabled{ false };


	//-----------------------------------------------------------------------
public:
	explicit Viewport(HWND hwnd, RepaintHandler repaintHandler, ResizeHandler resizeHandler);

public:
	virtual ~Viewport() = default;


	//-----------------------------------------------------------------------
public:
	virtual HWND getWindowHandle(void);
	virtual void repaint(void);

private:
	virtual void resize(std::int64_t cx, std::int64_t cy);


	//-----------------------------------------------------------------------
public:
	virtual void getWindowSize(std::int64_t& cx, std::int64_t& cy);
	virtual void getScale(double& scale);

	virtual void getDocumentSize(double& cx, double& cy);
	virtual void getDocumentViewportPoint(double& x, double& y);
	virtual void getDocumentViewportSize(double& cx, double& cy);

	virtual void getImageSize(std::int64_t& cx, std::int64_t& cy);
	virtual void getImageViewportPoint(std::int64_t& x, std::int64_t& y);
	virtual void getImageViewportSize(std::int64_t& cx, std::int64_t& cy);

public:
	virtual void WindowToDocument(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y);
	virtual void DocumentToWindow(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y);

public:
	virtual void setWindowSize(std::int64_t cx, std::int64_t cy);
	virtual void setScale(double scale);
	virtual void setDocumentSize(double cx, double cy);

protected:
	virtual void updateImageSize(void);
	virtual void updateViewport(void);
	virtual void updateViewScroll(void);
	virtual void updateScrollbarPosition(void);


	//-----------------------------------------------------------------------
public:
	virtual void handleVScrollbar(std::uint32_t scrollbarCode);
	virtual void handleHScrollbar(std::uint32_t scrollbarCode);

protected:
	virtual std::int64_t scroll(
		std::uint32_t scrollbarCode,
		std::int64_t  scrollbarPos,
		std::uint64_t scroll_page,
		std::uint64_t scroll_line,
		std::uint64_t scroll_min,
		std::uint64_t scroll_max,
		std::uint64_t scroll_pos);

public:
	virtual void enableScrollbar(bool enable);


	//-----------------------------------------------------------------------
public:
	virtual void zoom(bool zoom_in);
	virtual void fitDocumentToWindow(bool vertical = false);


	//-----------------------------------------------------------------------
public:
	virtual void getDocumentViewportPointTranslation(double& x, double& y);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class CenterViewport : public Viewport
{
public:
	explicit CenterViewport(HWND hwnd, RepaintHandler repaintHandler, ResizeHandler resizeHandler);

public:
	virtual ~CenterViewport() = default;

	//-----------------------------------------------------------------------
	// Viewport
public:
	virtual void WindowToDocument(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y) override;
	virtual void DocumentToWindow(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y) override;

protected:
	virtual void updateViewport(void) override;

public:
	virtual void getDocumentViewportPointTranslation(double& x, double& y) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




