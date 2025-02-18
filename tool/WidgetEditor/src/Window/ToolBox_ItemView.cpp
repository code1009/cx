﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <gw/gw.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "ToolBox.hpp"
#include "ToolBox_Item.hpp"
#include "ToolBox_Drawing.hpp"
#include "ToolBox_WindowMessageHandler.hpp"
#include "ToolBox_ItemView.hpp"
#include "ToolBox_ControlWindow.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static unsigned char _bitmap_expand_png[558] = {
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 0x61, 0x00, 0x00, 0x01,
	0x84, 0x69, 0x43, 0x43, 0x50, 0x49, 0x43, 0x43, 0x20, 0x70, 0x72, 0x6F,
	0x66, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x28, 0x91, 0x7D, 0x91, 0x3D, 0x48,
	0xC3, 0x50, 0x14, 0x85, 0x4F, 0x53, 0xB5, 0x45, 0x2A, 0x22, 0x76, 0x10,
	0x71, 0xC8, 0x50, 0x9D, 0xEC, 0xA2, 0x22, 0x8E, 0xA5, 0x8A, 0x45, 0xB0,
	0x50, 0xDA, 0x0A, 0xAD, 0x3A, 0x98, 0xBC, 0xF4, 0x0F, 0x9A, 0x34, 0x24,
	0x29, 0x2E, 0x8E, 0x82, 0x6B, 0xC1, 0xC1, 0x9F, 0xC5, 0xAA, 0x83, 0x8B,
	0xB3, 0xAE, 0x0E, 0xAE, 0x82, 0x20, 0xF8, 0x03, 0xE2, 0x2E, 0x38, 0x29,
	0xBA, 0x48, 0x89, 0xF7, 0x25, 0x85, 0x16, 0x31, 0x5E, 0x78, 0xBC, 0x8F,
	0xF3, 0xEE, 0x39, 0xBC, 0x77, 0x1F, 0x20, 0x34, 0xAB, 0x4C, 0x35, 0x7B,
	0x62, 0x80, 0xAA, 0x59, 0x46, 0x3A, 0x11, 0x17, 0x73, 0xF9, 0x55, 0x31,
	0xF0, 0x0A, 0x01, 0x43, 0xF0, 0xA1, 0x0F, 0x41, 0x89, 0x99, 0x7A, 0x32,
	0xB3, 0x98, 0x85, 0x67, 0x7D, 0xDD, 0x53, 0x27, 0xD5, 0x5D, 0x94, 0x67,
	0x79, 0xF7, 0xFD, 0x59, 0x03, 0x4A, 0xC1, 0x64, 0x80, 0x4F, 0x24, 0x8E,
	0x31, 0xDD, 0xB0, 0x88, 0x37, 0x88, 0x67, 0x37, 0x2D, 0x9D, 0xF3, 0x3E,
	0x71, 0x98, 0x95, 0x25, 0x85, 0xF8, 0x9C, 0x78, 0xD2, 0xA0, 0x0B, 0x12,
	0x3F, 0x72, 0x5D, 0x76, 0xF9, 0x8D, 0x73, 0xC9, 0x61, 0x81, 0x67, 0x86,
	0x8D, 0x6C, 0x7A, 0x9E, 0x38, 0x4C, 0x2C, 0x96, 0xBA, 0x58, 0xEE, 0x62,
	0x56, 0x36, 0x54, 0xE2, 0x19, 0xE2, 0x88, 0xA2, 0x6A, 0x94, 0x2F, 0xE4,
	0x5C, 0x56, 0x38, 0x6F, 0x71, 0x56, 0xAB, 0x75, 0xD6, 0xBE, 0x27, 0x7F,
	0x61, 0xA8, 0xA0, 0xAD, 0x64, 0xB8, 0x4E, 0x6B, 0x0C, 0x09, 0x2C, 0x21,
	0x89, 0x14, 0x44, 0xC8, 0xA8, 0xA3, 0x82, 0x2A, 0x2C, 0x44, 0x69, 0xD7,
	0x48, 0x31, 0x91, 0xA6, 0xF3, 0xB8, 0x87, 0x7F, 0xD4, 0xF1, 0xA7, 0xC8,
	0x25, 0x93, 0xAB, 0x02, 0x46, 0x8E, 0x05, 0xD4, 0xA0, 0x42, 0x72, 0xFC,
	0xE0, 0x7F, 0xF0, 0x7B, 0xB6, 0x66, 0x71, 0x7A, 0xCA, 0x4D, 0x0A, 0xC5,
	0x81, 0xDE, 0x17, 0xDB, 0xFE, 0x18, 0x07, 0x02, 0xBB, 0x40, 0xAB, 0x61,
	0xDB, 0xDF, 0xC7, 0xB6, 0xDD, 0x3A, 0x01, 0xFC, 0xCF, 0xC0, 0x95, 0xD6,
	0xF1, 0xD7, 0x9A, 0xC0, 0xDC, 0x27, 0xE9, 0x8D, 0x8E, 0x16, 0x39, 0x02,
	0x06, 0xB7, 0x81, 0x8B, 0xEB, 0x8E, 0x26, 0xEF, 0x01, 0x97, 0x3B, 0xC0,
	0xC8, 0x93, 0x2E, 0x19, 0x92, 0x23, 0xF9, 0x69, 0x09, 0xC5, 0x22, 0xF0,
	0x7E, 0x46, 0xDF, 0x94, 0x07, 0x86, 0x6F, 0x81, 0xFE, 0x35, 0x77, 0x6E,
	0xED, 0x73, 0x9C, 0x3E, 0x00, 0x59, 0x9A, 0xD5, 0xF2, 0x0D, 0x70, 0x70,
	0x08, 0x4C, 0x94, 0x28, 0x7B, 0xDD, 0xE3, 0xDD, 0xC1, 0xEE, 0xB9, 0xFD,
	0xDB, 0xD3, 0x9E, 0xDF, 0x0F, 0x15, 0x63, 0x72, 0x81, 0x36, 0x08, 0xB1,
	0xC4, 0x00, 0x00, 0x00, 0x06, 0x62, 0x4B, 0x47, 0x44, 0x00, 0xF8, 0x00,
	0xFF, 0x00, 0x35, 0x69, 0x2C, 0x50, 0x2D, 0x00, 0x00, 0x00, 0x09, 0x70,
	0x48, 0x59, 0x73, 0x00, 0x00, 0x0E, 0xC0, 0x00, 0x00, 0x0E, 0xC0, 0x01,
	0x6A, 0xD6, 0x89, 0x09, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45,
	0x07, 0xE9, 0x02, 0x11, 0x01, 0x0A, 0x2D, 0xD1, 0x08, 0x5D, 0xB8, 0x00,
	0x00, 0x00, 0x2B, 0x49, 0x44, 0x41, 0x54, 0x38, 0xCB, 0x63, 0x60, 0x18,
	0x05, 0xC4, 0x80, 0xFF, 0xF8, 0x24, 0x99, 0xA8, 0x61, 0x08, 0xB1, 0x9A,
	0xFF, 0x53, 0xC3, 0x0B, 0xFF, 0xA9, 0x11, 0x06, 0xFF, 0x07, 0xCC, 0x05,
	0x14, 0x85, 0xC1, 0x7F, 0x9A, 0xA5, 0x83, 0x61, 0x00, 0x00, 0x48, 0xE2,
	0x0E, 0xF4, 0xA9, 0xA0, 0x8C, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45,
	0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

static unsigned char _bitmap_collapse_png[557] = {
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
	0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
	0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0xF3, 0xFF, 0x61, 0x00, 0x00, 0x01,
	0x84, 0x69, 0x43, 0x43, 0x50, 0x49, 0x43, 0x43, 0x20, 0x70, 0x72, 0x6F,
	0x66, 0x69, 0x6C, 0x65, 0x00, 0x00, 0x28, 0x91, 0x7D, 0x91, 0x3D, 0x48,
	0xC3, 0x50, 0x14, 0x85, 0x4F, 0x53, 0xB5, 0x45, 0x2A, 0x22, 0x76, 0x10,
	0x71, 0xC8, 0x50, 0x9D, 0xEC, 0xA2, 0x22, 0x8E, 0xA5, 0x8A, 0x45, 0xB0,
	0x50, 0xDA, 0x0A, 0xAD, 0x3A, 0x98, 0xBC, 0xF4, 0x0F, 0x9A, 0x34, 0x24,
	0x29, 0x2E, 0x8E, 0x82, 0x6B, 0xC1, 0xC1, 0x9F, 0xC5, 0xAA, 0x83, 0x8B,
	0xB3, 0xAE, 0x0E, 0xAE, 0x82, 0x20, 0xF8, 0x03, 0xE2, 0x2E, 0x38, 0x29,
	0xBA, 0x48, 0x89, 0xF7, 0x25, 0x85, 0x16, 0x31, 0x5E, 0x78, 0xBC, 0x8F,
	0xF3, 0xEE, 0x39, 0xBC, 0x77, 0x1F, 0x20, 0x34, 0xAB, 0x4C, 0x35, 0x7B,
	0x62, 0x80, 0xAA, 0x59, 0x46, 0x3A, 0x11, 0x17, 0x73, 0xF9, 0x55, 0x31,
	0xF0, 0x0A, 0x01, 0x43, 0xF0, 0xA1, 0x0F, 0x41, 0x89, 0x99, 0x7A, 0x32,
	0xB3, 0x98, 0x85, 0x67, 0x7D, 0xDD, 0x53, 0x27, 0xD5, 0x5D, 0x94, 0x67,
	0x79, 0xF7, 0xFD, 0x59, 0x03, 0x4A, 0xC1, 0x64, 0x80, 0x4F, 0x24, 0x8E,
	0x31, 0xDD, 0xB0, 0x88, 0x37, 0x88, 0x67, 0x37, 0x2D, 0x9D, 0xF3, 0x3E,
	0x71, 0x98, 0x95, 0x25, 0x85, 0xF8, 0x9C, 0x78, 0xD2, 0xA0, 0x0B, 0x12,
	0x3F, 0x72, 0x5D, 0x76, 0xF9, 0x8D, 0x73, 0xC9, 0x61, 0x81, 0x67, 0x86,
	0x8D, 0x6C, 0x7A, 0x9E, 0x38, 0x4C, 0x2C, 0x96, 0xBA, 0x58, 0xEE, 0x62,
	0x56, 0x36, 0x54, 0xE2, 0x19, 0xE2, 0x88, 0xA2, 0x6A, 0x94, 0x2F, 0xE4,
	0x5C, 0x56, 0x38, 0x6F, 0x71, 0x56, 0xAB, 0x75, 0xD6, 0xBE, 0x27, 0x7F,
	0x61, 0xA8, 0xA0, 0xAD, 0x64, 0xB8, 0x4E, 0x6B, 0x0C, 0x09, 0x2C, 0x21,
	0x89, 0x14, 0x44, 0xC8, 0xA8, 0xA3, 0x82, 0x2A, 0x2C, 0x44, 0x69, 0xD7,
	0x48, 0x31, 0x91, 0xA6, 0xF3, 0xB8, 0x87, 0x7F, 0xD4, 0xF1, 0xA7, 0xC8,
	0x25, 0x93, 0xAB, 0x02, 0x46, 0x8E, 0x05, 0xD4, 0xA0, 0x42, 0x72, 0xFC,
	0xE0, 0x7F, 0xF0, 0x7B, 0xB6, 0x66, 0x71, 0x7A, 0xCA, 0x4D, 0x0A, 0xC5,
	0x81, 0xDE, 0x17, 0xDB, 0xFE, 0x18, 0x07, 0x02, 0xBB, 0x40, 0xAB, 0x61,
	0xDB, 0xDF, 0xC7, 0xB6, 0xDD, 0x3A, 0x01, 0xFC, 0xCF, 0xC0, 0x95, 0xD6,
	0xF1, 0xD7, 0x9A, 0xC0, 0xDC, 0x27, 0xE9, 0x8D, 0x8E, 0x16, 0x39, 0x02,
	0x06, 0xB7, 0x81, 0x8B, 0xEB, 0x8E, 0x26, 0xEF, 0x01, 0x97, 0x3B, 0xC0,
	0xC8, 0x93, 0x2E, 0x19, 0x92, 0x23, 0xF9, 0x69, 0x09, 0xC5, 0x22, 0xF0,
	0x7E, 0x46, 0xDF, 0x94, 0x07, 0x86, 0x6F, 0x81, 0xFE, 0x35, 0x77, 0x6E,
	0xED, 0x73, 0x9C, 0x3E, 0x00, 0x59, 0x9A, 0xD5, 0xF2, 0x0D, 0x70, 0x70,
	0x08, 0x4C, 0x94, 0x28, 0x7B, 0xDD, 0xE3, 0xDD, 0xC1, 0xEE, 0xB9, 0xFD,
	0xDB, 0xD3, 0x9E, 0xDF, 0x0F, 0x15, 0x63, 0x72, 0x81, 0x36, 0x08, 0xB1,
	0xC4, 0x00, 0x00, 0x00, 0x06, 0x62, 0x4B, 0x47, 0x44, 0x00, 0xF8, 0x00,
	0xFF, 0x00, 0x35, 0x69, 0x2C, 0x50, 0x2D, 0x00, 0x00, 0x00, 0x09, 0x70,
	0x48, 0x59, 0x73, 0x00, 0x00, 0x0E, 0xC0, 0x00, 0x00, 0x0E, 0xC0, 0x01,
	0x6A, 0xD6, 0x89, 0x09, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4D, 0x45,
	0x07, 0xE9, 0x02, 0x11, 0x01, 0x0C, 0x2B, 0x6E, 0x31, 0x5F, 0x0B, 0x00,
	0x00, 0x00, 0x2A, 0x49, 0x44, 0x41, 0x54, 0x38, 0xCB, 0x63, 0x60, 0x18,
	0x05, 0x03, 0x02, 0xFE, 0x23, 0x73, 0x98, 0x28, 0xD1, 0x4C, 0xAA, 0x01,
	0xFF, 0xB1, 0x09, 0x32, 0x51, 0xA2, 0x99, 0x58, 0x03, 0xFE, 0xE3, 0x93,
	0x64, 0xA2, 0x44, 0xF3, 0x28, 0xA0, 0x12, 0x00, 0x00, 0x04, 0x1D, 0x06,
	0x05, 0x09, 0x83, 0x01, 0x53, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E,
	0x44, 0xAE, 0x42, 0x60, 0x82
};




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
	_ItemViewDrawing = std::make_shared<ToolBox::ItemViewDrawing>();


	//-----------------------------------------------------------------------
	ToolBox::ItemDrawingSharedPtr itemDrawing;
	

	_GroupItemDrawing = std::make_shared<ToolBox::GroupItemDrawing>();
	_ItemDrawings.push_back(_GroupItemDrawing);
	_GroupItemButtonDrawing = std::make_shared<ToolBox::GroupItemButtonDrawing>();
	_ItemDrawings.push_back(_GroupItemButtonDrawing);
	_GroupItemHoverDrawing = std::make_shared<ToolBox::GroupItemHoverDrawing>();
	_ItemDrawings.push_back(_GroupItemHoverDrawing);

	_SubItemDrawing = std::make_shared<ToolBox::SubItemDrawing>();
	_ItemDrawings.push_back(_SubItemDrawing);
	_SubItemButtonDrawing = std::make_shared<ToolBox::SubItemButtonDrawing>();
	_ItemDrawings.push_back(_SubItemButtonDrawing);
	_SubItemHoverDrawing = std::make_shared<ToolBox::SubItemHoverDrawing>();
	_ItemDrawings.push_back(_SubItemHoverDrawing);


	//-----------------------------------------------------------------------
	_GroupItemExpandBitmap   = cx::gw::makeBitmap(_bitmap_expand_png, sizeof(_bitmap_expand_png));
	_GroupItemCollapseBitmap = cx::gw::makeBitmap(_bitmap_collapse_png, sizeof(_bitmap_collapse_png));
}

ToolBox::ItemView::~ItemView()
{
	clearItems();
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
bool ToolBox::ItemView::createDeviceResources(cx::gw::Context* ctx)
{
	bool rv;


	rv = _ItemViewDrawing->createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}

	for (auto& itemDrawing : _ItemDrawings)
	{
		rv = itemDrawing->createDeviceResources(ctx);
		if (!rv)
		{
			return false;
		}
	}

	rv = _GroupItemExpandBitmap->createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}

	rv = _GroupItemCollapseBitmap->createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}

	rv = _BitmapList.createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}

	return true;
}

void ToolBox::ItemView::destroyDeviceResources(void)
{
	_BitmapList.destroyDeviceResources();

	_GroupItemExpandBitmap->destroyDeviceResources();
	_GroupItemCollapseBitmap->destroyDeviceResources();

	for (auto& itemDrawing : _ItemDrawings)
	{
		itemDrawing->destroyDeviceResources();
	}

	_ItemViewDrawing->destroyDeviceResources();
}

void ToolBox::ItemView::draw(cx::gw::Context* ctx)
{
	_ItemViewDrawing->drawItemView(ctx, this);

	drawItems(ctx);
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

	item->getStatus()->setStatusChangedHandler(std::bind(&ToolBox::ItemView::onItemStatusChanged, this, item.get()));
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

	item->getStatus()->setStatusChangedHandler(nullptr);

	items.erase(it);
}

void ToolBox::ItemView::clearItems(void)
{
	for (auto& item : getItems())
	{
		item->getStatus()->setStatusChangedHandler(nullptr);
	}

	getItems().clear();
}

void ToolBox::ItemView::clearItems(ToolBox::ItemSharedPtrs& items)
{
	for (auto& item : items)
	{
		item->getStatus()->setStatusChangedHandler(nullptr);
	}

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
	cx::gw::Point itemViewSize;
	itemViewSize = _ItemViewSize;


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
	if (_ItemViewSize._y != itemViewSize._y)
	{
		getWindow()->getViewport()->setDocumentSize(_ItemViewSize._x, _ItemViewSize._y);
	}
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
cx::gw::BitmapSharedPtr ToolBox::ItemView::getGroupItemExpandBitmap(void)
{
	return _GroupItemExpandBitmap;
}

cx::gw::BitmapSharedPtr ToolBox::ItemView::getGroupItemCollapseBitmap(void)
{
	return _GroupItemCollapseBitmap;
}

cx::gw::BitmapList* ToolBox::ItemView::getBitmapList(void)
{
	return &_BitmapList;
}

//===========================================================================
ToolBox::ItemDrawingSharedPtr ToolBox::ItemView::getItemDrawing(ToolBox::ItemSharedPtr item)
{
	if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
	{
		if (groupItem->getStyle() == ToolBox::ItemStyle::Button)
		{
			return _GroupItemButtonDrawing;
		}
		if (groupItem->getStatus()->getHover())
		{
			return _GroupItemHoverDrawing;
		}

		return _GroupItemDrawing;
	}
	if (auto subItem = std::dynamic_pointer_cast<ToolBox::SubItem>(item))
	{
		if (subItem->getStyle() == ToolBox::ItemStyle::Button)
		{
			return _SubItemButtonDrawing;
		}
		if (subItem->getStatus()->getHover())
		{
			return _SubItemHoverDrawing;
		}

		return _SubItemDrawing;
	}

	return _GroupItemDrawing;
}

//===========================================================================
void ToolBox::ItemView::drawItems(cx::gw::Context* ctx)
{
	for (auto& item : getItems())
	{
		drawItem(ctx, item);
	}
}

void ToolBox::ItemView::drawItem(cx::gw::Context* ctx, ToolBox::ItemSharedPtr item)
{
	auto itemDrawing = getItemDrawing(item);
	if (itemDrawing)
	{
		itemDrawing->drawItem(ctx, this, item.get());

		if (auto groupItem = std::dynamic_pointer_cast<ToolBox::GroupItem>(item))
		{
			if (!groupItem->getSubItems().empty())
			{
				if (!groupItem->isCollapseSubItems())
				{
					for (auto& subItem : groupItem->getSubItems())
					{
						drawItem(ctx, subItem);
					}
				}
			}
		}
	}
}

//===========================================================================
void ToolBox::ItemView::onItemStatusChanged(ToolBox::Item* item)
{
	getWindow()->render();
}

void ToolBox::ItemView::onMouseEvent(ToolBox::EventType eventType, ToolBox::MouseEventParam& param)
{
	switch (eventType)
	{
	case ToolBox::EventType::Unknown:
		break;

	case ToolBox::EventType::MouseMove:
		break;
	case ToolBox::EventType::MouseLButtonDown:
		break;
	case ToolBox::EventType::MouseLButtonUp:
		break;

	default:
		break;
	}
}

void ToolBox::ItemView::onMouseEvent(ToolBox::EventType eventType, ToolBox::Item* item, ToolBox::MouseEventParam& param)
{
	switch (eventType)
	{
	case ToolBox::EventType::Unknown:
		break;

	case ToolBox::EventType::MousePressed:
		item->getStatus()->setPressed(true);
		break;
	case ToolBox::EventType::MouseReleased:
		item->getStatus()->setPressed(false);
		break;

	case ToolBox::EventType::MouseClicked:
		break;
	case ToolBox::EventType::MouseDbClicked:
		break;

	case ToolBox::EventType::MouseOver:
		item->getStatus()->setHover(true);
		break;
	case ToolBox::EventType::MouseLeave:
		item->getStatus()->setHover(false);
		break;

	case ToolBox::EventType::MouseDragging:
		break;

	default:
		break;
	}


	ToolBox::GroupItem* groupItem = dynamic_cast<ToolBox::GroupItem*>(item);
	if (groupItem)
	{
		onGroupItemMouseEvent(eventType, groupItem, param);
	}


	ToolBox::SubItem* subItem = dynamic_cast<ToolBox::SubItem*>(item);
	if (subItem)
	{
		onSubItemMouseEvent(eventType, subItem, param);
	}
}

void ToolBox::ItemView::onGroupItemMouseEvent(ToolBox::EventType eventType, ToolBox::GroupItem* item, ToolBox::MouseEventParam& param)
{
	switch (eventType)
	{
	case ToolBox::EventType::MouseClicked:
		onGroupItemMouseClicked(item);
		break;

	case ToolBox::EventType::MouseDbClicked:
		onGroupItemMouseDbClicked(item);
		break;

	case ToolBox::EventType::MouseDragging:
		onGroupItemMouseDragging(item);
		break;

	default:
		break;
	}
}

void ToolBox::ItemView::onGroupItemMouseClicked(ToolBox::GroupItem* item)
{
	if (item->isCollapseSubItems())
	{
		item->CollapseSubItems(false);
	}
	else
	{
		item->CollapseSubItems(true);
	}

	recalcLayout();
}

void ToolBox::ItemView::onGroupItemMouseDbClicked(ToolBox::GroupItem* item)
{

}

void ToolBox::ItemView::onGroupItemMouseDragging(ToolBox::GroupItem* item)
{

}

void ToolBox::ItemView::onSubItemMouseEvent(ToolBox::EventType eventType, ToolBox::SubItem* item, ToolBox::MouseEventParam& param)
{
	switch (eventType)
	{
	case ToolBox::EventType::MouseClicked:
		onSubItemMouseClicked(item);
		break;

	case ToolBox::EventType::MouseDbClicked:
		onSubItemMouseDbClicked(item);
		break;

	case ToolBox::EventType::MouseDragging:
		onSubItemMouseDragging(item);
		break;

	default:
		break;
	}
}

void ToolBox::ItemView::onSubItemMouseClicked(ToolBox::SubItem* item)
{

}

void ToolBox::ItemView::onSubItemMouseDbClicked(ToolBox::SubItem* item)
{

}

void ToolBox::ItemView::onSubItemMouseDragging(ToolBox::SubItem* item)
{

}
