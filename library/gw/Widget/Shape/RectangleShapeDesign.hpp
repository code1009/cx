/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class RectangleShapeDesign : public ShapeDesign
{
protected:
	std::shared_ptr<RectangleShape> _Target;

public:
	RectangleShapeDesign();

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
	// DesignWidget
public:
	virtual void moveMarker(const DesignWidgetMarkerId s, const Point& p) override;
	virtual void onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




