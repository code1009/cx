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
void DrawingSession::DrawText(std::wstring const& text, float x, float y, float w, float h, Color const& color, TextFormat const& format)
{
	auto brush = _Context->getCurrentSolidColorBrush();
	brush->SetColor(color._Value);


	_Context->getD2dHwndRenderTarget()->DrawText(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		format._Value.get(),
		D2D1::RectF(x, y, x + w, y + h),
		brush.get(),
		D2D1_DRAW_TEXT_OPTIONS_NONE,
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
