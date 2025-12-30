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
	using makeTargetWidget = std::function<WidgetSharedPtr(void)>;

protected:
	makeTargetWidget _makeTargetWidget;
	WidgetSharedPtr _TargetWidget;

public:
	ShapeDesignTemplate(std::wstring className, makeTargetWidget make);

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
	virtual WidgetSharedPtr getTargetWidget(void) override;
	virtual void onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param) override;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




