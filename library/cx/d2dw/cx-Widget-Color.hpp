#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	std::wstring to_std_wstring(Color const& v);
	Color to_Color(std::wstring const& v);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Widget
{
	//=======================================================================
	std::uint8_t getColorAByte(Color const& v);
	std::uint8_t getColorRByte(Color const& v);
	std::uint8_t getColorGByte(Color const& v);
	std::uint8_t getColorBByte(Color const& v);

	//=======================================================================
	Color fromARGB(
		std::uint8_t a,
		std::uint8_t r,
		std::uint8_t g,
		std::uint8_t b
	);
}







