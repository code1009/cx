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
		break;

	case WM_VSCROLL:
		onVScroll(windowMessage, handled);
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
		onHScroll_Top(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_BOTTOM:
		onHScroll_Bottom(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_LINEUP:
		onHScroll_LineUp(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_LINEDOWN:
		onHScroll_LineDown(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_PAGEUP:
		onHScroll_PageUp(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_PAGEDOWN:
		onHScroll_PageDown(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_THUMBTRACK:
		onHScroll_ThumbTrack(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_THUMBPOSITION:
		onHScroll_ThumbPosition(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_ENDSCROLL:
		onHScroll_EndScroll(nSBCode, nPos, wndScrollBar, handled);
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
		onVScroll_Top(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_BOTTOM:
		onVScroll_Bottom(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_LINEUP:
		onVScroll_LineUp(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_LINEDOWN:
		onVScroll_LineDown(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_PAGEUP:
		onVScroll_PageUp(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_PAGEDOWN:
		onVScroll_PageDown(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_THUMBTRACK:
		onVScroll_ThumbTrack(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_THUMBPOSITION:
		onVScroll_ThumbPosition(nSBCode, nPos, wndScrollBar, handled);
		break;

	case SB_ENDSCROLL:
		onVScroll_EndScroll(nSBCode, nPos, wndScrollBar, handled);
		break;

	default:
		break;
	}
}

//===========================================================================
void WindowScrollHandler::onHScroll_Top(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onHScroll_Bottom(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onHScroll_LineUp(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onHScroll_LineDown(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onHScroll_PageUp(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onHScroll_PageDown(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onHScroll_ThumbTrack(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onHScroll_ThumbPosition(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onHScroll_EndScroll(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

//===========================================================================
void WindowScrollHandler::onVScroll_Top(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onVScroll_Bottom(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onVScroll_LineUp(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onVScroll_LineDown(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onVScroll_PageUp(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onVScroll_PageDown(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onVScroll_ThumbTrack(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onVScroll_ThumbPosition(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}

void WindowScrollHandler::onVScroll_EndScroll(UINT nSBCode, UINT nPos, HWND wndScrollBar, bool& handled)
{

}



