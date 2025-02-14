/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "to_std_wstring.hpp"

//===========================================================================
#include "std_wstring_utility.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring to_std_wstring(const float value)
{
	std::wstringstream wss;
	wss << std::fixed << std::setprecision(6) << value;

	std::wstring result = wss.str();


	std::size_t dot_pos = result.find(L'.');
	if (dot_pos != std::wstring::npos)
	{
		std::size_t last_non_zero = result.find_last_not_of(L'0');
		if (last_non_zero == dot_pos)
		{
			result.erase(dot_pos);
		}
		else
		{
			result.erase(last_non_zero + 1);
		}
	}

	return result;
}

float to_float(const std::wstring value)
{
	return std::stof(value);
}

std::wstring to_std_wstring(const double value)
{
	std::wstringstream wss;
	wss << std::fixed << std::setprecision(14) << value;

	std::wstring result = wss.str();

	std::size_t dot_pos = result.find(L'.');
	if (dot_pos != std::wstring::npos)
	{
		std::size_t last_non_zero = result.find_last_not_of(L'0');
		if (last_non_zero == dot_pos)
		{
			result.erase(dot_pos);
		}
		else
		{
			result.erase(last_non_zero + 1);
		}
	}

	return result;
}

double to_double(const std::wstring value)
{
	return std::stod(value);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring to_std_wstring(const std::int8_t value)
{
	return std::to_wstring(static_cast<int>(value));
}

std::wstring to_std_wstring(const std::int16_t value)
{
	return std::to_wstring(static_cast<int>(value));
}

std::wstring to_std_wstring(const std::int32_t value)
{
	return std::to_wstring(value);
}

std::wstring to_std_wstring(const std::int64_t value)
{
	return std::to_wstring(value);
}

//===========================================================================
std::wstring to_std_wstring(const std::uint8_t value)
{
	return std::to_wstring(static_cast<unsigned int>(value));
}

std::wstring to_std_wstring(const std::uint16_t value)
{
	return std::to_wstring(static_cast<unsigned int>(value));
}

std::wstring to_std_wstring(const std::uint32_t value)
{
	return std::to_wstring(value);
}

std::wstring to_std_wstring(const std::uint64_t value)
{
	return std::to_wstring(value);
}

//===========================================================================
std::int8_t to_std_int8_t(const std::wstring value)
{
	return static_cast<std::int8_t>(std::stoi(value));
}

std::int16_t to_std_int16_t(const std::wstring value)
{
	return static_cast<std::int16_t>(std::stoi(value));
}

std::int32_t to_std_int32_t(const std::wstring value)
{
	return static_cast<std::int32_t>(std::stoi(value));
}

std::int64_t to_std_int64_t(const std::wstring value)
{
	return static_cast<std::int64_t>(std::stoll(value));
}

//===========================================================================
std::uint8_t to_std_uint8_t(const std::wstring value)
{
	return static_cast<std::uint8_t>(std::stoul(value));
}

std::uint16_t to_std_uint16_t(const std::wstring value)
{
	return static_cast<std::uint16_t>(std::stoul(value));
}

std::uint32_t to_std_uint32_t(const std::wstring value)
{
	return static_cast<std::uint32_t>(std::stoul(value));
}

std::uint64_t to_std_uint64_t(const std::wstring value)
{
	return static_cast<std::uint64_t>(std::stoull(value));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




