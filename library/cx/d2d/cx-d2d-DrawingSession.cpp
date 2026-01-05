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
void DrawingSession::DrawText(std::wstring const& text, float x, float y, float w, float h, Color const& color, TextFormat const& format)
{

}

//===========================================================================
void DrawingSession::DrawLine(float x0, float y0, float x1, float y1, Color const& color, float strokeWidth)
{

}

//===========================================================================
void DrawingSession::FillCircle(Point const& center, float radius, Color const& color)
{

}

void DrawingSession::DrawCircle(Point const& center, float radius, Color const& color, float strokeWidth)
{
}

void DrawingSession::FillRectangle(float x, float y, float w, float h, Color const& color)
{

}

void DrawingSession::DrawRectangle(float x, float y, float w, float h, Color const& color, float strokeWidth)
{

}

void DrawingSession::FillRoundedRectangle(float x, float y, float w, float h, float radiusX, float radiusY, Color const& color)
{

}

void DrawingSession::DrawRoundedRectangle(float x, float y, float w, float h, float radiusX, float radiusY, Color const& color, float strokeWidth)
{

}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
