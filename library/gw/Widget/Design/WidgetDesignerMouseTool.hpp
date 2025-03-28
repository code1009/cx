﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesigner;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerMouseTool
{
protected:
	WidgetDesigner* _WidgetDesigner{ nullptr };

protected:
	// move and select
	Point _p0{};
	Point _p1{};
	Point _MousePressedPoint{};
	bool _Action_SelectMultiple{ false };
	bool _Action_SelectSingle{ false };
	bool _Action_ToggleSelection{ false };
	bool _Action_MoveSelection{ false };

protected:
	// add
	Point           _NewWidget_Point;
	WidgetSharedPtr _NewWidget;

protected:
	WidgetFillStyle _Select_FillStyle{ Color{0.5f, 0.5f, 1.0f, 0.2f} };
	WidgetLineStyle _Select_LineStyle{ Color{0.5f, 0.5f, 1.0f, 1.0f} };
	WidgetTextStyle _Select_TextStyle;

protected:
	WidgetResource* _Select_Fill_Brush{ nullptr };
	WidgetResource* _Select_Line_Brush{ nullptr };
	WidgetResource* _Select_Text_Brush{ nullptr };
	WidgetResource* _Select_Text_TextFormat{ nullptr };

public:
	explicit WidgetDesignerMouseTool(WidgetDesigner* widgetDesigner);

public:
	virtual ~WidgetDesignerMouseTool() = default;

public:
	WidgetDesignerMouseTool(const WidgetDesignerMouseTool&) = delete;
	WidgetDesignerMouseTool& operator=(const WidgetDesignerMouseTool&) = delete;

	WidgetDesignerMouseTool(WidgetDesignerMouseTool&&) = delete;
	WidgetDesignerMouseTool& operator=(WidgetDesignerMouseTool&&) = delete;

public:
	void getSelectBounds(Point& p0, Point& p1);

public:
	void registerEventHandler(void);

public:
	void onMouseMove(const WidgetMouseEventParam& param);
	void onMouseLButtonDown(const WidgetMouseEventParam& param);
	void onMouseLButtonUp(const WidgetMouseEventParam& param);

public:
	void onMouseDragEnter(const WidgetMouseDragEnterEventParam& param);
	void onMouseDragOver(const WidgetMouseDragOverEventParam& param);
	void onMouseDragLeave(const WidgetMouseDragLeaveEventParam& param);
	void onMouseDrop(const WidgetMouseDropEventParam& param);

public:
	bool loadResources(WidgetResourceMap* widgetResourceMap);
	void draw(Context* ctx);
	void drawSelectBounds(Context* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




