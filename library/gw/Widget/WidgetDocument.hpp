/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetDocument
{
public:
	std::vector< WidgetSharedPtr > _Widgets;

public:
	Widget::PropertyChangedHandler _WidgetPropertyChangedHandler;

private:
	WidgetEventDispatcher _WidgetEventDispatcher;
	WidgetFactory _WidgetFactory;

public:
	WidgetDocument();

public:
	virtual ~WidgetDocument();

public:
	WidgetDocument(const WidgetDocument&) = delete;
	WidgetDocument& operator=(const WidgetDocument&) = delete;

	WidgetDocument(WidgetDocument&&) = delete;
	WidgetDocument& operator=(WidgetDocument&&) = delete;

public:
	virtual WidgetEventDispatcher* getWidgetEventDispatcher(void);
	virtual WidgetFactory* getWidgetFactory(void);

public:
	virtual void addWidget(WidgetSharedPtr widget);
	virtual void removeWidget(WidgetSharedPtr widget);
	virtual void clear(void);

public:
	virtual WidgetSharedPtr find(const Point& test);

public:
	virtual void setWidgetPropertyChangedHandler(
		Widget::PropertyChangedHandler handler
	);
	virtual void setWidgetPropertyChanged(
		Widget::PropertyChangedParam* param
	);

public:
	virtual bool loadResources(Context* ctx);
	virtual void draw(Context* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




