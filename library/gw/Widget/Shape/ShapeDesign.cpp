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


	//o->_PressedState = _PressedState;
	//o->_PressedPoint = _PressedPoint;
}

void ShapeDesign::registerEventHandler(WidgetDocument* doc)
{
	doc->getWidgetEventDispatcher()->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MousePressed,
		this,
		std::bind(&ShapeDesign::onMousePressed, this, std::placeholders::_1)
	);

	doc->getWidgetEventDispatcher()->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseReleased,
		this,
		std::bind(&ShapeDesign::onMouseReleased, this, std::placeholders::_1)
	);

	doc->getWidgetEventDispatcher()->registerEventHandler<const WidgetMouseEventParam&>(
		WidgetEventType::MouseDragging,
		this,
		std::bind(&ShapeDesign::onMouseDragging, this, std::placeholders::_1)
	);
}

//===========================================================================
void ShapeDesign::setTargetWidgetPropertyChanged(
	std::uint32_t code
)
{
	setPropertyChanged(code);
}

void ShapeDesign::moveMarker(const ShapeDesignMarker s, const Point& p)
{

}

ShapeDesignMarker ShapeDesign::findMarker(const Point& test)
{
	if (isPointInMarker(_Points[0], test))
	{
		return ShapeDesignMarker::LT;
	}
	if (isPointInMarker(_Points[1], test))
	{
		return ShapeDesignMarker::LB;
	}
	if (isPointInMarker(_Points[2], test))
	{
		return ShapeDesignMarker::RB;
	}
	if (isPointInMarker(_Points[3], test))
	{
		return ShapeDesignMarker::RT;
	}

	return ShapeDesignMarker::NONE;
}

void ShapeDesign::onMousePressed(const WidgetMouseEventParam& param)
{
	Point _MousePoint = getWidgetDesinger()->toSnappedPoint(param._MousePosition);
	_PressedPoint = _MousePoint;


	_PressedState = findMarker(_MousePoint);
}

void ShapeDesign::onMouseReleased(const WidgetMouseEventParam& param)
{
	Point _MousePoint = getWidgetDesinger()->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _PressedPoint;
	_PressedPoint = _MousePoint;


	if (ShapeDesignMarker::NONE != _PressedState)
	{
		moveMarker(_PressedState, _MousePoint);
	}


	_PressedState = ShapeDesignMarker::NONE;
}

void ShapeDesign::onMouseDragging(const WidgetMouseEventParam& param)
{
	Point _MousePoint = getWidgetDesinger()->toSnappedPoint(param._MousePosition);
	Point offset = _MousePoint - _PressedPoint;
	_PressedPoint = _MousePoint;


	if (ShapeDesignMarker::NONE != _PressedState)
	{
		moveMarker(_PressedState, _MousePoint);
	}
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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




