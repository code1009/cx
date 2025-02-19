/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Shape: public Widget
{
protected:
	WidgetDocument* _WidgetDocument{ nullptr };

protected:
	WidgetFillStyle _FillStyle{ Color{0.7f, 1.0f, 0.7f, 1.0f} };
	WidgetLineStyle _LineStyle{ Color{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f, 0, 0 };
	WidgetTextStyle _TextStyle;

protected:
	WidgetResource* _Fill_Brush     { nullptr };
	WidgetResource* _Line_Brush     { nullptr };
	WidgetResource* _Text_TextFormat{ nullptr };
	WidgetResource* _Text_Brush     { nullptr };

public:
	explicit Shape(std::wstring className);

public:
	virtual ~Shape() = default;

	//-----------------------------------------------------------------------
	// Widget
public:
	virtual void copyTo(WidgetSharedPtr widget) const override;
	virtual bool loadResources(WidgetResourceMap* widgetResourceMap) override;

	//-----------------------------------------------------------------------
public:
	virtual void setWidgetDocument(WidgetDocument* doc);

public:
	virtual void onWidgetDrawingStyleChanged(WidgetDrawingStyle::StyleChangedParam* param);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




