/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../../gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ShapeDesign::ShapeDesign(std::wstring class_name) :
	DesignWidget{ std::move(class_name) }
{
}

//===========================================================================
void ShapeDesign::copyTo(WidgetSharedPtr widget) const
{
	if (widget->getClassName() != getClassName())
	{
		return;
	}


	DesignWidget::copyTo(widget);


	ShapeDesign* o = dynamic_cast<ShapeDesign*>(widget.get());
}

//===========================================================================
void ShapeDesign::moveMarker(const DesignWidgetMarkerId s, const Point& p)
{

}

DesignWidgetMarkerId ShapeDesign::findMarker(const Point& test)
{
	if (isPointInMarker(_Points[0], test))
	{
		return static_cast<DesignWidgetMarkerId>(ShapeDesignMarker::LT);
	}
	if (isPointInMarker(_Points[1], test))
	{
		return static_cast<DesignWidgetMarkerId>(ShapeDesignMarker::LB);
	}
	if (isPointInMarker(_Points[2], test))
	{
		return static_cast<DesignWidgetMarkerId>(ShapeDesignMarker::RB);
	}
	if (isPointInMarker(_Points[3], test))
	{
		return static_cast<DesignWidgetMarkerId>(ShapeDesignMarker::RT);
	}

	return static_cast<DesignWidgetMarkerId>(DesignWidgetMarker::NONE);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool setShapeDesignMarkerPoint(
	Point& lt,
	Point& lb,
	Point& rb,
	Point& rt,
	const ShapeDesignMarker s,
	const Point& p
)
{
	switch (s)
	{
	case ShapeDesignMarker::LT:
		if (p == lb) {return false;}
		if (p == rt) {return false;}
		if (p == rb) {return false;}
		if (rb._x <= p._x) {return false;}
		if (rb._y <= p._y) {return false;}

		lb._x = p._x;
		rt._y = p._y;
		lt = p;
		break;

	case ShapeDesignMarker::RT:
		if (p == lt) {return false;}
		if (p == lb) {return false;}
		if (p == rb) {return false;}
		if (lb._x >= p._x) {return false;}
		if (lb._y <= p._y) {return false;}

		rb._x = p._x;
		lt._y = p._y;
		rt = p;
		break;

	case ShapeDesignMarker::RB:
		if (p == lt) {return false;}
		if (p == lb) {return false;}
		if (p == rt) {return false;}
		if (lt._x >= p._x) {return false;}
		if (lt._y >= p._y) {return false;}

		rt._x = p._x;
		lb._y = p._y;
		rb = p;
		break;

	case ShapeDesignMarker::LB:
		if (p == lt) {return false;}
		if (p == rt) {return false;}
		if (p == rb) {return false;}
		if (rt._x <= p._x) {return false;}
		if (rt._y >= p._y) {return false;}

		lt._x = p._x;
		rb._y = p._y;
		lb = p;
		break;

	default:
		return false;
	}

	return true;
}

bool setLineShapeDesignMarkerPoint(
	Point& lt,
	Point& rb,
	const ShapeDesignMarker s,
	const Point& p
)
{
	switch (s)
	{
	case ShapeDesignMarker::LT:
		if (p == rb) { return false; }
		if (rb._x < p._x) { return false; }
		if (rb._y < p._y) { return false; }

		lt = p;
		break;

	case ShapeDesignMarker::RB:
		if (p == lt) { return false; }
		if (lt._x > p._x) { return false; }
		if (lt._y > p._y) { return false; }

		rb = p;
		break;

	default:
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




