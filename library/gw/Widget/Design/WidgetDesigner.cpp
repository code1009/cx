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
	_WidgetDesignerMouseTool      = std::make_unique<WidgetDesignerMouseTool>(this);
	_WidgetDesignerFacility       = std::make_unique<WidgetDesignerFacility>(this);
	_WidgetDesignerCommandManager = std::make_unique<WidgetDesignerCommandManager>();
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

void WidgetDesigner::moveSelectedWidgets(Point offset)
{
	_WidgetDesignerFacility->moveSelectedWidgets(offset);
}

void WidgetDesigner::deleteSelectedWidgets(void)
{
	//_WidgetDesignerFacility->deleteSelectedWidgets();


	std::vector<WidgetSharedPtr> widgets;


	for (auto& widget : getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (designWidget->getMarkerVisible())
			{
				widgets.push_back(widget);
			}
		}
	}


	std::shared_ptr<WidgetDesignerCommand> command;

	command = std::make_shared<WidgetDesignerCommand_deleteWidgets>(_WidgetDesignerModel, widgets);

	_WidgetDesignerCommandManager->executeCommand(command);
}

void WidgetDesigner::addWidget(WidgetSharedPtr widget)
{
	std::shared_ptr<WidgetDesignerCommand> command;

	command = std::make_shared<WidgetDesignerCommand_addWidget>(_WidgetDesignerModel, widget);

	_WidgetDesignerCommandManager->executeCommand(command);
}

//===========================================================================
void WidgetDesigner::undo(void)
{
	_WidgetDesignerCommandManager->undo();
}

void WidgetDesigner::redo(void)
{
	_WidgetDesignerCommandManager->redo();
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




