/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class LineShapeDesign : public ShapeDesign
{
protected:
	std::shared_ptr<LineShape> _Target;

public:
	LineShapeDesign();

	//-----------------------------------------------------------------------
	// Widget
public:
	virtual void copyTo(WidgetSharedPtr widget) const override;
	virtual WidgetSharedPtr clone(void) const override;
	virtual bool isPointIn(const Point& test) const override;
	virtual void moveOffset(const Point& offset) override;
	virtual bool loadResources(WidgetResourceMap* widgetResourceMap) override;
	virtual void draw(Context* ctx) override;

	//-----------------------------------------------------------------------
	// ShapeDesign
public:
	virtual ShapeDesignMarker findMarker(const Point& test) override;
	virtual void moveMarker(const ShapeDesignMarker s, const Point& p) override;

	//-----------------------------------------------------------------------
public:
	virtual void onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




