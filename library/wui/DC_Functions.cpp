/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void fillSolidRect(HDC hdc, LPCRECT lpRect, COLORREF clr)
{
	COLORREF clrOld = ::SetBkColor(hdc, clr);


	if (clrOld != CLR_INVALID)
	{
		::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL);
		::SetBkColor(hdc, clrOld);
	}
}

void fillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr)
{
	RECT rect = { x, y, x + cx, y + cy };
	fillSolidRect(hdc, &rect, clr);
}

void draw3dRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	fillSolidRect(hdc, x, y, cx - 1, 1, clrTopLeft);
	fillSolidRect(hdc, x, y, 1, cy - 1, clrTopLeft);
	fillSolidRect(hdc, x + cx, y , -1, cy, clrBottomRight);
	fillSolidRect(hdc, x , y + cy, cx, -1, clrBottomRight);
}

void draw3dRect(HDC hdc, LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	draw3dRect(hdc, lpRect->left, lpRect->top, lpRect->right - lpRect->left, lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
}

void draw3dFrame(HDC hdc, LPCRECT lpRect, bool pressed)
{
	COLORREF color_light = RGB(0xff, 0xff, 0xff);
	COLORREF color_shadow = RGB(0xa0, 0xa0, 0xa0);
	COLORREF color_face = RGB(0xf0, 0xf0, 0xf0);

	COLORREF lt_color;
	COLORREF rb_color;


	lt_color = (pressed) ? color_shadow : color_light;
	rb_color = (pressed) ? color_light : color_shadow;

	draw3dRect(hdc, lpRect, lt_color, rb_color);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


