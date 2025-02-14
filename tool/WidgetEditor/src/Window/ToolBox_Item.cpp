﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "../../res/resource.h"

//===========================================================================
#include "ToolBox.hpp"

#include "ToolBox_Item.hpp"
#include "ToolBox_Drawing.hpp"
#include "ToolBox_ControlWindow.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::size_t   ToolBox::Item::getID(void) const       { return _ID; }
void          ToolBox::Item::setID(std::size_t id) { _ID = id; }

std::wstring  ToolBox::Item::getCaption(void) const                 { return _Caption; }
void          ToolBox::Item::setCaption(const std::wstring& caption){ _Caption = caption; }

std::wstring  ToolBox::Item::getDescription(void) const                      { return _Description; }
void          ToolBox::Item::setDescription(const std::wstring& description) { _Description = description; }

std::wstring  ToolBox::Item::getIcon(void) const               { return _Icon; }
void          ToolBox::Item::setIcon(const std::wstring& icon) { _Icon = icon; }

cx::gw::coord_t ToolBox::Item::getSize(void) const               { return _Size; }
void            ToolBox::Item::setSize(const cx::gw::coord_t size) { _Size = size; }

//===========================================================================
ToolBox::GroupItemWeakPtr ToolBox::Item::getParentItem(void) const
{
	return _ParentItem; 
}

void ToolBox::Item::setParentItem(ToolBox::GroupItemWeakPtr parentItem)
{
	_ParentItem = parentItem; 
}

std::size_t      ToolBox::Item::getDepth(void) const        { return _Depth; }
void             ToolBox::Item::setDepth(std::size_t depth) { _Depth = depth; }

cx::gw::coord_t  ToolBox::Item::getCX(void) const          { return _p1._x - _p0._x; }
cx::gw::coord_t  ToolBox::Item::getCY(void) const          { return _p1._y - _p0._y; }
void             ToolBox::Item::setCX(cx::gw::coord_t cx)  { _p1._x = _p0._x + cx; }
void             ToolBox::Item::setCY(cx::gw::coord_t cy)  { _p1._y = _p0._y + cy; }

cx::gw::coord_t  ToolBox::Item::getX(void) const         { return _p0._x; }
cx::gw::coord_t  ToolBox::Item::getY(void) const         { return _p0._y; }
void             ToolBox::Item::setX(cx::gw::coord_t x)  { _p0._x = x; }
void             ToolBox::Item::setY(cx::gw::coord_t y)  { _p0._y = y; }

void ToolBox::Item::setP0(const cx::gw::Point& p0)
{
	_p0 = p0;
}

void ToolBox::Item::setP1(const cx::gw::Point& p1)
{
	_p1 = p1;
}

cx::gw::Point ToolBox::Item::getP0(void) const
{
	return _p0;
}

cx::gw::Point ToolBox::Item::getP1(void) const
{
	return _p1;
}

ToolBox::GroupItemWeakPtr ToolBox::Item::getRootItem(void) const
{
	ToolBox::GroupItemWeakPtr item = getParentItem();


	while (auto parent = item.lock())
	{
		if (auto grandParent = parent->getParentItem().lock())
		{
			item = grandParent;
		}
		else
		{
			break;
		}
	}

	return item;
}

//===========================================================================
cx::gw::coord_t ToolBox::Item::calcHeight(void) { return getCX(); }
cx::gw::coord_t ToolBox::Item::calcWidth(void)  { return getCY(); }





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cx::gw::coord_t ToolBox::GroupItem::calcHeight(void)
{
	cx::gw::coord_t height = getCY();


	if (!_CollapseSubItems)
	{
		for (auto& item : _SubItems)
		{
			height += item->calcHeight();
		}
	}

	return height;
}

cx::gw::coord_t ToolBox::GroupItem::calcWidth(void)
{
	cx::gw::coord_t width = getCX();


	if (!_CollapseSubItems)
	{
		for (auto& item : _SubItems)
		{
			width += item->calcWidth();
		}
	}

	return width;
}

ToolBox::ItemSharedPtrs& ToolBox::GroupItem::getSubItems(void)
{
	return _SubItems;
}

void ToolBox::GroupItem::CollapseSubItems(bool collapse)
{
	_CollapseSubItems = collapse;
}

bool ToolBox::GroupItem::isCollapseSubItems(void) const
{
	return _CollapseSubItems;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::GroupItemSharedPtr makeGroupItem(
	std::size_t id,
	std::wstring caption, 
	std::wstring description, 
	std::wstring icon, 
	cx::gw::coord_t size
)
{
	auto item = std::make_shared<ToolBox::GroupItem>();

	item->setID(id);
	item->setCaption(caption);
	item->setDescription(description);
	item->setIcon(icon);
	item->setSize(size);

	return item;
}

ToolBox::SubItemSharedPtr makeSubItem(
	std::size_t id,
	std::wstring caption,
	std::wstring description,
	std::wstring icon,
	cx::gw::coord_t size
)
{
	auto item = std::make_shared<ToolBox::SubItem>();

	item->setID(id);
	item->setCaption(caption);
	item->setDescription(description);
	item->setIcon(icon);
	item->setSize(size);

	return item;
}



