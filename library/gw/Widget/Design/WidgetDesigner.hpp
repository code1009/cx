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




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDesigner
{
protected:
	WidgetDesignerModel* _WidgetDesignerModel{ nullptr };

protected:
	std::unique_ptr<WidgetDesignerMouseTool>       _WidgetDesignerMouseTool;
	std::unique_ptr<WidgetDesignerFacility>        _WidgetDesignerFacility;
	std::unique_ptr<WidgetDesignerCommandManager>  _WidgetDesignerCommandManager;

public:
	explicit WidgetDesigner(WidgetDesignerModel* widgetDesignerModel);

public:
	virtual ~WidgetDesigner() = default;

public:
	WidgetDesignerModel*          getWidgetDesignerModel         (void) const { return _WidgetDesignerModel;           }
	WidgetDesignerMouseTool*      getWidgetDesignerMouseTool     (void) const { return _WidgetDesignerMouseTool.get(); }
	WidgetDesignerFacility*       getWidgetDesignerFacility      (void) const { return _WidgetDesignerFacility.get();  }
	//WidgetDesignerCommandManager* getWidgetDesignerCommandManager(void) const { return _WidgetDesignerCommandManager.get(); }

public:
	WidgetDesigner(const WidgetDesigner&) = delete;
	WidgetDesigner& operator=(const WidgetDesigner&) = delete;

	WidgetDesigner(WidgetDesigner&&) = delete;
	WidgetDesigner& operator=(WidgetDesigner&&) = delete;

public:
	bool hasDesignWidget(void);
	bool isDesignWidgetIn(WidgetSharedPtr designWidgetSharedPtr);

public:
	Point toGridPoint(Point p) const;
	Point toSnappedPoint(Point p) const;
	void getGridSize(std::uint64_t& x, std::uint64_t& y) const;
	bool getSnapToGrid(void) const;
	void setGridSize(std::uint64_t x, std::uint64_t y);
	void setSnapToGrid(bool snapToGrid);

public:
	bool isSelectedWidget(WidgetSharedPtr widget);
	void selectSingleWidget(WidgetSharedPtr target);
	void toggleWidgetSelection(WidgetSharedPtr widget);

public:
	void selectAllWidgets(void);
	void deselectAllWidgets(void);
	void selectWidgetsInBounds(void);
	void moveSelectedWidgets(Point offset);
	void deleteSelectedWidgets(void);
	void addWidget(WidgetSharedPtr widget);

public:
	void moveDesignWidgetMarker(WidgetSharedPtr widget, DesignWidgetMarkerId markerId, Point point);

public:
	void undo(void);
	void redo(void);

public:
	bool loadResources(WidgetResourceMap* widgetResourceMap);
	void draw(Context* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




