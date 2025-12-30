#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
LRESULT sendWindowMessage(const HWND hwnd, WindowMessage& windowMessage);
LRESULT sendWindowMessage(WindowMessage& windowMessage);

BOOL postWindowMessage(const HWND hwnd, WindowMessage& windowMessage);
BOOL postWindowMessage(WindowMessage& windowMessage);

//===========================================================================
std::wstring getWindowText(const HWND hwnd);
void         setWindowText(const HWND hwnd, const std::wstring& s);

std::string getWindowTextA(const HWND hwnd);
void        setWindowTextA(const HWND hwnd, const std::string& s);

//===========================================================================
void moveWindow(const HWND hwnd, const RECT& rect, bool repaint = false);
void setWindowPos(const HWND hwnd, const HWND insertAfter, const RECT& rect, UINT flags);
bool centerWindow(HWND hwndWindow);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




