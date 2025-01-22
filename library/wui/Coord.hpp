#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class CoordSize;
class CoordPoint;
class CoordRect;





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class CoordSize : public tagSIZE
{
public:
	CoordSize();
	CoordSize(int initCX, int initCY);
	CoordSize(SIZE initSize);
	CoordSize(POINT initPt);
	CoordSize(DWORD dwSize);

	bool operator==(SIZE size) const;
	bool operator!=(SIZE size) const;
	void operator+=(SIZE size);
	void operator-=(SIZE size);

	void setSize(int CX, int CY);

	CoordSize operator+(SIZE size) const;
	CoordSize operator-(SIZE size) const;
	CoordSize operator-() const;

	CoordPoint operator+(POINT point) const;
	CoordPoint operator-(POINT point) const;

	CoordRect operator+(const RECT* lpRect) const;
	CoordRect operator-(const RECT* lpRect) const;
};





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class CoordPoint : public tagPOINT
{
public:
	CoordPoint();
	CoordPoint(int initX, int initY);
	CoordPoint(POINT initPt);
	CoordPoint(SIZE initSize);
	CoordPoint(LPARAM dwPoint);

	void offset(int xOffset, int yOffset);
	void offset(POINT point);
	void offset(SIZE size);
	void setPoint(int X, int Y);

	bool operator==(POINT point) const;
	bool operator!=(POINT point) const;
	void operator+=(SIZE size);
	void operator-=(SIZE size);
	void operator+=(POINT point);
	void operator-=(POINT point);

	CoordPoint operator+(SIZE size) const;
	CoordPoint operator-(SIZE size) const;
	CoordPoint operator-() const;
	CoordPoint operator+(POINT point) const;

	CoordSize operator-(POINT point) const;

	CoordRect operator+(const RECT* lpRect) const;
	CoordRect operator-(const RECT* lpRect) const;
};





//////////////////////////////////////////////////////////////////////////////
//============================================================================
class CoordRect : public tagRECT
{
public:
	CoordRect();
	CoordRect(int l, int t, int r, int b);
	CoordRect(const RECT& srcRect);
	CoordRect(LPCRECT lpSrcRect);
	CoordRect(POINT point, SIZE size);
	CoordRect(POINT topLeft, POINT bottomRight);

	int width() const;
	int height() const;

	CoordSize size() const;

	CoordPoint& topLeft();
	CoordPoint& bottomRight();
	const CoordPoint& topLeft() const;
	const CoordPoint& bottomRight() const;
	CoordPoint centerPoint() const;

	void swapLeftRight();
	static void WINAPI swapLeftRight(LPRECT lpRect);

	operator LPRECT();
	operator LPCRECT() const;

	bool isRectEmpty() const;
	bool isRectNull() const;
	bool ptInRect(POINT point) const;

	void setRect(int x1, int y1, int x2, int y2);
	void setRect(POINT topLeft, POINT bottomRight);
	void setRectEmpty();
	void copyRect(LPCRECT lpSrcRect);
	bool equalRect(LPCRECT lpRect) const;

	void inflateRect(int x, int y);
	void inflateRect(SIZE size);
	void inflateRect(LPCRECT lpRect);
	void inflateRect(int l, int t, int r, int b);

	void deflateRect(int x, int y);
	void deflateRect(SIZE size);
	void deflateRect(LPCRECT lpRect);
	void deflateRect(int l, int t, int r, int b);

	void offsetRect(int x, int y);
	void offsetRect(SIZE size);
	void offsetRect(POINT point);
	void normalizeRect();

	void moveToY(int y);
	void moveToX(int x);
	void moveToXY(int x, int y);
	void moveToXY(POINT point);

	bool intersectRect(LPCRECT lpRect1, LPCRECT lpRect2);

	bool unionRect(LPCRECT lpRect1, LPCRECT lpRect2);

	bool subtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2);

	void operator=(const RECT& srcRect);
	bool operator==(const RECT& rect) const;
	bool operator!=(const RECT& rect) const;
	void operator+=(POINT point);
	void operator+=(SIZE size);
	void operator+=(LPCRECT lpRect);
	void operator-=(POINT point);
	void operator-=(SIZE size);
	void operator-=(LPCRECT lpRect);
	void operator&=(const RECT& rect);
	void operator|=(const RECT& rect);

	CoordRect operator+(POINT point) const;
	CoordRect operator-(POINT point) const;
	CoordRect operator+(LPCRECT lpRect) const;
	CoordRect operator+(SIZE size) const;
	CoordRect operator-(SIZE size) const;
	CoordRect operator-(LPCRECT lpRect) const;
	CoordRect operator&(const RECT& rect2) const;
	CoordRect operator|(const RECT& rect2) const;

	CoordRect mulDiv(int nMultiplier, int nDivisor) const;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


