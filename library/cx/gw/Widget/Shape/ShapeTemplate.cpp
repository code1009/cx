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
ShapeTemplate::ShapeTemplate(std::wstring className, drawShape draw):
	Shape{ className },
	_drawShape{ draw }
{
	_FillStyle.setStyleChangedHandler(
		std::bind(&ShapeTemplate::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
	_LineStyle.setStyleChangedHandler(
		std::bind(&ShapeTemplate::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
	_TextStyle.setStyleChangedHandler(
		std::bind(&ShapeTemplate::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);


	_Points.resize(2);


	_Points[0] = Point{   0,   0 }; // lt
	_Points[1] = Point{ 100, 100 }; // rb
}

//===========================================================================
WidgetSharedPtr ShapeTemplate::clone(void) const
{
	auto o = std::make_shared<ShapeTemplate>(getClassName(), _drawShape);
	copyTo(o);
	return o;
}

void ShapeTemplate::draw(Context* ctx)
{
	_drawShape(ctx, this);
}

//===========================================================================
void ShapeTemplate::onWidgetDrawingStyleChanged(WidgetDrawingStyle::StyleChangedParam* param)
{
	setPropertyChanged(static_cast<std::uint32_t>(Widget::PropertyChangedParam::Code::ReloadResources));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




