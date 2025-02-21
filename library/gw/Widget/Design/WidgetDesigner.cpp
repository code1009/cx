﻿/////////////////////////////////////////////////////////////////////////////
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
	_WidgetDesignerTool = std::make_unique<WidgetDesignerTool>(this);
}

//===========================================================================
bool WidgetDesigner::getSnapToGrid(void) const
{
	return _SnapToGrid;
}

void WidgetDesigner::setSnapToGrid(bool snapToGrid)
{
	_SnapToGrid = snapToGrid;
}

Point WidgetDesigner::toGridPoint(Point p) const
{
	Point snap;
	std::int64_t x;
	std::int64_t y;
	bool x_minus = false;
	bool y_minus = false;


	x = static_cast<std::int64_t>(p._x);
	y = static_cast<std::int64_t>(p._y);

	if (x < 0)
	{
		x_minus = true;
		x = x * -1;
	}
	if (y < 0)
	{
		y_minus = true;
		y = y * -1;
	}

	x = ((x + _GridXSize / 2) / _GridXSize) * _GridXSize;
	y = ((y + _GridYSize / 2) / _GridYSize) * _GridYSize;

	if (x_minus)
	{
		x = x * -1;
	}
	if (y_minus)
	{
		y = y * -1;
	}

	snap._x = static_cast<coord_t>(x);
	snap._y = static_cast<coord_t>(y);

	return snap;
}

Point WidgetDesigner::toSnappedPoint(Point p) const
{
	if (getSnapToGrid())
	{
		return toGridPoint(p);
	}

	return p;
}

//===========================================================================
bool WidgetDesigner::isSelected(WidgetSharedPtr widget)
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
	if (designWidget)
	{
		return designWidget->getMarkerVisible();
	}

	return false;
}

void WidgetDesigner::unselectAll(void)
{
	for (auto& widget : _WidgetDesignerModel->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			designWidget->setMarkerVisible(false);
		}
	}
}

void WidgetDesigner::selectBounds(void)
{
	Point p0;
	Point p1;


	for (auto& widget : _WidgetDesignerModel->getWidgetDocument()->_Widgets)
	{
		_WidgetDesignerTool->getSelectBounds(p0, p1);

		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (isPointsInBounds(p0, p1, designWidget->getPoints()))
			{
				designWidget->setMarkerVisible(true);
			}
			else
			{
				designWidget->setMarkerVisible(false);
			}
		}
	}
}

void WidgetDesigner::selectSingle(WidgetSharedPtr target)
{
	for (auto& widget : _WidgetDesignerModel->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (widget == target)
			{
				designWidget->setMarkerVisible(true);
			}
			else
			{
				designWidget->setMarkerVisible(false);
			}
		}
	}
}

void WidgetDesigner::toggleSelection(WidgetSharedPtr widget)
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
	if (designWidget)
	{
		if (designWidget->getMarkerVisible())
		{
			designWidget->setMarkerVisible(false);
		}
		else
		{
			designWidget->setMarkerVisible(true);
		}
	}
}

void WidgetDesigner::moveSelection(Point offset)
{
	for (auto& widget : _WidgetDesignerModel->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			if (designWidget->getMarkerVisible())
			{
				designWidget->moveOffset(offset);
			}
		}
	}
}

void WidgetDesigner::deleteSelection(void)
{
	std::vector<WidgetSharedPtr> widgets;


	for (auto& widget : _WidgetDesignerModel->getWidgetDocument()->_Widgets)
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


	for (auto& widget : widgets)
	{
		_WidgetDesignerModel->getWidgetDocument()->removeWidget(widget);
	}
}

//===========================================================================
bool WidgetDesigner::loadResources(WidgetResourceMap* widgetResourceMap)
{
	return _WidgetDesignerTool->loadResources(widgetResourceMap);
}

void WidgetDesigner::draw(Context* ctx)
{
	_WidgetDesignerTool->draw(ctx);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




