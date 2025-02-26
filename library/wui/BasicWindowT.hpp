#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
From: 
https://github.com/DavidNash2024/Win32xx/blob/master/include/wxx_wincore.h
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename TBase>
class BasicWindowT : public TBase
{
public:
	HDC beginPaint(PAINTSTRUCT& ps) const;
	BOOL bringWindowToTop() const;
	LRESULT callWindowProc(WNDPROC pPrevWndFunc, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	BOOL checkDlgButton(UINT buttonID, UINT check) const;
	BOOL checkRadioButton(UINT firstButtonID, UINT lastButtonID, UINT checkButtonID) const;
	HWND childWindowFromPoint(POINT point) const;
	BOOL clientToScreen(POINT& point) const;
	BOOL clientToScreen(RECT& rect) const;
	void close() const;
	BOOL closeWindow() const;
	HDWP deferWindowPos(HDWP winPosInfo, HWND insertAfter, int x, int y, int cx, int cy, UINT flags) const;
	HDWP deferWindowPos(HDWP winPosInfo, HWND insertAfter, RECT rect, UINT flags) const;
	LRESULT defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	int dlgDirList(LPTSTR pathSpec, UINT listBoxID, UINT staticPathID, UINT fileType) const;
	int dlgDirListComboBox(LPTSTR pathSpec, UINT comboBoxID, UINT staticPathID, UINT fileType) const;
	BOOL dlgDirSelectEx(LPTSTR string, int count, UINT listBoxID) const;
	BOOL dlgDirSelectComboBoxEx(LPTSTR string, int count, UINT comboBoxID) const;
	BOOL drawAnimatedRects(UINT aniID, RECT from, RECT to) const;
	BOOL drawCaption(HDC dc, RECT rect, UINT flags) const;
	BOOL drawMenuBar() const;
	BOOL enableScrollBar(UINT flags, UINT arrows) const;
	BOOL enableWindow(BOOL enable = TRUE) const;
	BOOL endPaint(PAINTSTRUCT& ps) const;
	HWND getActiveWindow() const;
	HWND getCapture() const;
	ULONG_PTR getClassLongPtr(int index) const;
	CoordRect getClientRect() const;
	HWND getDesktopWindow() const;
	UINT getDlgCtrlID() const;
	HWND getDlgItem(UINT dlgItemID) const;
	UINT getDlgItemInt(UINT dlgItemID, BOOL& isTranslated, BOOL isSigned) const;
	UINT getDlgItemInt(UINT dlgItemID, BOOL isSigned) const;
	DWORD getExStyle() const;
	HWND getFocus() const;
	HFONT getFont() const;
	HICON getIcon(BOOL isBigIcon) const;
	HWND getLastActivePopup() const;
	HMENU getMenu() const;
	HWND getNextDlgGroupItem(HWND control, BOOL isPrevious) const;
	HWND getNextDlgTabItem(HWND control, BOOL isPrevious) const;
	HWND getParent() const;
	BOOL getScrollInfo(int barType, SCROLLINFO& si) const;
	int getScrollPos(int bar) const;
	BOOL getScrollRange(int bar, int& minPos, int& maxPos) const;
	DWORD getStyle() const;
	HMENU getSystemMenu(BOOL revert) const;
	HWND getTopWindow() const;
	BOOL getWindowPlacement(WINDOWPLACEMENT& wp) const;
	CoordRect getUpdateRect(BOOL erase) const;
	int getUpdateRgn(HRGN rgn, BOOL erase) const;
	HWND getWindow(UINT cmd) const;
	LONG_PTR getWindowLongPtr(int index) const;
	CoordRect getWindowRect() const;
	int getWindowTextLength() const;
	BOOL hiliteMenuItem(HMENU menu, UINT itemID, UINT hilite) const;
	void invalidate(BOOL erase = TRUE) const;
	BOOL invalidateRect(RECT rect, BOOL erase = TRUE) const;
	BOOL invalidateRect(BOOL erase = TRUE) const;
	BOOL invalidateRgn(HRGN rgn, BOOL erase = TRUE) const;
	BOOL isChild(HWND child) const;
	BOOL isDialogMessage(MSG& uMsg) const;
	UINT isDlgButtonChecked(UINT buttonID) const;
	BOOL isWindowEnabled() const;
	BOOL isIconic() const;
	BOOL isWindow() const;
	BOOL isWindowVisible() const;
	BOOL isZoomed() const;
	BOOL killTimer(UINT_PTR eventID) const;
	BOOL lockWindowUpdate() const;
	int mapWindowPoints(HWND to, POINT& point) const;
	int mapWindowPoints(HWND to, RECT& rect) const;
	int mapWindowPoints(HWND to, LPPOINT pointsArray, UINT count) const;
	int messageBox(LPCTSTR text, LPCTSTR caption, UINT type = MB_OK | MB_ICONASTERISK) const;
	BOOL moveWindow(int x, int y, int width, int height, BOOL repaint = TRUE) const;
	BOOL moveWindow(RECT rect, BOOL repaint = TRUE) const;
	BOOL postMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
	BOOL openIcon() const;
	BOOL postMessage(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	void print(HDC dc, DWORD flags) const;
	BOOL redrawWindow(RECT updateRect, UINT flags) const;
	BOOL redrawWindow(HRGN rgn, UINT flags) const;
	BOOL redrawWindow(UINT flags) const;
	int releaseDC(HDC dc) const;
	BOOL screenToClient(POINT& point) const;
	BOOL screenToClient(RECT& rect) const;
	BOOL scrollWindow(int xAmount, int yAmount, RECT scrollRect, LPCRECT pClipRect) const;
	BOOL scrollWindow(int xAmount, int yAmount, LPCRECT pClipRect) const;
	int scrollWindowEx(int dx, int dy, LPCRECT pScrollRect, LPCRECT pClipRect, HRGN update, LPRECT pUpdateRect, UINT flags) const;
	LRESULT sendDlgItemMessage(UINT dlgItemID, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	LRESULT sendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
	LRESULT sendMessage(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	BOOL sendNotifyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) const;
	HWND setActiveWindow() const;
	HWND setCapture() const;
	ULONG_PTR setClassLongPtr(int index, LONG_PTR newLong) const;
	LONG_PTR setDlgCtrlID(UINT id) const;
	BOOL setDlgItemInt(UINT dlgItemID, UINT value, BOOL isSigned) const;
	BOOL setDlgItemText(UINT dlgItemID, LPCTSTR string) const;
	void setExStyle(DWORD exStyle) const;
	HWND setFocus() const;
	void setFont(HFONT font, BOOL redraw = TRUE) const;
	BOOL setForegroundWindow() const;
	HICON setIcon(HICON icon, BOOL isBigIcon) const;
	BOOL setMenu(HMENU menu) const;
	HWND setParent(HWND parent) const;
	BOOL setRedraw(BOOL redraw = TRUE) const;
	int setScrollInfo(int barType, const SCROLLINFO& si, BOOL redraw) const;
	int setScrollPos(int barType, int pos, BOOL redraw) const;
	BOOL setScrollRange(int barType, int minPos, int maxPos, BOOL redraw) const;
	void setStyle(DWORD style) const;
	UINT_PTR setTimer(UINT_PTR eventID, UINT elapse, TIMERPROC pTimerFunc) const;
	LONG_PTR setWindowLongPtr(int index, LONG_PTR newLong) const;
	BOOL setWindowPlacement(const WINDOWPLACEMENT& wndpl) const;
	BOOL setWindowPos(HWND insertAfter, int x, int y, int cx, int cy, UINT flags) const;
	BOOL setWindowPos(HWND insertAfter, RECT rect, UINT flags) const;
	int setWindowRgn(HRGN rgn, BOOL redraw = TRUE) const;
	BOOL setWindowText(LPCTSTR text) const;
	HRESULT setWindowTheme(LPCWSTR subAppName, LPCWSTR subIdList) const;
	BOOL showOwnedPopups(BOOL show) const;
	BOOL showScrollBar(int bar, BOOL show) const;
	BOOL showWindow(int showCmd = SW_SHOWNORMAL) const;
	BOOL showWindowAsync(int showCmd) const;
	BOOL updateWindow() const;
	BOOL unlockWindowUpdate() const;
	BOOL validateRect(RECT rect) const;
	BOOL validateRect() const;
	BOOL validateRgn(HRGN rgn) const;
	BOOL dragAcceptFiles(BOOL fAccept) const;
	BOOL postRedraw(BOOL redraw = TRUE) const;
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
template <typename TBase>
HDC BasicWindowT<TBase>::beginPaint(PAINTSTRUCT& ps) const
{
	assert(isWindow());
	return ::BeginPaint(static_cast<TBase*>(this)->getWindowHandle(), &ps);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::bringWindowToTop() const
{
	assert(isWindow());
	return ::BringWindowToTop(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
LRESULT BasicWindowT<TBase>::callWindowProc(WNDPROC pPrevWndFunc, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	assert(isWindow());
	return ::CallWindowProc(pPrevWndFunc, static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::checkDlgButton(UINT buttonID, UINT check) const
{
	assert(isWindow());
	return ::CheckDlgButton(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(buttonID), check);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::checkRadioButton(UINT firstButtonID, UINT lastButtonID, UINT checkButtonID) const
{
	assert(isWindow());
	return ::CheckRadioButton(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(firstButtonID),
		static_cast<int>(lastButtonID), static_cast<int>(checkButtonID));
}

template <typename TBase>
HWND BasicWindowT<TBase>::childWindowFromPoint(POINT point) const
{
	assert(isWindow());
	return ::ChildWindowFromPoint(static_cast<TBase*>(this)->getWindowHandle(), point);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::clientToScreen(POINT& point) const
{
	assert(isWindow());
	return ::ClientToScreen(static_cast<TBase*>(this)->getWindowHandle(), &point);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::clientToScreen(RECT& rect) const
{
	assert(isWindow());
	return 
		static_cast<BOOL>(
			::MapWindowPoints(
				static_cast<TBase*>(this)->getWindowHandle(), 
				HWND_DESKTOP,
				reinterpret_cast<LPPOINT>(&rect), 
				2
			)
		);
}

template <typename TBase>
void BasicWindowT<TBase>::close() const
{
	assert(isWindow());
	postMessage(WM_CLOSE);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::closeWindow() const
{
	assert(isWindow());
	return ::CloseWindow(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
HDWP BasicWindowT<TBase>::deferWindowPos(HDWP winPosInfo, HWND insertAfter, int x, int y, int cx, int cy, UINT flags) const
{
	assert(isWindow());
	return ::DeferWindowPos(winPosInfo, static_cast<TBase*>(this)->getWindowHandle(), insertAfter, x, y, cx, cy, flags);
}

template <typename TBase>
HDWP BasicWindowT<TBase>::deferWindowPos(HDWP winPosInfo, HWND insertAfter, RECT rect, UINT flags) const
{
	assert(isWindow());
	return ::DeferWindowPos(winPosInfo, static_cast<TBase*>(this)->getWindowHandle(), insertAfter, rect.left,
		rect.top, rect.right - rect.left, rect.bottom - rect.top, flags);
}

template <typename TBase>
LRESULT BasicWindowT<TBase>::defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	assert(isWindow());
	return ::DefWindowProc(static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
}

template <typename TBase>
int BasicWindowT<TBase>::dlgDirList(LPTSTR pathSpec, UINT listBoxID, UINT staticPathID, UINT fileType) const
{
	assert(isWindow());
	return ::DlgDirList(static_cast<TBase*>(this)->getWindowHandle(), pathSpec, static_cast<int>(listBoxID),
		static_cast<int>(staticPathID), fileType);
}

template <typename TBase>
int BasicWindowT<TBase>::dlgDirListComboBox(LPTSTR pathSpec, UINT comboBoxID, UINT staticPathID, UINT fileType) const
{
	assert(isWindow());
	return ::DlgDirListComboBox(static_cast<TBase*>(this)->getWindowHandle(), pathSpec, static_cast<int>(comboBoxID),
		static_cast<int>(staticPathID), fileType);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::dlgDirSelectEx(LPTSTR string, int count, UINT listBoxID) const
{
	assert(isWindow());
	return ::DlgDirSelectEx(static_cast<TBase*>(this)->getWindowHandle(), string, count, static_cast<int>(listBoxID));
}

template <typename TBase>
BOOL BasicWindowT<TBase>::dlgDirSelectComboBoxEx(LPTSTR string, int count, UINT comboBoxID) const
{
	assert(isWindow());
	return ::DlgDirSelectComboBoxEx(static_cast<TBase*>(this)->getWindowHandle(), string, count, static_cast<int>(comboBoxID));
}

template <typename TBase>
BOOL BasicWindowT<TBase>::drawAnimatedRects(UINT aniID, RECT from, RECT to) const
{
	assert(isWindow());
	return ::DrawAnimatedRects(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(aniID), &from, &to);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::drawCaption(HDC dc, RECT rect, UINT flags) const
{
	assert(isWindow());
	return ::DrawCaption(static_cast<TBase*>(this)->getWindowHandle(), dc, &rect, flags);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::drawMenuBar() const
{
	assert(isWindow());
	return ::DrawMenuBar(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::enableScrollBar(UINT flags, UINT arrows) const
{
	assert(isWindow());
	return ::EnableScrollBar(static_cast<TBase*>(this)->getWindowHandle(), flags, arrows);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::enableWindow(BOOL enable) const
{
	assert(isWindow());
	return ::EnableWindow(static_cast<TBase*>(this)->getWindowHandle(), enable);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::endPaint(PAINTSTRUCT& ps) const
{
	assert(isWindow());
	return ::EndPaint(static_cast<TBase*>(this)->getWindowHandle(), &ps);
}

template <typename TBase>
HWND BasicWindowT<TBase>::getActiveWindow() const
{
	return ::GetActiveWindow();
}

template <typename TBase>
HWND BasicWindowT<TBase>::getCapture() const
{
	return ::GetCapture();
}

template <typename TBase>
ULONG_PTR BasicWindowT<TBase>::getClassLongPtr(int index) const
{
	assert(isWindow());
	return ::GetClassLongPtr(static_cast<TBase*>(this)->getWindowHandle(), index);
}

template <typename TBase>
CoordRect BasicWindowT<TBase>::getClientRect() const
{
	assert(isWindow());
	CoordRect rc;
	::GetClientRect(static_cast<TBase*>(this)->getWindowHandle(), &rc);
	return rc;
}

template <typename TBase>
HWND BasicWindowT<TBase>::getDesktopWindow() const
{
	return HWND(::GetDesktopWindow());
}

template <typename TBase>
UINT BasicWindowT<TBase>::getDlgCtrlID() const
{
	assert(isWindow());
	return static_cast<UINT>(::GetDlgCtrlID(static_cast<TBase*>(this)->getWindowHandle()));
}

template <typename TBase>
HWND BasicWindowT<TBase>::getDlgItem(UINT dlgItemID) const
{
	assert(isWindow());
	return ::GetDlgItem(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID));
}

template <typename TBase>
UINT BasicWindowT<TBase>::getDlgItemInt(UINT dlgItemID, BOOL& isTranslated, BOOL isSigned) const
{
	assert(isWindow());
	return ::GetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), &isTranslated, isSigned);
}

template <typename TBase>
UINT BasicWindowT<TBase>::getDlgItemInt(UINT dlgItemID, BOOL isSigned) const
{
	assert(isWindow());
	return ::GetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), nullptr, isSigned);
}

template <typename TBase>
DWORD BasicWindowT<TBase>::getExStyle() const
{
	assert(isWindow());
	return static_cast<DWORD>(getWindowLongPtr(GWL_EXSTYLE));
}

template <typename TBase>
HWND BasicWindowT<TBase>::getFocus() const
{
	return HWND(::GetFocus());
}

template <typename TBase>
HFONT BasicWindowT<TBase>::getFont() const
{
	assert(isWindow());
	return reinterpret_cast<HFONT>(sendMessage(WM_GETFONT, 0, 0));
}

template <typename TBase>
HICON BasicWindowT<TBase>::getIcon(BOOL isBigIcon) const
{
	assert(isWindow());
	WPARAM wParam = static_cast<WPARAM>(isBigIcon);
	return reinterpret_cast<HICON>(sendMessage(WM_GETICON, wParam, 0));
}

template <typename TBase>
HWND BasicWindowT<TBase>::getLastActivePopup() const
{
	assert(isWindow());
	return ::GetLastActivePopup(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
HMENU BasicWindowT<TBase>::getMenu() const
{
	assert(isWindow());
	return ::GetMenu(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
HWND BasicWindowT<TBase>::getNextDlgGroupItem(HWND control, BOOL isPrevious) const
{
	assert(isWindow());
	return ::GetNextDlgGroupItem(static_cast<TBase*>(this)->getWindowHandle(), control, isPrevious);
}

template <typename TBase>
HWND BasicWindowT<TBase>::getNextDlgTabItem(HWND control, BOOL isPrevious) const
{
	assert(isWindow());
	return ::GetNextDlgTabItem(static_cast<TBase*>(this)->getWindowHandle(), control, isPrevious);
}

template <typename TBase>
HWND BasicWindowT<TBase>::getParent() const
{
	assert(isWindow());
	return ::GetParent(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::getScrollInfo(int barType, SCROLLINFO& si) const
{
	assert(isWindow());
	return ::GetScrollInfo(static_cast<TBase*>(this)->getWindowHandle(), barType, &si);
}

template <typename TBase>
int BasicWindowT<TBase>::getScrollPos(int bar) const
{
	assert(isWindow());
	return ::GetScrollPos(static_cast<TBase*>(this)->getWindowHandle(), bar);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::getScrollRange(int bar, int& minPos, int& maxPos) const
{
	assert(isWindow());
	return ::GetScrollRange(static_cast<TBase*>(this)->getWindowHandle(), bar, &minPos, &maxPos);
}

template <typename TBase>
DWORD BasicWindowT<TBase>::getStyle() const
{
	assert(isWindow());
	return static_cast<DWORD>(getWindowLongPtr(GWL_STYLE));
}

template <typename TBase>
HMENU BasicWindowT<TBase>::getSystemMenu(BOOL revert) const
{
	assert(isWindow());
	return ::GetSystemMenu(static_cast<TBase*>(this)->getWindowHandle(), revert);
}

template <typename TBase>
HWND BasicWindowT<TBase>::getTopWindow() const
{
	assert(isWindow());
	return ::GetTopWindow(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::getWindowPlacement(WINDOWPLACEMENT& wp) const
{
	assert(isWindow());
	return ::GetWindowPlacement(static_cast<TBase*>(this)->getWindowHandle(), &wp);
}

template <typename TBase>
CoordRect BasicWindowT<TBase>::getUpdateRect(BOOL erase) const
{
	assert(isWindow());
	CoordRect rc;
	::GetUpdateRect(static_cast<TBase*>(this)->getWindowHandle(), &rc, erase);
	return rc;
}

template <typename TBase>
int BasicWindowT<TBase>::getUpdateRgn(HRGN rgn, BOOL erase) const
{
	assert(isWindow());
	return ::GetUpdateRgn(static_cast<TBase*>(this)->getWindowHandle(), rgn, erase);
}

template <typename TBase>
HWND BasicWindowT<TBase>::getWindow(UINT cmd) const
{
	assert(isWindow());
	return ::GetWindow(static_cast<TBase*>(this)->getWindowHandle(), cmd);
}

template <typename TBase>
LONG_PTR BasicWindowT<TBase>::getWindowLongPtr(int index) const
{
	assert(isWindow());
	return ::GetWindowLongPtr(static_cast<TBase*>(this)->getWindowHandle(), index);
}

template <typename TBase>
CoordRect BasicWindowT<TBase>::getWindowRect() const
{
	assert(isWindow());
	CoordRect rc;
	::GetWindowRect(static_cast<TBase*>(this)->getWindowHandle(), &rc);
	return rc;
}

template <typename TBase>
int BasicWindowT<TBase>::getWindowTextLength() const
{
	assert(isWindow());
	return ::GetWindowTextLength(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::hiliteMenuItem(HMENU menu, UINT itemID, UINT hilite) const
{
	assert(isWindow());
	return ::HiliteMenuItem(static_cast<TBase*>(this)->getWindowHandle(), menu, itemID, hilite);
}

template <typename TBase>
void BasicWindowT<TBase>::invalidate(BOOL erase) const
{
	assert(isWindow());
	::InvalidateRect(static_cast<TBase*>(this)->getWindowHandle(), nullptr, erase);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::invalidateRect(RECT rect, BOOL erase) const
{
	assert(isWindow());
	return ::InvalidateRect(static_cast<TBase*>(this)->getWindowHandle(), &rect, erase);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::invalidateRect(BOOL erase) const
{
	assert(isWindow());
	return ::InvalidateRect(static_cast<TBase*>(this)->getWindowHandle(), nullptr, erase);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::invalidateRgn(HRGN rgn, BOOL erase) const
{
	assert(isWindow());
	return ::InvalidateRgn(static_cast<TBase*>(this)->getWindowHandle(), rgn, erase);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::isChild(HWND child) const
{
	assert(isWindow());
	return ::IsChild(static_cast<TBase*>(this)->getWindowHandle(), child);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::isDialogMessage(MSG& uMsg) const
{
	assert(isWindow());
	return ::IsDialogMessage(static_cast<TBase*>(this)->getWindowHandle(), &uMsg);
}

template <typename TBase>
UINT BasicWindowT<TBase>::isDlgButtonChecked(UINT buttonID) const
{
	assert(isWindow());
	return ::IsDlgButtonChecked(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(buttonID));
}

template <typename TBase>
BOOL BasicWindowT<TBase>::isWindowEnabled() const
{
	assert(isWindow());
	return ::IsWindowEnabled(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::isIconic() const
{
	assert(isWindow());
	return ::IsIconic(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::isWindow() const
{
	return ::IsWindow(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::isWindowVisible() const
{
	assert(isWindow());
	return (getStyle() & WS_VISIBLE) != 0;
}

template <typename TBase>
BOOL BasicWindowT<TBase>::isZoomed() const
{
	assert(isWindow());
	return ::IsZoomed(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::killTimer(UINT_PTR eventID) const
{
	assert(isWindow());
	return ::KillTimer(static_cast<TBase*>(this)->getWindowHandle(), eventID);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::lockWindowUpdate() const
{
	assert(isWindow());
	return ::LockWindowUpdate(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
int BasicWindowT<TBase>::mapWindowPoints(HWND to, POINT& point) const
{
	assert(isWindow());
	return ::MapWindowPoints(static_cast<TBase*>(this)->getWindowHandle(), to, &point, 1);
}

template <typename TBase>
int BasicWindowT<TBase>::mapWindowPoints(HWND to, RECT& rect) const
{
	assert(isWindow());
	return ::MapWindowPoints(static_cast<TBase*>(this)->getWindowHandle(), to, reinterpret_cast<LPPOINT>(&rect), 2);
}

template <typename TBase>
int BasicWindowT<TBase>::mapWindowPoints(HWND to, LPPOINT pointsArray, UINT count) const
{
	assert(isWindow());
	return ::MapWindowPoints(static_cast<TBase*>(this)->getWindowHandle(), to, reinterpret_cast<LPPOINT>(pointsArray), count);
}

template <typename TBase>
int BasicWindowT<TBase>::messageBox(LPCTSTR text, LPCTSTR caption, UINT type) const
{
	assert(isWindow());
	return ::MessageBox(static_cast<TBase*>(this)->getWindowHandle(), text, caption, type);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::moveWindow(int x, int y, int width, int height, BOOL repaint) const
{
	assert(isWindow());
	return ::MoveWindow(static_cast<TBase*>(this)->getWindowHandle(), x, y, width, height, repaint = TRUE);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::moveWindow(RECT rect, BOOL repaint) const
{
	assert(isWindow());
	return ::MoveWindow(static_cast<TBase*>(this)->getWindowHandle(), rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top, repaint);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::postMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	assert(isWindow());
	return ::PostMessage(static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::openIcon() const
{
	assert(isWindow());
	return ::OpenIcon(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::postMessage(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	assert(isWindow());
	return ::PostMessage(wnd, uMsg, wParam, lParam);
}

template <typename TBase>
void BasicWindowT<TBase>::print(HDC dc, DWORD flags) const
{
	assert(isWindow());
	WPARAM wParam = reinterpret_cast<WPARAM>(dc);
	LPARAM lParam = static_cast<LPARAM>(flags);
	SendMessage(static_cast<TBase*>(this)->getWindowHandle(), WM_PRINT, wParam, lParam);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::redrawWindow(RECT updateRect, UINT flags) const
{
	assert(isWindow());
	return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), &updateRect, nullptr, flags);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::redrawWindow(HRGN rgn, UINT flags) const
{
	assert(isWindow());
	return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), nullptr, rgn, flags);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::redrawWindow(UINT flags) const
{
	assert(isWindow());
	return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), nullptr, nullptr, flags);
}

template <typename TBase>
int BasicWindowT<TBase>::releaseDC(HDC dc) const
{
	assert(isWindow());
	return ::ReleaseDC(static_cast<TBase*>(this)->getWindowHandle(), dc);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::screenToClient(POINT& point) const
{
	assert(isWindow());
	return ::ScreenToClient(static_cast<TBase*>(this)->getWindowHandle(), &point);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::screenToClient(RECT& rect) const
{
	assert(isWindow());
	return 
		static_cast<BOOL>(
			::MapWindowPoints(
				HWND_DESKTOP, 
				static_cast<TBase*>(this)->getWindowHandle(),
				reinterpret_cast<LPPOINT>(&rect), 
				2
			)
		);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::scrollWindow(int xAmount, int yAmount, RECT scrollRect, LPCRECT pClipRect) const
{
	assert(isWindow());
	return ::ScrollWindow(static_cast<TBase*>(this)->getWindowHandle(), xAmount, yAmount, &scrollRect, pClipRect);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::scrollWindow(int xAmount, int yAmount, LPCRECT pClipRect) const
{
	assert(isWindow());
	return ::ScrollWindow(static_cast<TBase*>(this)->getWindowHandle(), xAmount, yAmount, nullptr, pClipRect);
}

template <typename TBase>
int BasicWindowT<TBase>::scrollWindowEx(int dx, int dy, LPCRECT pScrollRect, LPCRECT pClipRect,
	HRGN update, LPRECT pUpdateRect, UINT flags) const
{
	assert(isWindow());
	return ::ScrollWindowEx(static_cast<TBase*>(this)->getWindowHandle(), dx, dy, pScrollRect, pClipRect, update, pUpdateRect, flags);
}

template <typename TBase>
LRESULT BasicWindowT<TBase>::sendDlgItemMessage(UINT dlgItemID, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	assert(isWindow());
	return ::SendDlgItemMessage(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), uMsg, wParam, lParam);
}

template <typename TBase>
LRESULT BasicWindowT<TBase>::sendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	assert(isWindow());
	return ::SendMessage(static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
}

template <typename TBase>
LRESULT BasicWindowT<TBase>::sendMessage(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	assert(isWindow());
	return ::SendMessage(wnd, uMsg, wParam, lParam);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::sendNotifyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	assert(isWindow());
	return ::SendNotifyMessage(static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
}

template <typename TBase>
HWND BasicWindowT<TBase>::setActiveWindow() const
{
	assert(isWindow());
	return ::SetActiveWindow(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
HWND BasicWindowT<TBase>::setCapture() const
{
	assert(isWindow());
	return ::SetCapture(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
ULONG_PTR BasicWindowT<TBase>::setClassLongPtr(int index, LONG_PTR newLong) const
{
	assert(isWindow());
	return ::SetClassLongPtr(static_cast<TBase*>(this)->getWindowHandle(), index, newLong);
}

template <typename TBase>
LONG_PTR BasicWindowT<TBase>::setDlgCtrlID(UINT id) const
{
	assert(isWindow());
	return setWindowLongPtr(GWLP_ID, id);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setDlgItemInt(UINT dlgItemID, UINT value, BOOL isSigned) const
{
	assert(isWindow());
	return ::SetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), value, isSigned);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setDlgItemText(UINT dlgItemID, LPCTSTR string) const
{
	assert(isWindow());
	return ::SetDlgItemText(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), string);
}

template <typename TBase>
void BasicWindowT<TBase>::setExStyle(DWORD exStyle) const
{
	assert(isWindow());
	::SetWindowLongPtr(static_cast<TBase*>(this)->getWindowHandle(), GWL_EXSTYLE, static_cast<LONG_PTR>(exStyle));
}

template <typename TBase>
HWND BasicWindowT<TBase>::setFocus() const
{
	assert(isWindow());
	return ::SetFocus(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
void BasicWindowT<TBase>::setFont(HFONT font, BOOL redraw) const
{
	assert(isWindow());
	WPARAM wParam = reinterpret_cast<WPARAM>(font);
	LPARAM lParam = static_cast<LPARAM>(redraw);
	sendMessage(WM_SETFONT, wParam, lParam);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setForegroundWindow() const
{
	assert(isWindow());
	return ::SetForegroundWindow(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
HICON BasicWindowT<TBase>::setIcon(HICON icon, BOOL isBigIcon) const
{
	assert(isWindow());
	WPARAM wParam = static_cast<WPARAM>(isBigIcon);
	LPARAM lParam = reinterpret_cast<LPARAM>(icon);
	return reinterpret_cast<HICON>(sendMessage(WM_SETICON, wParam, lParam));
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setMenu(HMENU menu) const
{
	assert(isWindow());
	return ::SetMenu(static_cast<TBase*>(this)->getWindowHandle(), menu);
}

template <typename TBase>
HWND BasicWindowT<TBase>::setParent(HWND parent) const
{
	assert(isWindow());
	return ::SetParent(static_cast<TBase*>(this)->getWindowHandle(), parent);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setRedraw(BOOL redraw) const
{
	assert(isWindow());
	WPARAM wParam = static_cast<WPARAM>(redraw);
	return static_cast<BOOL>(::SendMessage(static_cast<TBase*>(this)->getWindowHandle(), WM_SETREDRAW, wParam, 0));
}

template <typename TBase>
int BasicWindowT<TBase>::setScrollInfo(int barType, const SCROLLINFO& si, BOOL redraw) const
{
	assert(isWindow());
	return ::SetScrollInfo(static_cast<TBase*>(this)->getWindowHandle(), barType, &si, redraw);
}

template <typename TBase>
int BasicWindowT<TBase>::setScrollPos(int barType, int pos, BOOL redraw) const
{
	assert(isWindow());
	return ::SetScrollPos(static_cast<TBase*>(this)->getWindowHandle(), barType, pos, redraw);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setScrollRange(int barType, int minPos, int maxPos, BOOL redraw) const
{
	assert(isWindow());
	return ::SetScrollRange(static_cast<TBase*>(this)->getWindowHandle(), barType, minPos, maxPos, redraw);
}

template <typename TBase>
void BasicWindowT<TBase>::setStyle(DWORD style) const
{
	assert(isWindow());
	::SetWindowLongPtr(static_cast<TBase*>(this)->getWindowHandle(), GWL_STYLE, static_cast<LONG_PTR>(style));
}

template <typename TBase>
UINT_PTR BasicWindowT<TBase>::setTimer(UINT_PTR eventID, UINT elapse, TIMERPROC pTimerFunc) const
{
	assert(isWindow());
	return ::SetTimer(static_cast<TBase*>(this)->getWindowHandle(), eventID, elapse, pTimerFunc);
}

template <typename TBase>
LONG_PTR BasicWindowT<TBase>::setWindowLongPtr(int index, LONG_PTR newLong) const
{
	assert(isWindow());
	return ::SetWindowLongPtr(static_cast<TBase*>(this)->getWindowHandle(), index, newLong);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setWindowPlacement(const WINDOWPLACEMENT& wndpl) const
{
	assert(isWindow());
	return ::SetWindowPlacement(static_cast<TBase*>(this)->getWindowHandle(), &wndpl);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setWindowPos(HWND insertAfter, int x, int y, int cx, int cy, UINT flags) const
{
	assert(isWindow());
	return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), insertAfter, x, y, cx, cy, flags);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setWindowPos(HWND insertAfter, RECT rect, UINT flags) const
{
	assert(isWindow());
	return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), insertAfter, rect.left, rect.top, rect.right - rect.left,
		rect.bottom - rect.top, flags);
}

template <typename TBase>
int BasicWindowT<TBase>::setWindowRgn(HRGN rgn, BOOL redraw) const
{
	assert(isWindow());
	int iResult = ::SetWindowRgn(static_cast<TBase*>(this)->getWindowHandle(), rgn, redraw);
	return iResult;
}

template <typename TBase>
BOOL BasicWindowT<TBase>::setWindowText(LPCTSTR text) const
{
	assert(isWindow());
	return ::SetWindowText(static_cast<TBase*>(this)->getWindowHandle(), text);
}

template <typename TBase>
HRESULT BasicWindowT<TBase>::setWindowTheme(LPCWSTR subAppName, LPCWSTR subIdList) const
{
	HRESULT result = E_NOTIMPL;
	HMODULE dll = ::GetModuleHandle(_T("uxtheme.dll"));
	if (dll != nullptr)
	{
		using pfnSetWindowTheme = HRESULT(WINAPI*)(HWND, LPCWSTR, LPCWSTR);
		pfnSetWindowTheme pfn = reinterpret_cast<pfnSetWindowTheme>(
			reinterpret_cast<void*>(::GetProcAddress(dll, "SetWindowTheme")));

		result = pfn(static_cast<TBase*>(this)->getWindowHandle(), subAppName, subIdList);
	}

	return result;
}

template <typename TBase>
BOOL BasicWindowT<TBase>::showOwnedPopups(BOOL show) const
{
	assert(isWindow());
	return ::ShowOwnedPopups(static_cast<TBase*>(this)->getWindowHandle(), show);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::showScrollBar(int bar, BOOL show) const
{
	assert(isWindow());
	return ::ShowScrollBar(static_cast<TBase*>(this)->getWindowHandle(), bar, show);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::showWindow(int showCmd) const
{
	assert(isWindow());
	return ::ShowWindow(static_cast<TBase*>(this)->getWindowHandle(), showCmd);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::showWindowAsync(int showCmd) const
{
	assert(isWindow());
	return ::ShowWindowAsync(static_cast<TBase*>(this)->getWindowHandle(), showCmd);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::updateWindow() const
{
	assert(isWindow());
	return ::UpdateWindow(static_cast<TBase*>(this)->getWindowHandle());
}

template <typename TBase>
BOOL BasicWindowT<TBase>::unlockWindowUpdate() const
{
	assert(isWindow());
	return ::LockWindowUpdate(0);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::validateRect(RECT rect) const
{
	assert(isWindow());
	return ::ValidateRect(static_cast<TBase*>(this)->getWindowHandle(), &rect);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::validateRect() const
{
	assert(isWindow());
	return ::ValidateRect(static_cast<TBase*>(this)->getWindowHandle(), nullptr);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::validateRgn(HRGN rgn) const
{
	assert(isWindow());
	return ::ValidateRgn(static_cast<TBase*>(this)->getWindowHandle(), rgn);
}

template <typename TBase>
BOOL BasicWindowT<TBase>::dragAcceptFiles(BOOL fAccept) const
{
	HMODULE dll = ::GetModuleHandle(_T("Shell32.dll"));
	if (dll != nullptr)
	{
		using pfnDragAcceptFiles = void(WINAPI*)(HWND, BOOL);
		pfnDragAcceptFiles pfn = reinterpret_cast<pfnDragAcceptFiles>(
			reinterpret_cast<void*>(::GetProcAddress(dll, "DragAcceptFiles"))
		);

		if (pfn)
		{
			pfn(static_cast<TBase*>(this)->getWindowHandle(), fAccept);
			return TRUE;
		}
	}

	return FALSE;
}

template <typename TBase>
BOOL BasicWindowT<TBase>::postRedraw(BOOL redraw) const
{
	assert(isWindow());
	WPARAM wParam = static_cast<WPARAM>(redraw);
	return static_cast<BOOL>(::PostMessage(static_cast<TBase*>(this)->getWindowHandle(), WM_SETREDRAW, wParam, 0));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




