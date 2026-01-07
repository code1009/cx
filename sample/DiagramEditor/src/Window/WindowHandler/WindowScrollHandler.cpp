/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include "WindowScrollHandler.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if  0
From:
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

@    0 <= nPage <  nMax - nMin
@ nMin <= nPos  <= nMax - nPage

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 0

From:
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
	int    nBar,
	UINT   fMask,
	UINT64 nMax64,
	UINT64 nPos64,
	UINT64 nPage,
	BOOL   fRedraw
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
	int    nBar,
	UINT   fMask,
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
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WindowScrollHandler::WindowScrollHandler(HWND hwnd) :
	_Hwnd(hwnd)
{
}

//===========================================================================
bool WindowScrollHandler::setXSize(std::int64_t size)
{
	bool changed = false;
	std::int64_t newSize = _XSize;

	
	if (size < 0)
	{
		size = 0;
	}


	newSize = size;


	if (_XSize != newSize)
	{
		_XSize = newSize;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setXPage(std::int64_t page)
{
	bool changed = false;
	std::int64_t newPage = _XPage;


	if (page < 0)
	{
		page = 0;
	}


	// 0 <= nPage < nMax - nMin
	if (page < _XSize)
	{
		newPage = page;
	}
	else
	{
		newPage = 0;
	}


	if (_XPage != newPage)
	{
		_XPage = newPage;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setXPos(std::int64_t pos)
{
	bool changed = false;
	std::int64_t newPos = _XPos;


	if (pos < 0)
	{
		pos = 0;
	}


	// nMin <= nPos <= nMax - nPage
	if ((0 < _XPage) && (pos <= (_XSize - _XPage)))
	{
		newPos = pos;
	}
	else
	{
		newPos = 0;
	}


	if (_XPos != newPos)
	{
		_XPos = newPos;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setXLine(std::int64_t line)
{
	bool changed = false;
	std::int64_t newLine = _XLine;
	
	
	if (line < 1)
	{
		line = 1;
	}

	
	newLine = line;
	
	
	if (_XLine != newLine)
	{
		_XLine = newLine;
		changed = true;
	}
	return changed;
}

//===========================================================================
bool WindowScrollHandler::setYSize(std::int64_t size)
{
	bool changed = false;
	std::int64_t newSize = _YSize;

	if (size < 0)
	{
		size = 0;
	}


	newSize = size;


	if (_YSize != newSize)
	{
		_YSize = newSize;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setYPage(std::int64_t page)
{
	bool changed = false;
	std::int64_t newPage = _YPage;

	if (page < 0)
	{
		page = 0;
	}


	// 0 <= nPage < nMax - nMin
	if (page < _YSize)
	{
		newPage = page;
	}
	else
	{
		newPage = 0;
	}


	if (_YPage != newPage)
	{
		_YPage = newPage;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setYPos(std::int64_t pos)
{
	bool changed = false;
	std::int64_t newPos = _YPos;

	if (pos < 0)
	{
		pos = 0;
	}


	// nMin <= nPos <= nMax - nPage
	if ((0 < _YPage) && (pos <= (_YSize - _YPage)))
	{
		newPos = pos;
	}
	else
	{
		newPos = 0;
	}


	if (_YPos != newPos)
	{
		_YPos = newPos;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setYLine(std::int64_t line)
{
	bool changed = false;
	std::int64_t newLine = _YLine;


	if (line < 1)
	{
		line = 1;
	}


	newLine = line;


	if (_YLine != newLine)
	{
		_YLine = newLine;
		changed = true;
	}
	return changed;
}

//===========================================================================
void WindowScrollHandler::setXYScrollLine(std::int64_t xline, std::int64_t yline)
{
	bool changed = false;
	changed |= setXLine(xline);
	changed |= setYLine(yline);
	if (changed)
	{
		CX_RUNTIME_LOG(cxLTrace)
			<< L"Scroll Line: "
			<< std::format(L"{}, {}",
				_XLine, _YLine
			);
	}
}

//===========================================================================
void WindowScrollHandler::setXScroll(std::int64_t size, std::int64_t page, std::int64_t pos)
{
	bool changed = false;

	changed |= setXSize(size);
	changed |= setXPage(page);
	changed |= setXPos(pos);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged(false);
	}
}

void WindowScrollHandler::setXScroll(std::int64_t size, std::int64_t page)
{
	bool changed = false;

	changed |= setXSize(size);
	changed |= setXPage(page);
	changed |= setXPos(_XPos);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged(false);
	}
}

//===========================================================================
void WindowScrollHandler::setYScroll(std::int64_t size, std::int64_t page, std::int64_t pos)
{
	bool changed = false;

	changed |= setYSize(size);
	changed |= setYPage(page);
	changed |= setYPos(pos);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged(false);
	}
}

void WindowScrollHandler::setYScroll(std::int64_t size, std::int64_t page)
{
	bool changed = false;

	changed |= setYSize(size);
	changed |= setYPage(page);
	changed |= setYPos(_YPos);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged(false);
	}
}

//===========================================================================
void WindowScrollHandler::setXYScroll(
	std::int64_t xsize, std::int64_t xpage, std::int64_t xpos,
	std::int64_t ysize, std::int64_t ypage, std::int64_t ypos
)
{
	setXScroll(xsize, xpage, xpos);
	setYScroll(ysize, ypage, ypos);
}

void WindowScrollHandler::setXYScroll(
	std::int64_t xsize, std::int64_t xpage,
	std::int64_t ysize, std::int64_t ypage
)
{
	setXScroll(xsize, xpage);
	setYScroll(ysize, ypage);
}

//===========================================================================
void WindowScrollHandler::updateScrollBars(void)
{
	updateXScrollBar();
	updateYScrollBar();
}

void WindowScrollHandler::updateXScrollBar(void)
{
	SetScrollInfo64(_Hwnd, SB_HORZ, SIF_ALL, _XSize, _XPos, _XPage, TRUE);
}

void WindowScrollHandler::updateYScrollBar(void)
{
	SetScrollInfo64(_Hwnd, SB_VERT, SIF_ALL, _YSize, _YPos, _YPage, TRUE);
}

std::int64_t WindowScrollHandler::getXScrollTrackPos(void)
{
	return static_cast<std::int64_t>(GetScrollPos64(_Hwnd, SB_HORZ, SIF_TRACKPOS, _XSize));
}

std::int64_t WindowScrollHandler::getYScrollTrackPos(void)
{
	return static_cast<std::int64_t>(GetScrollPos64(_Hwnd, SB_VERT, SIF_TRACKPOS, _YSize));
}

std::int64_t WindowScrollHandler::getXScrollPos(void)
{
	return static_cast<std::int64_t>(GetScrollPos64(_Hwnd, SB_HORZ, SIF_POS, _XSize));
}

std::int64_t WindowScrollHandler::getYScrollPos(void)
{
	return static_cast<std::int64_t>(GetScrollPos64(_Hwnd, SB_VERT, SIF_POS, _YSize));
}

//===========================================================================
bool WindowScrollHandler::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	switch (windowMessage.uMsg)
	{
	case WM_HSCROLL: return onHScroll(windowMessage);
	case WM_VSCROLL: return onVScroll(windowMessage);
	default:
		break;
	}
	return false;
}

bool WindowScrollHandler::onHScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_HSCROLL_WindowMessageCrack wm{ windowMessage };

	UINT nSBCode = wm.nSBCode();
	//UINT nPos = wm.nPos();
	//HWND wndScrollBar = wm.wndScrollBar();

	switch (nSBCode)
	{
	case SB_TOP          : XScroll_Top          (); break;
	case SB_BOTTOM       : XScroll_Bottom       (); break;
	case SB_LINEUP       : XScroll_LineUp       (); break;
	case SB_LINEDOWN     : XScroll_LineDown     (); break;
	case SB_PAGEUP       : XScroll_PageUp       (); break;
	case SB_PAGEDOWN     : XScroll_PageDown     (); break;
	case SB_THUMBTRACK   : XScroll_ThumbTrack   (); break;
	case SB_THUMBPOSITION: XScroll_ThumbPosition(); break;
	case SB_ENDSCROLL    : XScroll_EndScroll    (); break;
	default:
		break;
	}

	return true;
}

bool WindowScrollHandler::onVScroll(cx::wui::WindowMessage& windowMessage)
{
	cx::wui::WM_VSCROLL_WindowMessageCrack wm{ windowMessage };

	UINT nSBCode = wm.nSBCode();
	//UINT nPos = wm.nPos();
	//HWND wndScrollBar = wm.wndScrollBar();

	switch (nSBCode)
	{
	case SB_TOP          : YScroll_Top          (); break;
	case SB_BOTTOM       : YScroll_Bottom       (); break;
	case SB_LINEUP       : YScroll_LineUp       (); break;
	case SB_LINEDOWN     : YScroll_LineDown     (); break;
	case SB_PAGEUP       : YScroll_PageUp       (); break;
	case SB_PAGEDOWN     : YScroll_PageDown     (); break;
	case SB_THUMBTRACK   : YScroll_ThumbTrack   (); break;
	case SB_THUMBPOSITION: YScroll_ThumbPosition(); break;
	case SB_ENDSCROLL    : YScroll_EndScroll    (); break;
	default:
		break;
	}

	return true;
}

//===========================================================================
void WindowScrollHandler::XScroll_Top(void)
{
	std::int64_t top = 0;


	bool changed = false;
	changed |= setXPos(top);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::XScroll_Bottom(void)
{
	std::int64_t bottom = (_XSize > _XPage) ? _XSize - _XPage : 0;

	bool changed = false;	
	changed |= setXPos(bottom);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::XScroll_LineUp(void)
{
	std::int64_t top = 0;
	std::int64_t pos = getXScrollPos();
	std::int64_t newPos;

	if (pos >= (top + _XLine))
	{
		newPos = pos - _XLine;
	}
	else
	{
		newPos = top;
	}


	bool changed = false;
	changed |= setXPos(newPos);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::XScroll_LineDown(void)
{
	std::int64_t bottom = (_XSize > _XPage) ? _XSize - _XPage : 0;
	std::int64_t pos = getXScrollPos();
	std::int64_t newPos;

	if (pos <= (bottom - _XLine))
	{
		newPos = pos + _XLine;
	}
	else
	{
		newPos = bottom;
	}


	bool changed = false;
	changed |= setXPos(newPos);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::XScroll_PageUp(void)
{
	std::int64_t top = 0;
	std::int64_t pos = getXScrollPos();
	std::int64_t newPos;

	if (pos >= (top + _XPage))
	{
		newPos = pos - _XPage;
	}
	else
	{
		newPos = top;
	}


	bool changed = false;
	changed |= setXPos(newPos);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::XScroll_PageDown(void)
{
	std::int64_t bottom = (_XSize > _XPage) ? _XSize - _XPage : 0;
	std::int64_t pos = getXScrollPos();
	std::int64_t newPos;

	if (pos <= (bottom - _XPage))
	{
		newPos = pos + _XPage;
	}
	else
	{
		newPos = bottom;
	}


	bool changed = false;
	changed |= setXPos(newPos);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::XScroll_ThumbTrack(void)
{
	std::int64_t top = 0;
	std::int64_t bottom = (_XSize > _XPage) ? _XSize - _XPage : 0;
	std::int64_t pos = getXScrollTrackPos();
	std::int64_t newPos;

	if (top <= pos && pos <= bottom)
	{
		newPos = pos;
	}
	else if (pos < top)
	{
		newPos = top;
	}
	else
	{
		newPos = bottom;
	}


	bool changed = false;
	changed |= setXPos(newPos);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::XScroll_ThumbPosition(void)
{
	std::int64_t top = 0;
	std::int64_t bottom = (_XSize > _XPage) ? _XSize - _XPage : 0;
	std::int64_t pos = getXScrollTrackPos();
	std::int64_t newPos;

	if (top <= pos && pos <= bottom)
	{
		newPos = pos;
	}
	else if (pos < top)
	{
		newPos = top;
	}
	else
	{
		newPos = bottom;
	}


	bool changed = false;
	changed |= setXPos(newPos);
	if (changed)
	{
		updateXScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::XScroll_EndScroll(void)
{

}

//===========================================================================
void WindowScrollHandler::YScroll_Top(void)
{
	std::int64_t top = 0;


	bool changed = false;
	changed |= setYPos(top);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::YScroll_Bottom(void)
{
	std::int64_t bottom = (_YSize > _YPage) ? _YSize - _YPage : 0;

	bool changed = false;
	changed |= setYPos(bottom);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::YScroll_LineUp(void)
{
	std::int64_t top = 0;
	std::int64_t pos = getYScrollPos();
	std::int64_t newPos;

	if (pos >= (top + _YLine))
	{
		newPos = pos - _YLine;
	}
	else
	{
		newPos = top;
	}


	bool changed = false;
	changed |= setYPos(newPos);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::YScroll_LineDown(void)
{
	std::int64_t bottom = (_YSize > _YPage) ? _YSize - _YPage : 0;
	std::int64_t pos = getYScrollPos();
	std::int64_t newPos;

	if (pos <= (bottom - _YLine))
	{
		newPos = pos + _YLine;
	}
	else
	{
		newPos = bottom;
	}


	bool changed = false;
	changed |= setYPos(newPos);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::YScroll_PageUp(void)
{
	std::int64_t top = 0;
	std::int64_t pos = getYScrollPos();
	std::int64_t newPos;

	if (pos >= (top + _YPage))
	{
		newPos = pos - _YPage;
	}
	else
	{
		newPos = top;
	}


	bool changed = false;
	changed |= setYPos(newPos);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::YScroll_PageDown(void)
{
	std::int64_t bottom = (_YSize > _YPage) ? _YSize - _YPage : 0;
	std::int64_t pos = getYScrollPos();
	std::int64_t newPos;

	if (pos <= (bottom - _YPage))
	{
		newPos = pos + _YPage;
	}
	else
	{
		newPos = bottom;
	}


	bool changed = false;
	changed |= setYPos(newPos);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::YScroll_ThumbTrack(void)
{
	std::int64_t top = 0;
	std::int64_t bottom = (_YSize > _YPage) ? _YSize - _YPage : 0;
	std::int64_t pos = getYScrollTrackPos();
	std::int64_t newPos;

	if (top <= pos && pos <= bottom)
	{
		newPos = pos;
	}
	else if (pos < top)
	{
		newPos = top;
	}
	else
	{
		newPos = bottom;
	}


	bool changed = false;
	changed |= setYPos(newPos);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::YScroll_ThumbPosition(void)
{
	std::int64_t top = 0;
	std::int64_t bottom = (_YSize > _YPage) ? _YSize - _YPage : 0;
	std::int64_t pos = getYScrollTrackPos();
	std::int64_t newPos;

	if (top <= pos && pos <= bottom)
	{
		newPos = pos;
	}
	else if (pos < top)
	{
		newPos = top;
	}
	else
	{
		newPos = bottom;
	}


	bool changed = false;
	changed |= setYPos(newPos);
	if (changed)
	{
		updateYScrollBar();
		notifyScrollChanged();
	}
}

void WindowScrollHandler::YScroll_EndScroll(void)
{

}

//===========================================================================
void WindowScrollHandler::notifyScrollChanged(bool byScrollBar)
{
#if 0
	CX_RUNTIME_LOG(cxLTrace)
		<< L"Scroll Changed: "
		<< std::format(L"byScrollBar={} offset=({}, {}) page=({}, {}) size=({}, {})",
			byScrollBar ? L"True" : L"False",
			_XPos, _YPos,
			_XPage, _YPage,
			_XSize, _YSize
		);
#endif

	if (scrollChangedHandler)
	{
		scrollChangedHandler(byScrollBar, _XPos, _YPos);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





