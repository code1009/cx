#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename TBase>
class BasicWindowT : public TBase
{
public:
	bool showWindow(int nCmdShow = SW_SHOWNORMAL) const
	{
		return ::ShowWindow(static_cast<TBase*>(this)->getWindowHandle(), 
			nCmdShow)!= FALSE;
	}
	bool showWindowAsync(int nCmdShow = SW_SHOWNORMAL) const
	{
		return ::ShowWindowAsync(static_cast<TBase*>(this)->getWindowHandle(), 
			nCmdShow)!= FALSE;
	}
	bool updateWindow(void) const
	{
		return ::UpdateWindow(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool redrawWindow(UINT flags = RDW_INVALIDATE) const
	{
		return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), 
			0, 0, flags)!= FALSE;
	}
	bool redrawWindow(const RECT& rcUpdate, UINT flags = RDW_INVALIDATE) const
	{
		return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), 
			&rcUpdate, 0, flags)!= FALSE;
	}
	bool redrawWindow(HRGN hrgnUpdate, UINT flags = RDW_INVALIDATE) const
	{
		return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), 
			0, hrgnUpdate, flags)!= FALSE;
	}
	bool invalidate(bool bErase = true) const
	{
		return ::InvalidateRect(static_cast<TBase*>(this)->getWindowHandle(), 
			0, bErase)!= FALSE;
	}
	bool invalidateRect(const RECT& rc, bool bErase = true) const
	{
		return ::InvalidateRect(static_cast<TBase*>(this)->getWindowHandle(), 
			&rc, bErase)!= FALSE;
	}
	bool invalidateRgn(HRGN hrgn, bool bErase = true) const
	{
		return ::InvalidateRgn(static_cast<TBase*>(this)->getWindowHandle(), 
			hrgn, bErase)!= FALSE;
	}
	bool validateRect(const RECT& rc) const
	{
		return ::ValidateRect(static_cast<TBase*>(this)->getWindowHandle(), 
			&rc)!= FALSE;
	}
	bool validateRgn(HRGN hrgn) const
	{
		return ::ValidateRgn(static_cast<TBase*>(this)->getWindowHandle(), 
			hrgn)!= FALSE;
	}
	bool bringWindowToTop(void) const
	{
		return ::BringWindowToTop(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool setForegroundWindow(void) const
	{
		return ::SetForegroundWindow(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	HWND childWindowFromPoint(const POINT& pt, UINT uFlags = CWP_ALL) const
	{
		return ::ChildWindowFromPointEx(static_cast<TBase*>(this)->getWindowHandle(), 
			pt, uFlags);
	}
	HWND getLastActivePopup(void) const
	{
		return ::GetLastActivePopup(static_cast<TBase*>(this)->getWindowHandle());
	}
	HWND getNextWindow(void) const
	{
		return ::GetWindow(static_cast<TBase*>(this)->getWindowHandle(), GW_HWNDNEXT);
	}
	HWND getPrevWindow(void) const
	{
		return ::GetWindow(static_cast<TBase*>(this)->getWindowHandle(), GW_HWNDPREV);
	}
	HWND getFirstWindow(void) const
	{
		return ::GetWindow(static_cast<TBase*>(this)->getWindowHandle(), GW_HWNDFIRST);
	}
	HWND getLastWindow(void) const
	{
		return ::GetWindow(static_cast<TBase*>(this)->getWindowHandle(), GW_HWNDLAST);
	}
	HWND getChildWindow(void) const
	{
		return ::GetWindow(static_cast<TBase*>(this)->getWindowHandle(), GW_CHILD);
	}
	HWND getTopWindow(void) const
	{
		return ::GetTopWindow(static_cast<TBase*>(this)->getWindowHandle());
	}
	HWND getParentWindow(void) const
	{
		return ::GetParent(static_cast<TBase*>(this)->getWindowHandle());
	}
	HWND setParentWindow(HWND hwndNewParent) const
	{
		return ::SetParent(static_cast<TBase*>(this)->getWindowHandle(), 
			hwndNewParent);
	}
	bool closeWindow(void) const
	{
		return ::CloseWindow(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool isIconic(void) const
	{
		return ::IsIconic(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool openIcon(void) const
	{
		return ::OpenIcon(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool enableWindow(bool bEnable = true) const
	{
		return ::EnableWindow(static_cast<TBase*>(this)->getWindowHandle(), 
			bEnable)!= FALSE;
	}
	bool isWindowEnabled(void) const
	{
		return ::IsWindowEnabled(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool showOwnedPopups(bool bShow = true) const
	{
		return ::ShowOwnedPopups(static_cast<TBase*>(this)->getWindowHandle(), 
			bShow)!= FALSE;
	}
	bool getClientRect(RECT* lpRect) const
	{
		return ::GetClientRect(static_cast<TBase*>(this)->getWindowHandle(), 
			lpRect)!= FALSE;
	}
	CoordRect getClientRect(void) const
	{
		CoordRect rc(0, 0, 0, 0); 
		::GetClientRect(&rc); 
		return rc;
	}
	bool getWindowRect(RECT* lpRect) const
	{
		return ::GetWindowRect(static_cast<TBase*>(this)->getWindowHandle(), 
			lpRect)!= FALSE;
	}
	CoordRect getWindowRect(void) const
	{
		CoordRect rc(0, 0, 0, 0);
		::GetWindowRect(&rc); 
		return rc;
	}
	bool isChildWindow(HWND hwndParent) const
	{
		return ::IsChild(hwndParent, static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool isWindow(void) const
	{
		return 
			static_cast<TBase*>(this)->getWindowHandle()!= NULL
			&& ::IsWindow(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool isWindowVisible(void) const
	{
		return ::IsWindowVisible(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool isZoomed(void) const
	{
		return ::IsZoomed(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	bool getWindowPlacement(WINDOWPLACEMENT* lpwndpl) const
	{
		return ::GetWindowPlacement(static_cast<TBase*>(this)->getWindowHandle(), 
			lpwndpl)!= FALSE;
	}
	bool setWindowPlacement(const WINDOWPLACEMENT* lpwndpl) const
	{
		return ::SetWindowPlacement(static_cast<TBase*>(this)->getWindowHandle(), 
			lpwndpl)!= FALSE;
	}
	int getWindowTextLength(void) const
	{
		return ::GetWindowTextLength(static_cast<TBase*>(this)->getWindowHandle());
	}
	bool getWindowText(LPCTSTR lpString) const
	{
		return ::SetWindowText(static_cast<TBase*>(this)->getWindowHandle(), lpString)!= FALSE;
	}
	bool moveWindow(const POINT& pt, int width, int height, bool bRepaint = true) const
	{
		return ::MoveWindow(static_cast<TBase*>(this)->getWindowHandle(), 
			pt.x, pt.y, width, height, 
			bRepaint)!= FALSE;
	}
	bool moveWindow(const RECT& rc, bool bRepaint = true) const
	{
		return ::MoveWindow(static_cast<TBase*>(this)->getWindowHandle(), 
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 
			bRepaint)!= FALSE;
	}
	bool setWindowPos(const POINT& pt) const
	{
		return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), 
			0, 
			pt.x, pt.y, 0, 0, 
			SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER)!= FALSE;
	}
	bool setWindowPos(int width, int height) const
	{
		return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), 
			0, 
			0, 0, width, height, 
			SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER)!= FALSE;
	}
	bool setWindowPos(const RECT& rc) const
	{
		return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), 
			0, 
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 
			SWP_ASYNCWINDOWPOS | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER)!= FALSE;
	}
	bool setWindowPos(HWND hwndAfter) const
	{
		return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), 
			hwndAfter, 
			0, 0, 0, 0, 
			SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE)!= FALSE;
	}
	bool setWindowPos(HWND hwndAfter, const RECT& rc, UINT uFlags) const
	{
		return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), 
			hwndAfter, 
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 
			uFlags)!= FALSE;
	}
	bool postMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const
	{
		return ::PostMessage(static_cast<TBase*>(this)->getWindowHandle(), 
			uMsg, wParam, lParam)!= FALSE;
	}
	int sendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const
	{
		return static_cast<int>(::SendMessage(static_cast<TBase*>(this)->getWindowHandle(), 
			uMsg, wParam, lParam));
	}
	bool sendMessageTimeout(UINT uMsg, WPARAM wParam, LPARAM lParam, UINT uFlags, UINT uTimeout, LPDWORD lpdwResult) const
	{
		return ::SendMessageTimeout(static_cast<TBase*>(this)->getWindowHandle(), 
			uMsg, wParam, lParam, 
			uFlags, uTimeout, lpdwResult)!= FALSE;
	}
	int messageBox(LPCTSTR lpText, LPCTSTR lpCaption, UINT uType = MB_OK | MB_ICONASTERISK) const
	{
		return ::MessageBox(static_cast<TBase*>(this)->getWindowHandle(), 
			lpText, lpCaption, uType);
	}
	int sendDlgItemMessage(int nIDDlgItem, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		return static_cast<int>(
			::SendDlgItemMessage(static_cast<TBase*>(this)->getWindowHandle(), 
				nIDDlgItem, 
				uMsg, wParam, lParam));
	}
	int getDlgCtrlID(void) const
	{
		return ::GetDlgCtrlID(static_cast<TBase*>(this)->getWindowHandle());
	}
	HWND getDlgItem(int nIDDlgItem) const
	{
		return ::GetDlgItem(static_cast<TBase*>(this)->getWindowHandle(), 
			nIDDlgItem);
	}
	HWND getNextDlgGroupItem(HWND hCtl) const
	{
		return ::GetNextDlgGroupItem(static_cast<TBase*>(this)->getWindowHandle(), 
			hCtl, FALSE);
	}
	HWND getPrevDlgGroupItem(HWND hCtl) const
	{
		return ::GetNextDlgGroupItem(static_cast<TBase*>(this)->getWindowHandle(), 
			hCtl, TRUE);
	}
	HWND getNextDlgTabItem(HWND hCtl) const
	{
		return ::GetNextDlgTabItem(static_cast<TBase*>(this)->getWindowHandle(), 
			hCtl, FALSE);
	}
	HWND getPrevDlgTabItem(HWND hCtl) const
	{
		return ::GetNextDlgTabItem(static_cast<TBase*>(this)->getWindowHandle(), 
			hCtl, TRUE);
	}
	int getDlgItemInt(int nIDDlgItem, bool* pbTranslated = 0) const
	{
		BOOL bTranslated = FALSE;
		int ret = ::GetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), nIDDlgItem, &bTranslated, TRUE);
		if (pbTranslated != 0)
			*pbTranslated = bTranslated != FALSE;
		return ret;
	}
	unsigned getDlgItemUInt(int nIDDlgItem, bool* pbTranslated = 0) const
	{
		BOOL bTranslated = FALSE;
		unsigned ret = ::GetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), nIDDlgItem, &bTranslated, FALSE);
		if (pbTranslated != 0)
			*pbTranslated = bTranslated != FALSE;
		return ret;
	}
	bool setDlgItemInt(int nIDDlgItem, int value) const
	{
		return ::SetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), 
			nIDDlgItem, value, TRUE)!= FALSE;
	}
	bool setDlgItemInt(int nIDDlgItem, unsigned value) const
	{
		return ::SetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), 
			nIDDlgItem, value, FALSE)!= FALSE;
	}
	int getDlgItemText(int nIDDlgItem, LPTSTR lpString, int nMaxCount) const
	{
		return ::GetDlgItemText(static_cast<TBase*>(this)->getWindowHandle(), 
			nIDDlgItem, lpString, nMaxCount);
	}
	bool getDlgItemText(int nIDDlgItem, LPCTSTR lpString) const
	{
		return ::SetDlgItemText(static_cast<TBase*>(this)->getWindowHandle(), 
			nIDDlgItem, lpString)!= FALSE;
	}
	bool mapDialogRect(RECT* lpRect) const
	{
		return ::MapDialogRect(static_cast<TBase*>(this)->getWindowHandle(), 
			lpRect)!= FALSE;
	}
	bool checkDlgButton(int nIDButton, UINT uCheck = BST_CHECKED) const
	{
		return ::CheckDlgButton(static_cast<TBase*>(this)->getWindowHandle(), 
			nIDButton, uCheck)!= FALSE;
	}
	bool checkRadioButton(int nIDFirst, int nIDLast, int nIDCheck) const
	{
		return ::CheckRadioButton(static_cast<TBase*>(this)->getWindowHandle(), 
			nIDFirst, nIDLast, nIDCheck)!= FALSE;
	}
	int isDlgButtonChecked(int nIDButton) const
	{
		return ::IsDlgButtonChecked(static_cast<TBase*>(this)->getWindowHandle(), 
			nIDButton);
	}
	bool enableScrollBar(UINT wSBFlags, UINT wArrows) const
	{
		return ::EnableScrollBar(static_cast<TBase*>(this)->getWindowHandle(), 
			wSBFlags, wArrows = ESB_ENABLE_BOTH)!= FALSE;
	}
	bool getScrollInfo(int fnBar, LPSCROLLINFO lpsi) const
	{
		return ::GetScrollInfo(static_cast<TBase*>(this)->getWindowHandle(), 
			fnBar, lpsi)!= FALSE;
	}
	int setScrollInfo(int fnBar, LPSCROLLINFO lpsi, bool bRedraw = true) const
	{
		return ::SetScrollInfo(static_cast<TBase*>(this)->getWindowHandle(), 
			fnBar, lpsi, bRedraw);
	}
	bool showScrollBar(int wBar, bool bShow = true) const
	{
		return ::ShowScrollBar(static_cast<TBase*>(this)->getWindowHandle(), 
			wBar, bShow)!= FALSE;
	}
	bool scrollWindow(int dx, int dy, UINT uFlags, LPCRECT rcScroll = 0, LPCRECT rcClip = 0, HRGN hrgnUpdate = 0, LPRECT rcUpdate = 0) const
	{
		return ::ScrollWindowEx(static_cast<TBase*>(this)->getWindowHandle(), 
			dx, dy, rcScroll, rcClip, 
			hrgnUpdate, rcUpdate, uFlags)!= FALSE;
	}
	void setFont(HFONT hFont, bool bRedraw = true) const
	{
		sendMessage(WM_SETFONT, reinterpret_cast<WPARAM>(hFont), bRedraw);
	}
	HFONT getFont(void) const
	{
		return reinterpret_cast<HFONT>(sendMessage(WM_GETFONT));
	}
	HICON setIcon(HICON hIcon, WPARAM type = ICON_BIG) const
	{
		return reinterpret_cast<HICON>(sendMessage(WM_SETICON, type, reinterpret_cast<LPARAM>(hIcon)));
	}
	HICON getIcon(WPARAM type = ICON_BIG) const
	{
		return reinterpret_cast<HICON>(sendMessage(WM_GETICON, type));
	}
	bool screenToClient(POINT& pt) const
	{
		return ::ScreenToClient(static_cast<TBase*>(this)->getWindowHandle(), &pt)!= FALSE;
	}
	bool clientToScreen(POINT& pt) const
	{
		return ::ClientToScreen(static_cast<TBase*>(this)->getWindowHandle(), &pt)!= FALSE;
	}
	bool screenToClient(RECT& rc) const
	{
		return screenToClient(*reinterpret_cast<POINT*>(&rc.left))
			&& screenToClient(*reinterpret_cast<POINT*>(&rc.right));
	}
	bool clientToScreen(RECT& rc) const
	{
		return clientToScreen(*reinterpret_cast<POINT*>(&rc.left))
			&& clientToScreen(*reinterpret_cast<POINT*>(&rc.right));
	}
	HWND setFocus(void) const
	{
		return ::SetFocus(static_cast<TBase*>(this)->getWindowHandle());
	}
	HWND setActiveWindow(void) const
	{
		return ::SetActiveWindow(static_cast<TBase*>(this)->getWindowHandle());
	}
	HWND setCapture(void) const
	{
		return ::SetCapture(static_cast<TBase*>(this)->getWindowHandle());
	}
	CoordRect getWindowRectOnParentWindow(void) const
	{
		CoordRect rc = getWindowRect();
		HWND parent = getParentWindow();
		::ScreenToClient(parent, &rc[0]);
		::ScreenToClient(parent, &rc[1]);
		return rc;
	}
	HMENU getMenu(void) const
	{
		return ::GetMenu(static_cast<TBase*>(this)->getWindowHandle());
	}
	bool drawMenuBar(void) const
	{
		return ::DrawMenuBar(static_cast<TBase*>(this)->getWindowHandle())!= FALSE;
	}
	HMENU getSystemMenu(bool b_revert = false) const
	{
		return ::GetSystemMenu(static_cast<TBase*>(this)->getWindowHandle(), 
			b_revert);
	}
	bool setMenu(HMENU hMenu) const
	{
		return ::SetMenu(static_cast<TBase*>(this)->getWindowHandle(), 
			hMenu)!= FALSE;
	}
	bool drawAnimatedRects(int idAni, const RECT& rcFrom, const RECT& rcTo) const
	{
		return ::DrawAnimatedRects(static_cast<TBase*>(this)->getWindowHandle(), 
			idAni, &rcFrom, &rcTo)!= FALSE;
	}
	template< class T >
	T* getWindowLong(int id)
	{
		return reinterpret_cast<T>(GetWindowLongW(static_cast<TBase*>(this)->getWindowHandle(), 
			id));
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




