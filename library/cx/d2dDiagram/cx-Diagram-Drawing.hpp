#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	void DrawPie(
		DrawingContext const& dctx,
		Coord centerX,
		Coord centerY,
		Coord radius,
		float startRadianAngle,
		float sweepRadianAngle,
		std::size_t segments,
		Color const& fillColor,
		Color const& lineColor,
		Coord lineSize = 1.0f
	);

	void DrawRotatedText(
		DrawingContext const& dctx,
		StringView const& text,
		Coord x,
		Coord y,
		float radianAngle,
		Color const& color,
		TextFormat const& textFormat
	);
}







