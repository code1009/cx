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
	std::uint8_t getColorA8(Color const& v);
	std::uint8_t getColorR8(Color const& v);
	std::uint8_t getColorG8(Color const& v);
	std::uint8_t getColorB8(Color const& v);
	
	std::uint32_t getColorRGB32(Color const& v);

	//=======================================================================
	Color fromARGB(
		std::uint8_t a,
		std::uint8_t r,
		std::uint8_t g,
		std::uint8_t b
	);
}







