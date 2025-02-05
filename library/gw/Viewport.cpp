/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace gw
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
BOOL SetScrollInfo64(HWND hwnd,
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

UINT64 GetScrollPos64(HWND hwnd,
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
	cx = _WindowCX;
	cy = _WindowCY;
}

void Viewport::getScale(double& scale)
{
	scale = _Scale;
}

void Viewport::getDocumentSize(double& cx, double& cy)
{
	cx = _DocumentCX;
	cy = _DocumentCY;
}

void Viewport::getDocumentViewportPoint(double& x, double& y)
{
	x = _DocumentViewportX;
	y = _DocumentViewportY;
}

void Viewport::getDocumentViewportSize(double& cx, double& cy)
{
	cx = _WindowCX / _Scale;
	cy = _WindowCY / _Scale;
}

void Viewport::getImageSize(std::int64_t& cx, std::int64_t& cy)
{
	cx = _ImageCX;
	cy = _ImageCY;
}

void Viewport::getImageViewportPoint(std::int64_t& x, std::int64_t& y)
{
	x = _ImageViewportX;
	y = _ImageViewportY;
}

void Viewport::getImageViewportSize(std::int64_t& cx, std::int64_t& cy)
{
	cx = _WindowCX;
	cy = _WindowCY;
}

//===========================================================================
void Viewport::WindowToDocument(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y)
{
	document_x = _DocumentViewportX + (window_x / _Scale);
	document_y = _DocumentViewportY + (window_y / _Scale);
}

void Viewport::DocumentToWindow(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y)
{
	window_x = static_cast<std::int64_t>((document_x - _DocumentViewportX) * _Scale);
	window_y = static_cast<std::int64_t>((document_y - _DocumentViewportY) * _Scale);
}

//===========================================================================
void Viewport::setWindowSize(std::int64_t cx, std::int64_t cy)
{
	resize(cx, cy);


	_WindowCX = cx;
	_WindowCY = cy;


	updateImageSize();
	updateViewport();
	updateViewScroll();
	updateScrollbarPosition();
}

void Viewport::setScale(double scale)
{
	if (scale > 0.0)
	{
		_Scale = scale;

		updateImageSize();
		updateViewport();
		updateViewScroll();
		updateScrollbarPosition();

		repaint();
	}
}

void Viewport::setDocumentSize(double cx, double cy)
{
	_DocumentViewportX = 0.0;
	_DocumentViewportY = 0.0;
	_DocumentCX = cx;
	_DocumentCY = cy;

	updateImageSize();
	updateViewport();
	updateViewScroll();
	updateScrollbarPosition();

	repaint();
}

//===========================================================================
void Viewport::updateImageSize(void)
{
	_ImageCX = static_cast<std::int64_t>(_DocumentCX * _Scale);
	_ImageCY = static_cast<std::int64_t>(_DocumentCY * _Scale);
}

void Viewport::updateViewport(void)
{
	//-----------------------------------------------------------------------
	// 변수명은 _WindowCX이지만 _imageViewportCX으로 해석 해야 함
	// _WindowCX == _imageViewportCX
	// _WindowCY == _imageViewportCY


	//-----------------------------------------------------------------------
	_ImageViewportX = static_cast<std::int64_t>(_DocumentViewportX * _Scale);
	if (_ImageCX < _WindowCX)
	{
		_ImageViewportX = 0;
	}
	else
	{
		if (_ImageCX < (_ImageViewportX + _WindowCX))
		{
			_ImageViewportX = _ImageCX - _WindowCX;
		}
	}


	//-----------------------------------------------------------------------
	_ImageViewportY = static_cast<std::int64_t>(_DocumentViewportY * _Scale);
	if (_ImageCY < _WindowCY)
	{
		_ImageViewportY = 0;
	}
	else
	{
		if (_ImageCY < (_ImageViewportY + _WindowCY))
		{
			_ImageViewportY = _ImageCY - _WindowCY;
		}
	}


	//-----------------------------------------------------------------------
	_DocumentViewportX = _ImageViewportX / _Scale;
	_DocumentViewportY = _ImageViewportY / _Scale;
}

void Viewport::updateViewScroll(void)
{
	constexpr std::uint64_t _Scrollbar_XLine = 20;
	constexpr std::uint64_t _Scrollbar_YLine = 20;


	if (_ScrollbarEnabled)
	{
		if (_WindowCX < _ImageCX)
		{
			_ViewXScrollMin = 0;
			_ViewXScrollMax = _ImageCX;
			_ViewXScrollPage = _WindowCX;
			_ViewXScrollLine = _Scrollbar_XLine;
		}
		else
		{
			_ViewXScrollMin = 0;
			_ViewXScrollMax = 0;
			_ViewXScrollPage = 0;
			_ViewXScrollLine = 0;
		}


		if (_WindowCY < _ImageCY)
		{
			_ViewYScrollMin = 0;
			_ViewYScrollMax = _ImageCY;
			_ViewYScrollPage = _WindowCY;
			_ViewYScrollLine = _Scrollbar_YLine;
		}
		else
		{
			_ViewYScrollMin = 0;
			_ViewYScrollMax = 0;
			_ViewYScrollPage = 0;
			_ViewYScrollLine = 0;
		}
	}
	else
	{
		_ViewXScrollMin = 0;
		_ViewXScrollMax = 0;
		_ViewXScrollPage = 0;
		_ViewXScrollLine = 0;

		_ViewYScrollMin = 0;
		_ViewYScrollMax = 0;
		_ViewYScrollPage = 0;
		_ViewYScrollLine = 0;
	}
}

void Viewport::updateScrollbarPosition(void)
{
	SetScrollInfo64(_WindowHandle, SB_HORZ, SIF_ALL, _ViewXScrollMax, _ImageViewportX, _ViewXScrollPage, TRUE);
	SetScrollInfo64(_WindowHandle, SB_VERT, SIF_ALL, _ViewYScrollMax, _ImageViewportY, _ViewYScrollPage, TRUE);
}

//===========================================================================
void Viewport::vscroll(std::uint32_t scroll_code)
{
	//-----------------------------------------------------------------------
	std::int64_t pos;


	pos = GetScrollPos64(_WindowHandle, SB_VERT, SIF_TRACKPOS, _ViewYScrollMax);


	//-----------------------------------------------------------------------
	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_pos;


	view_scroll_pos = _ImageViewportY;
	view_scroll_pos_current = scroll(scroll_code, pos,
		_ViewYScrollPage,
		_ViewYScrollLine,
		_ViewYScrollMin,
		_ViewYScrollMax,
		_ImageViewportY
	);


	//-----------------------------------------------------------------------
	if (view_scroll_pos != view_scroll_pos_current)
	{
		_ImageViewportY = view_scroll_pos_current;
		_DocumentViewportY = view_scroll_pos_current / _Scale;

		updateViewport();

		updateScrollbarPosition();

		repaint();
	}
}

void Viewport::hscroll(std::uint32_t scroll_code)
{
	//-----------------------------------------------------------------------
	std::int64_t pos;


	pos = GetScrollPos64(_WindowHandle, SB_HORZ, SIF_TRACKPOS, _ViewXScrollMax);


	//-----------------------------------------------------------------------
	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_pos;


	view_scroll_pos = _ImageViewportX;
	view_scroll_pos_current = scroll(scroll_code, pos,
		_ViewXScrollPage,
		_ViewXScrollLine,
		_ViewXScrollMin,
		_ViewXScrollMax,
		_ImageViewportX
	);


	//-----------------------------------------------------------------------
	if (view_scroll_pos != view_scroll_pos_current)
	{
		_ImageViewportX = view_scroll_pos_current;
		_DocumentViewportX = view_scroll_pos_current / _Scale;

		updateViewport();

		updateScrollbarPosition();

		repaint();
	}
}

//===========================================================================
std::int64_t Viewport::scroll(
	std::uint32_t scroll_code, 
	std::int64_t  scroll_pos,
	std::uint64_t a_view_scroll_page,
	std::uint64_t a_view_scroll_line,
	std::uint64_t a_view_scroll_min,
	std::uint64_t a_view_scroll_max,
	std::uint64_t a_view_scroll_pos
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
	view_scroll_page = a_view_scroll_page;
	view_scroll_line = a_view_scroll_line;


	view_scroll_min = a_view_scroll_min;
	view_scroll_max = a_view_scroll_max;


	view_scroll_top = view_scroll_min;
	if (view_scroll_page < view_scroll_max)
	{
		view_scroll_bottom = view_scroll_max - view_scroll_page;
	}
	else
	{
		view_scroll_bottom = view_scroll_min;
	}


	view_scroll_pos = a_view_scroll_pos;
	view_scroll_pos_current = a_view_scroll_pos;


	//-----------------------------------------------------------------------
	switch (scroll_code)
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
		view_scroll_pos_current = scroll_pos;
		break;

	case SB_THUMBPOSITION:
		view_scroll_pos_current = scroll_pos;
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
void Viewport::fitDocumentToWindow(bool vert)
{
	if (vert)
	{
		if (_WindowCY)
		{
			setScale(_WindowCY / _DocumentCY);
		}
	}
	else
	{
		if (_WindowCX)
		{
			setScale(_WindowCX / _DocumentCX);
		}
	}
}

void Viewport::zoom(bool zoom_in)
{
	constexpr double scale_max   = 10.0f;
	constexpr double scale_min   = 0.1f;
	constexpr double scale_delta = 0.1f;


	double scale;
	int scale_x10;


	getScale(scale);

	scale_x10 = static_cast<int>((scale + 0.05) * 10);
	scale = scale_x10 / 10.0;


	if (zoom_in)
	{
		scale = scale + scale_delta;
	}
	else
	{
		scale = scale - scale_delta;
	}


	if (scale > scale_max)
	{
		scale = scale_max;
	}
	if (scale < scale_min)
	{
		scale = scale_min;
	}


	setScale(scale);
}

void Viewport::getDocumentViewportPointTranslation(double& x, double& y)
{
	x = -_DocumentViewportX;
	y = -_DocumentViewportY;
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
	if (_ImageCX < _WindowCX)
	{
		document_x = (window_x / _Scale) - ((_WindowCX - _ImageCX) / _Scale / 2);
	}
	else
	{
		document_x = _DocumentViewportX + (window_x / _Scale);
	}

	if (_ImageCY < _WindowCY)
	{
		document_y = (window_y / _Scale) - ((_WindowCY - _ImageCY) / _Scale / 2);
	}
	else
	{
		document_y = _DocumentViewportY + (window_y / _Scale);
	}
}

void CenterViewport::DocumentToWindow(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y)
{
	if (_ImageCX < _WindowCX)
	{
		window_x = static_cast<std::int64_t>(((_WindowCX - _ImageCX) / 2) + (document_x * _Scale));
	}
	else
	{
		window_x = static_cast<std::int64_t>((document_x - _DocumentViewportX) * _Scale);
	}

	if (_ImageCY < _WindowCY)
	{
		window_y = static_cast<std::int64_t>(((_WindowCY - _ImageCY) / 2) + (document_y * _Scale));
	}
	else
	{
		window_y = static_cast<std::int64_t>((document_y - _DocumentViewportY) * _Scale);
	}
}

void CenterViewport::updateViewport(void)
{
	//-----------------------------------------------------------------------
	_ImageViewportX = static_cast<std::int64_t>(_DocumentViewportX * _Scale);
	if (_ImageCX < _WindowCX)
	{
		_ImageViewportX = (_WindowCX - _ImageCX) / 2;
	}
	else
	{
		if (_ImageCX < (_ImageViewportX + _WindowCX))
		{
			_ImageViewportX = _ImageCX - _WindowCX;
		}
	}


	//-----------------------------------------------------------------------
	_ImageViewportY = static_cast<std::int64_t>(_DocumentViewportY * _Scale);
	if (_ImageCY < _WindowCY)
	{
		_ImageViewportY = (_WindowCY - _ImageCY) / 2;
	}
	else
	{
		if (_ImageCY < (_ImageViewportY + _WindowCY))
		{
			_ImageViewportY = _ImageCY - _WindowCY;
		}
	}


	//-----------------------------------------------------------------------
	_DocumentViewportX = _ImageViewportX / _Scale;
	_DocumentViewportY = _ImageViewportY / _Scale;
}

void CenterViewport::getDocumentViewportPointTranslation(double& x, double& y)
{
	if (_ImageCX < _WindowCX)
	{
		x = +_DocumentViewportX;
	}
	else
	{
		x = -_DocumentViewportX;
	}

	if (_ImageCY < _WindowCY)
	{
		y = +_DocumentViewportY;
	}
	else
	{
		y = -_DocumentViewportY;
	}
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




