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




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesigner
{
protected:
	WidgetDesignerModel* _WidgetDesignerModel{ nullptr };

protected:
	std::unique_ptr<WidgetDesignerMouseTool> _WidgetDesignerMouseTool;

protected:
	std::uint64_t _GridXSize { 20 };
	std::uint64_t _GridYSize { 20 };
	bool _SnapToGrid { true };

public:
	explicit WidgetDesigner(WidgetDesignerModel* model);

public:
	virtual ~WidgetDesigner() = default;

public:
	WidgetDesignerModel* getWidgetDesignerModel(void) const { return _WidgetDesignerModel; }

public:
	WidgetDesigner(const WidgetDesigner&) = delete;
	WidgetDesigner& operator=(const WidgetDesigner&) = delete;

	WidgetDesigner(WidgetDesigner&&) = delete;
	WidgetDesigner& operator=(WidgetDesigner&&) = delete;

public:
	void setGridSize(std::uint64_t x, std::uint64_t y);
	void getGridSize(std::uint64_t& x, std::uint64_t& y) const;
	bool getSnapToGrid(void) const;
	void setSnapToGrid(bool snapToGrid);
	Point toGridPoint(Point p) const;
	Point toSnappedPoint(Point p) const;

public:
	bool isSelectedWidget(WidgetSharedPtr widget);
	void selectWidget(WidgetSharedPtr target);
	void toggleWidgetSelection(WidgetSharedPtr widget);

	void selectAllWidgets(void);
	void deselectAllWidgets(void);
	void selectWidgetsInBounds(void);
	void deleteSelectedWidgets(void);
	void moveSelectedWidgets(Point offset);

public:
	bool loadResources(WidgetResourceMap* widgetResourceMap);
	void draw(Context* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




