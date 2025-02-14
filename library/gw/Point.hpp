﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using coord_t = float;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Point
{
public:
	coord_t _x{ 0 };
	coord_t _y{ 0 };

public:
	Point() = default;

public:
	explicit Point(coord_t x, coord_t y);

public:
	virtual ~Point() = default;

public:
	bool is_zero(void) const;
	void zero(void);

public:
	bool operator== (const Point& v) const;
	bool operator!= (const Point& v) const;

public:
	void   operator+= (const Point& v);
	void   operator-= (const Point& v);
	Point  operator-  (void) const;
	Point  operator+  (const Point& v) const;
	Point  operator-  (const Point& v) const;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void calculateBounds(const std::vector<Point>& _Points, Point& p0, Point& p1);
void calculateBounds(const Point& _p0, const Point& _p1, Point& p0, Point& p1);

bool isPointInBounds(const std::vector<Point>& _Points, const Point& test);
bool isPointInBounds(const Point& _p0, const Point& _p1, const Point& test);

bool isPointsInBounds(const Point& _p0, const Point& _p1, const std::vector<Point>& test);

std::vector<Point> getBoundsPoints(const Point& _p0, const Point& _p1);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




