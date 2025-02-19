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
Shape::Shape(std::wstring className) :
	Widget{ std::move(className) }
{
}

//===========================================================================
void Shape::copyTo(WidgetSharedPtr widget) const
{
	if (widget->getClassName() != getClassName())
	{
		return;
	}


	Widget::copyTo(widget);


	Shape* o = dynamic_cast<Shape*>(widget.get());


	o->_WidgetDocument = _WidgetDocument;
	
	o->_FillStyle = _FillStyle;
	o->_LineStyle = _LineStyle;
	o->_TextStyle = _TextStyle;

	o->_Fill_Brush      = _Fill_Brush     ;
	o->_Line_Brush      = _Line_Brush     ;
	o->_Text_TextFormat = _Text_TextFormat;
	o->_Text_Brush      = _Text_Brush     ;
}

bool Shape::loadResources(WidgetResourceMap* widgetResourceMap)
{
	_Fill_Brush      = widgetResourceMap->getFill_SolidColorBrush(&_FillStyle);
	_Line_Brush      = widgetResourceMap->getLine_SolidColorBrush(&_LineStyle);
	_Text_TextFormat = widgetResourceMap->getText_TextFormat     (&_TextStyle);
	_Text_Brush      = widgetResourceMap->getText_SolidColorBrush(&_TextStyle);

	return true;
}

//===========================================================================
void Shape::setWidgetDocument(WidgetDocument* doc)
{
	_WidgetDocument = doc;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




