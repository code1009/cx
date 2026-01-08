#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	Coord to_Coord(const std::wstring& v);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	std::wstring to_std_wstring(Point const& v);
	Point to_Point(const std::wstring& v);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	/*
	inline bool operator==(Point const& lhs, Point const& rhs)
	{
		return (lhs.X == rhs.X && lhs.Y == rhs.Y);
	}

	inline bool operator!=(Point const& lhs, Point const& rhs)
	{
		return !(lhs == rhs);
	}
	*/

	//=======================================================================
	inline Point& operator+=(Point& lhs, Point const& rhs)
	{
		lhs.X += rhs.X;
		lhs.Y += rhs.Y;
		return lhs;
	}

	inline Point& operator-=(Point& lhs, Point const& rhs)
	{
		lhs.X -= rhs.X;
		lhs.Y -= rhs.Y;
		return lhs;
	}

	inline Point operator-(Point const& p)
	{
		return Point(-p.X, -p.Y);
	}

	inline Point operator+(Point const& lhs, Point const& rhs)
	{
		return Point(lhs.X + rhs.X, lhs.Y + rhs.Y);
	}

	inline Point operator-(Point const& lhs, Point const& rhs)
	{
		return Point(lhs.X - rhs.X, lhs.Y - rhs.Y);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	std::wstring to_std_wstring(Point const& v);
	Point to_Point(std::wstring const& v);

	std::wstring to_std_wstring(std::vector<Point> const& v);
	std::vector<Point> to_Points(std::wstring const& v);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	constexpr float M_PI = 3.14159265358979323846f;

	//=======================================================================
	// 1 radian = 180 / pi
	// 2*pi radian = 360  degree
	// radian = pi / 180 * degree
	// degree = 180 / pi * radian
	constexpr float degreeToRadian(float degree)
	{
		return degree * (static_cast<float>(M_PI) / 180.0f);
	}

	constexpr float radianToDegree(float radian)
	{
		return radian * (180.0f / static_cast<float>(M_PI));
	}

	//=======================================================================
	static inline bool isFpNegativeInfinity(double const v)
	{
		int f;


		f = _fpclass(v);
		if (f & _FPCLASS_NINF)
		{
			return true;
		}

		return false;
	}

	static inline bool isFpPositiveInfinity(double const v)
	{
		int f;


		f = _fpclass(v);
		if (f & _FPCLASS_PINF)
		{
			return true;
		}

		return false;
	}

	static inline bool isFpInfinity(double const v)
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

	static inline bool isFpNotANumber(double const v)
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

	static inline bool isFpFinite(double const v)
	{
		return std::isfinite(v);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	bool isPointZero(Point const& p);
	void setPointZero(Point& p);
	bool areZeroPoints(const std::vector<Point>& points);
	void setPointsZero(std::vector<Point>& points);

	void calculateBounds(const std::vector<Point>& points, Point& p0, Point& p1);
	void calculateBounds(Point const& _p0, Point const& _p1, Point& p0, Point& p1);

	bool isPointInBounds(const std::vector<Point>& points, Point const& test);
	bool isPointInBounds(Point const& _p0, Point const& _p1, Point const& test);

	bool arePointsInBounds(Point const& _p0, Point const& _p1, const std::vector<Point>& test);

	std::vector<Point> getBoundsPoints(Point const& _p0, Point const& _p1);

	Coord getLineDistance(Point const& p0, Point const& p1);

	bool isPointOnLine(Point const& p, Point const& p0, Point const& p1, Coord const t);
	Point getLineOffsetPoint(Point const& p0, Point const& p1, Coord const d);
	Point getLineCenterPoint(Point const& p0, Point const& p1);
	float getLineAngle(Point const& p0, Point const& p1);

	bool isPointOnPolygon(Point const& p, std::vector<Point> const& polygon);

	Point rotatePoint(Point const& base, Point const& p, float const angle);

	Point getLinePerpendicularPointBottom(const Point& p0, const Point& p1, const Point& base, Coord distance);
	Point getLinePerpendicularPointTop(const Point& p0, const Point& p1, const Point& base, Coord distance);
}




