/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cwctype>
#include <cwchar>
#include <limits>
#include <cerrno>

//===========================================================================
#include "std_wstring_utility.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring truncate_std_wstring(std::wstring str, size_t width, bool ellipsis)
{
	if (str.length() > width)
	{
		if (ellipsis)
		{
			return str.substr(0, width - 3U) + L"...";
		}
		else
		{
			return str.substr(0, width);
		}
	}

	return str;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
wchar_t to_uppercase_wchar_t(const wchar_t ch)
{
	return static_cast<wchar_t>(towupper(ch));
}

wchar_t to_lowercase_wchar_t(const wchar_t ch)
{
	return static_cast<wchar_t>(towlower(ch));
}

std::wstring to_uppercase_std_wstring(const std::wstring& s)
{
	std::wstring text = s;
	std::transform(text.begin(), text.end(), text.begin(), to_uppercase_wchar_t);
	return text;
}

std::wstring to_lowercase_std_wstring(const std::wstring& s)
{
	std::wstring text = s;
	std::transform(text.begin(), text.end(), text.begin(), to_lowercase_wchar_t);
	return text;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring ltrim_std_wstring(const std::wstring& s)
{
	std::wstring result = s;
	result.erase(result.begin(),
		std::find_if(result.begin(), result.end(),
			[](wchar_t ch)
			{
				return !std::iswspace(ch);
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
				return !std::iswspace(ch);
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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring unescape_object_std_wstring(const std::wstring& s)
{
	if (s.empty())
	{
		return s;
	}

	std::wstring result = s;

	if ((result.front() == L'\'' && result.back() == L'\'') ||
		(result.front() == L'\"' && result.back() == L'\"') ||
		(result.front() == L'(' && result.back() == L')') ||
		(result.front() == L'[' && result.back() == L']') ||
		(result.front() == L'{' && result.back() == L'}'))
	{
		result = result.substr(1, result.size() - 2);
	}

	return result;
}

std::wstring escape_object_std_wstring(const std::wstring& s, const std::wstring_view first, const std::wstring_view last)
{
	std::wstring result;
	
	
	result = first;
	result += s;
	result += last;

	return result;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::vector<std::wstring> tokenize_object_std_wstring(const std::wstring& s, std::wstring_view delimiter, bool trim)
{
	std::vector<std::wstring> tokens;
	std::wstring token;

	bool in_quotes = false;
	bool in_dquotes = false;
	bool in_parens = false;
	bool in_brackets = false;
	bool in_braces = false;

	std::size_t start = 0;


	for (std::size_t i = 0; i < s.length(); ++i)
	{
		if (s[i] == L'\'')
		{
			in_quotes = !in_quotes;
		}
		else if (s[i] == L'\"')
		{
			in_dquotes = !in_dquotes;
		}
		else if (s[i] == L'(')
		{
			in_parens = true;
		}
		else if (s[i] == L')')
		{
			in_parens = false;
		}
		else if (s[i] == L'[')
		{
			in_brackets = true;
		}
		else if (s[i] == L']')
		{
			in_brackets = false;
		}
		else if (s[i] == L'{')
		{
			in_braces = true;
		}
		else if (s[i] == L'}')
		{
			in_braces = false;
		}
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
std::wstring replace_std_wstring(const std::wstring& input, const std::wstring& findwhat, const std::wstring& replacewith)
{
	std::wstring::size_type findwhat_len    = findwhat.length();
	std::wstring::size_type replacewith_len = replacewith.length();
	std::wstring::size_type found;
	std::wstring            result;


	if (findwhat_len==0) 
	{
		return input;
	}

	result = input;
	found  = result.find(findwhat, 0);

	while (found != std::wstring::npos)
	{
		result.replace (found, findwhat.length(), replacewith);

		found = result.find (findwhat, found + replacewith_len);
	}

	return result;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool is_float_std_wstring(const std::wstring& s)
{
#pragma warning(push)
#pragma warning(disable:4189)
	const wchar_t* str = s.c_str();
	wchar_t* endptr = nullptr;
	errno = 0;
	float value = std::wcstof(str, &endptr);
	if (str == endptr) return false;
	if (errno == ERANGE) return false;
	while (*endptr) { if (!iswspace(*endptr)) return false; ++endptr; }
	return true;
#pragma warning(pop)
}

bool is_double_std_wstring(const std::wstring& s)
{
#pragma warning(push)
#pragma warning(disable:4189)
	const wchar_t* str = s.c_str();
	wchar_t* endptr = nullptr;
	errno = 0;
	double value = std::wcstod(str, &endptr);
	if (str == endptr) return false;
	if (errno == ERANGE) return false;
	while (*endptr) { if (!iswspace(*endptr)) return false; ++endptr; }
	return true;
#pragma warning(pop)
}

template<typename T>
static bool is_uinteger_std_wstring(const std::wstring& s)
{
	const wchar_t* str = s.c_str();
	wchar_t* endptr = nullptr;
	errno = 0;
	unsigned long long value = std::wcstoull(str, &endptr, 10);
	if (str == endptr) return false;
	if (errno == ERANGE) return false;
	if (value > std::numeric_limits<T>::max()) return false;
	while (*endptr) { if (!iswspace(*endptr)) return false; ++endptr; }
	return true;
}

template<typename T>
static bool is_integer_std_wstring(const std::wstring& s)
{
	const wchar_t* str = s.c_str();
	wchar_t* endptr = nullptr;
	errno = 0;
	long long value = std::wcstoll(str, &endptr, 10);
	if (str == endptr) return false;
	if (errno == ERANGE) return false;
	if (value < std::numeric_limits<T>::min() || value > std::numeric_limits<T>::max()) return false;
	while (*endptr) { if (!iswspace(*endptr)) return false; ++endptr; }
	return true;
}

bool is_uint8_std_wstring(const std::wstring& s)
{
	return is_uinteger_std_wstring<uint8_t>(s);	
}

bool is_uint16_std_wstring(const std::wstring& s)
{
	return is_uinteger_std_wstring<uint16_t>(s);
}

bool is_uint32_std_wstring(const std::wstring& s)
{
	return is_uinteger_std_wstring<uint32_t>(s);
}

bool is_uint64_std_wstring(const std::wstring& s)
{
	return is_uinteger_std_wstring<uint64_t>(s);
}

bool is_int8_std_wstring(const std::wstring& s)
{
	return is_integer_std_wstring<int8_t>(s);
}
bool is_int16_std_wstring(const std::wstring& s)
{
	return is_integer_std_wstring<int16_t>(s);
}
bool is_int32_std_wstring(const std::wstring& s)
{
	return is_integer_std_wstring<int32_t>(s);
}
bool is_int64_std_wstring(const std::wstring& s)
{
	return is_integer_std_wstring<int64_t>(s);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
} 




