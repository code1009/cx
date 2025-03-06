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
Point WidgetDesigner::toGridPoint(Point p) const
{
	return _WidgetDesignerFacility->toGridPoint(p);
}

Point WidgetDesigner::toSnappedPoint(Point p) const
{
	return _WidgetDesignerFacility->toSnappedPoint(p);
}

void WidgetDesigner::getGridSize(std::uint64_t& x, std::uint64_t& y) const
{
	_WidgetDesignerFacility->getGridSize(x, y);
}

bool WidgetDesigner::getSnapToGrid(void) const
{
	return _WidgetDesignerFacility->getSnapToGrid();
}

void WidgetDesigner::setGridSize(std::uint64_t x, std::uint64_t y)
{
	//_WidgetDesignerFacility->setGridSize(x, y);


	auto command = std::make_shared<WidgetDesignerCommand_setGridSize>(_WidgetDesignerModel, x, y);
	_WidgetDesignerCommandManager->executeCommand(command);
}

void WidgetDesigner::setSnapToGrid(bool snapToGrid)
{
	//_WidgetDesignerFacility->setSnapToGrid(snapToGrid);


	auto command = std::make_shared<WidgetDesignerCommand_setSnapToGrid>(_WidgetDesignerModel, snapToGrid);
	_WidgetDesignerCommandManager->executeCommand(command);
}

//===========================================================================
bool WidgetDesigner::isSelectedWidget(WidgetSharedPtr widget)
{
	return _WidgetDesignerFacility->isSelectedWidget(widget);
}

void WidgetDesigner::selectSingleWidget(WidgetSharedPtr target)
{
	//_WidgetDesignerFacility->selectSingleWidget(target);


	if (getWidgetDesignerModel()->getWidgetDocument()->_Widgets.empty())
	{
		return;
	}


	std::vector<WidgetSharedPtr> preselectedWidgets;
	std::vector<WidgetSharedPtr> selectedWidgets;

	for (auto& widget : getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (designWidget->getMarkerVisible())
			{
				preselectedWidgets.push_back(widget);
			}
		}
	}
	selectedWidgets.push_back(target);


	std::shared_ptr<WidgetDesignerCommand> command;
	command = std::make_shared<WidgetDesignerCommand_selectWidgets>(
		_WidgetDesignerModel,
		preselectedWidgets,
		selectedWidgets
	);
	_WidgetDesignerCommandManager->executeCommand(command);
}

void WidgetDesigner::toggleWidgetSelection(WidgetSharedPtr widget)
{
	//_WidgetDesignerFacility->toggleWidgetSelection(widget);


	if (getWidgetDesignerModel()->getWidgetDocument()->_Widgets.empty())
	{
		return;
	}


	std::shared_ptr<WidgetDesignerCommand> command;
	command = std::make_shared<WidgetDesignerCommand_toggleWidgetSelection>(
		_WidgetDesignerModel,
		widget
	);
	_WidgetDesignerCommandManager->executeCommand(command);
}

//===========================================================================
void WidgetDesigner::selectAllWidgets(void)
{
	//_WidgetDesignerFacility->selectAllWidgets();


	if (getWidgetDesignerModel()->getWidgetDocument()->_Widgets.empty())
	{
		return;
	}


	std::vector<WidgetSharedPtr> preselectedWidgets;
	std::vector<WidgetSharedPtr> selectedWidgets;

	for (auto& widget : getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (designWidget->getMarkerVisible())
			{
				preselectedWidgets.push_back(widget);
			}

			selectedWidgets.push_back(widget);
		}
	}


	std::shared_ptr<WidgetDesignerCommand> command;
	command = std::make_shared<WidgetDesignerCommand_selectWidgets>(
		_WidgetDesignerModel,
		preselectedWidgets,
		selectedWidgets
	);
	_WidgetDesignerCommandManager->executeCommand(command);
}

void WidgetDesigner::deselectAllWidgets(void)
{
	//_WidgetDesignerFacility->deselectAllWidgets();


	if (getWidgetDesignerModel()->getWidgetDocument()->_Widgets.empty())
	{
		return;
	}


	std::vector<WidgetSharedPtr> preselectedWidgets;
	std::vector<WidgetSharedPtr> selectedWidgets;

	for (auto& widget : getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (designWidget->getMarkerVisible())
			{
				preselectedWidgets.push_back(widget);
			}
		}
	}


	std::shared_ptr<WidgetDesignerCommand> command;
	command = std::make_shared<WidgetDesignerCommand_selectWidgets>(
		_WidgetDesignerModel,
		preselectedWidgets,
		selectedWidgets
	);
	_WidgetDesignerCommandManager->executeCommand(command);
}

void WidgetDesigner::selectWidgetsInBounds(void)
{
	//WidgetDesignerFacility->selectWidgetsInBounds();


	if (getWidgetDesignerModel()->getWidgetDocument()->_Widgets.empty())
	{
		return;
	}


	std::vector<WidgetSharedPtr> preselectedWidgets;
	std::vector<WidgetSharedPtr> selectedWidgets;

	Point p0;
	Point p1;


	for (auto& widget : getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		getWidgetDesignerMouseTool()->getSelectBounds(p0, p1);

		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (designWidget->getMarkerVisible())
			{
				preselectedWidgets.push_back(widget);
			}

			if (isPointsInBounds(p0, p1, designWidget->getPoints()))
			{
				selectedWidgets.push_back(widget);				
			}
		}
	}


	std::shared_ptr<WidgetDesignerCommand> command;
	command = std::make_shared<WidgetDesignerCommand_selectWidgets>(
		_WidgetDesignerModel, 
		preselectedWidgets, 
		selectedWidgets
	);
	_WidgetDesignerCommandManager->executeCommand(command);
}

void WidgetDesigner::moveSelectedWidgets(Point offset)
{
	//_WidgetDesignerFacility->moveSelectedWidgets(offset);


	if (getWidgetDesignerModel()->getWidgetDocument()->_Widgets.empty())
	{
		return;
	}


	if (offset.is_zero())
	{
		return;
	}


	std::vector<WidgetSharedPtr> selectedWidgets;
	for (auto& widget : getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (designWidget->getMarkerVisible())
			{
				selectedWidgets.push_back(widget);
			}
		}
	}


	std::shared_ptr<WidgetDesignerCommand> command;
	command = _WidgetDesignerCommandManager->getLastExecutedCommand();
	auto command_moveWidgets = std::dynamic_pointer_cast<WidgetDesignerCommand_moveWidgets>(command);
	if (command_moveWidgets)
	{
		if (selectedWidgets==command_moveWidgets->getWidgets())
		{
			command_moveWidgets->moveOffset(offset);
			return;
		}
	}
	command = std::make_shared<WidgetDesignerCommand_moveWidgets>(
		_WidgetDesignerModel,
		selectedWidgets,
		offset
	);
	_WidgetDesignerCommandManager->executeCommand(command);
}

void WidgetDesigner::deleteSelectedWidgets(void)
{
	//_WidgetDesignerFacility->deleteSelectedWidgets();


	if (getWidgetDesignerModel()->getWidgetDocument()->_Widgets.empty())
	{
		return;
	}


	std::vector<WidgetSharedPtr> selectedWidgets;
	for (auto& widget : getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (designWidget->getMarkerVisible())
			{
				selectedWidgets.push_back(widget);
			}
		}
	}


	std::shared_ptr<WidgetDesignerCommand> command;
	command = std::make_shared<WidgetDesignerCommand_deleteWidgets>(
		_WidgetDesignerModel, 
		selectedWidgets
	);
	_WidgetDesignerCommandManager->executeCommand(command);
}

void WidgetDesigner::addWidget(WidgetSharedPtr widget)
{
	std::shared_ptr<WidgetDesignerCommand> command;
	command = std::make_shared<WidgetDesignerCommand_addWidget>(
		_WidgetDesignerModel, 
		widget
	);
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




