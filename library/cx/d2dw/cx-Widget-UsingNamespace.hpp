#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	using String = std::wstring;
	using StringStream = std::wstringstream;
	using StringView = std::wstring_view;

	//=======================================================================
	using Coord = float;
	using Point = d2d::Point;
	using Color = d2d::Color;
	using Colors = d2d::Colors;

	using DrawingContext = d2d::DrawingSession;

	using StrokeStyle = d2d::StrokeStyle;
	using CapStyle = d2d::CapStyle;

	using TextFormat = d2d::TextFormat;
	using TextHAlignment = d2d::TextHAlignment;
	using TextVAlignment = d2d::TextVAlignment;
}
