/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WidgetFactory
{
public:
	std::unordered_map < std::wstring, WidgetSharedPtr > _WidgetSharedPtrMap;

public:
	WidgetFactory();

public:
	virtual ~WidgetFactory();

public:
	WidgetFactory(const WidgetFactory&) = delete;
	WidgetFactory& operator=(const WidgetFactory&) = delete;

	WidgetFactory(WidgetFactory&&) = delete;
	WidgetFactory& operator=(WidgetFactory&&) = delete;

public:
	void load(void);

public:
	void addWidget(WidgetSharedPtr widget);
	void removeWidget(std::wstring className);
	void clear(void);

public:
	WidgetSharedPtr find(std::wstring className);

public:
	bool loadResources(Context* ctx);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




