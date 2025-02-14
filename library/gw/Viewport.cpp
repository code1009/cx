/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if  0
https://www.codeproject.com/Articles/1042516/Custom-Controls-in-Win-API-Scrolling

typedef struct tagSCROLLINFO {
	UINT cbSize;
	UINT fMask;
	int  nMin;
	int  nMax;
	UINT nPage;
	int  nPos;
	int  nTrackPos;
} SCROLLINFO, * LPSCROLLINFO;



https://www.catch22.net/tuts/win32/64bit-scrollbars/

void example(HWND hwnd)
{
	UINT64 max = 0xffffffffffffffff;
	UINT64 pos = 0x1234567812345678;

	SetScrollInfo64(hwnd, SB_VERT, SIF_ALL, max, pos, 15, TRUE);

	// when reacting to a WM_VSCROLL, with SB_THUMBTRACK/SB_THUMBPOS:
	pos = GetScrollPos64(hwnd, SB_VERT, SIF_TRACKPOS, max);
}

#endif

//===========================================================================
#define WIN16_SCROLLBAR_MAX 0x7fff
#define WIN32_SCROLLBAR_MAX 0x7fffffff

//===========================================================================
// Wrapper around SetScrollInfo, performs scaling to 
// allow massive 64bit scroll ranges
static BOOL SetScrollInfo64(HWND hwnd,
	int nBar,
	UINT fMask,
	UINT64 nMax64,
	UINT64 nPos64,
	UINT64 nPage,
	BOOL fRedraw
)
{
	SCROLLINFO si = { static_cast<UINT>(sizeof(si)), fMask };

	// normal scroll range requires no adjustment
	if (nMax64 <= WIN32_SCROLLBAR_MAX)
	{
		si.nMin = (int)0;
		si.nMax = (int)nMax64;
		si.nPage = (int)nPage;
		si.nPos = (int)nPos64;
	}
	// scale the scrollrange down into allowed bounds
	else
	{
		si.nMin = (int)0;
		si.nMax = (int)WIN16_SCROLLBAR_MAX;
		si.nPage = (int)nPage;
		si.nPos = (int)(nPos64 / (nMax64 / WIN16_SCROLLBAR_MAX));
	}

	return SetScrollInfo(hwnd, nBar, &si, fRedraw);
}

static UINT64 GetScrollPos64(HWND hwnd,
	int nBar,
	UINT fMask,
	UINT64 nMax64
)
{
	SCROLLINFO si = { static_cast<UINT>(sizeof(si)), fMask | SIF_PAGE };
	UINT64 nPos32;


	if (!GetScrollInfo(hwnd, nBar, &si))
	{
		return 0;
	}


	nPos32 = (fMask & SIF_TRACKPOS) ? si.nTrackPos : si.nPos;

	// special-case: scroll position at the very end
	if (nPos32 == WIN16_SCROLLBAR_MAX - si.nPage + 1)
	{
		return nMax64 - si.nPage + 1;
	}
	// normal scroll range requires no adjustment
	else if (nMax64 <= WIN32_SCROLLBAR_MAX)
	{
		return nPos32;
	}
	// adjust the scroll position to be relative to maximum value
	else
	{
		return nPos32 * (nMax64 / WIN16_SCROLLBAR_MAX);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Viewport::Viewport(HWND hwnd, RepaintHandler repaintHandler, ResizeHandler resizeHandler) :
	_WindowHandle{ hwnd },
	_RepaintHandler{ repaintHandler },
	_ResizeHandler{ resizeHandler }
{
}

//===========================================================================
HWND Viewport::getWindowHandle(void)
{
	return _WindowHandle;
}

void Viewport::repaint(void)
{
	if (_RepaintHandler)
	{
		_RepaintHandler();
	}
}

void Viewport::resize(std::int64_t cx, std::int64_t cy)
{
	if (_ResizeHandler)
	{
		_ResizeHandler(cx, cy);
	}
}

//===========================================================================
void Viewport::getWindowSize(std::int64_t& cx, std::int64_t& cy)
{
	cx = _Window_CX;
	cy = _Window_CY;
}

void Viewport::getScale(double& scale)
{
	scale = _Scale;
}

void Viewport::getDocumentSize(double& cx, double& cy)
{
	cx = _Document_CX;
	cy = _Document_CY;
}

void Viewport::getDocumentViewportPoint(double& x, double& y)
{
	x = _DocumentViewport_X;
	y = _DocumentViewport_Y;
}

void Viewport::getDocumentViewportSize(double& cx, double& cy)
{
	cx = _Window_CX / _Scale;
	cy = _Window_CY / _Scale;
}

void Viewport::getImageSize(std::int64_t& cx, std::int64_t& cy)
{
	cx = _Image_CX;
	cy = _Image_CY;
}

void Viewport::getImageViewportPoint(std::int64_t& x, std::int64_t& y)
{
	x = _ImageViewport_X;
	y = _ImageViewport_Y;
}

void Viewport::getImageViewportSize(std::int64_t& cx, std::int64_t& cy)
{
	cx = _Window_CX;
	cy = _Window_CY;
}

//===========================================================================
void Viewport::WindowToDocument(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y)
{
	document_x = _DocumentViewport_X + (window_x / _Scale);
	document_y = _DocumentViewport_Y + (window_y / _Scale);
}

void Viewport::DocumentToWindow(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y)
{
	window_x = static_cast<std::int64_t>((document_x - _DocumentViewport_X) * _Scale);
	window_y = static_cast<std::int64_t>((document_y - _DocumentViewport_Y) * _Scale);
}

//===========================================================================
void Viewport::setWindowSize(std::int64_t cx, std::int64_t cy)
{
	if (_Window_CX == cx && _Window_CY == cy)
	{
		return;
	}


	_Window_CX = cx;
	_Window_CY = cy;


	updateImageSize();
	updateViewport();
	updateViewScroll();
	updateScrollbarPosition();

	repaint();

	resize(cx, cy);
}

void Viewport::setScale(double scale)
{
	if (scale <= 0.0)
	{
		return;
	}

	_Scale = scale;

	updateImageSize();
	updateViewport();
	updateViewScroll();
	updateScrollbarPosition();

	repaint();
}

void Viewport::setDocumentSize(double cx, double cy)
{
	_DocumentViewport_X = 0.0;
	_DocumentViewport_Y = 0.0;
	_Document_CX = cx;
	_Document_CY = cy;

	updateImageSize();
	updateViewport();
	updateViewScroll();
	updateScrollbarPosition();

	repaint();
}

//===========================================================================
void Viewport::updateImageSize(void)
{
	_Image_CX = static_cast<std::int64_t>(_Document_CX * _Scale);
	_Image_CY = static_cast<std::int64_t>(_Document_CY * _Scale);
}

void Viewport::updateViewport(void)
{
	//-----------------------------------------------------------------------
	// 변수명은 _Window_CX이지만 _imageViewportCX으로 해석 해야 함
	// _Window_CX == _ImageViewport_CX
	// _Window_CY == _ImageViewport_CY


	//-----------------------------------------------------------------------
	_ImageViewport_X = static_cast<std::int64_t>(_DocumentViewport_X * _Scale);
	if (_Image_CX < _Window_CX)
	{
		_ImageViewport_X = 0;
	}
	else
	{
		if (_Image_CX < (_ImageViewport_X + _Window_CX))
		{
			_ImageViewport_X = _Image_CX - _Window_CX;
		}
	}


	//-----------------------------------------------------------------------
	_ImageViewport_Y = static_cast<std::int64_t>(_DocumentViewport_Y * _Scale);
	if (_Image_CY < _Window_CY)
	{
		_ImageViewport_Y = 0;
	}
	else
	{
		if (_Image_CY < (_ImageViewport_Y + _Window_CY))
		{
			_ImageViewport_Y = _Image_CY - _Window_CY;
		}
	}


	//-----------------------------------------------------------------------
	_DocumentViewport_X = _ImageViewport_X / _Scale;
	_DocumentViewport_Y = _ImageViewport_Y / _Scale;
}

void Viewport::updateViewScroll(void)
{
	constexpr std::uint64_t _X_Scroll_Line = 20;
	constexpr std::uint64_t _Y_Scroll_Line = 20;


	if (_ScrollbarEnabled)
	{
		if (_Window_CX < _Image_CX)
		{
			_View_X_Scroll_Min  = 0;
			_View_X_Scroll_Max  = _Image_CX;
			_View_X_Scroll_Page = _Window_CX;
			_View_X_Scroll_Line = _X_Scroll_Line;
		}
		else
		{
			_View_X_Scroll_Min  = 0;
			_View_X_Scroll_Max  = 0;
			_View_X_Scroll_Page = 0;
			_View_X_Scroll_Line = 0;
		}


		if (_Window_CY < _Image_CY)
		{
			_View_Y_Scroll_Min  = 0;
			_View_Y_Scroll_Max  = _Image_CY;
			_View_Y_Scroll_Page = _Window_CY;
			_View_Y_Scroll_Line = _Y_Scroll_Line;
		}
		else
		{
			_View_Y_Scroll_Min  = 0;
			_View_Y_Scroll_Max  = 0;
			_View_Y_Scroll_Page = 0;
			_View_Y_Scroll_Line = 0;
		}
	}
	else
	{
		_View_X_Scroll_Min  = 0;
		_View_X_Scroll_Max  = 0;
		_View_X_Scroll_Page = 0;
		_View_X_Scroll_Line = 0;

		_View_Y_Scroll_Min  = 0;
		_View_Y_Scroll_Max  = 0;
		_View_Y_Scroll_Page = 0;
		_View_Y_Scroll_Line = 0;
	}
}

void Viewport::updateScrollbarPosition(void)
{
	SetScrollInfo64(_WindowHandle, SB_HORZ, SIF_ALL, _View_X_Scroll_Max, _ImageViewport_X, _View_X_Scroll_Page, TRUE);
	SetScrollInfo64(_WindowHandle, SB_VERT, SIF_ALL, _View_Y_Scroll_Max, _ImageViewport_Y, _View_Y_Scroll_Page, TRUE);
}

//===========================================================================
void Viewport::handleVScrollbar(std::uint32_t scrollbarCode)
{
	//-----------------------------------------------------------------------
	std::int64_t pos;


	pos = GetScrollPos64(_WindowHandle, SB_VERT, SIF_TRACKPOS, _View_Y_Scroll_Max);


	//-----------------------------------------------------------------------
	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_pos;


	view_scroll_pos = _ImageViewport_Y;
	view_scroll_pos_current = scroll(scrollbarCode, pos,
		_View_Y_Scroll_Page,
		_View_Y_Scroll_Line,
		_View_Y_Scroll_Min,
		_View_Y_Scroll_Max,
		_ImageViewport_Y
	);


	//-----------------------------------------------------------------------
	if (view_scroll_pos != view_scroll_pos_current)
	{
		_ImageViewport_Y = view_scroll_pos_current;
		_DocumentViewport_Y = view_scroll_pos_current / _Scale;

		updateViewport();

		updateScrollbarPosition();

		repaint();
	}
}

void Viewport::handleHScrollbar(std::uint32_t scrollbarCode)
{
	//-----------------------------------------------------------------------
	std::int64_t pos;


	pos = GetScrollPos64(_WindowHandle, SB_HORZ, SIF_TRACKPOS, _View_X_Scroll_Max);


	//-----------------------------------------------------------------------
	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_pos;


	view_scroll_pos = _ImageViewport_X;
	view_scroll_pos_current = scroll(scrollbarCode, pos,
		_View_X_Scroll_Page,
		_View_X_Scroll_Line,
		_View_X_Scroll_Min,
		_View_X_Scroll_Max,
		_ImageViewport_X
	);


	//-----------------------------------------------------------------------
	if (view_scroll_pos != view_scroll_pos_current)
	{
		_ImageViewport_X = view_scroll_pos_current;
		_DocumentViewport_X = view_scroll_pos_current / _Scale;

		updateViewport();

		updateScrollbarPosition();

		repaint();
	}
}

//===========================================================================
std::int64_t Viewport::scroll(
	std::uint32_t scrollbarCode,
	std::int64_t  scrollbarPos,
	std::uint64_t scroll_page,
	std::uint64_t scroll_line,
	std::uint64_t scroll_min,
	std::uint64_t scroll_max,
	std::uint64_t scroll_pos
)
{
	std::int64_t view_scroll_min;
	std::int64_t view_scroll_max;
	std::int64_t view_scroll_top;
	std::int64_t view_scroll_bottom;
	std::int64_t view_scroll_pos;
	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_page;
	std::int64_t view_scroll_line;


	//-----------------------------------------------------------------------
	view_scroll_page = scroll_page;
	view_scroll_line = scroll_line;


	view_scroll_min = scroll_min;
	view_scroll_max = scroll_max;


	view_scroll_top = view_scroll_min;
	if (view_scroll_page < view_scroll_max)
	{
		view_scroll_bottom = view_scroll_max - view_scroll_page;
	}
	else
	{
		view_scroll_bottom = view_scroll_min;
	}


	view_scroll_pos = scroll_pos;
	view_scroll_pos_current = scroll_pos;


	//-----------------------------------------------------------------------
	switch (scrollbarCode)
	{
	case SB_TOP:
		view_scroll_pos_current = view_scroll_top;
		break;

	case SB_BOTTOM:
		view_scroll_pos_current = view_scroll_bottom;
		break;

	case SB_LINEUP:
		if (view_scroll_top < (view_scroll_pos - view_scroll_line))
		{
			view_scroll_pos_current = view_scroll_pos - view_scroll_line;
		}
		else
		{
			view_scroll_pos_current = view_scroll_top;
		}
		break;

	case SB_LINEDOWN:
		if ((view_scroll_pos + view_scroll_line) < view_scroll_bottom)
		{
			view_scroll_pos_current = view_scroll_pos + view_scroll_line;
		}
		else
		{
			view_scroll_pos_current = view_scroll_bottom;
		}
		break;

	case SB_PAGEUP:
		if (view_scroll_top < (view_scroll_pos - view_scroll_page))
		{
			view_scroll_pos_current = view_scroll_pos - view_scroll_page;
		}
		else
		{
			view_scroll_pos_current = view_scroll_top;
		}
		break;

	case SB_PAGEDOWN:
		if ((view_scroll_pos + view_scroll_page) < view_scroll_bottom)
		{
			view_scroll_pos_current = view_scroll_pos + view_scroll_page;
		}
		else
		{
			view_scroll_pos_current = view_scroll_bottom;
		}
		break;

	case SB_THUMBTRACK:
		view_scroll_pos_current = scrollbarPos;
		break;

	case SB_THUMBPOSITION:
		view_scroll_pos_current = scrollbarPos;
		break;

	case SB_ENDSCROLL:
		break;
	}


	//-----------------------------------------------------------------------
	if (view_scroll_pos_current < view_scroll_top)
	{
		view_scroll_pos_current = view_scroll_top;
	}

	if (view_scroll_bottom < view_scroll_pos_current)
	{
		view_scroll_pos_current = view_scroll_bottom;
	}


	return view_scroll_pos_current;
}

void Viewport::enableScrollbar(bool enable)
{
	_ScrollbarEnabled = enable;

	updateViewScroll();
	updateScrollbarPosition();
}

//===========================================================================
void Viewport::fitDocumentToWindow(bool vertical)
{
	if (vertical)
	{
		if (_Window_CY)
		{
			setScale(_Window_CY / _Document_CY);
		}
	}
	else
	{
		if (_Window_CX)
		{
			setScale(_Window_CX / _Document_CX);
		}
	}
}

void Viewport::zoom(bool zoom_in)
{
	constexpr double _Scale_Max   = 10.0f;
	constexpr double _Scale_Min   = 0.1f;
	constexpr double _Scale_Delta = 0.1f;


	double scale;
	int scale_x10;


	getScale(scale);

	scale_x10 = static_cast<int>((scale + 0.05) * 10);
	scale = scale_x10 / 10.0;


	if (zoom_in)
	{
		scale = scale + _Scale_Delta;
	}
	else
	{
		scale = scale - _Scale_Delta;
	}


	if (scale > _Scale_Max)
	{
		scale = _Scale_Max;
	}
	if (scale < _Scale_Min)
	{
		scale = _Scale_Min;
	}


	setScale(scale);
}

void Viewport::getDocumentViewportPointTranslation(double& x, double& y)
{
	x = -_DocumentViewport_X;
	y = -_DocumentViewport_Y;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
CenterViewport::CenterViewport(HWND hwnd, RepaintHandler repaintHandler, ResizeHandler resizeHandler) :
	Viewport{ hwnd, repaintHandler, resizeHandler }
{
}

//===========================================================================
void CenterViewport::WindowToDocument(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y)
{
	if (_Image_CX < _Window_CX)
	{
		document_x = (window_x / _Scale) - ((_Window_CX - _Image_CX) / _Scale / 2);
	}
	else
	{
		document_x = _DocumentViewport_X + (window_x / _Scale);
	}

	if (_Image_CY < _Window_CY)
	{
		document_y = (window_y / _Scale) - ((_Window_CY - _Image_CY) / _Scale / 2);
	}
	else
	{
		document_y = _DocumentViewport_Y + (window_y / _Scale);
	}
}

void CenterViewport::DocumentToWindow(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y)
{
	if (_Image_CX < _Window_CX)
	{
		window_x = static_cast<std::int64_t>(((_Window_CX - _Image_CX) / 2) + (document_x * _Scale));
	}
	else
	{
		window_x = static_cast<std::int64_t>((document_x - _DocumentViewport_X) * _Scale);
	}

	if (_Image_CY < _Window_CY)
	{
		window_y = static_cast<std::int64_t>(((_Window_CY - _Image_CY) / 2) + (document_y * _Scale));
	}
	else
	{
		window_y = static_cast<std::int64_t>((document_y - _DocumentViewport_Y) * _Scale);
	}
}

void CenterViewport::updateViewport(void)
{
	//-----------------------------------------------------------------------
	_ImageViewport_X = static_cast<std::int64_t>(_DocumentViewport_X * _Scale);
	if (_Image_CX < _Window_CX)
	{
		_ImageViewport_X = (_Window_CX - _Image_CX) / 2;
	}
	else
	{
		if (_Image_CX < (_ImageViewport_X + _Window_CX))
		{
			_ImageViewport_X = _Image_CX - _Window_CX;
		}
	}


	//-----------------------------------------------------------------------
	_ImageViewport_Y = static_cast<std::int64_t>(_DocumentViewport_Y * _Scale);
	if (_Image_CY < _Window_CY)
	{
		_ImageViewport_Y = (_Window_CY - _Image_CY) / 2;
	}
	else
	{
		if (_Image_CY < (_ImageViewport_Y + _Window_CY))
		{
			_ImageViewport_Y = _Image_CY - _Window_CY;
		}
	}


	//-----------------------------------------------------------------------
	_DocumentViewport_X = _ImageViewport_X / _Scale;
	_DocumentViewport_Y = _ImageViewport_Y / _Scale;
}

void CenterViewport::getDocumentViewportPointTranslation(double& x, double& y)
{
	if (_Image_CX < _Window_CX)
	{
		x = +_DocumentViewport_X;
	}
	else
	{
		x = -_DocumentViewport_X;
	}

	if (_Image_CY < _Window_CY)
	{
		y = +_DocumentViewport_Y;
	}
	else
	{
		y = -_DocumentViewport_Y;
	}
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




