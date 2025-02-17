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
bool ToolBox::ItemDrawing::createDeviceResources(cx::gw::Context* ctx)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	if (!_pFrameFillBrush)
	{
		cx::gw::Color Frame_FillColor;
		getFrame_FillColor(Frame_FillColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Frame_FillColor._r, Frame_FillColor._g, Frame_FillColor._b, Frame_FillColor._a),
			&_pFrameFillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFrameLineBrush)
	{
		cx::gw::Color Frame_LineColor;		
		getFrame_LineColor(Frame_LineColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Frame_LineColor._r, Frame_LineColor._g, Frame_LineColor._b, Frame_LineColor._a),
			&_pFrameLineBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}

	//-----------------------------------------------------------------------
	if (!_pCaptionTextBrush)
	{
		cx::gw::Color Caption_TextColor;
		getCaption_TextColor(Caption_TextColor);
		hr = ctx->getD2dRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(Caption_TextColor._r, Caption_TextColor._g, Caption_TextColor._b, Caption_TextColor._a),
			&_pCaptionTextBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pCaptionTextFormat)
	{
		bool Caption_TextFontBold;
		getCaption_TextFontBold(Caption_TextFontBold);
		hr = ctx->getDWriteFactory()->CreateTextFormat(
			//L"Arial",
			//L"돋움",
			//L"FixedSys",
			L"맑은 고딕",
			nullptr,
			Caption_TextFontBold ? DWRITE_FONT_WEIGHT_BOLD: DWRITE_FONT_WEIGHT_NORMAL, // DWRITE_FONT_WEIGHT_ULTRA_BLACK 
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			12.0f,
			L"ko-kr",
			&_pCaptionTextFormat
		);
		if (FAILED(hr))
		{
			return false;
		}
		//_pCaptionTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // hcenter
		_pCaptionTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // vcenter
	}

	return true;
}

void ToolBox::ItemDrawing::destroyDeviceResources(void)
{
	//-----------------------------------------------------------------------
	if (_pFrameFillBrush)
	{
		_pFrameFillBrush->Release();
		_pFrameFillBrush = nullptr;
	}
	if (_pFrameLineBrush)
	{
		_pFrameLineBrush->Release();
		_pFrameLineBrush = nullptr;
	}

	//-----------------------------------------------------------------------
	if (_pCaptionTextBrush)
	{
		_pCaptionTextBrush->Release();
		_pCaptionTextBrush = nullptr;
	}
	if (_pCaptionTextFormat)
	{
		_pCaptionTextFormat->Release();
		_pCaptionTextFormat = nullptr;
	}
}

//===========================================================================
void ToolBox::ItemDrawing::drawItem(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	drawFrame(ctx, itemView, item);

	drawIcon(ctx, itemView, item);
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
	rect.right  = p1._x;
	rect.top    = p0._y;
	rect.bottom = p1._y;


	ctx->getD2dRenderTarget()->FillRectangle(&rect, _pFrameFillBrush);
	if (lineSize>0)
	{
		ctx->getD2dRenderTarget()->DrawRectangle(&rect, _pFrameLineBrush, lineSize);
	}
}

void ToolBox::ItemDrawing::drawIcon(cx::gw::Context* ctx, ToolBox::ItemView* itemView, ToolBox::Item* item)
{
	cx::gw::BitmapSharedPtr bitmapSharedPtr;
	bitmapSharedPtr = itemView->getBitmapList()->findBitmap(item->getIcon());
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
	rect.right  = p1._x;
	rect.top    = p0._y;
	rect.bottom = p1._y;


	ctx->getD2dRenderTarget()->DrawTextW(
		item->getCaption().c_str(),
		static_cast<UINT32>(item->getCaption().length()),
		_pCaptionTextFormat,
		rect,
		_pCaptionTextBrush
	);
}

//===========================================================================
void ToolBox::ItemDrawing::getFrame_FillColor(cx::gw::Color& color)
{
	color = cx::gw::Color(0.80f, 0.80f, 1.00f, 1.0f);
}

void ToolBox::ItemDrawing::getFrame_LineColor(cx::gw::Color& color)
{
	color = cx::gw::Color(0.50f, 0.50f, 1.00f, 1.0f);
}

void ToolBox::ItemDrawing::getFrame_LineSize(cx::gw::coord_t& size)
{
	size = 0;
}

void ToolBox::ItemDrawing::getCaption_TextColor(cx::gw::Color& color)
{
	color = cx::gw::Color(0.0f, 0.0f, 0.0f, 1.0f);
}

void ToolBox::ItemDrawing::getCaption_TextFontBold(bool& bold)
{
	bold = false;
}

//===========================================================================
void ToolBox::ItemDrawing::getFrame_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	p0._x = item->getX();
	p1._x = item->getX() + item->getCX();
	p0._y = item->getY();
	p1._y = item->getY() + item->getCY();
}

void ToolBox::ItemDrawing::getIcon_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t indentSpace_Size;
	cx::gw::coord_t depth_Size;
	cx::gw::coord_t icon_Size;

	getIndentSpace_Size(item, indentSpace_Size);
	getDepth_Size(item, depth_Size);
	getIcon_Size(item, icon_Size);

	getFrame_Bounds(item, p0, p1);
	p0._x = indentSpace_Size + item->getDepth() * depth_Size;
	p1._x = p0._x + icon_Size;
}

void ToolBox::ItemDrawing::getCaption_Bounds(ToolBox::Item* item, cx::gw::Point& p0, cx::gw::Point& p1)
{
	cx::gw::coord_t indentSpace_Size;
	cx::gw::coord_t depth_Size;
	cx::gw::coord_t icon_Size;
	cx::gw::coord_t iconSpace_Size;

	getIndentSpace_Size(item, indentSpace_Size);
	getDepth_Size(item, depth_Size);
	getIcon_Size(item, icon_Size);
	getIconSpace_Size(item, iconSpace_Size);

	getFrame_Bounds(item, p0, p1);
	p0._x = indentSpace_Size + item->getDepth() * depth_Size + icon_Size + iconSpace_Size;
}

//===========================================================================
void ToolBox::ItemDrawing::getIndentSpace_Size(ToolBox::Item* item, cx::gw::coord_t& size)
{
	size = 4;
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
	if (item->getIcon().empty())
	{
		size = 0;
		return;
	}

	size = 2;
}


