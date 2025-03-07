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
bool ToolBox::ItemViewDrawing::createDeviceIndependentResources(cx::gw::Context* ctx)
{
	return true;
}

void ToolBox::ItemViewDrawing::destroyDeviceIndependentResources(void)
{
}

bool ToolBox::ItemViewDrawing::createDeviceResources(cx::gw::Context* ctx)
{
	HRESULT hr;

	if (!_pFrame_FillBrush)
	{
		cx::gw::Color Frame_FillColor;
		getFrame_FillColor(Frame_FillColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Frame_FillColor._r, Frame_FillColor._g, Frame_FillColor._b, Frame_FillColor._a),
			&_pFrame_FillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFrame_LineBrush)
	{
		cx::gw::Color Frame_LineColor;
		getFrame_LineColor(Frame_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Frame_LineColor._r, Frame_LineColor._g, Frame_LineColor._b, Frame_LineColor._a),
			&_pFrame_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ToolBox::ItemViewDrawing::destroyDeviceResources(void)
{
	if (_pFrame_FillBrush)
	{
		_pFrame_FillBrush->Release();
		_pFrame_FillBrush = nullptr;
	}
	if (_pFrame_LineBrush)
	{
		_pFrame_LineBrush->Release();
		_pFrame_LineBrush = nullptr;
	}
}

//===========================================================================
void ToolBox::ItemViewDrawing::drawItemView(cx::gw::Context* ctx, ToolBox::ItemView* itemView)
{
	drawFrame(ctx, itemView);
}

void ToolBox::ItemViewDrawing::drawFrame(cx::gw::Context* ctx, ToolBox::ItemView* itemView)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getFrame_Bounds(itemView, p0, p1);


	cx::gw::coord_t lineSize;
	getFrame_LineSize(lineSize);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p1._y;


	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFrame_FillBrush);
	if (lineSize > 0)
	{
		rect.left   += lineSize;
		rect.top    += lineSize;
		rect.right  -= lineSize;
		rect.bottom -= lineSize;
		ctx->getD2dRenderTarget()->DrawRectangle(&rect, _pFrame_LineBrush, lineSize);
	}
}

//===========================================================================
void ToolBox::ItemViewDrawing::getFrame_Bounds(ToolBox::ItemView* itemView, cx::gw::Point& p0, cx::gw::Point& p1)
{
	double cx;
	double cy;
	double x;
	double y;


	itemView->getWindow()->getViewport()->getDocumentViewportPoint(x, y);
	itemView->getWindow()->getViewport()->getDocumentViewportSize(cx, cy);

	p0 = cx::gw::Point(static_cast<cx::gw::coord_t>(x), static_cast<cx::gw::coord_t>(y));
	p1 = cx::gw::Point(static_cast<cx::gw::coord_t>(x+cx), static_cast<cx::gw::coord_t>(y+cy));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool ToolBox::ItemDrawing::createDeviceIndependentResources(cx::gw::Context* ctx)
{
	HRESULT hr;


	if (!_pCaption_TextFormat)
	{
		bool Caption_TextFontBold;
		getCaption_TextFontBold(Caption_TextFontBold);
		hr = ctx->getDWriteFactory()->CreateTextFormat(
			//L"Arial",
			//L"돋움",
			//L"FixedSys",
			L"맑은 고딕",
			nullptr,
			Caption_TextFontBold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL, // DWRITE_FONT_WEIGHT_ULTRA_BLACK 
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12.0f,
			L"ko-kr",
			&_pCaption_TextFormat
		);
		if (FAILED(hr))
		{
			return false;
		}
		//_pCaption_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // hcenter
		_pCaption_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // vcenter
	}


	return true;
}

void ToolBox::ItemDrawing::destroyDeviceIndependentResources(void)
{
	if (_pCaption_TextFormat)
	{
		_pCaption_TextFormat->Release();
		_pCaption_TextFormat = nullptr;
	}
}

//===========================================================================
bool ToolBox::ItemDrawing::createDeviceResources(cx::gw::Context* ctx)
{
	HRESULT hr;

	//-----------------------------------------------------------------------
	if (!_pFrame_FillBrush)
	{
		cx::gw::Color Frame_FillColor;
		getFrame_FillColor(Frame_FillColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Frame_FillColor._r, Frame_FillColor._g, Frame_FillColor._b, Frame_FillColor._a),
			&_pFrame_FillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFrame_LineBrush)
	{
		cx::gw::Color Frame_LineColor;		
		getFrame_LineColor(Frame_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Frame_LineColor._r, Frame_LineColor._g, Frame_LineColor._b, Frame_LineColor._a),
			&_pFrame_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//-----------------------------------------------------------------------
	if (!_pFace_FillBrush)
	{
		cx::gw::Color Face_FillColor;
		getFace_FillColor(Face_FillColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_FillColor._r, Face_FillColor._g, Face_FillColor._b, Face_FillColor._a),
			&_pFace_FillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFace_LineBrush)
	{
		cx::gw::Color Face_LineColor;
		getFace_LineColor(Face_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_LineColor._r, Face_LineColor._g, Face_LineColor._b, Face_LineColor._a),
			&_pFace_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//-----------------------------------------------------------------------
	if (!_pCaption_TextBrush)
	{
		cx::gw::Color Caption_TextColor;
		getCaption_TextColor(Caption_TextColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Caption_TextColor._r, Caption_TextColor._g, Caption_TextColor._b, Caption_TextColor._a),
			&_pCaption_TextBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ToolBox::ItemDrawing::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pFrame_FillBrush)
	{
		_pFrame_FillBrush->Release();
		_pFrame_FillBrush = nullptr;
	}
	if (_pFrame_LineBrush)
	{
		_pFrame_LineBrush->Release();
		_pFrame_LineBrush = nullptr;
	}

	//-----------------------------------------------------------------------
	if (_pFace_LineBrush)
	{
		_pFace_LineBrush->Release();
		_pFace_LineBrush = nullptr;
	}
	if (_pFace_LineBrush)
	{
		_pFace_LineBrush->Release();
		_pFace_LineBrush = nullptr;
	}

	//-----------------------------------------------------------------------
	if (_pCaption_TextBrush)
	{
		_pCaption_TextBrush->Release();
		_pCaption_TextBrush = nullptr;
	}
}

//===========================================================================
void ToolBox::ItemDrawing::drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	drawFrame(ctx, itemView, item);

	drawFace(ctx, itemView, item);

	cx::gw::BitmapSharedPtr bitmapSharedPtr;
	bitmapSharedPtr = itemView->getBitmapList()->findBitmap(item->getIcon());
	drawIcon(ctx, itemView, item, bitmapSharedPtr);

	drawCaption(ctx, itemView, item);
}

void ToolBox::ItemDrawing::drawFrame(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getFrame_Bounds(item, p0, p1);


	cx::gw::coord_t lineSize;
	getFrame_LineSize(lineSize);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p1._y;


	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFrame_FillBrush);
	if (lineSize>0)
	{
		rect.left   += lineSize;
		rect.top    += lineSize;
		rect.right  -= lineSize;
		rect.bottom -= lineSize;
		ctx->getD2dRenderTarget()->DrawRectangle(&rect, _pFrame_LineBrush, lineSize);
	}
}

void ToolBox::ItemDrawing::drawFace(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getFace_Bounds(item, p0, p1);


	cx::gw::coord_t lineSize;
	getFace_LineSize(lineSize);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p1._y;


	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFace_FillBrush);
	if (lineSize > 0)
	{
		rect.left   += lineSize;
		rect.top    += lineSize;
		rect.right  -= lineSize;
		rect.bottom -= lineSize;
		ctx->getD2dRenderTarget()->DrawRectangle(&rect, _pFace_LineBrush, lineSize);
	}
}

void ToolBox::ItemDrawing::drawIcon(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item, cx::gw::BitmapSharedPtr bitmapSharedPtr)
{
	if (!bitmapSharedPtr)
	{
		return;
	}


	cx::gw::Point p0;
	cx::gw::Point p1;
	getIcon_Bounds(item, p0, p1);
	
	
	cx::gw::coord_t cx = p1._x - p0._x;
	cx::gw::coord_t cy = p1._y - p0._y;


	std::size_t bitmapCx;
	std::size_t bitmapCy;
	bitmapSharedPtr->getBitmapSize(bitmapCx, bitmapCy);


	cx::gw::coord_t offsetX = cx - bitmapCx;
	cx::gw::coord_t offsetY = cy - bitmapCy;
	cx::gw::Point bitmapP0;
	cx::gw::Point bitmapP1;
	bitmapP0._x = p0._x + offsetX / 2.0f;
	bitmapP0._y = p0._y + offsetY / 2.0f;
	bitmapP1._x = bitmapP0._x + bitmapCx;
	bitmapP1._y = bitmapP0._y + bitmapCx;


	ctx->getD2dRenderTarget()->DrawBitmap(
		bitmapSharedPtr->getDBitmap(),
		D2D1::RectF(bitmapP0._x, bitmapP0._y, bitmapP1._x, bitmapP1._y),
		1.0f
	);
}

void ToolBox::ItemDrawing::drawCaption(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getCaption_Bounds(item, p0, p1);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p1._y;


	ctx->getD2dRenderTarget()->DrawTextW(
		item->getCaption().c_str(),
		static_cast<UINT32>(item->getCaption().length()),
		_pCaption_TextFormat,
		rect,
		_pCaption_TextBrush
	);
}

//===========================================================================
void ToolBox::ItemDrawing::getFrame_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	p0._x = item->getX();
	p1._x = item->getX() + item->getCX();
	p0._y = item->getY();
	p1._y = item->getY() + item->getCY();
}

void ToolBox::ItemDrawing::getFace_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t indentSpace_Size;
	cx::gw::coord_t depth_Size;
	getIndentSpace_Size(item, indentSpace_Size);
	getDepth_Size(item, depth_Size);

	getFrame_Bounds(item, p0, p1);
	p0._x = p0._x + indentSpace_Size + item->getDepth() * depth_Size;
}

void ToolBox::ItemDrawing::getIcon_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t icon_Size;
	cx::gw::coord_t iconSpace_Size;
	getIcon_Size(item, icon_Size);
	getIconSpace_Size(item, iconSpace_Size);

	getFace_Bounds(item, p0, p1);
	p0._x = p0._x + iconSpace_Size;
	p1._x = p0._x + icon_Size;
}

void ToolBox::ItemDrawing::getCaption_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t icon_Size;
	cx::gw::coord_t iconSpace_Size;
	getIcon_Size(item, icon_Size);
	getIconSpace_Size(item, iconSpace_Size);

	getFace_Bounds(item, p0, p1);
	p0._x = p0._x + iconSpace_Size + icon_Size + iconSpace_Size;
}

//===========================================================================
void ToolBox::ItemDrawing::getIndentSpace_Size(ToolBox::Item* item, cx::gw::coord_t& size)
{
	size = 0;
}

void ToolBox::ItemDrawing::getDepth_Size(ToolBox::Item* item, cx::gw::coord_t& size)
{
	size = 24;
}

void ToolBox::ItemDrawing::getIcon_Size(ToolBox::Item* item, cx::gw::coord_t& size)
{
	if (item->getIcon().empty())
	{
		size = 0;
		return;
	}

	size = 16;
}

void ToolBox::ItemDrawing::getIconSpace_Size(ToolBox::Item* item, cx::gw::coord_t& size)
{
	size = 8;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ToolBox::GroupItemDrawing::drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	drawBorder(ctx, itemView, item);

	drawFace(ctx, itemView, item);

	cx::gw::BitmapSharedPtr bitmapSharedPtr;
	bitmapSharedPtr =
		dynamic_cast<ToolBox::GroupItem*>(item)->isCollapseSubItems()
		? itemView->getGroupItemCollapseBitmap()
		: itemView->getGroupItemExpandBitmap();
	drawIcon(ctx, itemView, item, bitmapSharedPtr);

	drawCaption(ctx, itemView, item);
}

void ToolBox::GroupItemDrawing::drawBorder(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getFace_Bounds(item, p0, p1);


	cx::gw::coord_t lineSize;
	getFace_LineSize(lineSize);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p0._y + item->calcHeight();


	//ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFace_FillBrush);
	if (lineSize > 0)
	{
		rect.left   += lineSize;
		rect.top    += lineSize;
		rect.right  -= lineSize;
		rect.bottom -= lineSize;
		ctx->getD2dRenderTarget()->DrawRectangle(&rect, _pFace_LineBrush, lineSize);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool ToolBox::GroupItemButtonDrawing::createDeviceResources(cx::gw::Context* ctx)
{
	//-----------------------------------------------------------------------
	bool rv;
	rv = ToolBox::GroupItemDrawing::createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	HRESULT hr;
	if (!_pFace_Button_FillBrush)
	{
		cx::gw::Color Face_Button_FillColor;
		getFace_Button_FillColor(Face_Button_FillColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Button_FillColor._r, Face_Button_FillColor._g, Face_Button_FillColor._b, Face_Button_FillColor._a),
			&_pFace_Button_FillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFace_Button_H_LineBrush)
	{
		cx::gw::Color Face_Button_H_LineColor;
		getFace_Button_H_LineColor(Face_Button_H_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Button_H_LineColor._r, Face_Button_H_LineColor._g, Face_Button_H_LineColor._b, Face_Button_H_LineColor._a),
			&_pFace_Button_H_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFace_Button_S_LineBrush)
	{
		cx::gw::Color Face_Button_S_LineColor;
		getFace_Button_S_LineColor(Face_Button_S_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Button_S_LineColor._r, Face_Button_S_LineColor._g, Face_Button_S_LineColor._b, Face_Button_S_LineColor._a),
			&_pFace_Button_S_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ToolBox::GroupItemButtonDrawing::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pFace_Button_FillBrush)
	{
		_pFace_Button_FillBrush->Release();
		_pFace_Button_FillBrush = nullptr;
	}
	if (_pFace_Button_H_LineBrush)
	{
		_pFace_Button_H_LineBrush->Release();
		_pFace_Button_H_LineBrush = nullptr;
	}
	if (_pFace_Button_S_LineBrush)
	{
		_pFace_Button_S_LineBrush->Release();
		_pFace_Button_S_LineBrush = nullptr;
	}


	//-----------------------------------------------------------------------
	ToolBox::GroupItemDrawing::destroyDeviceResources();
}

void ToolBox::GroupItemButtonDrawing::drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	drawBorder(ctx, itemView, item);

	drawFace(ctx, itemView, item);

	drawFace_Button(ctx, itemView, item);

	cx::gw::BitmapSharedPtr bitmapSharedPtr;
	bitmapSharedPtr =
		dynamic_cast<ToolBox::GroupItem*>(item)->isCollapseSubItems()
		? itemView->getGroupItemCollapseBitmap()
		: itemView->getGroupItemExpandBitmap();
	drawIcon(ctx, itemView, item, bitmapSharedPtr);

	drawCaption(ctx, itemView, item);
}

//===========================================================================
void ToolBox::GroupItemButtonDrawing::drawFace_Button(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getFace_Button_Bounds(item, p0, p1);


	cx::gw::coord_t lineSize;
	getFace_Button_LineSize(lineSize);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p1._y;


	ID2D1SolidColorBrush* ltBrush;
	ID2D1SolidColorBrush* rbBrush;
	bool pressed;
	pressed = item->getStatus()->getPressed();
	if (pressed)
	{
		ltBrush = _pFace_Button_S_LineBrush;
		rbBrush = _pFace_Button_H_LineBrush;
	}
	else
	{
		ltBrush = _pFace_Button_H_LineBrush;
		rbBrush = _pFace_Button_S_LineBrush;
	}


	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFace_Button_FillBrush);
	if (lineSize > 0)
	{
		cx::gw::Point lp0 = p0;
		cx::gw::Point lp1 = p1;
		lp0._x += lineSize;
		lp0._y += lineSize;
		lp1._x -= lineSize;
		lp1._y -= lineSize;


		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp1._x, lp1._y },
			D2D1_POINT_2F{ lp1._x, lp0._y },
			rbBrush,
			lineSize
		);
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp1._x, lp1._y },
			D2D1_POINT_2F{ lp0._x, lp1._y },
			rbBrush,
			lineSize
		);


		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp0._x, lp0._y },
			D2D1_POINT_2F{ lp1._x, lp0._y },
			ltBrush,
			lineSize
		);
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp0._x, lp0._y },
			D2D1_POINT_2F{ lp0._x, lp1._y },
			ltBrush,
			lineSize
		);
	}
}

void ToolBox::GroupItemButtonDrawing::getFace_Button_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t line_Size;
	getFace_LineSize(line_Size);

	getFace_Bounds(item, p0, p1);
	p0._x = p0._x + line_Size;
	p0._y = p0._y + line_Size;
	p1._x = p1._x - line_Size;
	p1._y = p1._y - line_Size;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool ToolBox::GroupItemHoverDrawing::createDeviceResources(cx::gw::Context* ctx)
{
	//-----------------------------------------------------------------------
	bool rv;
	rv = ToolBox::GroupItemDrawing::createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	HRESULT hr;
	if (!_pFace_Hover_FillBrush)
	{
		cx::gw::Color Face_Hover_FillColor;
		getFace_Hover_FillColor(Face_Hover_FillColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Hover_FillColor._r, Face_Hover_FillColor._g, Face_Hover_FillColor._b, Face_Hover_FillColor._a),
			&_pFace_Hover_FillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFace_Hover_LineBrush)
	{
		cx::gw::Color Face_Hover_LineColor;
		getFace_Hover_LineColor(Face_Hover_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Hover_LineColor._r, Face_Hover_LineColor._g, Face_Hover_LineColor._b, Face_Hover_LineColor._a),
			&_pFace_Hover_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ToolBox::GroupItemHoverDrawing::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pFace_Hover_LineBrush)
	{
		_pFace_Hover_LineBrush->Release();
		_pFace_Hover_LineBrush = nullptr;
	}
	if (_pFace_Hover_LineBrush)
	{
		_pFace_Hover_LineBrush->Release();
		_pFace_Hover_LineBrush = nullptr;
	}


	//-----------------------------------------------------------------------
	ToolBox::GroupItemDrawing::destroyDeviceResources();
}

void ToolBox::GroupItemHoverDrawing::drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	drawBorder(ctx, itemView, item);

	drawFace(ctx, itemView, item);

	drawFace_Hover(ctx, itemView, item);

	cx::gw::BitmapSharedPtr bitmapSharedPtr;
	bitmapSharedPtr =
		dynamic_cast<ToolBox::GroupItem*>(item)->isCollapseSubItems()
		? itemView->getGroupItemCollapseBitmap()
		: itemView->getGroupItemExpandBitmap();
	drawIcon(ctx, itemView, item, bitmapSharedPtr);

	drawCaption(ctx, itemView, item);
}

void ToolBox::GroupItemHoverDrawing::drawFace_Hover(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getFace_Hover_Bounds(item, p0, p1);


	cx::gw::coord_t lineSize;
	getFace_Hover_LineSize(lineSize);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p1._y;


	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFace_Hover_FillBrush);
	if (lineSize > 0)
	{
#if 1
		rect.left   += lineSize;
		rect.top    += lineSize;
		rect.right  -= lineSize;
		rect.bottom -= lineSize;
		ctx->getD2dRenderTarget()->DrawRectangle(&rect, _pFace_Hover_LineBrush, lineSize);
#else
		cx::gw::Point lp0 = p0;
		cx::gw::Point lp1 = p1;
		lp0._x += lineSize;
		lp0._y += lineSize;
		lp1._x -= lineSize;
		lp1._y -= lineSize;


		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp1._x, lp1._y },
			D2D1_POINT_2F{ lp1._x, lp0._y },
			_pFace_Hover_Button_S_LineBrush,
			lineSize
		);
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp1._x, lp1._y },
			D2D1_POINT_2F{ lp0._x, lp1._y },
			_pFace_Hover_Button_S_LineBrush,
			lineSize
		);


		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp0._x, lp0._y },
			D2D1_POINT_2F{ lp1._x, lp0._y },
			_pFace_Hover_Button_H_LineBrush,
			lineSize
		);
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp0._x, lp0._y },
			D2D1_POINT_2F{ lp0._x, lp1._y },
			_pFace_Hover_Button_H_LineBrush,
			lineSize
		);
#endif
	}
}

void ToolBox::GroupItemHoverDrawing::getFace_Hover_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t line_Size;
	getFace_LineSize(line_Size);

	getFace_Bounds(item, p0, p1);
	p0._x = p0._x + line_Size;
	p0._y = p0._y + line_Size;
	p1._x = p1._x - line_Size;
	p1._y = p1._y - line_Size;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void ToolBox::SubItemDrawing::drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	drawFace(ctx, itemView, item);

	cx::gw::BitmapSharedPtr bitmapSharedPtr;
	bitmapSharedPtr = itemView->getBitmapList()->findBitmap(item->getIcon());
	drawIcon(ctx, itemView, item, bitmapSharedPtr);

	drawCaption(ctx, itemView, item);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool ToolBox::SubItemButtonDrawing::createDeviceResources(cx::gw::Context* ctx)
{
	//-----------------------------------------------------------------------
	bool rv;
	rv = ToolBox::SubItemDrawing::createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	HRESULT hr;
	if (!_pFace_Button_FillBrush)
	{
		cx::gw::Color Face_Button_FillColor;
		getFace_Button_FillColor(Face_Button_FillColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Button_FillColor._r, Face_Button_FillColor._g, Face_Button_FillColor._b, Face_Button_FillColor._a),
			&_pFace_Button_FillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFace_Button_H_LineBrush)
	{
		cx::gw::Color Face_Button_H_LineColor;
		getFace_Button_H_LineColor(Face_Button_H_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Button_H_LineColor._r, Face_Button_H_LineColor._g, Face_Button_H_LineColor._b, Face_Button_H_LineColor._a),
			&_pFace_Button_H_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFace_Button_S_LineBrush)
	{
		cx::gw::Color Face_Button_S_LineColor;
		getFace_Button_S_LineColor(Face_Button_S_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Button_S_LineColor._r, Face_Button_S_LineColor._g, Face_Button_S_LineColor._b, Face_Button_S_LineColor._a),
			&_pFace_Button_S_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ToolBox::SubItemButtonDrawing::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pFace_Button_FillBrush)
	{
		_pFace_Button_FillBrush->Release();
		_pFace_Button_FillBrush = nullptr;
	}
	if (_pFace_Button_H_LineBrush)
	{
		_pFace_Button_H_LineBrush->Release();
		_pFace_Button_H_LineBrush = nullptr;
	}
	if (_pFace_Button_S_LineBrush)
	{
		_pFace_Button_S_LineBrush->Release();
		_pFace_Button_S_LineBrush = nullptr;
	}


	//-----------------------------------------------------------------------
	ToolBox::SubItemDrawing::destroyDeviceResources();
}

void ToolBox::SubItemButtonDrawing::drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	drawFace(ctx, itemView, item);

	if (item->getStatus()->getHover())
	{
		drawFace_Button(ctx, itemView, item);
	}

	cx::gw::BitmapSharedPtr bitmapSharedPtr;
	bitmapSharedPtr = itemView->getBitmapList()->findBitmap(item->getIcon());
	drawIcon(ctx, itemView, item, bitmapSharedPtr);

	drawCaption(ctx, itemView, item);
}

//===========================================================================
void ToolBox::SubItemButtonDrawing::drawFace_Button(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getFace_Button_Bounds(item, p0, p1);


	cx::gw::coord_t lineSize;
	getFace_Button_LineSize(lineSize);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p1._y;


	ID2D1SolidColorBrush* ltBrush;
	ID2D1SolidColorBrush* rbBrush;
	bool pressed;
	pressed = item->getStatus()->getPressed();
	if (pressed)
	{
		ltBrush = _pFace_Button_S_LineBrush;
		rbBrush = _pFace_Button_H_LineBrush;
	}
	else
	{
		ltBrush = _pFace_Button_H_LineBrush;
		rbBrush = _pFace_Button_S_LineBrush;
	}


	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFace_Button_FillBrush);
	if (lineSize > 0)
	{
		cx::gw::Point lp0 = p0;
		cx::gw::Point lp1 = p1;
		lp0._x += lineSize;
		lp0._y += lineSize;
		lp1._x -= lineSize;
		lp1._y -= lineSize;


		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp1._x, lp1._y },
			D2D1_POINT_2F{ lp1._x, lp0._y },
			rbBrush,
			lineSize
		);
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp1._x, lp1._y },
			D2D1_POINT_2F{ lp0._x, lp1._y },
			rbBrush,
			lineSize
		);


		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp0._x, lp0._y },
			D2D1_POINT_2F{ lp1._x, lp0._y },
			ltBrush,
			lineSize
		);
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp0._x, lp0._y },
			D2D1_POINT_2F{ lp0._x, lp1._y },
			ltBrush,
			lineSize
		);
	}
}

void ToolBox::SubItemButtonDrawing::getFace_Button_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t line_Size;
	getFace_LineSize(line_Size);

	getFace_Bounds(item, p0, p1);
	p0._x = p0._x + line_Size;
	p0._y = p0._y + line_Size;
	p1._x = p1._x - line_Size;
	p1._y = p1._y - line_Size;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool ToolBox::SubItemHoverDrawing::createDeviceResources(cx::gw::Context* ctx)
{
	//-----------------------------------------------------------------------
	bool rv;
	rv = ToolBox::SubItemDrawing::createDeviceResources(ctx);
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	HRESULT hr;
	if (!_pFace_Hover_FillBrush)
	{
		cx::gw::Color Face_Hover_FillColor;
		getFace_Hover_FillColor(Face_Hover_FillColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Hover_FillColor._r, Face_Hover_FillColor._g, Face_Hover_FillColor._b, Face_Hover_FillColor._a),
			&_pFace_Hover_FillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFace_Hover_LineBrush)
	{
		cx::gw::Color Face_Hover_LineColor;
		getFace_Hover_LineColor(Face_Hover_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Face_Hover_LineColor._r, Face_Hover_LineColor._g, Face_Hover_LineColor._b, Face_Hover_LineColor._a),
			&_pFace_Hover_LineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	return true;
}

void ToolBox::SubItemHoverDrawing::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pFace_Hover_LineBrush)
	{
		_pFace_Hover_LineBrush->Release();
		_pFace_Hover_LineBrush = nullptr;
	}
	if (_pFace_Hover_LineBrush)
	{
		_pFace_Hover_LineBrush->Release();
		_pFace_Hover_LineBrush = nullptr;
	}


	//-----------------------------------------------------------------------
	ToolBox::SubItemDrawing::destroyDeviceResources();
}

void ToolBox::SubItemHoverDrawing::drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	drawFace(ctx, itemView, item);

	drawFace_Hover(ctx, itemView, item);

	cx::gw::BitmapSharedPtr bitmapSharedPtr;
	bitmapSharedPtr = itemView->getBitmapList()->findBitmap(item->getIcon());
	drawIcon(ctx, itemView, item, bitmapSharedPtr);

	drawCaption(ctx, itemView, item);
}

void ToolBox::SubItemHoverDrawing::drawFace_Hover(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::Point p0;
	cx::gw::Point p1;
	getFace_Hover_Bounds(item, p0, p1);


	cx::gw::coord_t lineSize;
	getFace_Hover_LineSize(lineSize);


	D2D1_RECT_F rect;
	rect.left   = p0._x;
	rect.top    = p0._y;
	rect.right  = p1._x;
	rect.bottom = p1._y;


	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFace_Hover_FillBrush);
	if (lineSize > 0)
	{
#if 1
		rect.left += lineSize;
		rect.top += lineSize;
		rect.right -= lineSize;
		rect.bottom -= lineSize;
		ctx->getD2dRenderTarget()->DrawRectangle(&rect, _pFace_Hover_LineBrush, lineSize);
#else
		cx::gw::Point lp0 = p0;
		cx::gw::Point lp1 = p1;
		lp0._x += lineSize;
		lp0._y += lineSize;
		lp1._x -= lineSize;
		lp1._y -= lineSize;


		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp1._x, lp1._y },
			D2D1_POINT_2F{ lp1._x, lp0._y },
			_pFace_Hover_Button_S_LineBrush,
			lineSize
		);
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp1._x, lp1._y },
			D2D1_POINT_2F{ lp0._x, lp1._y },
			_pFace_Hover_Button_S_LineBrush,
			lineSize
		);


		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp0._x, lp0._y },
			D2D1_POINT_2F{ lp1._x, lp0._y },
			_pFace_Hover_Button_H_LineBrush,
			lineSize
		);
		ctx->getD2dRenderTarget()->DrawLine(
			D2D1_POINT_2F{ lp0._x, lp0._y },
			D2D1_POINT_2F{ lp0._x, lp1._y },
			_pFace_Hover_Button_H_LineBrush,
			lineSize
		);
#endif
	}
}

void ToolBox::SubItemHoverDrawing::getFace_Hover_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t line_Size;
	getFace_LineSize(line_Size);

	getFace_Bounds(item, p0, p1);
	p0._x = p0._x + line_Size;
	p0._y = p0._y + line_Size;
	p1._x = p1._x - line_Size;
	p1._y = p1._y - line_Size;
}
