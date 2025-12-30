/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../gw.hpp"






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void WidgetDrawingStyle::setStyleChangedHandler(StyleChangedHandler handler)
{
	_StyleChangedHandler = handler;
}

void WidgetDrawingStyle::setStyleChanged(std::uint32_t code, void* data)
{
	if (_StyleChangedHandler)
	{
		StyleChangedParam param;
		param._Sender = this;
		param._Code = code;
		param._Data = data;
		_StyleChangedHandler(&param);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetFillStyle::WidgetFillStyle(Color color) :
	_Color(color) 
{
}

//===========================================================================
Color WidgetFillStyle::getColor() const
{
	return _Color;
}

void WidgetFillStyle::setColor(const Color& color)
{
	if (_Color != color)
	{
		_Color = color;
		setStyleChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetLineStyle::WidgetLineStyle(
	Color color, 
	float width, 
	std::uint64_t attr, 
	std::uint64_t dash
):
	_Color{ color },
	_Width{width}, 
	_Attr {attr }, 
	_Dash {dash } 
{
}

//===========================================================================
Color WidgetLineStyle::getColor(void) const
{
	return _Color;
}

void WidgetLineStyle::setColor(const Color& color)
{
	if (_Color != color)
	{
		_Color = color;
		setStyleChanged();
	}
}

float WidgetLineStyle::getWidth(void) const
{
	return _Width;
}

void WidgetLineStyle::setWidth(float width)
{
	if (_Width != width)
	{
		_Width = width;
		setStyleChanged();
	}
}

std::uint64_t WidgetLineStyle::getAttr(void) const
{
	return _Attr;
}

void WidgetLineStyle::setAttr(std::uint64_t attr)
{
	if (_Attr != attr)
	{
		_Attr = attr;
		setStyleChanged();
	}
}

std::uint64_t WidgetLineStyle::getDash(void) const
{
	return _Dash;
}

void WidgetLineStyle::setDash(std::uint64_t dash)
{
	if (_Dash != dash)
	{
		_Dash = dash;
		setStyleChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WidgetTextStyle::WidgetTextStyle(
	Color         color,
	float         fontSize,
	std::wstring  fontFace,
	std::uint64_t fontAttr,
	std::uint64_t align
):
	_Color   { color               }, 
	_FontSize{ fontSize            },
	_FontFace{ std::move(fontFace) },
	_FontAttr{ fontAttr            },
	_Align   { align               } 
{
}

//===========================================================================
Color WidgetTextStyle::getColor(void) const
{
	return _Color;
}

void WidgetTextStyle::setColor(const Color& color)
{
	if (_Color != color)
	{
		_Color = color;
		setStyleChanged();
	}
}

float WidgetTextStyle::getFontSize(void) const
{
	return _FontSize;
}

void WidgetTextStyle::setFontSize(float fontSize)
{
	if (_FontSize != fontSize)
	{
		_FontSize = fontSize;
		setStyleChanged();
	}
}

std::wstring WidgetTextStyle::getFontFace(void) const
{
	return _FontFace;
}

void WidgetTextStyle::setFontFace(const std::wstring& fontFace)
{
	if (_FontFace != fontFace)
	{
		_FontFace = fontFace;
		setStyleChanged();
	}
}

std::uint64_t WidgetTextStyle::getFontAttr(void) const
{
	return _FontAttr;
}

void WidgetTextStyle::setFontAttr(std::uint64_t fontAttr)
{
	if (_FontAttr != fontAttr)
	{
		_FontAttr = fontAttr;
		setStyleChanged();
	}
}

std::uint64_t WidgetTextStyle::getAlign(void) const
{
	return _Align;
}

void WidgetTextStyle::setAlign(std::uint64_t align)
{
	if (_Align != align)
	{
		_Align = align;
		setStyleChanged();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring to_std_wstring(const WidgetFillStyle& o)
{
	std::wstring s;

	s = L"{";
	s += to_std_wstring(o.getColor());
	s += L"}";

	return s;
}

std::wstring to_std_wstring(const WidgetLineStyle& o)
{
	std::wstring s;
	
	s = L"{";
	s += to_std_wstring(o.getColor());
	s += L",";
	s += cx::to_std_wstring(o.getWidth());
	s += L",";
	s += std::to_wstring(o.getAttr());
	s += L",";
	s += std::to_wstring(o.getDash());
	s += L"}";

	return s;
}

std::wstring to_std_wstring(const WidgetTextStyle& o)
{
	std::wstring s;

	s  = L"{";
	s += to_std_wstring(o.getColor());
	s += L",";
	s += cx::to_std_wstring(o.getFontSize());
	s += L",";
	s += o.getFontFace();
	s += L",";
	s += std::to_wstring(o.getFontAttr());
	s += L",";
	s += std::to_wstring(o.getAlign());
	s += L"}";

	return s;
}

WidgetFillStyle to_WidgetFillStyle(std::wstring s)
{
	WidgetFillStyle o;


	auto tokens = cx::tokenize_object_std_wstring(s);
	if (1 == tokens.size())
	{
		o.setColor(to_Color(tokens[0]));
	}

	return o;
}

WidgetLineStyle to_WidgetLineStyle(std::wstring s)
{
	WidgetLineStyle o;


	auto tokens = cx::tokenize_object_std_wstring(s);
	if (4 == tokens.size())
	{
		o.setColor(to_Color(tokens[0]));
		o.setWidth(cx::to_float(tokens[1]));
		o.setAttr(cx::to_std_uint64_t(tokens[2]));
		o.setDash(cx::to_std_uint64_t(tokens[3]));
	}

	return o;
}

WidgetTextStyle to_WidgetTextStyle(std::wstring s)
{
	WidgetTextStyle o;

	auto tokens = cx::tokenize_object_std_wstring(s);
	if (5 == tokens.size())
	{
		o.setColor(to_Color(tokens[0]));
		o.setFontSize(cx::to_float(tokens[1]));
		o.setFontFace(tokens[2]);
		o.setFontAttr(cx::to_std_uint64_t(tokens[3]));
		o.setAlign(cx::to_std_uint64_t(tokens[4]));
	}

	return o;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




