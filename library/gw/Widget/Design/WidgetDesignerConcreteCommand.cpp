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
WidgetDesignerCommand_addWidget::WidgetDesignerCommand_addWidget(WidgetDesignerModel* widgetDesignerModel, WidgetSharedPtr widget):
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
WidgetDesignerCommand_deleteWidgets::WidgetDesignerCommand_deleteWidgets(WidgetDesignerModel* widgetDesignerModel, std::vector<WidgetSharedPtr>& widgets) :
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
}




