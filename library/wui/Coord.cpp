/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





//////////////////////////////////////////////////////////////////////////////
//============================================================================
CoordSize::CoordSize()
{
	cx = 0;
	cy = 0;
}

CoordSize::CoordSize(int initCX, int initCY)
{
	cx = initCX;
	cy = initCY;
}

CoordSize::CoordSize(SIZE initSize)
{
	*(SIZE*)this = initSize;
}

CoordSize::CoordSize(POINT initPt)
{
	*(POINT*)this = initPt;
}

CoordSize::CoordSize(DWORD dwSize)
{
	cx = (short)LOWORD(dwSize);
	cy = (short)HIWORD(dwSize);
}

bool CoordSize::operator==(SIZE size) const
{
	return (cx == size.cx && cy == size.cy);
}

bool CoordSize::operator!=(SIZE size) const
{
	return (cx != size.cx || cy != size.cy);
}

void CoordSize::operator+=(SIZE size)
{
	cx += size.cx;
	cy += size.cy;
}

void CoordSize::operator-=(SIZE size)
{
	cx -= size.cx;
	cy -= size.cy;
}

void CoordSize::setSize(int CX, int CY)
{
	cx = CX;
	cy = CY;
}

CoordSize CoordSize::operator+(SIZE size) const
{
	return CoordSize(cx + size.cx, cy + size.cy);
}

CoordSize CoordSize::operator-(SIZE size) const
{
	return CoordSize(cx - size.cx, cy - size.cy);
}

CoordSize CoordSize::operator-() const
{
	return CoordSize(-cx, -cy);
}

CoordPoint CoordSize::operator+(POINT point) const
{
	return CoordPoint(cx + point.x, cy + point.y);
}

CoordPoint CoordSize::operator-(POINT point) const
{
	return CoordPoint(cx - point.x, cy - point.y);
}

CoordRect CoordSize::operator+(const RECT* lpRect) const
{
	return CoordRect(lpRect) + *this;
}

CoordRect CoordSize::operator-(const RECT* lpRect) const
{
	return CoordRect(lpRect) - *this;
}





//////////////////////////////////////////////////////////////////////////////
//============================================================================
CoordPoint::CoordPoint()
{
	x = 0;
	y = 0;
}

CoordPoint::CoordPoint(int initX, int initY)
{
	x = initX;
	y = initY;
}

CoordPoint::CoordPoint(POINT initPt)
{
	*(POINT*)this = initPt;
}

CoordPoint::CoordPoint(SIZE initSize)
{
	*(SIZE*)this = initSize;
}

CoordPoint::CoordPoint(LPARAM dwPoint)
{
	x = (short)LOWORD(dwPoint);
	y = (short)HIWORD(dwPoint);
}

void CoordPoint::offset(int xOffset, int yOffset)
{
	x += xOffset;
	y += yOffset;
}

void CoordPoint::offset(POINT point)
{
	x += point.x;
	y += point.y;
}

void CoordPoint::offset(SIZE size)
{
	x += size.cx;
	y += size.cy;
}

void CoordPoint::setPoint(int X, int Y)
{
	x = X;
	y = Y;
}

bool CoordPoint::operator==(POINT point) const
{
	return (x == point.x && y == point.y);
}

bool CoordPoint::operator!=(POINT point) const
{
	return (x != point.x || y != point.y);
}

void CoordPoint::operator+=(SIZE size)
{
	x += size.cx;
	y += size.cy;
}

void CoordPoint::operator-=(SIZE size)
{
	x -= size.cx;
	y -= size.cy;
}

void CoordPoint::operator+=(POINT point)
{
	x += point.x;
	y += point.y;
}

void CoordPoint::operator-=(POINT point)
{
	x -= point.x;
	y -= point.y;
}

CoordPoint CoordPoint::operator+(SIZE size) const
{
	return CoordPoint(x + size.cx, y + size.cy);
}

CoordPoint CoordPoint::operator-(SIZE size) const
{
	return CoordPoint(x - size.cx, y - size.cy);
}

CoordPoint CoordPoint::operator-() const
{
	return CoordPoint(-x, -y);
}

CoordPoint CoordPoint::operator+(POINT point) const
{
	return CoordPoint(x + point.x, y + point.y);
}

CoordSize CoordPoint::operator-(POINT point) const
{
	return CoordSize(x - point.x, y - point.y);
}

CoordRect CoordPoint::operator+(const RECT* lpRect) const
{
	return CoordRect(lpRect) + *this;
}

CoordRect CoordPoint::operator-(const RECT* lpRect) const
{
	return CoordRect(lpRect) - *this;
}





//////////////////////////////////////////////////////////////////////////////
//============================================================================
CoordRect::CoordRect()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

CoordRect::CoordRect(int l, int t, int r, int b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

CoordRect::CoordRect(const RECT& srcRect)
{
	::CopyRect(this, &srcRect);
}

CoordRect::CoordRect(LPCRECT lpSrcRect)
{
	::CopyRect(this, lpSrcRect);
}

CoordRect::CoordRect(POINT point, SIZE size)
{
	right = (left = point.x) + size.cx;
	bottom = (top = point.y) + size.cy;
}

CoordRect::CoordRect(POINT topLeft, POINT bottomRight)
{
	left = topLeft.x;
	top = topLeft.y;
	right = bottomRight.x;
	bottom = bottomRight.y;
}

int CoordRect::width() const
{
	return right - left;
}

int CoordRect::height() const
{
	return bottom - top;
}

CoordSize CoordRect::size() const
{
	return CoordSize(right - left, bottom - top);
}

CoordPoint& CoordRect::topLeft()
{
	return *((CoordPoint*)this);
}

CoordPoint& CoordRect::bottomRight()
{
	return *((CoordPoint*)this + 1);
}

const CoordPoint& CoordRect::topLeft() const
{
	return *((CoordPoint*)this);
}

const CoordPoint& CoordRect::bottomRight() const
{
	return *((CoordPoint*)this + 1);
}

CoordPoint CoordRect::centerPoint() const
{
	return CoordPoint((left + right) / 2, (top + bottom) / 2);
}

void CoordRect::swapLeftRight ()
{
	swapLeftRight(LPRECT(this));
}

void WINAPI CoordRect::swapLeftRight(LPRECT lpRect)
{
	LONG temp = lpRect->left;
	lpRect->left = lpRect->right;
	lpRect->right = temp;
}

CoordRect::operator LPRECT()
{
	return this;
}

CoordRect::operator LPCRECT() const
{
	return this;
}

bool CoordRect::isRectEmpty() const
{
	return ::IsRectEmpty(this);
}

bool CoordRect::isRectNull() const
{
	return (left == 0 && right == 0 && top == 0 && bottom == 0);
}

bool CoordRect::ptInRect(POINT point) const
{
	return ::PtInRect(this, point);
}

void CoordRect::setRect(int x1, int y1, int x2, int y2)
{
	::SetRect(this, x1, y1, x2, y2);
}

void CoordRect::setRect(POINT topLeft, POINT bottomRight)
{
	::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}

void CoordRect::setRectEmpty()
{
	::SetRectEmpty(this);
}

void CoordRect::copyRect(LPCRECT lpSrcRect)
{
	::CopyRect(this, lpSrcRect);
}

bool CoordRect::equalRect(LPCRECT lpRect) const
{
	return ::EqualRect(this, lpRect);
}

void CoordRect::inflateRect(int x, int y)
{
	::InflateRect(this, x, y);
}

void CoordRect::inflateRect(SIZE size)
{
	::InflateRect(this, size.cx, size.cy);
}

void CoordRect::deflateRect(int x, int y)
{
	::InflateRect(this, -x, -y);
}

void CoordRect::deflateRect(SIZE size)
{
	::InflateRect(this, -size.cx, -size.cy);
}

void CoordRect::offsetRect(int x, int y)
{
	::OffsetRect(this, x, y);
}

void CoordRect::offsetRect(POINT point)
{
	::OffsetRect(this, point.x, point.y);
}

void CoordRect::offsetRect(SIZE size)
{
	::OffsetRect(this, size.cx, size.cy);
}

void CoordRect::moveToY(int y)
{
	bottom = height() + y;
	top = y;
}

void CoordRect::moveToX(int x)
{
	right = width() + x;
	left = x;
}

void CoordRect::moveToXY(int x, int y)
{
	moveToX(x);
	moveToY(y);
}

void CoordRect::moveToXY(POINT pt)
{
	moveToX(pt.x);
	moveToY(pt.y);
}

bool CoordRect::intersectRect(LPCRECT lpRect1, LPCRECT lpRect2)
{
	return ::IntersectRect(this, lpRect1, lpRect2);
}

bool CoordRect::unionRect(LPCRECT lpRect1, LPCRECT lpRect2)
{
	return ::UnionRect(this, lpRect1, lpRect2);
}

void CoordRect::operator=(const RECT& srcRect)
{
	::CopyRect(this, &srcRect);
}

bool CoordRect::operator==(const RECT& rect) const
{
	return ::EqualRect(this, &rect);
}

bool CoordRect::operator!=(const RECT& rect) const
{
	return !::EqualRect(this, &rect);
}

void CoordRect::operator+=(POINT point)
{
	::OffsetRect(this, point.x, point.y);
}

void CoordRect::operator+=(SIZE size)
{
	::OffsetRect(this, size.cx, size.cy);
}

void CoordRect::operator+=(LPCRECT lpRect)
{
	inflateRect(lpRect);
}

void CoordRect::operator-=(POINT point)
{
	::OffsetRect(this, -point.x, -point.y);
}

void CoordRect::operator-=(SIZE size)
{
	::OffsetRect(this, -size.cx, -size.cy);
}

void CoordRect::operator-=(LPCRECT lpRect)
{
	deflateRect(lpRect);
}

void CoordRect::operator&=(const RECT& rect)
{
	::IntersectRect(this, this, &rect);
}

void CoordRect::operator|=(const RECT& rect)
{
	::UnionRect(this, this, &rect);
}

CoordRect CoordRect::operator+(POINT pt) const
{
	CoordRect rect(*this);
	::OffsetRect(&rect, pt.x, pt.y);
	return rect;
}

CoordRect CoordRect::operator-(POINT pt) const
{
	CoordRect rect(*this);
	::OffsetRect(&rect, -pt.x, -pt.y);
	return rect;
}

CoordRect CoordRect::operator+(SIZE size) const
{
	CoordRect rect(*this);
	::OffsetRect(&rect, size.cx, size.cy);
	return rect;
}

CoordRect CoordRect::operator-(SIZE size) const
{
	CoordRect rect(*this);
	::OffsetRect(&rect, -size.cx, -size.cy);
	return rect;
}

CoordRect CoordRect::operator+(LPCRECT lpRect) const
{
	CoordRect rect(this);
	rect.inflateRect(lpRect);
	return rect;
}

CoordRect CoordRect::operator-(LPCRECT lpRect) const
{
	CoordRect rect(this);
	rect.deflateRect(lpRect);
	return rect;
}

CoordRect CoordRect::operator&(const RECT& rect2) const
{
	CoordRect rect;
	::IntersectRect(&rect, this, &rect2);
	return rect;
}

CoordRect CoordRect::operator|(const RECT& rect2) const
{
	CoordRect rect;
	::UnionRect(&rect, this, &rect2);
	return rect;
}

bool CoordRect::subtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2)
{
	return ::SubtractRect(this, lpRectSrc1, lpRectSrc2);
}

void CoordRect::normalizeRect()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

void CoordRect::inflateRect(LPCRECT lpRect)
{
	left -= lpRect->left;
	top -= lpRect->top;
	right += lpRect->right;
	bottom += lpRect->bottom;
}

void CoordRect::inflateRect(int l, int t, int r, int b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

void CoordRect::deflateRect(LPCRECT lpRect)
{
	left += lpRect->left;
	top += lpRect->top;
	right -= lpRect->right;
	bottom -= lpRect->bottom;
}

void CoordRect::deflateRect(int l, int t, int r, int b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

CoordRect CoordRect::mulDiv(int nMultiplier, int nDivisor) const
{
	return CoordRect(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


