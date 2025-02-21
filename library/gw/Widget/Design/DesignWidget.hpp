﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesignerModel;
class DesigeWidgetContext;
class WidgetDesigner;





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

public:
	explicit DesignWidget(std::wstring className);

public:
	virtual ~DesignWidget() = default;

	//-----------------------------------------------------------------------
	// Widget
public:
	virtual void copyTo(WidgetSharedPtr widget) const override;
	virtual bool isPointIn(const Point& test) const override;

	//-----------------------------------------------------------------------
public:
	virtual void setWidgetDesignerModel(WidgetDesignerModel* widgetDesignerModel);
	virtual DesigeWidgetContext* getDesigeWidgetContext(void) const;
	virtual WidgetDesigner* getWidgetDesinger(void) const;
	virtual void setMarkerVisible(bool visible);
	virtual bool getMarkerVisible(void) const;
	virtual void getMarkerBounds(const Point& marker, Point& p0, Point& p1) const;
	virtual bool isPointInMarker(const Point& makrer, const Point& test) const;
	virtual bool isPointInMarkers(const Point& test) const;
	virtual void drawMarker(Context* ctx, Point marker);
	virtual void drawMarkers(Context* ctx);
	virtual void drawBounds(Context* ctx, const Point& p0, const Point& p1);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




