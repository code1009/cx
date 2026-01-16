/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"
#include "cx-d2d.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
C:\Program Files(x86)\Windows Kits\10\Include\10.0.26100.0\um\d2d1helper.h
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::d2d
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool StrokeStyle::createResources(Context* context)
{
	HRESULT hr;
	wil::com_ptr_nothrow<ID2D1StrokeStyle> D2D1StrokeStyle;
	D2D1_STROKE_STYLE_PROPERTIES _Properties = {};
	_Properties.startCap = static_cast<D2D1_CAP_STYLE>(_StartCap);
	_Properties.endCap = static_cast<D2D1_CAP_STYLE>(_EndCap);
	_Properties.dashCap = static_cast<D2D1_CAP_STYLE>(_DashCap);
	_Properties.lineJoin = static_cast<D2D1_LINE_JOIN>(_LineJoin);
	_Properties.miterLimit = _MiterLimit;
	_Properties.dashStyle = static_cast<D2D1_DASH_STYLE>(_DashStyle);
	_Properties.dashOffset = _DashOffset;
	hr = context->getD2dFactory()->CreateStrokeStyle(
		&_Properties,
		nullptr,
		0,
		D2D1StrokeStyle.put()
	);
	if (FAILED(hr))
	{
		return false;
	}

	_Value = D2D1StrokeStyle;
	registerResource(context->getDeviceIndependentResourceManager());
	return true;
}

void StrokeStyle::releaseResources(void)
{
	_Value.reset();
}

void StrokeStyle::StartCap(Stroke::CapStyle value)
{
	if (value != _StartCap)
	{
		_StartCap = value;
		_Value.reset();
	}
}

void StrokeStyle::EndCap(Stroke::CapStyle value)
{
	if (value != _EndCap)
	{
		_EndCap = value;
		_Value.reset();
	}
}

void StrokeStyle::DashCap(Stroke::CapStyle value)
{
	if (value != _DashCap)
	{
		_DashCap = value;
		_Value.reset();
	}
}

void StrokeStyle::LineJoin(Stroke::LineJoin value)
{
	if (value != _LineJoin)
	{
		_LineJoin = value;
		_Value.reset();
	}
}

void StrokeStyle::MiterLimit(float value)
{
	if (value != _MiterLimit)
	{
		_MiterLimit = value;
		_Value.reset();
	}
}

void StrokeStyle::DashStyle(Stroke::DashStyle value)
{
	if (value != _DashStyle)
	{
		_DashStyle = value;
		_Value.reset();
	}
}

void StrokeStyle::DashOffset(float value)
{
	if (value != _DashOffset)
	{
		_DashOffset = value;
		_Value.reset();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool TextFormat::createResources(Context* context)
{
	HRESULT hr;
	wil::com_ptr_nothrow<IDWriteTextFormat> DWriteTextFormat;

	hr = context->getDWriteFactory()->CreateTextFormat(
		_FontFamily.c_str(),
		nullptr,
		_FontBold ? DWRITE_FONT_WEIGHT_ULTRA_BLACK : DWRITE_FONT_WEIGHT_NORMAL,
		_FontItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		_FontSize,
		L"ko-kr",
		DWriteTextFormat.put()
	);
	if (FAILED(hr))
	{
		return false;
	}

	switch (_TextHAlign)
	{
	case TextHAlignment::Left:
		DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		break;
	case TextHAlignment::Right:
		DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		break;
	case TextHAlignment::Center:
		DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	case TextHAlignment::Justified:
		DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		break;
	default:
		DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		break;
	}

	switch (_TextVAlign)
	{
	case TextVAlignment::Top:
		DWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;
	case TextVAlignment::Bottom:
		DWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;
	case TextVAlignment::Center:
		DWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	default:
		DWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;
	}

	DWriteTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);


	_Value = DWriteTextFormat;
	registerResource(context->getDeviceIndependentResourceManager());

	return true;
}

void TextFormat::releaseResources(void)
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
