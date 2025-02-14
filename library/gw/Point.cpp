/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Point::Point(coord_t x, coord_t y) :
	_x{x}, 
	_y{y} 
{
}

//===========================================================================
bool Point::is_zero(void) const
{
	if ((static_cast<coord_t>(0u) == _x) &&
		(static_cast<coord_t>(0u) == _y))
	{
		return true;
	}

	return false;
}

void Point::zero(void)
{
	_x = 0;
	_y = 0;
}

//===========================================================================
bool Point::operator== (const Point& v) const
{
	return (_x == v._x && _y == v._y);
}

bool Point::operator!= (const Point& v) const
{
	return !operator==(v);
}

//===========================================================================
void Point::operator+= (const Point& v)
{
	_x += v._x;
	_y += v._y;
}

void Point::operator-= (const Point& v)
{
	_x -= v._x;
	_y -= v._y;
}

Point Point::operator- (void) const
{
	return Point(-_x, -_y);
}

Point Point::operator+ (const Point& v) const
{
	return Point(_x + v._x, _y + v._y);
}

Point Point::operator- (const Point& v) const
{
	return Point(_x - v._x, _y - v._y);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring to_std_wstring(const Point& v)
{
	std::wstring s;


	s = cx::to_std_wstring(v._x);
	s += L",";
	s += cx::to_std_wstring(v._y);

	return s;
}

Point to_Point(const std::wstring& v)
{
	Point point;


	auto tokens = cx::tokenizeObject_std_wstring(v);
	if (tokens.size() == 2)
	{
		point._x = cx::to_float(tokens[0]);
		point._y = cx::to_float(tokens[1]);
	}

	return point;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void calculateBounds(const std::vector<Point>& _points, Point& p0, Point& p1)
{
	if (!_points.empty())
	{
		p0 = _points[0];
		p1 = _points[0];
	}
	else
	{
		p0.zero();
		p1.zero();
	}


	for (auto& p : _points)
	{
		if (p._x < p0._x)
		{
			p0._x = p._x;
		}
		if (p._y < p0._y)
		{
			p0._y = p._y;
		}

		if (p._x > p1._x)
		{
			p1._x = p._x;
		}
		if (p._y > p1._y)
		{
			p1._y = p._y;
		}
	}
}

void calculateBounds(const Point& _p0, const Point& _p1, Point& p0, Point& p1)
{
	if (_p0._x < _p1._x)
	{
		p0._x = _p0._x;
		p1._x = _p1._x;
	}
	else
	{
		p0._x = _p1._x;
		p1._x = _p0._x;
	}

	if (_p0._y < _p1._y)
	{
		p0._y = _p0._y;
		p1._y = _p1._y;
	}
	else
	{
		p0._y = _p1._y;
		p1._y = _p0._y;
	}
}

bool isPointInBounds(const std::vector<Point>& _points, const Point& test)
{
	Point p0;
	Point p1;


	calculateBounds(_points, p0, p1);
	if (
		(p0._y <= test._y && test._y <= p1._y) &&
		(p0._x <= test._x && test._x <= p1._x)
		)
	{
		return true;
	}

	return false;
}

bool isPointInBounds(const Point& _p0, const Point& _p1, const Point& test)
{
	Point p0;
	Point p1;


	calculateBounds(_p0, _p1, p0, p1);
	if ( 
		(p0._y <= test._y && test._y <= p1._y) &&
		(p0._x <= test._x && test._x <= p1._x)
		)
	{
		return true;
	}

	return false;
}

bool isPointsInBounds(const Point& _p0, const Point& _p1, const std::vector<Point>& test)
{
	Point p0;
	Point p1;


	calculateBounds(_p0, _p1, p0, p1);
	for (auto& p : test)
	{
		if (
			!(
				(p0._y <= p._y && p._y <= p1._y) &&
				(p0._x <= p._x && p._x <= p1._x)
			)
			)
		{
			return false;
		}
	}

	return true;
}

std::vector<Point> getBoundsPoints(const Point& _p0, const Point& _p1)
{
	Point p0;
	Point p1;

	
	calculateBounds(_p0, _p1, p0, p1);


	std::vector<Point> points;


	points.reserve(4);
	points.push_back(Point{ p0._x, p0._y }); // lt: left-top
	points.push_back(Point{ p0._x, p1._y }); // lb: left-bottom
	points.push_back(Point{ p1._x, p1._y }); // rb: right-bottom
	points.push_back(Point{ p1._x, p0._y }); // rt: right-top

	return points;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




