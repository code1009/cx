/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class ShapeDesignMarker
{
	NONE,
	LT,
	LB,
	RB,
	RT,
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ShapeDesign : public DesignWidget
{
protected:
	ShapeDesignMarker _PressedState{ ShapeDesignMarker::NONE };
	Point             _PressedPoint;

public:
	explicit ShapeDesign(std::wstring className);

	//-----------------------------------------------------------------------
	// Widget
public:
	virtual void copyTo(WidgetSharedPtr widget) const override;
	virtual void registerEventHandler(WidgetDocument* doc) override;

	//-----------------------------------------------------------------------
public:
	virtual void setTargetWidgetPropertyChanged(std::uint32_t code);
	virtual ShapeDesignMarker findMarker(const Point& test);
	virtual void moveMarker(const ShapeDesignMarker s, const Point& p);
	virtual void onMousePressed(const WidgetMouseEventParam& param);
	virtual void onMouseReleased(const WidgetMouseEventParam& param);
	virtual void onMouseDragging(const WidgetMouseEventParam& param);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool setShapeDesignMarkerPoint(
	Point& lt,
	Point& lb,
	Point& rb,
	Point& rt,
	const ShapeDesignMarker s,
	const Point& p
);

bool setLineShapeDesignMarkerPoint(
	Point& lt,
	Point& rb,
	const ShapeDesignMarker s,
	const Point& p
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




