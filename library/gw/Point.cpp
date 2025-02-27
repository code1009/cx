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


	s = L"{";
	s += cx::to_std_wstring(v._x);
	s += L",";
	s += cx::to_std_wstring(v._y);
	s = L"}";

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

//===========================================================================
bool isPointOnLine(Point p, Point p0, Point p1, coord_t t)
{
	coord_t slope;
	coord_t x;
	coord_t y;


	slope = (p1._y - p0._y) / (p1._x - p0._x);

	y = (slope * p._x) + (p1._y - slope * p1._x);

	if (((y - t) <= p._y) && (p._y <= (y + t)))
	{
		if ((p0._x <= p._x) && (p._x <= p1._x))
		{
			return true;
		}
		if ((p1._x <= p._x) && (p._x <= p0._x))
		{
			return true;
		}
	}


	slope = (p1._x - p0._x) / (p1._y - p0._y);

	x = (slope * p._y) + (p1._x - slope * p1._y);

	if (((x - t) <= p._x) && (p._x <= (x + t)))
	{
		if ((p0._y <= p._y) && (p._y <= p1._y))
		{
			return true;
		}
		if ((p1._y <= p._y) && (p._y <= p0._y))
		{
			return true;
		}
	}


	return false;
}

//===========================================================================
static float PI = 3.14159265358979323846f;

//===========================================================================
inline static float degree_to_radian(float angle)
{
	// 1 radian = 180 / pi

	// 2*pi radian = 360  degree
	// radian = pi / 180 * degree
	// degree = 180 / pi * radian

	return PI * angle / 180.0f;
}

inline static float radian_to_degree(float angle)
{
	return angle * (180.0f / PI);
}

inline static bool is_negative_infinity(double v)
{
	int f;


	f = _fpclass(v);
	if (f & _FPCLASS_NINF)
	{
		return true;
	}

	return false;
}

inline static bool is_positive_infinity(double v)
{
	int f;


	f = _fpclass(v);
	if (f & _FPCLASS_PINF)
	{
		return true;
	}

	return false;
}

inline static bool is_infinity(double v)
{
	int f;


	f = _fpclass(v);
	if (f & _FPCLASS_NINF)
	{
		return true;
	}
	if (f & _FPCLASS_PINF)
	{
		return true;
	}

	return false;
}

inline static bool is_nan(double v)
{
	int f;


	f = _fpclass(v);
	if (f & _FPCLASS_SNAN)
	{
		return true;
	}
	if (f & _FPCLASS_QNAN)
	{
		return true;
	}

	return false;
}

//===========================================================================
Point getLineOffsetPoint(Point p0, Point p1, coord_t d)
{
	Point p;

	coord_t xLength;
	coord_t yLength;
	coord_t  slope;
	float angle;

	coord_t x = 0;
	coord_t y = 0;


	xLength = p1._x - p0._x;
	yLength = p1._y - p0._y;
	slope = yLength / xLength;

	if (is_negative_infinity(slope))
	{
		x = 0.0F;
		y = -d;
	}
	else if (is_positive_infinity(slope))
	{
		x = 0.0F;
		y = d;
	}
	else if (is_nan(slope))
	{
		x = p0._x;
		y = p0._y;
	}
	else
	{
		angle = std::atan(slope);

		x = (coord_t)(std::cos(angle) * d);
		y = (coord_t)(std::sin(angle) * d);

		if (p0._x > p1._x)
		{
			x = x * -1.0F;
			y = y * -1.0F;
		}
	}

	p._x = p0._x + x;
	p._y = p0._y + y;

	return p;
}

Point getLineCenterPoint(Point p0, Point p1)
{
	Point p;

	coord_t slope;

	coord_t x = 0;
	coord_t y = 0;


	slope = (p1._y - p0._y) / (p1._x - p0._x);


	x = p0._x + (p1._x - p0._x) / 2.0F;

	p._x = x;


	if (is_infinity(slope))
	{
		y = p0._y + (p1._y - p0._y) / 2.0F;
	}
	else if (is_nan(slope))
	{
		y = p0._y;
	}
	else
	{
		y = (slope * p._x) + (p1._y - slope * p1._x);
	}


	p._y = y;

	return p;
}

float getLineAngle(Point p0, Point p1)
{
	float dx = p1._x - p0._x;
	float dy = p1._y - p0._y;


	return (float)std::atan2(dy, dx);
}

//===========================================================================
bool isPointOnPolygon(Point p, std::vector<Point>& polygon)
{
	// http://stackoverflow.com/questions/217578/point-in-polygon-aka-hit-test
	// http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	// http://msdn.microsoft.com/en-us/library/ms969920.aspx

	std::size_t polygon_point_size;


	polygon_point_size = polygon.size();
	if (polygon.empty())
	{
		return false;
	}

	coord_t min_x = polygon[0]._x;
	coord_t max_x = polygon[0]._x;
	coord_t min_y = polygon[0]._y;
	coord_t max_y = polygon[0]._y;

	Point q;
	std::size_t i;
	std::size_t j;


	for (i = 1; i < polygon_point_size; i++)
	{
		q = polygon[i];

		min_x = std::min(q._x, min_x);
		max_x = std::max(q._x, max_x);
		min_y = std::min(q._y, min_y);
		max_y = std::max(q._y, max_y);
	}

	if (p._x < min_x || p._x > max_x || p._y < min_y || p._y > max_y)
	{
		return false;
	}


	bool inside = false;


	for (i = 0, j = polygon_point_size - 1; i < polygon_point_size; j = i++)
	{
		if (

			(polygon[i]._y > p._y) != (polygon[j]._y > p._y)
			&&
			p._x
			<
			(polygon[j]._x - polygon[i]._x)
			*
			(p._y - polygon[i]._y)
			/
			(polygon[j]._y - polygon[i]._y)
			+
			polygon[i]._x

			)
		{
			inside = !inside;
		}
	}

	return inside;
}

//===========================================================================
Point rotatePoint(Point p0, Point p, float angle)
{
	// u = 기준점 X
	// v = 기준점 Y
	// x' = (x-u)*  cos(a) + (y-v)*sin(a) + u;
	// y' = (x-u)* -sin(a) + (y-v)*cos(a) + v;

	/*
	* OpenGl 좌표계
	Point r;


	r._x = (coord_t)(  (p._x-p0._x)*(  std::cos(angle) ) + (p._y-p0._y)*(std::sin(angle)) + p0._x  );
	r._y = (coord_t)(  (p._x-p0._x)*( -std::sin(angle) ) + (p._y-p0._y)*(std::cos(angle)) + p0._y  );
	*/

	Point r;


	r._x = (coord_t)((p._x - p0._x) * (std::cos(angle)) - (p._y - p0._y) * (std::sin(angle)) + p0._x);
	r._y = (coord_t)((p._x - p0._x) * (std::sin(angle)) + (p._y - p0._y) * (std::cos(angle)) + p0._y);

	return r;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




