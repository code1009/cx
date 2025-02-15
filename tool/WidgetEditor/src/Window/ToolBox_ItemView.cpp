/////////////////////////////////////////////////////////////////////////////
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
#include "ToolBox_ItemView.hpp"
#include "ToolBox_ControlWindow.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::ItemView::ItemView(ToolBox::ControlWindow* window) :
	_Window(window)
{
	//-----------------------------------------------------------------------
	getWindow()->getViewport()->setWindowSize(0, 0);
	getWindow()->getViewport()->setDocumentSize(0, 0);
	getWindow()->getViewport()->enableVScrollbar(true);


	//-----------------------------------------------------------------------
	ToolBox::ItemDrawingSharedPtr itemDrawing;
	

	_GroupItemDrawing = std::make_shared<ToolBox::ItemDrawing>();
	_ItemDrawings.push_back(_GroupItemDrawing);
	_SubItemDrawing = std::make_shared<ToolBox::SubItemDrawing>();
	_ItemDrawings.push_back(_SubItemDrawing);


	//-----------------------------------------------------------------------
	ToolBox::GroupItemSharedPtr groupItem;
	ToolBox::SubItemSharedPtr subItem;

	groupItem = makeGroupItem(makeID(), L"GroupItem1");
	addItem(nullptr, groupItem);

	subItem = makeSubItem(makeID(), L"SubItem1");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem2");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem3");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem4");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem5");
	addItem(nullptr, subItem);

	subItem = makeSubItem(makeID(), L"SubItem0");
	addItem(groupItem, subItem);

	recalcLayout();
}

//===========================================================================
ToolBox::ControlWindow* ToolBox::ItemView::getWindow(void)
{
	return _Window;
}

cx::gw::Point ToolBox::ItemView::getItemViewSize(void)
{
	return _ItemViewSize;
}

//===========================================================================
bool ToolBox::ItemView::createDeviceResources(void)
{
	//-----------------------------------------------------------------------
	bool rv;


	//-----------------------------------------------------------------------
	for (auto& itemDrawing : _ItemDrawings)
	{
		rv = itemDrawing->createDeviceResources(this);
		if (!rv)
		{
			return false;
		}
	}

	return true;
}

void ToolBox::ItemView::destroyDeviceResources(void)
{
	for (auto& itemDrawing : _ItemDrawings)
	{
		itemDrawing->destroyDeviceResources();
	}
}

void ToolBox::ItemView::draw(void)
{
	drawItems();
}

//===========================================================================
std::size_t ToolBox::ItemView::makeID(void)
{
	return ++_LastMadeID;
}

ToolBox::ItemSharedPtrs& ToolBox::ItemView::getItems(void)
{
	return _Items;
}

void ToolBox::ItemView::addItem(ToolBox::GroupItemSharedPtr parentItem, ToolBox::ItemSharedPtr item)
{
	if (parentItem)
	{
		item->setDepth(parentItem->getDepth() + 1);
		item->setParentItem(parentItem);

		parentItem->getSubItems().push_back(item);
	}
	else
	{
		item->setDepth(0);
		item->setParentItem(ToolBox::GroupItemSharedPtr());

		getItems().push_back(item);
	}
}

void ToolBox::ItemView::removeItem(ToolBox::ItemSharedPtr item)
{
	if (!item)
	{
		return;
	}


	if (auto parentItem = item->getParentItem().lock())
	{
		removeItem(parentItem->getSubItems(), item);
	}
	else
	{
		removeItem(getItems(), item);
	}
}

void ToolBox::ItemView::removeItem(ToolBox::ItemSharedPtrs& items, ToolBox::ItemSharedPtr item)
{
	auto it = std::find(items.begin(), items.end(), item);

	if (it == items.end())
	{
		return;
	}
	items.erase(it);
}

void ToolBox::ItemView::clearItems(void)
{
	getItems().clear();
}

void ToolBox::ItemView::clearItems(ToolBox::ItemSharedPtrs& items)
{
	items.clear();
}

void ToolBox::ItemView::clearSubItems(ToolBox::GroupItemSharedPtr& groupItem)
{
	clearItems(groupItem->getSubItems());
}

//===========================================================================
ToolBox::ItemSharedPtr ToolBox::ItemView::findItem(std::size_t id)
{
	return findItem(getItems(), id);
}

ToolBox::ItemSharedPtr ToolBox::ItemView::findItem(ToolBox::ItemSharedPtrs& items, std::size_t id)
{
	for (auto& item : items)
	{
		if (item->getID() == id)
		{
			return item;
		}

		if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
		{
			if (!groupItem->getSubItems().empty())
			{
				auto subItem = findItem(groupItem->getSubItems(), id);
				if (subItem)
				{
					return subItem;
				}
			}
		}
	}

	return ToolBox::ItemSharedPtr();
}

//===========================================================================
void ToolBox::ItemView::recalcLayout(void)
{
	std::int64_t window_cx;
	std::int64_t window_cy;
	getWindow()->getViewport()->getWindowSize(window_cx, window_cy);


	cx::gw::coord_t size;
	size = static_cast<cx::gw::coord_t>(window_cx);


	cx::gw::coord_t offset;
	offset = 0;
	for (auto& item : getItems())
	{
		offset = recalcItemLayout(offset, size, item);
	}


	_ItemViewSize = cx::gw::Point{ size, offset };
}

cx::gw::coord_t ToolBox::ItemView::recalcItemLayout(cx::gw::coord_t offset, cx::gw::coord_t size, ToolBox::ItemSharedPtr item)
{
	item->setX(0);
	item->setY(offset);

	item->setCX(size);
	item->setCY(item->getSize());

	offset += item->getSize();


	if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
	{
		if (!groupItem->getSubItems().empty())
		{
			if (!groupItem->isCollapseSubItems())
			{
				for (auto& subItem : groupItem->getSubItems())
				{
					offset = recalcItemLayout(offset, size, subItem);
				}
			}
		}
	}
	
	return offset;
}

//===========================================================================
ToolBox::ItemDrawingSharedPtr ToolBox::ItemView::getItemDrawing(ToolBox::ItemSharedPtr item)
{
	if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
	{
		return _GroupItemDrawing;
	}
	if (auto subItem = std::dynamic_pointer_cast<ToolBox::SubItem>(item))
	{
		return _SubItemDrawing;
	}

	return _GroupItemDrawing;
}

//===========================================================================
void ToolBox::ItemView::drawItems(void)
{
	for (auto& item : getItems())
	{
		drawItem(item);
	}
}

void ToolBox::ItemView::drawItem(ToolBox::ItemSharedPtr item)
{
	auto itemDrawing = getItemDrawing(item);
	if (itemDrawing)
	{
		itemDrawing->draw(this, item.get());

		if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
		{
			if (!groupItem->getSubItems().empty())
			{
				if (!groupItem->isCollapseSubItems())
				{
					for (auto& subItem : groupItem->getSubItems())
					{
						drawItem(subItem);
					}
				}
			}
		}
	}
}

