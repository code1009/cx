/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/wui/wui.hpp>
#include <cx/runtime/runtime.hpp>

//===========================================================================
#include <cx/d2d/cx-d2d.hpp>
#include <cx/d2dw/cx-Widget.hpp>

//===========================================================================
#include "Catalog.hpp"

//===========================================================================
#include "../WindowHandler/WindowHandler.hpp"
#include "Designer.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void Catalog::addLabel(std::wstring Label)
{
	CatalogInfo info{ L"Label", Label, L"" };
	_Items.push_back(info);
}

void Catalog::addSpare()
{
	CatalogInfo info{ L"Spare", L"", L"" };
	_Items.push_back(info);
}

void Catalog::addItem(
	std::shared_ptr<cx::Widget::Item> const& item, 
	cx::Widget::ClassInfo::MakePropertiesFunction const& makeProperties, 
	cx::Widget::StringView const& friendlyName
)
{
	if (!item)
	{
		return;
	}
	if (!makeProperties)
	{
		return;
	}


	_Designer->edit()->factory().registerItem(item, makeProperties, friendlyName);


	CatalogInfo info{ L"Item", friendlyName.data(), L"" };
	_Items.push_back(info);
}
