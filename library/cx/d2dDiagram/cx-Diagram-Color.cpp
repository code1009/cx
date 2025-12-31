/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#include "cx-Diagram.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	std::wstring to_std_wstring(Color const& v)
	{
		std::wstring s;


		s = L"{";
		s += cx::to_std_wstring(v.A);
		s += L",";
		s += cx::to_std_wstring(v.R);
		s += L",";
		s += cx::to_std_wstring(v.G);
		s += L",";
		s += cx::to_std_wstring(v.B);
		s += L"}";

		return s;
	}

	Color to_Color(std::wstring const& v)
	{
		Color color;


		auto tokens = cx::tokenize_object_std_wstring(cx::unescape_object_std_wstring(v));
		if (tokens.size() == 3)
		{
			color.A = 255;
			color.R = cx::to_std_uint8_t(tokens[0]);
			color.G = cx::to_std_uint8_t(tokens[1]);
			color.B = cx::to_std_uint8_t(tokens[2]);
		}
		else if (tokens.size() == 4)
		{
			color.A = cx::to_std_uint8_t(tokens[0]);
			color.R = cx::to_std_uint8_t(tokens[1]);
			color.G = cx::to_std_uint8_t(tokens[2]);
			color.B = cx::to_std_uint8_t(tokens[3]);
		}

		return color;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::Diagram
{
	//=======================================================================
	Color fromARGB(
		std::uint8_t a,
		std::uint8_t r,
		std::uint8_t g,
		std::uint8_t b
	)
	{
		return Color{ a, r, g, b };
	}
}
