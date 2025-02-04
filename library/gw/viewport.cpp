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
viewport::viewport(HWND hwnd, repainter r) :
	_hwnd{ hwnd },
	_repainter{ r }
{
}

//===========================================================================
HWND viewport::get_hwnd(void)
{
	return _hwnd;
}

void viewport::repaint(void)
{
	_repainter();
}

//===========================================================================
void viewport::get_window_size(std::int64_t& cx, std::int64_t& cy)
{
	cx = _window_cx;
	cy = _window_cy;
}

void viewport::get_scale(double& scale)
{
	scale = _scale;
}

void viewport::get_document_size(double& cx, double& cy)
{
	cx = _document_cx;
	cy = _document_cy;
}

void viewport::get_document_viewport_point(double& x, double& y)
{
	x = _document_viewport_x;
	y = _document_viewport_y;
}

void viewport::get_document_viewport_size(double& cx, double& cy)
{
	cx = _window_cx / _scale;
	cy = _window_cy / _scale;
}

void viewport::get_image_size(std::int64_t& cx, std::int64_t& cy)
{
	cx = _image_cx;
	cy = _image_cy;
}

void viewport::get_image_viewport_point(std::int64_t& x, std::int64_t& y)
{
	x = _image_viewport_x;
	y = _image_viewport_y;
}

void viewport::get_image_viewport_size(std::int64_t& cx, std::int64_t& cy)
{
	cx = _window_cx;
	cy = _window_cy;
}

//===========================================================================
void viewport::window_to_document(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y)
{
	document_x = _document_viewport_x + (window_x / _scale);
	document_y = _document_viewport_y + (window_y / _scale);
}

void viewport::document_to_window(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y)
{
	window_x = static_cast<std::int64_t>((document_x - _document_viewport_x) * _scale);
	window_y = static_cast<std::int64_t>((document_y - _document_viewport_y) * _scale);
}

//===========================================================================
void viewport::set_window_size(std::int64_t cx, std::int64_t cy)
{
	_window_cx = cx;
	_window_cy = cy;


	update_image_size();
	update_viewport();
	update_view_scroll();
	update_scrollbar_position();
}

void viewport::set_scale(double scale)
{
	if (scale > 0.0)
	{
		_scale = scale;

		update_image_size();
		update_viewport();
		update_view_scroll();
		update_scrollbar_position();

		repaint();
	}
}

void viewport::set_document_size(double cx, double cy)
{
	_document_viewport_x = 0.0;
	_document_viewport_y = 0.0;
	_document_cx = cx;
	_document_cy = cy;

	update_image_size();
	update_viewport();
	update_view_scroll();
	update_scrollbar_position();

	repaint();
}

//===========================================================================
void viewport::update_image_size(void)
{
	_image_cx = static_cast<std::int64_t>(_document_cx * _scale);
	_image_cy = static_cast<std::int64_t>(_document_cy * _scale);
}

void viewport::update_viewport(void)
{
	//-----------------------------------------------------------------------
	// 변수명은 _window_cx이지만 _image_viewport_cx으로 해석 해야 함
	// _window_cx == _image_viewport_cx
	// _window_cy == _image_viewport_cy


	//-----------------------------------------------------------------------
	_image_viewport_x = static_cast<std::int64_t>(_document_viewport_x * _scale);
	if (_image_cx < _window_cx)
	{
		_image_viewport_x = 0;
	}
	else
	{
		if (_image_cx < (_image_viewport_x + _window_cx))
		{
			_image_viewport_x = _image_cx - _window_cx;
		}
	}


	//-----------------------------------------------------------------------
	_image_viewport_y = static_cast<std::int64_t>(_document_viewport_y * _scale);
	if (_image_cy < _window_cy)
	{
		_image_viewport_y = 0;
	}
	else
	{
		if (_image_cy < (_image_viewport_y + _window_cy))
		{
			_image_viewport_y = _image_cy - _window_cy;
		}
	}


	//-----------------------------------------------------------------------
	_document_viewport_x = _image_viewport_x / _scale;
	_document_viewport_y = _image_viewport_y / _scale;
}

void viewport::update_view_scroll(void)
{
	constexpr std::uint64_t _scrollbar_x_line_size = 20;
	constexpr std::uint64_t _scrollbar_y_line_size = 20;


	if (_scrollbar_enabled)
	{
		if (_window_cx < _image_cx)
		{
			_view_x_scroll_min = 0;
			_view_x_scroll_max = _image_cx;
			_view_x_scroll_page = _window_cx;
			_view_x_scroll_line = _scrollbar_x_line_size;
		}
		else
		{
			_view_x_scroll_min = 0;
			_view_x_scroll_max = 0;
			_view_x_scroll_page = 0;
			_view_x_scroll_line = 0;
		}


		if (_window_cy < _image_cy)
		{
			_view_y_scroll_min = 0;
			_view_y_scroll_max = _image_cy;
			_view_y_scroll_page = _window_cy;
			_view_y_scroll_line = _scrollbar_y_line_size;
		}
		else
		{
			_view_y_scroll_min = 0;
			_view_y_scroll_max = 0;
			_view_y_scroll_page = 0;
			_view_y_scroll_line = 0;
		}
	}
	else
	{
		_view_x_scroll_min = 0;
		_view_x_scroll_max = 0;
		_view_x_scroll_page = 0;
		_view_x_scroll_line = 0;

		_view_y_scroll_min = 0;
		_view_y_scroll_max = 0;
		_view_y_scroll_page = 0;
		_view_y_scroll_line = 0;
	}
}

void viewport::update_scrollbar_position(void)
{
	SetScrollInfo64(_hwnd, SB_HORZ, SIF_ALL, _view_x_scroll_max, _image_viewport_x, _view_x_scroll_page, TRUE);
	SetScrollInfo64(_hwnd, SB_VERT, SIF_ALL, _view_y_scroll_max, _image_viewport_y, _view_y_scroll_page, TRUE);
}

//===========================================================================
void viewport::vscroll(std::uint32_t scroll_code)
{
	//-----------------------------------------------------------------------
	std::int64_t pos;


	pos = GetScrollPos64(_hwnd, SB_VERT, SIF_TRACKPOS, _view_y_scroll_max);


	//-----------------------------------------------------------------------
	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_pos;


	view_scroll_pos = _image_viewport_y;
	view_scroll_pos_current = scroll(scroll_code, pos,
		_view_y_scroll_page,
		_view_y_scroll_line,
		_view_y_scroll_min,
		_view_y_scroll_max,
		_image_viewport_y
	);


	//-----------------------------------------------------------------------
	if (view_scroll_pos != view_scroll_pos_current)
	{
		_image_viewport_y = view_scroll_pos_current;
		_document_viewport_y = view_scroll_pos_current / _scale;

		update_viewport();

		update_scrollbar_position();

		repaint();
	}
}

void viewport::hscroll(std::uint32_t scroll_code)
{
	//-----------------------------------------------------------------------
	std::int64_t pos;


	pos = GetScrollPos64(_hwnd, SB_HORZ, SIF_TRACKPOS, _view_x_scroll_max);


	//-----------------------------------------------------------------------
	std::int64_t view_scroll_pos_current;
	std::int64_t view_scroll_pos;


	view_scroll_pos = _image_viewport_x;
	view_scroll_pos_current = scroll(scroll_code, pos,
		_view_x_scroll_page,
		_view_x_scroll_line,
		_view_x_scroll_min,
		_view_x_scroll_max,
		_image_viewport_x
	);


	//-----------------------------------------------------------------------
	if (view_scroll_pos != view_scroll_pos_current)
	{
		_image_viewport_x = view_scroll_pos_current;
		_document_viewport_x = view_scroll_pos_current / _scale;

		update_viewport();

		update_scrollbar_position();

		repaint();
	}
}

//===========================================================================
std::int64_t viewport::scroll(
	std::uint32_t scroll_code, std::int64_t scroll_pos,
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

void viewport::enable_scrollbar(bool enable)
{
	_scrollbar_enabled = enable;

	update_view_scroll();
	update_scrollbar_position();
}

//===========================================================================
void viewport::fit_document_to_window(bool vert)
{
	if (vert)
	{
		if (_window_cy)
		{
			set_scale(_window_cy / _document_cy);
		}
	}
	else
	{
		if (_window_cx)
		{
			set_scale(_window_cx / _document_cx);
		}
	}
}

void viewport::zoom(bool zoom_in)
{
	constexpr double scale_max   = 10.0f;
	constexpr double scale_min   = 0.1f;
	constexpr double scale_delta = 0.1f;


	double scale;
	int scale_x10;


	get_scale(scale);

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


	set_scale(scale);
}

void viewport::get_document_viewport_point_translation(double& x, double& y)
{
	x = -_document_viewport_x;
	y = -_document_viewport_y;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
center_viewport::center_viewport(HWND hwnd, repainter r) :
	viewport{ hwnd, r }
{
}

//===========================================================================
void center_viewport::window_to_document(std::int64_t window_x, std::int64_t window_y, double& document_x, double& document_y)
{
	if (_image_cx < _window_cx)
	{
		document_x = (window_x / _scale) - ((_window_cx - _image_cx) / _scale / 2);
	}
	else
	{
		document_x = _document_viewport_x + (window_x / _scale);
	}

	if (_image_cy < _window_cy)
	{
		document_y = (window_y / _scale) - ((_window_cy - _image_cy) / _scale / 2);
	}
	else
	{
		document_y = _document_viewport_y + (window_y / _scale);
	}
}

void center_viewport::document_to_window(double document_x, double document_y, std::int64_t& window_x, std::int64_t& window_y)
{
	if (_image_cx < _window_cx)
	{
		window_x = static_cast<std::int64_t>(((_window_cx - _image_cx) / 2) + (document_x * _scale));
	}
	else
	{
		window_x = static_cast<std::int64_t>((document_x - _document_viewport_x) * _scale);
	}

	if (_image_cy < _window_cy)
	{
		window_y = static_cast<std::int64_t>(((_window_cy - _image_cy) / 2) + (document_y * _scale));
	}
	else
	{
		window_y = static_cast<std::int64_t>((document_y - _document_viewport_y) * _scale);
	}
}

void center_viewport::update_viewport(void)
{
	//-----------------------------------------------------------------------
	// 변수명은 _window_cx이지만 _image_viewport_cx으로 해석 해야 함
	// _window_cx == _image_viewport_cx
	// _window_cy == _image_viewport_cy


	//-----------------------------------------------------------------------
	_image_viewport_x = static_cast<std::int64_t>(_document_viewport_x * _scale);
	if (_image_cx < _window_cx)
	{
		_image_viewport_x = (_window_cx - _image_cx) / 2;
	}
	else
	{
		if (_image_cx < (_image_viewport_x + _window_cx))
		{
			_image_viewport_x = _image_cx - _window_cx;
		}
	}


	//-----------------------------------------------------------------------
	_image_viewport_y = static_cast<std::int64_t>(_document_viewport_y * _scale);
	if (_image_cy < _window_cy)
	{
		_image_viewport_y = (_window_cy - _image_cy) / 2;
	}
	else
	{
		if (_image_cy < (_image_viewport_y + _window_cy))
		{
			_image_viewport_y = _image_cy - _window_cy;
		}
	}


	//-----------------------------------------------------------------------
	_document_viewport_x = _image_viewport_x / _scale;
	_document_viewport_y = _image_viewport_y / _scale;
}

void center_viewport::get_document_viewport_point_translation(double& x, double& y)
{
	if (_image_cx < _window_cx)
	{
		x = +_document_viewport_x;
	}
	else
	{
		x = -_document_viewport_x;
	}

	if (_image_cy < _window_cy)
	{
		y = +_document_viewport_y;
	}
	else
	{
		y = -_document_viewport_y;
	}
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




