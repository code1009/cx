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
	WidgetDesignerCommandManager* getWidgetDesignerCommandManager(void) const { return _WidgetDesignerCommandManager.get(); }

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
	void moveSelectedWidgets(Point offset);
	void deleteSelectedWidgets(void);
	void addWidget(WidgetSharedPtr widget);

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




