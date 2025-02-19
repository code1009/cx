/////////////////////////////////////////////////////////////////////////////
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
class WidgetDesignerTool
{
protected:
	WidgetDesigner* _WidgetDesigner{ nullptr };

protected:
	// move and select
	Point _p0{};
	Point _p1{};
	Point _MousePressedPoint{};
	bool _Action_MultipleSelection{ false };
	bool _Action_SingleSelection{ false };
	bool _Action_ToggleSelection{ false };
	bool _Action_Move{ false };

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
	explicit WidgetDesignerTool(WidgetDesigner* widgetDesigner);

public:
	virtual ~WidgetDesignerTool() = default;

public:
	WidgetDesignerTool(const WidgetDesignerTool&) = delete;
	WidgetDesignerTool& operator=(const WidgetDesignerTool&) = delete;

	WidgetDesignerTool(WidgetDesignerTool&&) = delete;
	WidgetDesignerTool& operator=(WidgetDesignerTool&&) = delete;

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




