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
ShapeDesignTemplate::ShapeDesignTemplate(std::wstring className, makeTarget make):
	ShapeDesign{ className },
	_makeTarget{ make }
{
	//-----------------------------------------------------------------------
	_Target = _makeTarget();


	//-----------------------------------------------------------------------
	_Target->setPropertyChangedHandler(
		std::bind(&ShapeDesignTemplate::onTargetWidgetPropertyChanged, this, std::placeholders::_1)
	);


	//-----------------------------------------------------------------------
	Point p0;
	Point p1;
	_Target->getBounds(p0, p1);
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
	_Target->copyTo(o->_Target);
}

WidgetSharedPtr ShapeDesignTemplate::clone(void) const
{
	auto o = std::make_shared<ShapeDesignTemplate>(getClassName(), _makeTarget);
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


	return _Target->isPointIn(test);
}

void ShapeDesignTemplate::moveOffset(const Point& offset)
{
	_Target->moveOffset(offset);

	ShapeDesign::moveOffset(offset);
}

//===========================================================================
bool ShapeDesignTemplate::loadResources(WidgetResourceMap* widgetResourceMap)
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

void ShapeDesignTemplate::draw(Context* ctx)
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
	_Target->getPoints()[0] = _Points[0];
	_Target->getPoints()[1] = _Points[2];


	//-----------------------------------------------------------------------
	setPropertyChanged();
}

//===========================================================================
void ShapeDesignTemplate::onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param)
{
	setTargetWidgetPropertyChanged(param->_Code);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




