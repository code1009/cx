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
ShapeDesignTemplate::ShapeDesignTemplate(std::wstring className, makeTargetWidget make):
	ShapeDesign{ className },
	_makeTargetWidget{ make }
{
	//-----------------------------------------------------------------------
	_TargetWidget = _makeTargetWidget();


	//-----------------------------------------------------------------------
	_TargetWidget->setPropertyChangedHandler(
		std::bind(&ShapeDesignTemplate::onTargetWidgetPropertyChanged, this, std::placeholders::_1)
	);


	//-----------------------------------------------------------------------
	Point p0;
	Point p1;
	_TargetWidget->getBounds(p0, p1);
	_Points = getBoundsPoints(p0, p1);
}

//===========================================================================
void ShapeDesignTemplate::copyTo(WidgetSharedPtr widget) const
{
	if (widget->getClassName() != getClassName())
	{
		return;
	}


	ShapeDesign::copyTo(widget);


	ShapeDesignTemplate* o = dynamic_cast<ShapeDesignTemplate*>(widget.get());
	_TargetWidget->copyTo(o->_TargetWidget);
}

WidgetSharedPtr ShapeDesignTemplate::clone(void) const
{
	auto o = std::make_shared<ShapeDesignTemplate>(getClassName(), _makeTargetWidget);
	copyTo(o);
	return o;
}

//===========================================================================
bool ShapeDesignTemplate::isPointIn(const Point& test) const
{
	bool rv;


	rv = ShapeDesign::isPointIn(test);
	if (rv)
	{
		return true;
	}


	return _TargetWidget->isPointIn(test);
}

void ShapeDesignTemplate::moveOffset(const Point& offset)
{
	_TargetWidget->moveOffset(offset);

	ShapeDesign::moveOffset(offset);
}

//===========================================================================
bool ShapeDesignTemplate::loadResources(WidgetResourceMap* widgetResourceMap)
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

void ShapeDesignTemplate::draw(Context* ctx)
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
void ShapeDesignTemplate::moveMarker(const DesignWidgetMarkerId s, const Point& p)
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
WidgetSharedPtr ShapeDesignTemplate::getTargetWidget(void)
{
	return _TargetWidget;
}

void ShapeDesignTemplate::onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param)
{
	setTargetWidgetPropertyChanged(param->_Code);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




