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
WindowScrollHandler::WindowScrollHandler(HWND hwnd) :
	_Hwnd(hwnd)
{
}

//===========================================================================
bool WindowScrollHandler::setXSize(std::int64_t size)
{
	bool changed = false;
	std::int64_t newSize = _xSize;

	
	if (size < 0)
	{
		size = 0;
	}


	newSize = size;


	if (_xSize != newSize)
	{
		_xSize = newSize;
		changed = true;
	}


	if (setXPage(_xPage))
	{
		changed = true;
	}
	if (setXPos(_xPos))
	{
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setXPage(std::int64_t page)
{
	bool changed = false;
	std::int64_t newPage = _xPage;


	if (page < 0)
	{
		page = 0;
	}


	// 0 <= nPage < nMax - nMin
	if (page < _xSize)
	{
		newPage = page;
	}
	else
	{
		newPage = 0;
	}


	if (_xPage != newPage)
	{
		_xPage = newPage;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setXPos(std::int64_t pos)
{
	bool changed = false;
	std::int64_t newPos = _xPos;


	if (pos < 0)
	{
		pos = 0;
	}


	// nMin <= nPos <= nMax - nPage
	if (pos <= (_xSize - _xPage))
	{
		newPos = pos;
	}
	else
	{
		newPos = 0;
	}


	if (_xPos != newPos)
	{
		_xPos = newPos;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setXLine(std::int64_t line)
{
	bool changed = false;
	std::int64_t newLine = _xLine;
	
	
	if (line < 1)
	{
		line = 1;
	}

	
	newLine = line;
	
	
	if (_xLine != newLine)
	{
		_xLine = newLine;
		changed = true;
	}
	return changed;
}

//===========================================================================
bool WindowScrollHandler::setYSize(std::int64_t size)
{
	bool changed = false;
	std::int64_t newSize = _ySize;

	if (size < 0)
	{
		size = 0;
	}


	newSize = size;


	if (_ySize != newSize)
	{
		_ySize = newSize;
		changed = true;
	}


	if (setYPage(_yPage))
	{
		changed = true;
	}
	if (setYPos(_yPos))
	{
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setYPage(std::int64_t page)
{
	bool changed = false;
	std::int64_t newPage = _yPage;

	if (page < 0)
	{
		page = 0;
	}


	// 0 <= nPage < nMax - nMin
	if (page < _ySize)
	{
		newPage = page;
	}
	else
	{
		newPage = 0;
	}


	if (_yPage != newPage)
	{
		_yPage = newPage;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setYPos(std::int64_t pos)
{
	bool changed = false;
	std::int64_t newPos = _yPos;

	if (pos < 0)
	{
		pos = 0;
	}


	// nMin <= nPos <= nMax - nPage
	if (pos <= (_ySize - _yPage))
	{
		newPos = pos;
	}
	else
	{
		newPos = 0;
	}


	if (_yPos != newPos)
	{
		_yPos = newPos;
		changed = true;
	}
	return changed;
}

bool WindowScrollHandler::setYLine(std::int64_t line)
{
	bool changed = false;
	std::int64_t newLine = _yLine;


	if (line < 1)
	{
		line = 1;
	}


	newLine = line;


	if (_yLine != newLine)
	{
		_yLine = newLine;
		changed = true;
	}
	return changed;
}

//===========================================================================
void WindowScrollHandler::updateScrollBars(void)
{
	updateXScrollBar();
	updateYScrollBar();
}

void WindowScrollHandler::updateXScrollBar(void)
{
	SetScrollInfo64(_Hwnd, SB_HORZ, SIF_ALL, _xSize, _xPos, _xPage, TRUE);
}

void WindowScrollHandler::updateYScrollBar(void)
{
	SetScrollInfo64(_Hwnd, SB_VERT, SIF_ALL, _ySize, _yPos, _yPage, TRUE);
}

//===========================================================================
bool WindowScrollHandler::onWindowMessage(cx::wui::WindowMessage& windowMessage)
{
	bool handled = false;
	onWindowScrollMessage(windowMessage, handled);
	return handled;
}

void WindowScrollHandler::onWindowScrollMessage(cx::wui::WindowMessage& windowMessage, bool& handled)
{
	switch (windowMessage.uMsg)
	{
	case WM_HSCROLL:
		onHScroll(windowMessage, handled);
		handled = true;
		break;

	case WM_VSCROLL:
		onVScroll(windowMessage, handled);
		handled = true;
		break;

	default:
		break;
	}
}

void WindowScrollHandler::onHScroll(cx::wui::WindowMessage& windowMessage, bool& handled)
{
	cx::wui::WM_HSCROLL_WindowMessageCrack wm{ windowMessage };

	UINT nPos = wm.nPos();
	UINT nSBCode = wm.nSBCode();
	HWND wndScrollBar = wm.wndScrollBar();

	switch (nSBCode)
	{
	case SB_TOP:
		onHScroll_Top();
		break;

	case SB_BOTTOM:
		onHScroll_Bottom();
		break;

	case SB_LINEUP:
		onHScroll_LineUp();
		break;

	case SB_LINEDOWN:
		onHScroll_LineDown();
		break;

	case SB_PAGEUP:
		onHScroll_PageUp();
		break;

	case SB_PAGEDOWN:
		onHScroll_PageDown();
		break;

	case SB_THUMBTRACK:
		onHScroll_ThumbTrack();
		break;

	case SB_THUMBPOSITION:
		onHScroll_ThumbPosition();
		break;

	case SB_ENDSCROLL:
		onHScroll_EndScroll();
		break;
	
	default:
		break;
	}
}

void WindowScrollHandler::onVScroll(cx::wui::WindowMessage& windowMessage, bool& handled)
{
	cx::wui::WM_HSCROLL_WindowMessageCrack wm{ windowMessage };

	UINT nPos = wm.nPos();
	UINT nSBCode = wm.nSBCode();
	HWND wndScrollBar = wm.wndScrollBar();

	switch (nSBCode)
	{
	case SB_TOP:
		onVScroll_Top();
		break;

	case SB_BOTTOM:
		onVScroll_Bottom();
		break;

	case SB_LINEUP:
		onVScroll_LineUp();
		break;

	case SB_LINEDOWN:
		onVScroll_LineDown();
		break;

	case SB_PAGEUP:
		onVScroll_PageUp();
		break;

	case SB_PAGEDOWN:
		onVScroll_PageDown();
		break;

	case SB_THUMBTRACK:
		onVScroll_ThumbTrack();
		break;

	case SB_THUMBPOSITION:
		onVScroll_ThumbPosition();
		break;

	case SB_ENDSCROLL:
		onVScroll_EndScroll();
		break;

	default:
		break;
	}
}

//===========================================================================
void WindowScrollHandler::onHScroll_Top(void)
{

}

void WindowScrollHandler::onHScroll_Bottom(void)
{

}

void WindowScrollHandler::onHScroll_LineUp(void)
{

}

void WindowScrollHandler::onHScroll_LineDown(void)
{

}

void WindowScrollHandler::onHScroll_PageUp(void)
{

}

void WindowScrollHandler::onHScroll_PageDown(void)
{

}

void WindowScrollHandler::onHScroll_ThumbTrack(void)
{

}

void WindowScrollHandler::onHScroll_ThumbPosition(void)
{

}

void WindowScrollHandler::onHScroll_EndScroll(void)
{

}

//===========================================================================
void WindowScrollHandler::onVScroll_Top(void)
{

}

void WindowScrollHandler::onVScroll_Bottom(void)
{

}

void WindowScrollHandler::onVScroll_LineUp(void)
{

}

void WindowScrollHandler::onVScroll_LineDown(void)
{

}

void WindowScrollHandler::onVScroll_PageUp(void)
{

}

void WindowScrollHandler::onVScroll_PageDown(void)
{

}

void WindowScrollHandler::onVScroll_ThumbTrack(void)
{

}

void WindowScrollHandler::onVScroll_ThumbPosition(void)
{

}

void WindowScrollHandler::onVScroll_EndScroll(void)
{

}



