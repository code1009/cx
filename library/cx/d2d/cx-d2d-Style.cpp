/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include "cx-d2d.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void TextFormat::releaseDeviceResources(void)
{
	_Value.reset();
}

//===========================================================================
void TextFormat::FontFamily(std::wstring fontFamily)
{
	if (fontFamily!= _FontFamily)
	{
		_FontFamily = fontFamily;
		_Value.reset();
	}
}

void TextFormat::FontSize(float fontSize)
{
	if (fontSize != _FontSize)
	{
		_FontSize = fontSize;
		_Value.reset();
	}
}

void TextFormat::FontWeight(bool fontBold)
{
	if (fontBold != _FontBold)
	{
		_FontBold = fontBold;
		_Value.reset();
	}
}

void TextFormat::FontStyle(bool fontItalic)
{
	if (fontItalic != _FontItalic)
	{
		_FontItalic = fontItalic;
		_Value.reset();
	}
}

void TextFormat::HorizontalAlignment(TextHAlignment textHAlign)
{
	if (textHAlign != _TextHAlign)
	{
		_TextHAlign = textHAlign;
		_Value.reset();
	}
}

void TextFormat::VerticalAlignment(TextVAlignment textVAlign)
{
	if (textVAlign != _TextVAlign)
	{
		_TextVAlign = textVAlign;
		_Value.reset();
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
