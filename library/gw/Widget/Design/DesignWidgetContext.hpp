﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DesignWidgetContext final
{
public:
	WidgetFillStyle _Marker_FillStyle { Color{0.5f, 0.5f, 1.0f, 0.5f} };
	WidgetLineStyle _Marker_LineStyle { Color{0.5f, 0.5f, 1.0f, 1.0f} };
	WidgetTextStyle _Marker_TextStyle;
	WidgetLineStyle _Guide_LineStyle  { Color{1.0f, 0.5f, 0.5f, 0.8f} };

private:
	WidgetResource* _Marker_Fill_Brush     { nullptr };// ID2D1SolidColorBrush* 
	WidgetResource* _Marker_Line_Brush     { nullptr };// ID2D1SolidColorBrush* 
	WidgetResource* _Marker_Text_Brush     { nullptr };// ID2D1SolidColorBrush* 
	WidgetResource* _Marker_Text_TextFormat{ nullptr };// IDWriteTextFormat*   
	WidgetResource* _Guide_Line_Brush      { nullptr };// ID2D1SolidColorBrush* 
	WidgetResource* _Guide_Line_StrokeStyle{ nullptr };// ID2D1StrokeStyle* 

public:
	DesignWidgetContext() = default;
	virtual ~DesignWidgetContext() = default;

public:
	DesignWidgetContext(const DesignWidgetContext&) = delete;
	DesignWidgetContext& operator=(const DesignWidgetContext&) = delete;

	DesignWidgetContext(DesignWidgetContext&&) = delete;
	DesignWidgetContext& operator=(DesignWidgetContext&&) = delete;

public:
	WidgetResource* getMarker_Fill_Brush     (void) { return _Marker_Fill_Brush;      }
	WidgetResource* getMarker_Line_Brush     (void) { return _Marker_Line_Brush;      }
	WidgetResource* getMarker_Text_Brush     (void) { return _Marker_Fill_Brush;      }
	WidgetResource* getMarker_Text_TextFormat(void) { return _Marker_Text_TextFormat; }
	WidgetResource* getGuide_Line_Brush      (void) { return _Guide_Line_Brush;       }
	WidgetResource* getGuide_Line_StrokeStyle(void) { return _Guide_Line_StrokeStyle; }

public:
	bool loadResources(WidgetResourceMap* widgetResourceMap);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




