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
bool ToolBox::ItemDrawing::createDeviceResources(ToolBox::ControlWindow* window)
{
	//-----------------------------------------------------------------------
	HRESULT hr;


	//-----------------------------------------------------------------------
	if (!_pFrameFillBrush)
	{
		hr = window->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.75f, 0.75f, 0.75f, 0.25f),
			&_pFrameFillBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pFrameLineBrush)
	{
		hr = window->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 0.5f),
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
		hr = window->getDRenderTarget()->CreateSolidColorBrush(
			D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f),
			&_pCaptionTextBrush
		);
		if (FAILED(hr))
		{
			return false;
		}
	}
	if (!_pCaptionTextFormat)
	{
		hr = window->getDWriteFactory()->CreateTextFormat(
			//L"Arial",
			L"돋움",
			//L"FixedSys",
			nullptr,
			DWRITE_FONT_WEIGHT_ULTRA_BLACK, // DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			16.0f,
			//L"en-us",
			L"ko-kr",
			&_pCaptionTextFormat
		);
		if (FAILED(hr))
		{
			return false;
		}
		//_pCaptionTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // hcenter
		//_pCaptionTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); // vcenter
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

void ToolBox::ItemDrawing::draw(ToolBox::ControlWindow* window, ToolBox::Item* item)
{
	drawFrame(window, item);
	drawCaption(window, item);
}

void ToolBox::ItemDrawing::drawFrame(ToolBox::ControlWindow* window, ToolBox::Item* item)
{
	D2D1_RECT_F rect;


	rect.left = item->getX();
	rect.right = item->getX() + item->getCX();
	rect.top = item->getY();
	rect.bottom = item->getY() + item->getCY();

	window->getDRenderTarget()->FillRectangle(&rect, _pFrameFillBrush);
	window->getDRenderTarget()->DrawRectangle(&rect, _pFrameLineBrush);
}

void ToolBox::ItemDrawing::drawCaption(ToolBox::ControlWindow* window, ToolBox::Item* item)
{
	D2D1_RECT_F rect;


	rect.left = item->getX();
	rect.right = item->getX() + item->getCX();
	rect.top = item->getY();
	rect.bottom = item->getY() + item->getCY();


	window->getDRenderTarget()->DrawTextW(
		item->getCaption().c_str(),
		static_cast<UINT32>(item->getCaption().length()),
		_pCaptionTextFormat,
		rect,
		_pCaptionTextBrush
	);
}
