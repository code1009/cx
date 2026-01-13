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
#include "UILayout.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void UILayoutManager::clear(void)
{
	_Grid._Rows.clear();

	_RemainCX = UICoordZero;
	_RemainCY = UICoordZero;

	_X = UICoordZero;	
	_Y = UICoordZero;
	_RowCX = UICoordZero;
	_RowCY = UICoordZero;

	_CX = UICoordZero;
	_CY = UICoordZero;
}

void UILayoutManager::add(
	bool firstCol,
	UILayoutStyle const& style,
	std::function<void(UILayout*)> layoutChangedHandler,
	void* data, std::uint32_t itemFlag
)
{
	UILayout layout;
	layout._Item._Flag = itemFlag;
	layout._Item._Data = data;
	layout._Style = style;
	layout.layoutChangedHandler = layoutChangedHandler;


	if (firstCol || _Grid._Rows.empty())
	{
		UIRowLayout newRow;
		newRow._Cols.push_back(layout);

		_Grid._Rows.push_back(std::move(newRow));
		return;
	}


	UIRowLayout& lastRow = _Grid._Rows.back();
	lastRow._Cols.push_back(std::move(layout));
}

void UILayoutManager::updateRow(UIRowLayout& row)
{
	row._MinCX = UICoordZero;
	row._MinCY = UICoordZero;
	row._RowAlignment = UILayoutAlignment::Fixed;
	row._ColAlignment = UILayoutAlignment::Fixed;
	row._ColFixedCount = 0;
	row._ColFixedCX = UICoordZero;

	for (auto& layout : row._Cols)
	{
		UICoord cx = layout._Style._CX;
		UICoord cy = layout._Style._CY;

		row._MinCX += cx;
		if (row._MinCY < cy)
		{
			row._MinCY = cy;
		}

		if (layout._Style._RowAlignment != UILayoutAlignment::Fixed)
		{
			row._RowAlignment = UILayoutAlignment::Fill;
		}
		if (layout._Style._ColAlignment != UILayoutAlignment::Fixed)
		{
			row._ColAlignment = UILayoutAlignment::Fill;
		}

		if (layout._Style._ColAlignment == UILayoutAlignment::Fixed)
		{
			row._ColFixedCount++;
			row._ColFixedCX += cx;
		}
	}
}

void UILayoutManager::updateGrid(void)
{
	_Grid._MinCX = UICoordZero;
	_Grid._MinCY = UICoordZero;
	for (auto& row : _Grid._Rows)
	{
		updateRow(row);

		UICoord cx = row._MinCX;
		UICoord cy = row._MinCY;

		if (_Grid._MinCX < cx)
		{
			_Grid._MinCX = cx;
		}
		_Grid._MinCY += cy;
	}
}

void UILayoutManager::recalcItemLayout(UILayout& item, UICoord x, UICoord y, UICoord cx, UICoord cy)
{
	item._Item._L = x;
	item._Item._T = y;
	item._Item._R = x + cx;
	item._Item._B = y + cy;

	if (item.layoutChangedHandler)
	{
		item.layoutChangedHandler(&item);
	}
}

void UILayoutManager::recalcRowLayout(UIRowLayout& row)
{
	std::size_t colCount = row._Cols.size();
	if (colCount == 0)
	{
		return;
	}


	UICoord remainCX;
	UICoord remainCY;

	if (row._ColFixedCount)
	{
		remainCX = (_RowCX < row._ColFixedCX) ? UICoordZero : (_RowCX - row._ColFixedCX);
	}
	else
	{
		remainCX = (_RowCX < row._MinCX) ? UICoordZero : (_RowCX - row._MinCX);
	}
	remainCY = (_RowCY < row._MinCY) ? UICoordZero : (_RowCY - row._MinCY);


	UICoord itemExpCX = UICoordZero;
	UICoord itemExpCY = UICoordZero;
	if (remainCX > UICoordZero)
	{
		if (row._ColFixedCount)
		{
			itemExpCX = remainCX / row._ColFixedCount;
		}
		else
		{
			itemExpCX = remainCX / static_cast<UICoord>(colCount);
		}
	}
	if (remainCY > UICoordZero)
	{
		itemExpCY = remainCY;
	}


	UICoord x;
	UICoord y;

	UICoord itemOffsetX;
	UICoord itemOffsetY;
	UICoord itemCX;
	UICoord itemCY;

	x = _X;
	y = _Y;
	for (auto& layout : row._Cols)
	{
		switch (layout._Style._ColAlignment)
		{
		case UILayoutAlignment::Fixed:
			itemCX = layout._Style._CX;
			itemOffsetX = UICoordZero;
			break;
		case UILayoutAlignment::Fill:
			itemCX = layout._Style._CX + itemExpCX;
			itemOffsetX = UICoordZero;
			break;
		case UILayoutAlignment::Near:
			itemCX = layout._Style._CX;
			itemOffsetX = UICoordZero;
			break;
		case UILayoutAlignment::Center:
			itemCX = layout._Style._CX;
			itemOffsetX = (itemExpCX / 2);
			break;
		case UILayoutAlignment::Far:
			itemCX = layout._Style._CX;
			itemOffsetX = itemExpCX;
			break;
		default:
			itemCX = layout._Style._CX;
			itemOffsetX = UICoordZero;
			break;
		}

		switch (layout._Style._RowAlignment)
		{
		case UILayoutAlignment::Fixed:
			itemCY = layout._Style._CY;
			itemOffsetY = UICoordZero;
			break;
		case UILayoutAlignment::Fill:
			itemCY = layout._Style._CY + itemExpCY;
			itemOffsetY = UICoordZero;
			break;
		case UILayoutAlignment::Near:
			itemCY = layout._Style._CY;
			itemOffsetY = UICoordZero;
			break;
		case UILayoutAlignment::Center:
			itemCY = layout._Style._CY;
			itemOffsetY = (itemExpCY / 2);
			break;
		case UILayoutAlignment::Far:
			itemCY = layout._Style._CY;
			itemOffsetY = itemExpCY;
			break;
		default:
			itemCY = layout._Style._CY;
			itemOffsetY = UICoordZero;
			break;
		}


		//-------------------------------------------------------------------
		recalcItemLayout(layout, x + itemOffsetX, y + itemOffsetY, itemCX, itemCY);

		//x += (layout._Style._CX + itemExpCX);
		x += itemCX;


		//-------------------------------------------------------------------
		if (x > _CX)
		{
			_CX = x;
		}
	}
}

void UILayoutManager::recalcLayout(UICoord cx, UICoord cy)
{
	updateGrid();


	//-----------------------------------------------------------------------
	_ViewCX = cx;
	_ViewCY = cy;
	_RemainCX = (_ViewCX < _Grid._MinCX) ? UICoordZero : (_ViewCX - _Grid._MinCX);
	_RemainCY = (_ViewCY < _Grid._MinCY) ? UICoordZero : (_ViewCY - _Grid._MinCY);


	//-----------------------------------------------------------------------
	_X = UICoordZero;
	_Y = UICoordZero;
	_CX = UICoordZero;
	_CY = UICoordZero;


	//-----------------------------------------------------------------------
	std::size_t rowCount = _Grid._Rows.size();
	if (rowCount == 0)
	{
		return;
	}


	//-----------------------------------------------------------------------
	UICoord remainCX;
	UICoord remainCY;
	remainCX = _RemainCX;
	remainCY = _RemainCY;


	//-----------------------------------------------------------------------
	UICoord itemExpCX = UICoordZero;
	UICoord itemExpCY = UICoordZero;
	if (remainCX > UICoordZero)
	{
		itemExpCX = remainCX;
	}
	if (remainCY > UICoordZero)
	{
		itemExpCY = remainCY / static_cast<UICoord>(rowCount);
	}


	//-----------------------------------------------------------------------
	for (auto& row : _Grid._Rows)
	{
		if (row._ColAlignment != UILayoutAlignment::Fixed)
		{
			_RowCX = row._MinCX + itemExpCX;
		}
		else
		{
			_RowCX = row._MinCX;
		}
		if (row._RowAlignment != UILayoutAlignment::Fixed)
		{
			_RowCY = row._MinCY + itemExpCY;
		}
		else
		{
			_RowCY = row._MinCY;
		}

		recalcRowLayout(row);

		_Y += _RowCY;
	}


	//-----------------------------------------------------------------------
	_CY = _Y;
}
