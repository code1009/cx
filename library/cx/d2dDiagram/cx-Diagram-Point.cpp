/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Coord to_Coord(const std::wstring& v)
	{
		return cx::to_float(v);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	std::wstring to_std_wstring(Point const& v)
	{
		std::wstring s;


		s = L"{";
		s += cx::to_std_wstring(v.X);
		s += L",";
		s += cx::to_std_wstring(v.Y);
		s += L"}";

		return s;
	}

	Point to_Point(const std::wstring& v)
	{
		Point point;


		auto tokens = cx::tokenize_object_std_wstring(cx::unescape_object_std_wstring(v));
		if (tokens.size() == 2)
		{
			point.X = cx::to_float(tokens[0]);
			point.Y = cx::to_float(tokens[1]);
		}

		return point;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	std::wstring to_std_wstring(std::vector<Point> const& v)
	{
		std::wstring s;


		s = L"{";
		for (auto& point : v)
		{
			s += to_std_wstring(point);

			if (&point != &v.back())
			{
				s += L",";
			}
		}
		s += L"}";

		return s;
	}

	std::vector<Point> to_Points(const std::wstring& v)
	{
		std::vector<Point> points;


		auto tokens = cx::tokenize_object_std_wstring(cx::unescape_object_std_wstring(v));
		for (auto& token : tokens)
		{
			auto point = to_Point(token);
			points.push_back(point);
		}

		return points;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	bool isPointZero(Point const& p)
	{
		return (p.X == 0.0F && p.Y == 0.0F);
	}

	void setPointZero(Point& p)
	{
		p.X = 0.0F;
		p.Y = 0.0F;
	}

	bool areZeroPoints(const std::vector<Point>& points)
	{
		for (auto& p : points)
		{
			if (!isPointZero(p))
			{
				return false;
			}
		}

		return true;
	}

	void setPointsZero(std::vector<Point>& points)
	{
		for (auto& p : points)
		{
			setPointZero(p);
		}
	}

	void calculateBounds(const std::vector<Point>& points, Point& p0, Point& p1)
	{
		if (!points.empty())
		{
			p0 = points[0];
			p1 = points[0];
		}
		else
		{
			p0 = { 0.0F, 0.0F };
			p1 = { 0.0F, 0.0F };
		}


		for (auto& p : points)
		{
			if (p.X < p0.X)
			{
				p0.X = p.X;
			}
			if (p.Y < p0.Y)
			{
				p0.Y = p.Y;
			}

			if (p.X > p1.X)
			{
				p1.X = p.X;
			}
			if (p.Y > p1.Y)
			{
				p1.Y = p.Y;
			}
		}
	}

	void calculateBounds(Point const& _p0, Point const& _p1, Point& p0, Point& p1)
	{
		if (_p0.X < _p1.X)
		{
			p0.X = _p0.X;
			p1.X = _p1.X;
		}
		else
		{
			p0.X = _p1.X;
			p1.X = _p0.X;
		}

		if (_p0.Y < _p1.Y)
		{
			p0.Y = _p0.Y;
			p1.Y = _p1.Y;
		}
		else
		{
			p0.Y = _p1.Y;
			p1.Y = _p0.Y;
		}
	}

	bool isPointInBounds(const std::vector<Point>& points, Point const& test)
	{
		Point p0;
		Point p1;


		calculateBounds(points, p0, p1);
		if (
			(p0.Y <= test.Y && test.Y <= p1.Y) &&
			(p0.X <= test.X && test.X <= p1.X)
			)
		{
			return true;
		}

		return false;
	}

	bool isPointInBounds(Point const& _p0, Point const& _p1, Point const& test)
	{
		Point p0;
		Point p1;


		calculateBounds(_p0, _p1, p0, p1);
		if (
			(p0.Y <= test.Y && test.Y <= p1.Y) &&
			(p0.X <= test.X && test.X <= p1.X)
			)
		{
			return true;
		}

		return false;
	}

	bool arePointsInBounds(Point const& _p0, Point const& _p1, const std::vector<Point>& test)
	{
		Point p0;
		Point p1;


		calculateBounds(_p0, _p1, p0, p1);
		for (auto& p : test)
		{
			if (
				!(
					(p0.Y <= p.Y && p.Y <= p1.Y) &&
					(p0.X <= p.X && p.X <= p1.X)
					)
				)
			{
				return false;
			}
		}

		return true;
	}

	std::vector<Point> getBoundsPoints(Point const& _p0, Point const& _p1)
	{
		Point p0;
		Point p1;


		calculateBounds(_p0, _p1, p0, p1);


		std::vector<Point> points;


		points.reserve(4);
		points.push_back(Point{ p0.X, p0.Y }); // lt: left-top
		points.push_back(Point{ p0.X, p1.Y }); // lb: left-bottom
		points.push_back(Point{ p1.X, p1.Y }); // rb: right-bottom
		points.push_back(Point{ p1.X, p0.Y }); // rt: right-top

		return points;
	}

	//=======================================================================
	Coord getLineDistance(Point const& p0, Point const& p1)
	{
		Coord dx = p1.X - p0.X;
		Coord dy = p1.Y - p0.Y;
		return std::sqrt(dx * dx + dy * dy);
	}

	//=======================================================================
	bool isPointOnLine(Point const& p, Point const& p0, Point const& p1, Coord const t)
	{
		Coord slope;
		Coord x;
		Coord y;


		slope = (p1.Y - p0.Y) / (p1.X - p0.X);

		y = (slope * p.X) + (p1.Y - slope * p1.X);

		if (((y - t) <= p.Y) && (p.Y <= (y + t)))
		{
			if ((p0.X <= p.X) && (p.X <= p1.X))
			{
				return true;
			}
			if ((p1.X <= p.X) && (p.X <= p0.X))
			{
				return true;
			}
		}


		slope = (p1.X - p0.X) / (p1.Y - p0.Y);

		x = (slope * p.Y) + (p1.X - slope * p1.Y);

		if (((x - t) <= p.X) && (p.X <= (x + t)))
		{
			if ((p0.Y <= p.Y) && (p.Y <= p1.Y))
			{
				return true;
			}
			if ((p1.Y <= p.Y) && (p.Y <= p0.Y))
			{
				return true;
			}
		}


		return false;
	}

	//=======================================================================
	Point getLineOffsetPoint(Point const& p0, Point const& p1, Coord const d)
	{
		Point p;

		Coord xLength;
		Coord yLength;
		Coord slope;
		float angle;

		Coord x = 0;
		Coord y = 0;


		xLength = p1.X - p0.X;
		yLength = p1.Y - p0.Y;
		slope = yLength / xLength;

		if (isFpNegativeInfinity(slope))
		{
			x = 0.0F;
			y = -d;
		}
		else if (isFpPositiveInfinity(slope))
		{
			x = 0.0F;
			y = d;
		}
		else if (isFpNotANumber(slope))
		{
			x = p0.X;
			y = p0.Y;
		}
		else
		{
			angle = std::atan(slope);

			x = (Coord)(std::cos(angle) * d);
			y = (Coord)(std::sin(angle) * d);

			if (p0.X > p1.X)
			{
				x = x * -1.0F;
				y = y * -1.0F;
			}
		}

		p.X = p0.X + x;
		p.Y = p0.Y + y;

		return p;
	}

	Point getLineCenterPoint(Point const& p0, Point const& p1)
	{
		Point p;

		Coord slope;

		Coord x = 0;
		Coord y = 0;


		slope = (p1.Y - p0.Y) / (p1.X - p0.X);


		x = p0.X + (p1.X - p0.X) / 2.0F;

		p.X = x;


		if (isFpInfinity(slope))
		{
			y = p0.Y + (p1.Y - p0.Y) / 2.0F;
		}
		else if (isFpNotANumber(slope))
		{
			y = p0.Y;
		}
		else
		{
			y = (slope * p.X) + (p1.Y - slope * p1.X);
		}


		p.Y = y;

		return p;
	}

	float getLineAngle(Point const& p0, Point const& p1)
	{
		float dx = p1.X - p0.X;
		float dy = p1.Y - p0.Y;


		return (float)std::atan2(dy, dx);
	}

	//=======================================================================
	bool isPointOnPolygon(Point const& p, std::vector<Point> const& polygon)
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

		Coord min_x = polygon[0].X;
		Coord max_x = polygon[0].X;
		Coord min_y = polygon[0].Y;
		Coord max_y = polygon[0].Y;

		Point q;
		std::size_t i;
		std::size_t j;


		for (i = 1; i < polygon_point_size; i++)
		{
			q = polygon[i];

			min_x = std::min(q.X, min_x);
			max_x = std::max(q.X, max_x);
			min_y = std::min(q.Y, min_y);
			max_y = std::max(q.Y, max_y);
		}

		if (p.X < min_x || p.X > max_x || p.Y < min_y || p.Y > max_y)
		{
			return false;
		}


		bool inside = false;


		for (i = 0, j = polygon_point_size - 1; i < polygon_point_size; j = i++)
		{
			if (

				(polygon[i].Y > p.Y) != (polygon[j].Y > p.Y)
				&&
				p.X
				<
				(polygon[j].X - polygon[i].X)
				*
				(p.Y - polygon[i].Y)
				/
				(polygon[j].Y - polygon[i].Y)
				+
				polygon[i].X

				)
			{
				inside = !inside;
			}
		}

		return inside;
	}

	//=======================================================================
	Point rotatePoint(Point const& base, Point const& p, float const angle)
	{
		// u = 기준점 X
		// v = 기준점 Y
		// x' = (x-u)*  cos(a) + (y-v)*sin(a) + u;
		// y' = (x-u)* -sin(a) + (y-v)*cos(a) + v;

#if 0
		OpenGl 좌표계
		Point r;
		r.X = (Coord)((p.X - base.X) * (std::cos(angle)) + (p.Y - base.Y) * (std::sin(angle)) + base.X);
		r.Y = (Coord)((p.X - base.X) * (-std::sin(angle)) + (p.Y - base.Y) * (std::cos(angle)) + base.Y);
		return r;
#endif

		Point r;
		r.X = (Coord)((p.X - base.X) * (std::cos(angle)) - (p.Y - base.Y) * (std::sin(angle)) + base.X);
		r.Y = (Coord)((p.X - base.X) * (std::sin(angle)) + (p.Y - base.Y) * (std::cos(angle)) + base.Y);
		return r;
	}

	//===================================================================
	// 선분 p0-p1, 기준점 base에서 90도 방향으로 distance만큼 이동한 점 반환
	Point getLinePerpendicularPointBottom(const Point& p0, const Point& p1, const Point& base, Coord distance)
	{
		// 선분 방향 벡터
		Coord dx = p1.X - p0.X;
		Coord dy = p1.Y - p0.Y;

		// 길이 계산
		Coord len = std::sqrt(dx * dx + dy * dy);
		if (len == 0.0f)
			return base; // 선분 길이가 0이면 기준점 반환

		// 단위 벡터로 정규화
		Coord ux = dx / len;
		Coord uy = dy / len;

		// 90도 회전(수직 벡터)
		Coord perp_x = -uy;
		Coord perp_y = ux;

		// 거리만큼 이동
		Point result;
		result.X = base.X + perp_x * distance;
		result.Y = base.Y + perp_y * distance;
		return result;
	}

	Point getLinePerpendicularPointTop(const Point& p0, const Point& p1, const Point& base, Coord distance)
	{
		Coord dx = p1.X - p0.X;
		Coord dy = p1.Y - p0.Y;

		Coord len = std::sqrt(dx * dx + dy * dy);
		if (len == 0.0f)
			return base;

		Coord ux = dx / len;
		Coord uy = dy / len;

		// 270도(-90도) 회전
		Coord perp_x = uy;
		Coord perp_y = -ux;

		Point result;
		result.X = base.X + perp_x * distance;
		result.Y = base.Y + perp_y * distance;
		return result;
	}
}
