/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../../gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetDesigner::WidgetDesigner(WidgetDesignerModel* model):
	_WidgetDesignerModel{ model }
{
	_WidgetDesignerMouseTool = std::make_unique<WidgetDesignerMouseTool>(this);
	_WidgetDesignerFacility  = std::make_unique<WidgetDesignerFacility>(this);
}

//===========================================================================
void WidgetDesigner::setGridSize(std::uint64_t x, std::uint64_t y)
{
	_WidgetDesignerFacility->setGridSize(x, y);
}

void WidgetDesigner::getGridSize(std::uint64_t& x, std::uint64_t& y) const
{
	_WidgetDesignerFacility->getGridSize(x, y);
}

bool WidgetDesigner::getSnapToGrid(void) const
{
	return _WidgetDesignerFacility->getSnapToGrid();
}

void WidgetDesigner::setSnapToGrid(bool snapToGrid)
{
	_WidgetDesignerFacility->setSnapToGrid(snapToGrid);
}

Point WidgetDesigner::toGridPoint(Point p) const
{
	return _WidgetDesignerFacility->toGridPoint(p);
}

Point WidgetDesigner::toSnappedPoint(Point p) const
{
	return _WidgetDesignerFacility->toSnappedPoint(p);
}

//===========================================================================
bool WidgetDesigner::isSelectedWidget(WidgetSharedPtr widget)
{
	return _WidgetDesignerFacility->isSelectedWidget(widget);
}

void WidgetDesigner::selectWidget(WidgetSharedPtr target)
{
	_WidgetDesignerFacility->selectWidget(target);
}

void WidgetDesigner::toggleWidgetSelection(WidgetSharedPtr widget)
{
	_WidgetDesignerFacility->toggleWidgetSelection(widget);
}

//===========================================================================
void WidgetDesigner::selectAllWidgets(void)
{
	_WidgetDesignerFacility->selectAllWidgets();
}

void WidgetDesigner::deselectAllWidgets(void)
{
	_WidgetDesignerFacility->deselectAllWidgets();
}

void WidgetDesigner::selectWidgetsInBounds(void)
{
	_WidgetDesignerFacility->selectWidgetsInBounds();
}

void WidgetDesigner::deleteSelectedWidgets(void)
{
	_WidgetDesignerFacility->deleteSelectedWidgets();
}

void WidgetDesigner::moveSelectedWidgets(Point offset)
{
	_WidgetDesignerFacility->moveSelectedWidgets(offset);
}

//===========================================================================
bool WidgetDesigner::loadResources(WidgetResourceMap* widgetResourceMap)
{
	return _WidgetDesignerMouseTool->loadResources(widgetResourceMap);
}

void WidgetDesigner::draw(Context* ctx)
{
	_WidgetDesignerMouseTool->draw(ctx);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




