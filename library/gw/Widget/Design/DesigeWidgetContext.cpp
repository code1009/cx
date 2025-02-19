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
bool DesigeWidgetContext::loadResources(WidgetResourceMap* widgetResourceMap)
{
	//--------------------------------------------------------------------------
	_Marker_Fill_Brush = widgetResourceMap->getFill_SolidColorBrush(&_Marker_FillStyle);
	if (nullptr== _Marker_Fill_Brush)
	{
		return false;
	}
	_Marker_Line_Brush = widgetResourceMap->getLine_SolidColorBrush(&_Marker_LineStyle);
	if (nullptr == _Marker_Line_Brush)
	{
		return false;
	}
	_Marker_Text_Brush = widgetResourceMap->getText_SolidColorBrush(&_Marker_TextStyle);
	if (nullptr == _Marker_Text_Brush)
	{
		return false;
	}
	_Marker_Text_TextFormat = widgetResourceMap->getText_TextFormat(&_Marker_TextStyle);
	if (nullptr == _Marker_Text_TextFormat)
	{
		return false;
	}

	_Guide_Line_Brush = widgetResourceMap->getLine_SolidColorBrush(&_Guide_LineStyle);
	if (nullptr == _Guide_Line_Brush)
	{
		return false;
	}
	_Guide_Line_StrokeStyle = widgetResourceMap->getLine_StrokeStyle(&_Guide_LineStyle);
	if (nullptr == _Guide_Line_StrokeStyle)
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




