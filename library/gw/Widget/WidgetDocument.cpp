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
WidgetDocument::WidgetDocument()
{
}

WidgetDocument::~WidgetDocument()
{
}

//===========================================================================
WidgetEventDispatcher* WidgetDocument::getWidgetEventDispatcher(void)
{
	return &_WidgetEventDispatcher;
}

WidgetFactory* WidgetDocument::getWidgetFactory(void)
{
	return &_WidgetFactory;
}

//===========================================================================
void WidgetDocument::addWidget(WidgetSharedPtr widget)
{
	if (widget.get())
	{
		widget->setPropertyChangedHandler(
			std::bind(&WidgetDocument::setWidgetPropertyChanged, this, std::placeholders::_1)
		);

		widget->registerEventHandler(this);

		_Widgets.push_back(widget);
	}
}

void WidgetDocument::removeWidget(WidgetSharedPtr widget)
{
	_Widgets.erase(
		std::remove(_Widgets.begin(), _Widgets.end(), widget),
		_Widgets.end()
	);
}

void WidgetDocument::clear(void) 
{
	_Widgets.clear();
}

//===========================================================================
WidgetSharedPtr WidgetDocument::find(const Point& test)
{
	WidgetSharedPtr widget;


	for (auto it = _Widgets.rbegin(); it!= _Widgets.rend(); it++)
	{
		widget = *it;


		if (widget->isPointIn(test))
		{
			return widget;
		}
	}

	return nullptr;
}

//===========================================================================
void WidgetDocument::setWidgetPropertyChangedHandler(
	Widget::PropertyChangedHandler handler
)
{
	_WidgetPropertyChangedHandler = handler;
}

void WidgetDocument::setWidgetPropertyChanged(
	Widget::PropertyChangedParam* param
)
{
	if (_WidgetPropertyChangedHandler)
	{
		_WidgetPropertyChangedHandler(param);
	}
}

//===========================================================================
bool WidgetDocument::loadResources(Context* ctx)
{
	_WidgetFactory.loadResources(ctx);


	for (auto& widget : _Widgets)
	{
		if (!widget->loadResources(ctx))
		{
			return false;
		}
	}

	return true;
}

void WidgetDocument::draw(Context* ctx)
{
	for (auto& widget : _Widgets)
	{
		widget->draw(ctx);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




