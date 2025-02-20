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
	_FillStyle.setStyleChangedHandler(
		std::bind(&Shape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
	_LineStyle.setStyleChangedHandler(
		std::bind(&Shape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
	_TextStyle.setStyleChangedHandler(
		std::bind(&Shape::onWidgetDrawingStyleChanged, this, std::placeholders::_1)
	);
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
std::wstring Shape::getText(void) const
{
	return _Text;
}

void Shape::setText(std::wstring text)
{
	if (_Text != text)
	{
		_Text = text;
		setPropertyChanged();
	}
}

void Shape::setWidgetDocument(WidgetDocument* doc)
{
	_WidgetDocument = doc;
}

//===========================================================================
void Shape::onWidgetDrawingStyleChanged(WidgetDrawingStyle::StyleChangedParam* param)
{
	setPropertyChanged(0x80000000);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




