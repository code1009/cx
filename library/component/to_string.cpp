/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "to_string.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring ltrim_std_wstring(const std::wstring& s)
{
	std::wstring result = s;
	result.erase(result.begin(), 
		std::find_if(result.begin(), result.end(), 
			[](wchar_t ch)
			{
				return !std::isspace(ch);
			}
		)
	);
	return result;
}

std::wstring rtrim_std_wstring(const std::wstring& s)
{
	std::wstring result = s;
	result.erase(
		std::find_if(
			result.rbegin(), result.rend(), 
			[](wchar_t ch)
			{
				return !std::isspace(ch);
			}
		).base(), 
		result.end()
	);

	return result;
}

std::wstring trim_std_wstring(const std::wstring& s)
{
	return ltrim_std_wstring(rtrim_std_wstring(s));
}

std::wstring unescapeObject_std_wstring(const std::wstring& s)
{
	if (s.empty())
	{
		return s;
	}

	std::wstring result = s;

	if ((result.front() == L'\'' && result.back() == L'\'') ||
		(result.front() == L'\"' && result.back() == L'\"') ||
		(result.front() == L'('  && result.back() == L')' ) ||
		(result.front() == L'['  && result.back() == L']' ) ||
		(result.front() == L'{'  && result.back() == L'}' ))
	{
		result = result.substr(1, result.size() - 2);
	}

	return result;
}

std::wstring escapeObject_std_wstring(const std::wstring& s, const std::wstring& first, const std::wstring& last)
{
	std::wstring result = first + s + last;

	return result;
}

std::vector<std::wstring> tokenize_std_wstring(const std::wstring& s, std::wstring_view delimiter, bool trim)
{
	std::vector<std::wstring> tokens;
	std::wstring token;

	bool in_quotes   = false;
	bool in_dquotes  = false;
	bool in_parens   = false;
	bool in_brackets = false;
	bool in_braces   = false;

	std::size_t start = 0;


	for (std::size_t i = 0; i < s.length(); ++i)
	{
		if      (s[i] == L'\'') { in_quotes   = !in_quotes;  }
		else if (s[i] == L'\"') { in_dquotes  = !in_dquotes; }
		else if (s[i] == L'(' ) { in_parens   = true;        }
		else if (s[i] == L')' ) { in_parens   = false;       }
		else if (s[i] == L'[' ) { in_brackets = true;        }
		else if (s[i] == L']' ) { in_brackets = false;       }
		else if (s[i] == L'{' ) { in_braces   = true;        }
		else if (s[i] == L'}' ) { in_braces   = false;       }
		else if ((!in_quotes) && (!in_dquotes) && (!in_parens) && (!in_brackets) && (!in_braces))
		{
			if (s.substr(i, delimiter.length()) == delimiter)
			{
				if (trim)
				{
					token = trim_std_wstring(s.substr(start, i - start));
				}
				else
				{
					token = s.substr(start, i - start);
				}
				tokens.push_back(token);

				i += delimiter.length() - 1;
				start = i + 1;
			}
		}
	}

	if (trim)
	{
		token = trim_std_wstring(s.substr(start));
	}
	else
	{
		token = s.substr(start);
	}
	tokens.push_back(token);

	return tokens;
}





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




