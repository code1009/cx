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
	std::unique_ptr<WidgetDesignerTool> _WidgetDesignerTool;

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
	bool getSnapToGrid(void) const;
	void setSnapToGrid(bool snapToGrid);
	Point toGridPoint(Point p) const;
	Point toSnappedPoint(Point p) const;

public:
	bool isSelected(WidgetSharedPtr widget);
	void unselectAll(void);
	void selectBounds(void);
	void selectSingle(WidgetSharedPtr target);
	void toggleSelection(WidgetSharedPtr widget);
	void moveSelection(Point offset);
	void deleteSelection(void);

public:
	bool loadResources(WidgetResourceMap* widgetResourceMap);
	void draw(Context* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




