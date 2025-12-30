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
class WidgetDesignerFacility
{
protected:
	WidgetDesigner* _WidgetDesigner{ nullptr };

protected:
	std::uint64_t _GridXSize { 20 };
	std::uint64_t _GridYSize { 20 };
	bool _SnapToGrid { true };

public:
	explicit WidgetDesignerFacility(WidgetDesigner* widgetDesigner);

public:
	virtual ~WidgetDesignerFacility() = default;

public:
	WidgetDesignerFacility(const WidgetDesignerFacility&) = delete;
	WidgetDesignerFacility& operator=(const WidgetDesignerFacility&) = delete;

	WidgetDesignerFacility(WidgetDesignerFacility&&) = delete;
	WidgetDesignerFacility& operator=(WidgetDesignerFacility&&) = delete;

public:
	Point toGridPoint(Point p) const;
	Point toSnappedPoint(Point p) const;
	void getGridSize(std::uint64_t& x, std::uint64_t& y) const;
	bool getSnapToGrid(void) const;
	void setGridSize(std::uint64_t x, std::uint64_t y);
	void setSnapToGrid(bool snapToGrid);

public:
	bool isSelectedWidget(WidgetSharedPtr widget);
#if 0
	void selectSingleWidget(WidgetSharedPtr target);
	void toggleWidgetSelection(WidgetSharedPtr widget);
#endif

#if 0
public:
	void selectAllWidgets(void);
	void deselectAllWidgets(void);
	void selectWidgetsInBounds(void);
	void moveSelectedWidgets(Point offset);
	void deleteSelectedWidgets(void);
#endif
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




