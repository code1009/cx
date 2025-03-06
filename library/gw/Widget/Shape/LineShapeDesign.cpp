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
LineShapeDesign::LineShapeDesign():
	ShapeDesign{ L"Shape.Line.Design" }
{
	//-----------------------------------------------------------------------
	_Target = std::make_shared<LineShape>();


	//-----------------------------------------------------------------------
	_Target->setPropertyChangedHandler(
		std::bind(&LineShapeDesign::onTargetWidgetPropertyChanged, this, std::placeholders::_1)
	);


	//-----------------------------------------------------------------------
	_Points = _Target->getPoints();
}

//===========================================================================
void LineShapeDesign::copyTo(WidgetSharedPtr widget) const
{
	if (widget->getClassName() != getClassName())
	{
		return;
	}


	ShapeDesign::copyTo(widget);


	LineShapeDesign* o = dynamic_cast<LineShapeDesign*>(widget.get());
	_Target->copyTo(o->_Target);
}

WidgetSharedPtr LineShapeDesign::clone(void) const
{
	auto o = std::make_shared<LineShapeDesign>();
	copyTo(o);
	return o;
}

//===========================================================================
bool LineShapeDesign::isPointIn(const Point& test) const
{
	bool rv;


	rv = ShapeDesign::isPointIn(test);
	if (rv)
	{
		return true;
	}


	return _Target->isPointIn(test);
}

void LineShapeDesign::moveOffset(const Point& offset)
{
	_Target->moveOffset(offset);

	ShapeDesign::moveOffset(offset);
}

//===========================================================================
bool LineShapeDesign::loadResources(WidgetResourceMap* widgetResourceMap)
{
	bool rv;


	rv = _Target->loadResources(widgetResourceMap);
	if (false == rv)
	{
		return false;
	}


	rv = ShapeDesign::loadResources(widgetResourceMap);
	if (false == rv)
	{
		return false;
	}

	return true;
}

void LineShapeDesign::draw(Context* ctx)
{
	_Target->draw(ctx);

	if (getMarkerVisible())
	{
		Point p0;
		Point p1;
		_Target->getBounds(p0, p1);
		drawBounds(ctx, p0, p1);

		drawMarkers(ctx);
	}
}

//===========================================================================
DesignWidgetMarkerId LineShapeDesign::findMarker(const Point& test)
{
	if (isPointInMarker(_Points[0], test))
	{
		return static_cast<DesignWidgetMarkerId>(ShapeDesignMarker::LT);
	}
	if (isPointInMarker(_Points[1], test))
	{
		return static_cast<DesignWidgetMarkerId>(ShapeDesignMarker::RB);
	}

	return static_cast<DesignWidgetMarkerId>(DesignWidgetMarker::NONE);
}

void LineShapeDesign::moveMarker(const DesignWidgetMarkerId s, const Point& p)
{
	//-----------------------------------------------------------------------
	bool rv;
	rv = setLineShapeDesignMarkerPoint(
		_Points[0],
		_Points[1],
		static_cast<ShapeDesignMarker>(s),
		p
	);
	if (false == rv)
	{
		return;
	}


	//-----------------------------------------------------------------------
	_Target->getPoints()[0] = _Points[0];
	_Target->getPoints()[1] = _Points[1];


	//-----------------------------------------------------------------------
	setPropertyChanged();
}

//===========================================================================
void LineShapeDesign::onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param)
{
	setTargetWidgetPropertyChanged(param->_Code);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




