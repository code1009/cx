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
#include "ToolBox_ControlWindow.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ToolBox::ControlWindow::ControlWindow(HWND hwnd) :
	cx::gw::Window(hwnd, false),
	_WindowHandle(hwnd)
{
	getViewport()->setWindowSize(0, 0);
	getViewport()->setDocumentSize(0, 0);
	getViewport()->enableScrollbar(true);

	//-----------------------------------------------------------------------
	ToolBox::ItemDrawingSharedPtr itemDrawing;
	

	_GroupItemDrawing = std::make_shared<ToolBox::ItemDrawing>();
	_ItemDrawings.push_back(_GroupItemDrawing);
	_SubItemDrawing = std::make_shared<ToolBox::ItemDrawing>();
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
void ToolBox::ControlWindow::resize(std::int64_t width, std::int64_t height)
{
	recalcLayout();
}

bool ToolBox::ControlWindow::createDeviceResources(void)
{
	//-----------------------------------------------------------------------
	bool rv;


	rv = cx::gw::Window::createDeviceResources();
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	for (auto& itemDrawing : _ItemDrawings)
	{
		rv = itemDrawing->createDeviceResources(this);
		if (!rv)
		{
			return false;
		}
	}


	//-----------------------------------------------------------------------
	HRESULT hr;


	if (!_Brush)
	{
		hr = getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 1.0f, 0.5f),
			&_Brush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ToolBox::ControlWindow::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	for (auto& itemDrawing : _ItemDrawings)
	{
		itemDrawing->destroyDeviceResources();
	}
	
	if (_Brush)
	{
		_Brush->Release();
		_Brush = nullptr;
	}


	//-----------------------------------------------------------------------
	cx::gw::Window::destroyDeviceResources();
}

void ToolBox::ControlWindow::draw(void)
{
	//-----------------------------------------------------------------------
	cx::gw::Window::draw();


	//-----------------------------------------------------------------------
#if 0
	D2D1_ROUNDED_RECT rrect;


	rrect.rect.left = 100.0f;
	rrect.rect.top = 100.0f;
	rrect.rect.right = 200.0f;
	rrect.rect.bottom = 200.0f;
	rrect.radiusX = 10.0f;
	rrect.radiusY = 10.0f;

	getDRenderTarget()->FillRoundedRectangle(&rrect, _Brush);
#endif


	//-----------------------------------------------------------------------
	drawItems();
}

//===========================================================================
std::size_t ToolBox::ControlWindow::makeID(void)
{
	return ++_LastMadeID;
}

ToolBox::ItemSharedPtrs& ToolBox::ControlWindow::getItems(void)
{
	return _Items;
}

void ToolBox::ControlWindow::addItem(ToolBox::GroupItemSharedPtr parentItem, ToolBox::ItemSharedPtr item)
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

void ToolBox::ControlWindow::removeItem(ToolBox::ItemSharedPtr item)
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

void ToolBox::ControlWindow::removeItem(ToolBox::ItemSharedPtrs& items, ToolBox::ItemSharedPtr item)
{
	auto it = std::find(items.begin(), items.end(), item);

	if (it == items.end())
	{
		return;
	}
	items.erase(it);
}

void ToolBox::ControlWindow::clearItems(void)
{
	getItems().clear();
}

void ToolBox::ControlWindow::clearItems(ToolBox::ItemSharedPtrs& items)
{
	items.clear();
}

void ToolBox::ControlWindow::clearSubItems(ToolBox::GroupItemSharedPtr& groupItem)
{
	clearItems(groupItem->getSubItems());
}

//===========================================================================
ToolBox::ItemSharedPtr ToolBox::ControlWindow::findItem(std::size_t id)
{
	return findItem(getItems(), id);
}

ToolBox::ItemSharedPtr ToolBox::ControlWindow::findItem(ToolBox::ItemSharedPtrs& items, std::size_t id)
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
void ToolBox::ControlWindow::recalcLayout(void)
{
	cx::gw::coord_t offset;


	offset = 0;
	for (auto& item : getItems())
	{
		offset = recalcItemLayout(offset, item);
	}
}

cx::gw::coord_t ToolBox::ControlWindow::recalcItemLayout(cx::gw::coord_t offset, ToolBox::ItemSharedPtr item)
{
	std::int64_t window_cx;
	std::int64_t window_cy;
	getViewport()->getWindowSize(window_cx, window_cy);

	
	item->setX(0);
	item->setY(offset);

	item->setCX(static_cast<cx::gw::coord_t>(window_cx));
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
					offset = recalcItemLayout(offset, subItem);
				}
			}
		}
	}
	
	return offset;

#if 0
	/*
	item->setCX(size);
	item->setCY(size);
	item->setP0({ 0, 0 });
	item->setP1({ size, size });
	item->CollapseSubItems(false);
	item->getSubItems().clear();
	item->getSubItems().shrink_to_fit();
	*/
	if (parent)
	{
		item->setDepth(parent->getDepth() + 1);
	}
	else
	{
		item->setDepth(0);
	}
#endif
}

//===========================================================================
ToolBox::ItemDrawingSharedPtr ToolBox::ControlWindow::getItemDrawing(ToolBox::ItemSharedPtr item)
{
	//_GroupItemDrawing;
	//_SubItemDrawing;

	return _GroupItemDrawing;
}

//===========================================================================
void ToolBox::ControlWindow::drawItems(void)
{
	for (auto& item : getItems())
	{
		drawItem(item);
	}
}

void ToolBox::ControlWindow::drawItem(ToolBox::ItemSharedPtr item)
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

