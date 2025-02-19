/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ShapeDesignTemplate : public ShapeDesign
{
public:
	using makeTarget = std::function<WidgetSharedPtr(void)>;

protected:
	makeTarget _makeTarget;
	WidgetSharedPtr _Target;

public:
	ShapeDesignTemplate(std::wstring className, makeTarget make);

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
	virtual void moveMarker(const ShapeDesignMarker s, const Point& p) override;

	//-----------------------------------------------------------------------
public:
	virtual void onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




