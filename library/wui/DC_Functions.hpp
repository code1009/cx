#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void fillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr);
void fillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr);
void draw3dRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight);
void draw3dRect(HDC hdc, LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
void draw3dFrame(HDC hdc, LPCRECT lpRect, bool pressed);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


