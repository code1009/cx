/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerModel;
class DesignWidgetContext;
class WidgetDesigner;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using DesignWidgetMarkerId = std::uint32_t;

//===========================================================================
enum class DesignWidgetMarker : DesignWidgetMarkerId
{
	NONE = 0,
	Next
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DesignWidget;
using DesignWidgetWeakPtr = std::weak_ptr<DesignWidget>;
using DesignWidgetSharedPtr = std::shared_ptr<DesignWidget>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class DesignWidget : public Widget
{
protected:
	WidgetDesignerModel* _WidgetDesignerModel{ nullptr };

protected:
	bool    _MarkerVisible{ false };
	coord_t _MarkerWidth  { 10.0f };

protected:
	DesignWidgetMarkerId _PressedMarkerId{ static_cast<DesignWidgetMarkerId>(DesignWidgetMarker::NONE) };
	Point                _PressedPoint;

public:
	explicit DesignWidget(std::wstring className);

public:
	virtual ~DesignWidget() = default;

	//-----------------------------------------------------------------------
	// Widget
public:
	virtual void copyTo(WidgetSharedPtr widget) const override;
	virtual bool isPointIn(const Point& test) const override;
	virtual void registerEventHandler(WidgetDocument* doc) override;

	//-----------------------------------------------------------------------
public:
	virtual void setWidgetDesignerModel(WidgetDesignerModel* widgetDesignerModel);
	virtual DesignWidgetContext* getDesignWidgetContext(void) const;
	virtual WidgetDesigner* getWidgetDesinger(void) const;

public:
	virtual void setMarkerVisible(bool visible);
	virtual bool getMarkerVisible(void) const;
	virtual void getMarkerBounds(const Point& marker, Point& p0, Point& p1) const;
	virtual bool isPointInMarker(const Point& makrer, const Point& test) const;
	virtual bool isPointInMarkers(const Point& test) const;

public:
	virtual void drawMarker(Context* ctx, Point marker);
	virtual void drawMarkers(Context* ctx);
	virtual void drawBounds(Context* ctx, const Point& p0, const Point& p1);

public:
	virtual DesignWidgetMarkerId findMarker(const Point& test);
	virtual void moveMarker(const DesignWidgetMarkerId s, const Point& p);
	virtual void onMousePressed(const WidgetMouseEventParam& param);
	virtual void onMouseReleased(const WidgetMouseEventParam& param);
	virtual void onMouseDragging(const WidgetMouseEventParam& param);

public:
	virtual WidgetSharedPtr getTargetWidget(void);
	virtual void setTargetWidgetPropertyChanged(std::uint32_t code);
	virtual void onTargetWidgetPropertyChanged(Widget::PropertyChangedParam* param);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




