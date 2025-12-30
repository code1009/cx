/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class ShapeDesignMarker : DesignWidgetMarkerId
{
	LT = static_cast<std::uint32_t>(DesignWidgetMarker::Next),
	LB,
	RB,
	RT,
	Next
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class ShapeDesign : public DesignWidget
{
public:
	explicit ShapeDesign(std::wstring className);

	//-----------------------------------------------------------------------
	// Widget
public:
	virtual void copyTo(WidgetSharedPtr widget) const override;

	//-----------------------------------------------------------------------
	// DesignWidget
public:
	virtual DesignWidgetMarkerId findMarker(const Point& test) override;
	virtual void moveMarker(const DesignWidgetMarkerId s, const Point& p) override;
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




