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
RectangleShapeDesign::RectangleShapeDesign():
	ShapeDesign{ L"Shape.Rectangle.Design" }
{
	//-----------------------------------------------------------------------
	_TargetWidget = std::make_shared<RectangleShape>();


	//-----------------------------------------------------------------------
	_TargetWidget->setPropertyChangedHandler(
		std::bind(&RectangleShapeDesign::onTargetWidgetPropertyChanged, this, std::placeholders::_1)
	);


	//-----------------------------------------------------------------------
	Point p0;
	Point p1;
	_TargetWidget->getBounds(p0, p1);
	_Points = getBoundsPoints(p0, p1);
}

//===========================================================================
void RectangleShapeDesign::copyTo(WidgetSharedPtr widget) const
{
	if (widget->getClassName() != getClassName())
	{
		return;
	}


	ShapeDesign::copyTo(widget);


	RectangleShapeDesign* o = dynamic_cast<RectangleShapeDesign*>(widget.get());
	_TargetWidget->copyTo(o->_TargetWidget);
}

WidgetSharedPtr RectangleShapeDesign::clone(void) const
{
	auto o = std::make_shared<RectangleShapeDesign>();
	copyTo(o);
	return o;
}

//===========================================================================
bool RectangleShapeDesign::isPointIn(const Point& test) const
{
	bool rv;


	rv = ShapeDesign::isPointIn(test);
	if (rv)
	{
		return true;
	}


	return _TargetWidget->isPointIn(test);
}

void RectangleShapeDesign::moveOffset(const Point& offset)
{
	_TargetWidget->moveOffset(offset);

	ShapeDesign::moveOffset(offset);
}

//===========================================================================
bool RectangleShapeDesign::loadResources(WidgetResourceMap* widgetResourceMap)
{
	bool rv;


	rv = _TargetWidget->loadResources(widgetResourceMap);
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

void RectangleShapeDesign::draw(Context* ctx)
{
	_TargetWidget->draw(ctx);

	if (getMarkerVisible())
	{
		Point p0;
		Point p1;
		_TargetWidget->getBounds(p0, p1);
		drawBounds(ctx, p0, p1);

		drawMarkers(ctx);
	}
}

//===========================================================================
void RectangleShapeDesign::moveMarker(const DesignWidgetMarkerId s, const Point& p)
{
	//-----------------------------------------------------------------------
	bool rv;
	rv = setShapeDesignMarkerPoint(
		_Points[0],
		_Points[1],
		_Points[2],
		_Points[3],
		static_cast<ShapeDesignMarker>(s),
		p
	);
	if (false == rv)
	{
		return;
	}


	//-----------------------------------------------------------------------
	_TargetWidget->getPoints()[0] = _Points[0];
	_TargetWidget->getPoints()[1] = _Points[2];


	//-----------------------------------------------------------------------
	setPropertyChanged();
}

//===========================================================================
WidgetSharedPtr RectangleShapeDesign::getTargetWidget(void)
{
	return _TargetWidget;
}

void RectangleShapeDesign::onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param)
{
	setTargetWidgetPropertyChanged(param->_Code);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




