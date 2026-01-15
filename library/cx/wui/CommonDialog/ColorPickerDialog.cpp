/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <commdlg.h>

//===========================================================================
#include <cx/wui/wui.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
ColorPickerDialog::ColorPickerDialog()
{
}

//===========================================================================
bool ColorPickerDialog::doModal (HWND hWnd, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b)
{
	CHOOSECOLORW cc;
	COLORREF      acrCustClr[16];
	COLORREF      rgbCurrent;
	
	rgbCurrent = RGB(r, g, b);
	
	ZeroMemory(&cc, sizeof(CHOOSECOLORW));
	cc.lStructSize     = sizeof(CHOOSECOLORW);
	cc.hwndOwner       = hWnd;
	cc.rgbResult       = rgbCurrent;
	cc.lpCustColors    = (LPDWORD)acrCustClr;
	cc.Flags           = CC_FULLOPEN | CC_RGBINIT;
	
	if (ChooseColorW(&cc) == TRUE)
	{
		r = GetRValue(cc.rgbResult);
		g = GetGValue(cc.rgbResult);
		b = GetBValue(cc.rgbResult);
		
		return true;
	}
	
	return false;
}

bool ColorPickerDialog::doModal(HWND hWnd, std::uint32_t& rgb)
{
	std::uint8_t r = (rgb >> 16) & 0xFF;
	std::uint8_t g = (rgb >> 8) & 0xFF;
	std::uint8_t b = (rgb >> 0) & 0xFF;
	if (doModal(hWnd, r, g, b))
	{
		rgb = (static_cast<std::uint32_t>(r) << 16) |
			  (static_cast<std::uint32_t>(g) << 8)  |
			  (static_cast<std::uint32_t>(b) << 0);
		return true;
	}
	return false;
}

bool ColorPickerDialog::doModal(HWND hWnd, float& R, float& G, float& B)
{
	std::uint8_t r = static_cast<std::uint8_t>(R * 255);
	std::uint8_t g = static_cast<std::uint8_t>(G * 255);
	std::uint8_t b = static_cast<std::uint8_t>(B * 255);
	if (doModal(hWnd, r, g, b))
	{
		R = static_cast<float>(r / 255);
		G = static_cast<float>(r / 255);
		B = static_cast<float>(r / 255);
		return true;
	}
	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





