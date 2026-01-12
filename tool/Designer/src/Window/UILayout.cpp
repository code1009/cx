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
void UILayoutManager::add(void* item, UILayoutStyle const& style, bool first)
{
	UILayout layout;
	layout._Item.item = item;
	layout._Style = style;
	

	if (first || _Grid._Rows.empty())
	{
		UIRowLayout newRow;
		newRow._Cols.push_back(layout);

		_Grid._Rows.push_back(std::move(newRow));
		return;
	}


	UIRowLayout& lastRow = _Grid._Rows.back();
	lastRow._Cols.push_back(std::move(layout));
}

void UILayoutManager::recalcLayout(UICoord cx, UICoord cy)
{
	updateGrid();


	_X = UICoordZero;
	_Y = UICoordZero;
	_CX = UICoordZero;
	_CY = UICoordZero;

	
	_ViewCX = cx;
	_ViewCY = cy;
	_RemainingViewCX = (_ViewCX < _Grid._MinCX) ? UICoordZero : (_ViewCX - _Grid._MinCX);
	_RemainingViewCY = (_ViewCY < _Grid._MinCY) ? UICoordZero : (_ViewCY - _Grid._MinCY);


	std::size_t _RowCount = _Grid._Rows.size();
	UICoord _ItemExpCY = UICoordZero;
	if (_RemainingViewCY > UICoordZero)
	{
		_ItemExpCY = _RemainingViewCY / static_cast<UICoord>(_ItemExpCY);
	}
	

	_RowCY = UICoordZero;	
	for (auto& row : _Grid._Rows)
	{
		if (row._RowStyle != UIRowStyle::Fixed)
		{
			_RowCY = row._MinCY + _ItemExpCY;
		}
		else
		{
			_RowCY = row._MinCY;
		}

		recalcRowLayout(row);

		_Y += _RowCY;
	}


	_CY = _Y;
}

void UILayoutManager::recalcRowLayout(UIRowLayout& row)
{
	UICoord remainingViewCX = (_ViewCX < row._MinCX) ? UICoordZero : (_ViewCX - row._MinCX);
	UICoord remainingViewCY = (_ViewCY < row._MinCY) ? UICoordZero : (_ViewCY - row._MinCY);

	std::size_t colCount = row._Cols.size();
	std::size_t rowCount = _Grid._Rows.size();
	
	UICoord cxAddtionSize = UICoordZero;
	if (remainingViewCX > UICoordZero)
	{
		cxAddtionSize = remainingViewCX / static_cast<UICoord>(colCount);
	}


	UICoord x = _X;
	UICoord y = _Y;

	UICoord l;
	UICoord t;
	UICoord r;
	UICoord b;


	for(auto& layout : row._Cols)
	{
	}
}

void UILayoutManager::updateRow(UIRowLayout& row)
{
	row._MinCX = UICoordZero;
	row._MinCY = UICoordZero;
	row._RowStyle = UIRowStyle::Fixed;
	row._ColStyle = UIColStyle::Fixed;

	for (auto& layout : row._Cols)
	{
		UICoord cx = layout._Style._CX;
		UICoord cy = layout._Style._CY;

		if (row._MinCX < cx)
		{
			row._MinCX = cx;
		}
		if (row._MinCY < cy)
		{
			row._MinCY = cy;
		}

		if (layout._Style._RowStyle != UIRowStyle::Fixed)
		{
			row._RowStyle = UIRowStyle::Fill;
		}
		if (layout._Style._ColStyle != UIColStyle::Fixed)
		{
			row._ColStyle = UIColStyle::Fill;
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
		if (_Grid._MinCY < cy)
		{
			_Grid._MinCY = cy;
		}
	}
}
