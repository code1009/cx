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
DrawingSession::DrawingSession(Context* context) :
	_Context{ context }
{
}

//===========================================================================
void DrawingSession::Clear(Color const& color)
{
	_Context->getD2dHwndRenderTarget()->Clear(color._Value);
}

//===========================================================================
void DrawingSession::DrawText(std::wstring const& text, float x, float y, float w, float h, Color const& color, TextFormat& format)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);


	if (nullptr == format._Value.get())
	{
		HRESULT hr;
		wil::com_ptr_nothrow<IDWriteTextFormat> DWriteTextFormat;

		hr = _Context->getDWriteFactory()->CreateTextFormat(
			format._FontFamily.c_str(),
			nullptr,
			format._FontBold   ? DWRITE_FONT_WEIGHT_ULTRA_BLACK : DWRITE_FONT_WEIGHT_NORMAL,
			format._FontItalic ? DWRITE_FONT_STYLE_ITALIC       : DWRITE_FONT_STYLE_NORMAL ,
			DWRITE_FONT_STRETCH_NORMAL,
			format._FontSize,
			L"ko-kr",
			DWriteTextFormat.put()
		);
		if (FAILED(hr))
		{
			return;
		}

		switch (format._TextHAlign)
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

		switch (format._TextVAlign)
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


		format._Value = DWriteTextFormat;		
		format.registerResource(_Context->getDeviceIndependentResourceManager());
	}
		

	_Context->getD2dHwndRenderTarget()->DrawText(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		format._Value.get(),
		D2D1::RectF(x, y, x + w, y + h),
		brush.get(),
		D2D1_DRAW_TEXT_OPTIONS_CLIP | D2D1_DRAW_TEXT_OPTIONS_NO_SNAP,
		DWRITE_MEASURING_MODE_NATURAL
	);
}

//===========================================================================
void DrawingSession::DrawLine(float x0, float y0, float x1, float y1, Color const& color, float strokeWidth)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->DrawLine(
		D2D1::Point2F(x0, y0),
		D2D1::Point2F(x1, y1),
		brush.get(),
		strokeWidth,
		nullptr
	);
}

//===========================================================================
void DrawingSession::FillRectangle(float x, float y, float w, float h, Color const& color)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->FillRectangle(
		D2D1::RectF(x, y, x + w, y + h),
		brush.get()
	);
}

void DrawingSession::DrawRectangle(float x, float y, float w, float h, Color const& color, float strokeWidth)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->DrawRectangle(
		D2D1::RectF(x, y, x + w, y + h),
		brush.get(),
		strokeWidth,
		nullptr
	);
}

void DrawingSession::FillRoundedRectangle(float x, float y, float w, float h, float radiusX, float radiusY, Color const& color)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->FillRoundedRectangle(
		D2D1::RoundedRect(
			D2D1::RectF(x, y, x + w, y + h),
			radiusX,
			radiusY
		),
		brush.get()
	);
}

void DrawingSession::DrawRoundedRectangle(float x, float y, float w, float h, float radiusX, float radiusY, Color const& color, float strokeWidth)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->DrawRoundedRectangle(
		D2D1::RoundedRect(
			D2D1::RectF(x, y, x + w, y + h),
			radiusX,
			radiusY
		),
		brush.get(),
		strokeWidth,
		nullptr
	);
}

//===========================================================================
void DrawingSession::FillCircle(Point const& center, float radius, Color const& color)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->FillEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(center.X, center.Y),
			radius,
			radius
		),
		brush.get()
	);
}

void DrawingSession::DrawCircle(Point const& center, float radius, Color const& color, float strokeWidth)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(center.X, center.Y),
			radius,
			radius
		),
		brush.get(),
		strokeWidth,
		nullptr
	);
}

void DrawingSession::FillEllipse(float x, float y, float radiusX, float radiusY, Color const& color)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->FillEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(x, y),
			radiusX,
			radiusY
		),
		brush.get()
	);
}

void DrawingSession::DrawEllipse(float x, float y, float radiusX, float radiusY, Color const& color, float strokeWidth)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);

	_Context->getD2dHwndRenderTarget()->DrawEllipse(
		D2D1::Ellipse(
			D2D1::Point2F(x, y),
			radiusX,
			radiusY
		),
		brush.get(),
		strokeWidth,
		nullptr
	);
}

//===========================================================================
void DrawingSession::Transform(D2D1_MATRIX_3X2_F const& value)
{
	_Context->getD2dHwndRenderTarget()->SetTransform(value);
}

D2D1_MATRIX_3X2_F DrawingSession::Transform(void)
{
	D2D1_MATRIX_3X2_F value;
	_Context->getD2dHwndRenderTarget()->GetTransform(&value);
	return value;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
