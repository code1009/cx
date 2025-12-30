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
WidgetDesignerFacility::WidgetDesignerFacility(WidgetDesigner* widgetDesigner):
	_WidgetDesigner{ widgetDesigner }
{
}

//===========================================================================
Point WidgetDesignerFacility::toGridPoint(Point p) const
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

Point WidgetDesignerFacility::toSnappedPoint(Point p) const
{
	if (getSnapToGrid())
	{
		return toGridPoint(p);
	}

	return p;
}

void WidgetDesignerFacility::getGridSize(std::uint64_t& x, std::uint64_t& y) const
{
	x = _GridXSize;
	y = _GridYSize;
}

bool WidgetDesignerFacility::getSnapToGrid(void) const
{
	return _SnapToGrid;
}

void WidgetDesignerFacility::setGridSize(std::uint64_t x, std::uint64_t y)
{
	_GridXSize = x;
	_GridYSize = y;
}

void WidgetDesignerFacility::setSnapToGrid(bool snapToGrid)
{
	_SnapToGrid = snapToGrid;
}

//===========================================================================
bool WidgetDesignerFacility::isSelectedWidget(WidgetSharedPtr widget)
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
	if (designWidget)
	{
		return designWidget->getMarkerVisible();
	}

	return false;
}

#if 0
void WidgetDesignerFacility::selectSingleWidget(WidgetSharedPtr target)
{
	for (auto& widget : _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
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

void WidgetDesignerFacility::toggleWidgetSelection(WidgetSharedPtr widget)
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

//===========================================================================
void WidgetDesignerFacility::selectAllWidgets(void)
{
	for (auto& widget : _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			designWidget->setMarkerVisible(true);
		}
	}
}

void WidgetDesignerFacility::deselectAllWidgets(void)
{
	for (auto& widget : _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			designWidget->setMarkerVisible(false);
		}
	}
}

void WidgetDesignerFacility::selectWidgetsInBounds(void)
{
	Point p0;
	Point p1;


	for (auto& widget : _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
	{
		_WidgetDesigner->getWidgetDesignerMouseTool()->getSelectBounds(p0, p1);

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

void WidgetDesignerFacility::moveSelectedWidgets(Point offset)
{
	if (offset.is_zero())
	{
		return;
	}


	for (auto& widget : _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
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

void WidgetDesignerFacility::deleteSelectedWidgets(void)
{
	std::vector<WidgetSharedPtr> widgets;


	for (auto& widget : _WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->_Widgets)
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
		_WidgetDesigner->getWidgetDesignerModel()->getWidgetDocument()->removeWidget(widget);
	}
}
#endif






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




