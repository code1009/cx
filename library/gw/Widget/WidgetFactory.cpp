/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetFactory::WidgetFactory()
{
	load();
}

WidgetFactory::~WidgetFactory()
{
}

//===========================================================================
void WidgetFactory::load(void)
{
	WidgetSharedPtr widget;


	//widget = std::make_shared<RectangleShapeDesignWidget>(); 
	addWidget(widget);
}

//===========================================================================
void WidgetFactory::addWidget(WidgetSharedPtr widget)
{
	if (!widget.get())
	{
		return;
	}


	std::wstring className = widget->getClassName();
	if (_WidgetSharedPtrMap.end() == _WidgetSharedPtrMap.find(className))
	{
		_WidgetSharedPtrMap[className] = widget;
	}
}

void WidgetFactory::removeWidget(std::wstring className)
{
	_WidgetSharedPtrMap.erase(className);
}

void WidgetFactory::clear(void) 
{
	_WidgetSharedPtrMap.clear();
}

//===========================================================================
WidgetSharedPtr WidgetFactory::find(std::wstring class_name)
{
	WidgetSharedPtr widget;


	for (auto& pair : _WidgetSharedPtrMap)
	{
		if (pair.first == class_name)
		{
			return pair.second;
		}
	}

	return nullptr;
}

//===========================================================================
bool WidgetFactory::loadResources(WidgetResourceMap* widgetResourceMap)
{
	WidgetSharedPtr widget;


	for (auto& pair : _WidgetSharedPtrMap)
	{
		if (!pair.second->loadResources(widgetResourceMap))
		{
			return false;
		}
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




