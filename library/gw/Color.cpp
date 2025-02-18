/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "gw.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::gw
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Color::Color(float R, float G, float B, float A) :
	_r{R}, 
	_g{G}, 
	_b{B}, 
	_a{A} 
{
}

//===========================================================================
bool Color::operator== (const Color& v) const
{
	return (_r == v._r)
		&& (_g == v._g)
		&& (_b == v._b)
		&& (_a == v._a)
		;
}

bool Color::operator!= (const Color& v) const
{
	return !Color::operator==(v);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring to_std_wstring(const Color& v)
{
	std::wstring s;


	s = L"{";
	s += cx::to_std_wstring(v._r);
	s += L",";
	s += cx::to_std_wstring(v._g);
	s += L",";
	s += cx::to_std_wstring(v._b);
	s += L",";
	s += cx::to_std_wstring(v._a);
	s += L"}";

	return s;
}

Color to_Color(const std::wstring& v)
{
	Color color;

	
	auto tokens = cx::tokenizeObject_std_wstring(v);
	if (tokens.size() == 3)
	{
		color._r = cx::to_float(tokens[0]);
		color._g = cx::to_float(tokens[1]);
		color._b = cx::to_float(tokens[2]);
		color._a = 1.0f;
	}
	else if (tokens.size() == 4)
	{
		color._r = cx::to_float(tokens[0]);
		color._g = cx::to_float(tokens[1]);
		color._b = cx::to_float(tokens[2]);
		color._a = cx::to_float(tokens[3]);
	}

	return color;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




