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
		s += cx::to_std_wstring(static_cast<uint8_t>(v._Value.a * 255.0f));
		s += L",";
		s += cx::to_std_wstring(static_cast<uint8_t>(v._Value.r * 255.0f));
		s += L",";
		s += cx::to_std_wstring(static_cast<uint8_t>(v._Value.g * 255.0f));
		s += L",";
		s += cx::to_std_wstring(static_cast<uint8_t>(v._Value.b * 255.0f));
		s += L"}";

		return s;
	}

	Color to_Color(std::wstring const& v)
	{
		Color color;


		auto tokens = cx::tokenize_object_std_wstring(cx::unescape_object_std_wstring(v));
		if (tokens.size() == 3)
		{
			color._Value.a = 1.0f;
			color._Value.r = cx::to_std_uint8_t(tokens[0]) / 255.0f;
			color._Value.g = cx::to_std_uint8_t(tokens[1]) / 255.0f;
			color._Value.b = cx::to_std_uint8_t(tokens[2]) / 255.0f;
		}
		else if (tokens.size() == 4)
		{
			color._Value.a = cx::to_std_uint8_t(tokens[0]) / 255.0f;
			color._Value.r = cx::to_std_uint8_t(tokens[1]) / 255.0f;
			color._Value.g = cx::to_std_uint8_t(tokens[2]) / 255.0f;
			color._Value.b = cx::to_std_uint8_t(tokens[3]) / 255.0f;
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
