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
	HDC beginPaint(PAINTSTRUCT& ps) const
	{
		assert(isWindow());
		return ::BeginPaint(static_cast<TBase*>(this)->getWindowHandle(), &ps);
	}
	BOOL bringWindowToTop() const
	{
		assert(isWindow());
		return ::BringWindowToTop(static_cast<TBase*>(this)->getWindowHandle());
	}
	LRESULT callWindowProc(WNDPROC pPrevWndFunc, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		assert(isWindow());
		return ::CallWindowProc(pPrevWndFunc, static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
	}
	BOOL checkDlgButton(UINT buttonID, UINT check) const
	{
		assert(isWindow());
		return ::CheckDlgButton(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(buttonID), check);
	}
	BOOL checkRadioButton(UINT firstButtonID, UINT lastButtonID, UINT checkButtonID) const
	{
		assert(isWindow());
		return ::CheckRadioButton(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(firstButtonID),
			static_cast<int>(lastButtonID), static_cast<int>(checkButtonID));
	}
	HWND childWindowFromPoint(POINT point) const
	{
		assert(isWindow());
		return ::ChildWindowFromPoint(static_cast<TBase*>(this)->getWindowHandle(), point);
	}
	BOOL clientToScreen(POINT& point) const
	{
		assert(isWindow());
		return ::ClientToScreen(static_cast<TBase*>(this)->getWindowHandle(), &point);
	}
	BOOL clientToScreen(RECT& rect) const
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
	void close() const
	{
		assert(isWindow());
		postMessage(WM_CLOSE);
	}
	BOOL closeWindow() const
	{
		assert(isWindow());
		return ::CloseWindow(static_cast<TBase*>(this)->getWindowHandle());
	}
	HDWP deferWindowPos(HDWP winPosInfo, HWND insertAfter, int x, int y, int cx, int cy, UINT flags) const
	{
		assert(isWindow());
		return ::DeferWindowPos(winPosInfo, static_cast<TBase*>(this)->getWindowHandle(), insertAfter, x, y, cx, cy, flags);
	}
	HDWP deferWindowPos(HDWP winPosInfo, HWND insertAfter, RECT rect, UINT flags) const
	{
		assert(isWindow());
		return ::DeferWindowPos(winPosInfo, static_cast<TBase*>(this)->getWindowHandle(), insertAfter, rect.left,
			rect.top, rect.right - rect.left, rect.bottom - rect.top, flags);
	}
	LRESULT defWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		assert(isWindow());
		return ::DefWindowProc(static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
	}
	int dlgDirList(LPTSTR pathSpec, UINT listBoxID, UINT staticPathID, UINT fileType) const
	{
		assert(isWindow());
		return ::DlgDirList(static_cast<TBase*>(this)->getWindowHandle(), pathSpec, static_cast<int>(listBoxID),
			static_cast<int>(staticPathID), fileType);
	}
	int dlgDirListComboBox(LPTSTR pathSpec, UINT comboBoxID, UINT staticPathID, UINT fileType) const
	{
		assert(isWindow());
		return ::DlgDirListComboBox(static_cast<TBase*>(this)->getWindowHandle(), pathSpec, static_cast<int>(comboBoxID),
			static_cast<int>(staticPathID), fileType);
	}
	BOOL dlgDirSelectEx(LPTSTR string, int count, UINT listBoxID) const
	{
		assert(isWindow());
		return ::DlgDirSelectEx(static_cast<TBase*>(this)->getWindowHandle(), string, count, static_cast<int>(listBoxID));
	}
	BOOL dlgDirSelectComboBoxEx(LPTSTR string, int count, UINT comboBoxID) const
	{
		assert(isWindow());
		return ::DlgDirSelectComboBoxEx(static_cast<TBase*>(this)->getWindowHandle(), string, count, static_cast<int>(comboBoxID));
	}
	BOOL drawAnimatedRects(UINT aniID, RECT from, RECT to) const
	{
		assert(isWindow());
		return ::DrawAnimatedRects(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(aniID), &from, &to);
	}
	BOOL drawCaption(HDC dc, RECT rect, UINT flags) const
	{
		assert(isWindow());
		return ::DrawCaption(static_cast<TBase*>(this)->getWindowHandle(), dc, &rect, flags);
	}
	BOOL drawMenuBar() const
	{
		assert(isWindow());
		return ::DrawMenuBar(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL enableScrollBar(UINT flags, UINT arrows) const
	{
		assert(isWindow());
		return ::EnableScrollBar(static_cast<TBase*>(this)->getWindowHandle(), flags, arrows);
	}
	BOOL enableWindow(BOOL enable = TRUE) const
	{
		assert(isWindow());
		return ::EnableWindow(static_cast<TBase*>(this)->getWindowHandle(), enable);
	}
	BOOL endPaint(PAINTSTRUCT& ps) const
	{
		assert(isWindow());
		return ::EndPaint(static_cast<TBase*>(this)->getWindowHandle(), &ps);
	}
	HWND getActiveWindow() const
	{
		return ::GetActiveWindow();
	}
	HWND getCapture() const
	{
		return ::GetCapture();
	}
	ULONG_PTR getClassLongPtr(int index) const
	{
		assert(isWindow());
		return ::GetClassLongPtr(static_cast<TBase*>(this)->getWindowHandle(), index);
	}
	CoordRect getClientRect() const
	{
		assert(isWindow());
		CoordRect rc;
		::GetClientRect(static_cast<TBase*>(this)->getWindowHandle(), &rc);
		return rc;
	}
	HWND getDesktopWindow() const
	{
		return HWND(::GetDesktopWindow());
	}
	UINT getDlgCtrlID() const
	{
		assert(isWindow());
		return static_cast<UINT>(::GetDlgCtrlID(static_cast<TBase*>(this)->getWindowHandle()));
	}
	HWND getDlgItem(UINT dlgItemID) const
	{
		assert(isWindow());
		return ::GetDlgItem(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID));
	}
	UINT getDlgItemInt(UINT dlgItemID, BOOL& isTranslated, BOOL isSigned) const
	{
		assert(isWindow());
		return ::GetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), &isTranslated, isSigned);
	}
	UINT getDlgItemInt(UINT dlgItemID, BOOL isSigned) const
	{
		assert(isWindow());
		return ::GetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), nullptr, isSigned);
	}
	DWORD getExStyle() const
	{
		assert(isWindow());
		return static_cast<DWORD>(getWindowLongPtr(GWL_EXSTYLE));
	}
	HWND getFocus() const
	{
		return HWND(::GetFocus());
	}
	HFONT getFont() const
	{
		assert(isWindow());
		return reinterpret_cast<HFONT>(sendMessage(WM_GETFONT, 0, 0));
	}
	HICON getIcon(BOOL isBigIcon) const
	{
		assert(isWindow());
		WPARAM wParam = static_cast<WPARAM>(isBigIcon);
		return reinterpret_cast<HICON>(sendMessage(WM_GETICON, wParam, 0));
	}
	HWND getLastActivePopup() const
	{
		assert(isWindow());
		return ::GetLastActivePopup(static_cast<TBase*>(this)->getWindowHandle());
	}
	HMENU getMenu() const
	{
		assert(isWindow());
		return ::GetMenu(static_cast<TBase*>(this)->getWindowHandle());
	}
	HWND getNextDlgGroupItem(HWND control, BOOL isPrevious) const
	{
		assert(isWindow());
		return ::GetNextDlgGroupItem(static_cast<TBase*>(this)->getWindowHandle(), control, isPrevious);
	}
	HWND getNextDlgTabItem(HWND control, BOOL isPrevious) const
	{
		assert(isWindow());
		return ::GetNextDlgTabItem(static_cast<TBase*>(this)->getWindowHandle(), control, isPrevious);
	}
	HWND getParent() const
	{
		assert(isWindow());
		return ::GetParent(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL getScrollInfo(int barType, SCROLLINFO& si) const
	{
		assert(isWindow());
		return ::GetScrollInfo(static_cast<TBase*>(this)->getWindowHandle(), barType, &si);
	}
	int getScrollPos(int bar) const
	{
		assert(isWindow());
		return ::GetScrollPos(static_cast<TBase*>(this)->getWindowHandle(), bar);
	}
	BOOL getScrollRange(int bar, int& minPos, int& maxPos) const
	{
		assert(isWindow());
		return ::GetScrollRange(static_cast<TBase*>(this)->getWindowHandle(), bar, &minPos, &maxPos);
	}
	DWORD getStyle() const
	{
		assert(isWindow());
		return static_cast<DWORD>(getWindowLongPtr(GWL_STYLE));
	}
	HMENU getSystemMenu(BOOL revert) const
	{
		assert(isWindow());
		return ::GetSystemMenu(static_cast<TBase*>(this)->getWindowHandle(), revert);
	}
	HWND getTopWindow() const
	{
		assert(isWindow());
		return ::GetTopWindow(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL getWindowPlacement(WINDOWPLACEMENT& wp) const
	{
		assert(isWindow());
		return ::GetWindowPlacement(static_cast<TBase*>(this)->getWindowHandle(), &wp);
	}
	CoordRect getUpdateRect(BOOL erase) const
	{
		assert(isWindow());
		CoordRect rc;
		::GetUpdateRect(static_cast<TBase*>(this)->getWindowHandle(), &rc, erase);
		return rc;
	}
	int getUpdateRgn(HRGN rgn, BOOL erase) const
	{
		assert(isWindow());
		return ::GetUpdateRgn(static_cast<TBase*>(this)->getWindowHandle(), rgn, erase);
	}
	HWND getWindow(UINT cmd) const
	{
		assert(isWindow());
		return ::GetWindow(static_cast<TBase*>(this)->getWindowHandle(), cmd);
	}
	LONG_PTR getWindowLongPtr(int index) const
	{
		assert(isWindow());
		return ::GetWindowLongPtr(static_cast<TBase*>(this)->getWindowHandle(), index);
	}
	CoordRect getWindowRect() const
	{
		assert(isWindow());
		CoordRect rc;
		::GetWindowRect(static_cast<TBase*>(this)->getWindowHandle(), &rc);
		return rc;
	}
	int getWindowTextLength() const
	{
		assert(isWindow());
		return ::GetWindowTextLength(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL hiliteMenuItem(HMENU menu, UINT itemID, UINT hilite) const
	{
		assert(isWindow());
		return ::HiliteMenuItem(static_cast<TBase*>(this)->getWindowHandle(), menu, itemID, hilite);
	}
	void invalidate(BOOL erase = TRUE) const
	{
		assert(isWindow());
		::InvalidateRect(static_cast<TBase*>(this)->getWindowHandle(), nullptr, erase);
	}
	BOOL invalidateRect(RECT rect, BOOL erase = TRUE) const
	{
		assert(isWindow());
		return ::InvalidateRect(static_cast<TBase*>(this)->getWindowHandle(), &rect, erase);
	}
	BOOL invalidateRect(BOOL erase = TRUE) const
	{
		assert(isWindow());
		return ::InvalidateRect(static_cast<TBase*>(this)->getWindowHandle(), nullptr, erase);
	}
	BOOL invalidateRgn(HRGN rgn, BOOL erase = TRUE) const
	{
		assert(isWindow());
		return ::InvalidateRgn(static_cast<TBase*>(this)->getWindowHandle(), rgn, erase);
	}
	BOOL isChild(HWND child) const
	{
		assert(isWindow());
		return ::IsChild(static_cast<TBase*>(this)->getWindowHandle(), child);
	}
	BOOL isDialogMessage(MSG& uMsg) const
	{
		assert(isWindow());
		return ::IsDialogMessage(static_cast<TBase*>(this)->getWindowHandle(), &uMsg);
	}
	UINT isDlgButtonChecked(UINT buttonID) const
	{
		assert(isWindow());
		return ::IsDlgButtonChecked(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(buttonID));
	}
	BOOL isWindowEnabled() const
	{
		assert(isWindow());
		return ::IsWindowEnabled(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL isIconic() const
	{
		assert(isWindow());
		return ::IsIconic(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL isWindow() const
	{
		return ::IsWindow(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL isWindowVisible() const
	{
		assert(isWindow());
		return (getStyle() & WS_VISIBLE) != 0;
	}
	BOOL isZoomed() const
	{
		assert(isWindow());
		return ::IsZoomed(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL killTimer(UINT_PTR eventID) const
	{
		assert(isWindow());
		return ::KillTimer(static_cast<TBase*>(this)->getWindowHandle(), eventID);
	}
	BOOL lockWindowUpdate() const
	{
		assert(isWindow());
		return ::LockWindowUpdate(static_cast<TBase*>(this)->getWindowHandle());
	}
	int mapWindowPoints(HWND to, POINT& point) const
	{
		assert(isWindow());
		return ::MapWindowPoints(static_cast<TBase*>(this)->getWindowHandle(), to, &point, 1);
	}
	int mapWindowPoints(HWND to, RECT& rect) const
	{
		assert(isWindow());
		return ::MapWindowPoints(static_cast<TBase*>(this)->getWindowHandle(), to, reinterpret_cast<LPPOINT>(&rect), 2);
	}
	int mapWindowPoints(HWND to, LPPOINT pointsArray, UINT count) const
	{
		assert(isWindow());
		return ::MapWindowPoints(static_cast<TBase*>(this)->getWindowHandle(), to, reinterpret_cast<LPPOINT>(pointsArray), count);
	}
	int messageBox(LPCTSTR text, LPCTSTR caption, UINT type = MB_OK | MB_ICONASTERISK) const
	{
		assert(isWindow());
		return ::MessageBox(static_cast<TBase*>(this)->getWindowHandle(), text, caption, type);
	}
	BOOL moveWindow(int x, int y, int width, int height, BOOL repaint = TRUE) const
	{
		assert(isWindow());
		return ::MoveWindow(static_cast<TBase*>(this)->getWindowHandle(), x, y, width, height, repaint = TRUE);
	}
	BOOL moveWindow(RECT rect, BOOL repaint = TRUE) const
	{
		assert(isWindow());
		return ::MoveWindow(static_cast<TBase*>(this)->getWindowHandle(), rect.left, rect.top, rect.right - rect.left,
			rect.bottom - rect.top, repaint);
	}
	BOOL postMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const
	{
		assert(isWindow());
		return ::PostMessage(static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
	}
	BOOL openIcon() const
	{
		assert(isWindow());
		return ::OpenIcon(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL postMessage(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		assert(isWindow());
		return ::PostMessage(wnd, uMsg, wParam, lParam);
	}
	void print(HDC dc, DWORD flags) const
	{
		assert(isWindow());
		WPARAM wParam = reinterpret_cast<WPARAM>(dc);
		LPARAM lParam = static_cast<LPARAM>(flags);
		SendMessage(static_cast<TBase*>(this)->getWindowHandle(), WM_PRINT, wParam, lParam);
	}
	BOOL redrawWindow(RECT updateRect, UINT flags) const
	{
		assert(isWindow());
		return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), &updateRect, nullptr, flags);
	}
	BOOL redrawWindow(HRGN rgn, UINT flags) const
	{
		assert(isWindow());
		return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), nullptr, rgn, flags);
	}
	BOOL redrawWindow(UINT flags) const
	{
		assert(isWindow());
		return ::RedrawWindow(static_cast<TBase*>(this)->getWindowHandle(), nullptr, nullptr, flags);
	}
	int releaseDC(HDC dc) const
	{
		assert(isWindow());
		return ::ReleaseDC(static_cast<TBase*>(this)->getWindowHandle(), dc);
	}
	BOOL screenToClient(POINT& point) const
	{
		assert(isWindow());
		return ::ScreenToClient(static_cast<TBase*>(this)->getWindowHandle(), &point);
	}
	BOOL screenToClient(RECT& rect) const
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
	BOOL scrollWindow(int xAmount, int yAmount, RECT scrollRect, LPCRECT pClipRect) const
	{
		assert(isWindow());
		return ::ScrollWindow(static_cast<TBase*>(this)->getWindowHandle(), xAmount, yAmount, &scrollRect, pClipRect);
	}
	BOOL scrollWindow(int xAmount, int yAmount, LPCRECT pClipRect) const
	{
		assert(isWindow());
		return ::ScrollWindow(static_cast<TBase*>(this)->getWindowHandle(), xAmount, yAmount, nullptr, pClipRect);
	}
	int scrollWindowEx(int dx, int dy, LPCRECT pScrollRect, LPCRECT pClipRect,
		HRGN update, LPRECT pUpdateRect, UINT flags) const
	{
		assert(isWindow());
		return ::ScrollWindowEx(static_cast<TBase*>(this)->getWindowHandle(), dx, dy, pScrollRect, pClipRect, update, pUpdateRect, flags);
	}
	LRESULT sendDlgItemMessage(UINT dlgItemID, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		assert(isWindow());
		return ::SendDlgItemMessage(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), uMsg, wParam, lParam);
	}
	LRESULT sendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const
	{
		assert(isWindow());
		return ::SendMessage(static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
	}
	LRESULT sendMessage(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		assert(isWindow());
		return ::SendMessage(wnd, uMsg, wParam, lParam);
	}
	BOOL sendNotifyMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) const
	{
		assert(isWindow());
		return ::SendNotifyMessage(static_cast<TBase*>(this)->getWindowHandle(), uMsg, wParam, lParam);
	}
	HWND setActiveWindow() const
	{
		assert(isWindow());
		return ::SetActiveWindow(static_cast<TBase*>(this)->getWindowHandle());
	}
	HWND setCapture() const
	{
		assert(isWindow());
		return ::SetCapture(static_cast<TBase*>(this)->getWindowHandle());
	}
	ULONG_PTR setClassLongPtr(int index, LONG_PTR newLong) const
	{
		assert(isWindow());
		return ::SetClassLongPtr(static_cast<TBase*>(this)->getWindowHandle(), index, newLong);
	}
	LONG_PTR setDlgCtrlID(UINT id) const
	{
		assert(isWindow());
		return setWindowLongPtr(GWLP_ID, id);
	}
	BOOL setDlgItemInt(UINT dlgItemID, UINT value, BOOL isSigned) const
	{
		assert(isWindow());
		return ::SetDlgItemInt(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), value, isSigned);
	}
	BOOL setDlgItemText(UINT dlgItemID, LPCTSTR string) const
	{
		assert(isWindow());
		return ::SetDlgItemText(static_cast<TBase*>(this)->getWindowHandle(), static_cast<int>(dlgItemID), string);
	}
	void setExStyle(DWORD exStyle) const
	{
		assert(isWindow());
		::SetWindowLongPtr(static_cast<TBase*>(this)->getWindowHandle(), GWL_EXSTYLE, static_cast<LONG_PTR>(exStyle));
	}
	HWND setFocus() const
	{
		assert(isWindow());
		return ::SetFocus(static_cast<TBase*>(this)->getWindowHandle());
	}
	void setFont(HFONT font, BOOL redraw = TRUE) const
	{
		assert(isWindow());
		WPARAM wParam = reinterpret_cast<WPARAM>(font);
		LPARAM lParam = static_cast<LPARAM>(redraw);
		sendMessage(WM_SETFONT, wParam, lParam);
	}
	BOOL setForegroundWindow() const
	{
		assert(isWindow());
		return ::SetForegroundWindow(static_cast<TBase*>(this)->getWindowHandle());
	}
	HICON setIcon(HICON icon, BOOL isBigIcon) const
	{
		assert(isWindow());
		WPARAM wParam = static_cast<WPARAM>(isBigIcon);
		LPARAM lParam = reinterpret_cast<LPARAM>(icon);
		return reinterpret_cast<HICON>(sendMessage(WM_SETICON, wParam, lParam));
	}
	BOOL setMenu(HMENU menu) const
	{
		assert(isWindow());
		return ::SetMenu(static_cast<TBase*>(this)->getWindowHandle(), menu);
	}
	HWND setParent(HWND parent) const
	{
		assert(isWindow());
		return ::SetParent(static_cast<TBase*>(this)->getWindowHandle(), parent);
	}
	BOOL setRedraw(BOOL redraw = TRUE) const
	{
		assert(isWindow());
		WPARAM wParam = static_cast<WPARAM>(redraw);
		return static_cast<BOOL>(::SendMessage(static_cast<TBase*>(this)->getWindowHandle(), WM_SETREDRAW, wParam, 0));
	}
	int setScrollInfo(int barType, const SCROLLINFO& si, BOOL redraw) const
	{
		assert(isWindow());
		return ::SetScrollInfo(static_cast<TBase*>(this)->getWindowHandle(), barType, &si, redraw);
	}
	int setScrollPos(int barType, int pos, BOOL redraw) const
	{
		assert(isWindow());
		return ::SetScrollPos(static_cast<TBase*>(this)->getWindowHandle(), barType, pos, redraw);
	}
	BOOL setScrollRange(int barType, int minPos, int maxPos, BOOL redraw) const
	{
		assert(isWindow());
		return ::SetScrollRange(static_cast<TBase*>(this)->getWindowHandle(), barType, minPos, maxPos, redraw);
	}
	void setStyle(DWORD style) const
	{
		assert(isWindow());
		::SetWindowLongPtr(static_cast<TBase*>(this)->getWindowHandle(), GWL_STYLE, static_cast<LONG_PTR>(style));
	}
	UINT_PTR setTimer(UINT_PTR eventID, UINT elapse, TIMERPROC pTimerFunc) const
	{
		assert(isWindow());
		return ::SetTimer(static_cast<TBase*>(this)->getWindowHandle(), eventID, elapse, pTimerFunc);
	}
	LONG_PTR setWindowLongPtr(int index, LONG_PTR newLong) const
	{
		assert(isWindow());
		return ::SetWindowLongPtr(static_cast<TBase*>(this)->getWindowHandle(), index, newLong);
	}
	BOOL setWindowPlacement(const WINDOWPLACEMENT& wndpl) const
	{
		assert(isWindow());
		return ::SetWindowPlacement(static_cast<TBase*>(this)->getWindowHandle(), &wndpl);
	}
	BOOL setWindowPos(HWND insertAfter, int x, int y, int cx, int cy, UINT flags) const
	{
		assert(isWindow());
		return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), insertAfter, x, y, cx, cy, flags);
	}
	BOOL setWindowPos(HWND insertAfter, RECT rect, UINT flags) const
	{
		assert(isWindow());
		return ::SetWindowPos(static_cast<TBase*>(this)->getWindowHandle(), insertAfter, rect.left, rect.top, rect.right - rect.left,
			rect.bottom - rect.top, flags);
	}
	int setWindowRgn(HRGN rgn, BOOL redraw = TRUE) const
	{
		assert(isWindow());
		int iResult = ::SetWindowRgn(static_cast<TBase*>(this)->getWindowHandle(), rgn, redraw);
		return iResult;
	}
	BOOL setWindowText(LPCTSTR text) const
	{
		assert(isWindow());
		return ::SetWindowText(static_cast<TBase*>(this)->getWindowHandle(), text);
	}
	HRESULT setWindowTheme(LPCWSTR subAppName, LPCWSTR subIdList) const
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
	BOOL showOwnedPopups(BOOL show) const
	{
		assert(isWindow());
		return ::ShowOwnedPopups(static_cast<TBase*>(this)->getWindowHandle(), show);
	}
	BOOL showScrollBar(int bar, BOOL show) const
	{
		assert(isWindow());
		return ::ShowScrollBar(static_cast<TBase*>(this)->getWindowHandle(), bar, show);
	}
	BOOL showWindow(int showCmd = SW_SHOWNORMAL) const
	{
		assert(isWindow());
		return ::ShowWindow(static_cast<TBase*>(this)->getWindowHandle(), showCmd);
	}
	BOOL showWindowAsync(int showCmd) const
	{
		assert(isWindow());
		return ::ShowWindowAsync(static_cast<TBase*>(this)->getWindowHandle(), showCmd);
	}
	BOOL updateWindow() const
	{
		assert(isWindow());
		return ::UpdateWindow(static_cast<TBase*>(this)->getWindowHandle());
	}
	BOOL unlockWindowUpdate() const
	{
		assert(isWindow());
		return ::LockWindowUpdate(0);
	}
	BOOL validateRect(RECT rect) const
	{
		assert(isWindow());
		return ::ValidateRect(static_cast<TBase*>(this)->getWindowHandle(), &rect);
	}
	BOOL validateRect() const
	{
		assert(isWindow());
		return ::ValidateRect(static_cast<TBase*>(this)->getWindowHandle(), nullptr);
	}
	BOOL validateRgn(HRGN rgn) const
	{
		assert(isWindow());
		return ::ValidateRgn(static_cast<TBase*>(this)->getWindowHandle(), rgn);
	}

	BOOL dragAcceptFiles(BOOL fAccept) const
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
	BOOL postRedraw(BOOL redraw = TRUE) const
	{
		assert(isWindow());
		WPARAM wParam = static_cast<WPARAM>(redraw);
		return static_cast<BOOL>(::PostMessage(static_cast<TBase*>(this)->getWindowHandle(), WM_SETREDRAW, wParam, 0));
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




