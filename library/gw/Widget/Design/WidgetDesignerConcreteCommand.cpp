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
WidgetDesignerCommand_addWidget::WidgetDesignerCommand_addWidget(
	WidgetDesignerModel* widgetDesignerModel, 
	WidgetSharedPtr widget
) :
	_WidgetDesignerModel{ widgetDesignerModel },
	_Widget { widget }
{
}

void WidgetDesignerCommand_addWidget::execute(void)
{
	_WidgetDesignerModel->getWidgetDocument()->addWidget(_Widget);
}

void WidgetDesignerCommand_addWidget::undo(void)
{
	_WidgetDesignerModel->getWidgetDocument()->removeWidget(_Widget);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetDesignerCommand_deleteWidgets::WidgetDesignerCommand_deleteWidgets(
	WidgetDesignerModel* widgetDesignerModel, 
	std::vector<WidgetSharedPtr>& widgets
) :
	_WidgetDesignerModel{ widgetDesignerModel },
	_Widgets{ widgets }
{
}

void WidgetDesignerCommand_deleteWidgets::execute(void)
{
	_WidgetMap.clear();


	for (auto& widget : _Widgets)
	{
		auto found = std::find(
			_WidgetDesignerModel->getWidgetDocument()->_Widgets.begin(), 
			_WidgetDesignerModel->getWidgetDocument()->_Widgets.end(), 
			widget);
		if (found != _WidgetDesignerModel->getWidgetDocument()->_Widgets.end())
		{
			auto index = found - _WidgetDesignerModel->getWidgetDocument()->_Widgets.begin();
			_WidgetMap[index] = widget;
		}
	}

	for (auto& widget : _Widgets)
	{
		_WidgetDesignerModel->getWidgetDocument()->removeWidget(widget);
	}
}

void WidgetDesignerCommand_deleteWidgets::undo(void)
{
	for (auto& [index, widget] : _WidgetMap)
	{
		_WidgetDesignerModel->getWidgetDocument()->_Widgets.insert(
			_WidgetDesignerModel->getWidgetDocument()->_Widgets.begin() + index,
			widget);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetDesignerCommand_moveWidgets::WidgetDesignerCommand_moveWidgets(
	WidgetDesignerModel* widgetDesignerModel,
	std::vector<WidgetSharedPtr>& widgets,
	Point& offset
) :
	_WidgetDesignerModel{ widgetDesignerModel },
	_Widgets{ widgets },
	_Offset{ offset }
{
}

void WidgetDesignerCommand_moveWidgets::execute(void)
{
	for (auto& widget : _Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			designWidget->moveOffset(_Offset);
		}
	}
}

void WidgetDesignerCommand_moveWidgets::undo(void)
{
	for (auto& widget : _Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			designWidget->moveOffset(-_Offset);
		}
	}
}

void WidgetDesignerCommand_moveWidgets::moveOffset(Point& offset)
{
	_Offset += offset;


	for (auto& widget : _Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			designWidget->moveOffset(offset);
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetDesignerCommand_selectWidgets::WidgetDesignerCommand_selectWidgets(
	WidgetDesignerModel* widgetDesignerModel,
	std::vector<WidgetSharedPtr>& preselectedWidgets,
	std::vector<WidgetSharedPtr>& selectedWidgets
) :
	_WidgetDesignerModel{ widgetDesignerModel },
	_PreselectedWidgets{ preselectedWidgets },
	_SelectedWidgets{ selectedWidgets }
{
}

void WidgetDesignerCommand_selectWidgets::execute(void)
{
	for (auto& widget : _WidgetDesignerModel->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			auto found = std::find(
				_SelectedWidgets.begin(),
				_SelectedWidgets.end(),
				widget);
			if (found != _SelectedWidgets.end())
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

void WidgetDesignerCommand_selectWidgets::undo(void)
{
	for (auto& widget : _WidgetDesignerModel->getWidgetDocument()->_Widgets)
	{
		DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(widget);
		if (designWidget)
		{
			auto found = std::find(
				_PreselectedWidgets.begin(),
				_PreselectedWidgets.end(),
				widget);
			if (found != _PreselectedWidgets.end())
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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetDesignerCommand_toggleWidgetSelection::WidgetDesignerCommand_toggleWidgetSelection(
	WidgetDesignerModel* widgetDesignerModel,
	WidgetSharedPtr widget
) :
	_WidgetDesignerModel{ widgetDesignerModel },
	_Widget{ widget }
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_Widget);
	if (designWidget)
	{
		if (designWidget->getMarkerVisible())
		{
			_IsSelected = true;
		}
		else
		{
			_IsSelected = false;
		}
	}
}

void WidgetDesignerCommand_toggleWidgetSelection::execute(void)
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_Widget);
	if (designWidget)
	{
		if (_IsSelected)
		{
			designWidget->setMarkerVisible(false);
		}
		else
		{
			designWidget->setMarkerVisible(true);
		}
	}
}

void WidgetDesignerCommand_toggleWidgetSelection::undo(void)
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_Widget);
	if (designWidget)
	{
		if (_IsSelected)
		{
			designWidget->setMarkerVisible(true);
		}
		else
		{
			designWidget->setMarkerVisible(false);
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetDesignerCommand_moveDesignWidgetMarker::WidgetDesignerCommand_moveDesignWidgetMarker(
	WidgetDesignerModel* widgetDesignerModel,
	WidgetSharedPtr widget,
	DesignWidgetMarkerId markerId,
	const Point& point
) :
	_WidgetDesignerModel{ widgetDesignerModel },
	_Widget{ widget },
	_MarkerId { markerId },
	_Point { point }
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_Widget);
	if (!designWidget)
	{
		return;
	}

	_OldTargetWidgetPoints = designWidget->getTargetWidget()->getPoints();
	_OldDesignWidgetPoints = designWidget->getPoints();
}

void WidgetDesignerCommand_moveDesignWidgetMarker::execute(void)
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_Widget);
	if (!designWidget)
	{
		return;
	}

	designWidget->moveMarker(_MarkerId, _Point);

	_NewTargetWidgetPoints = designWidget->getTargetWidget()->getPoints();
	_NewDesignWidgetPoints = designWidget->getPoints();
}

void WidgetDesignerCommand_moveDesignWidgetMarker::undo(void)
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_Widget);
	if (!designWidget)
	{
		return;
	}

	designWidget->getTargetWidget()->setPoints(_OldTargetWidgetPoints);
	designWidget->setPoints(_OldDesignWidgetPoints);
}

bool WidgetDesignerCommand_moveDesignWidgetMarker::moveDesignWidgetMarker(const Point& point)
{
	DesignWidgetSharedPtr designWidget = std::dynamic_pointer_cast<DesignWidget>(_Widget);
	if (!designWidget)
	{
		return false;
	}

	_Point = point;
	designWidget->moveMarker(_MarkerId, _Point);

	_NewTargetWidgetPoints = designWidget->getTargetWidget()->getPoints();
	_NewDesignWidgetPoints = designWidget->getPoints();

	if ( (_OldTargetWidgetPoints == _NewTargetWidgetPoints) &&
	     (_OldDesignWidgetPoints == _NewDesignWidgetPoints) )
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




