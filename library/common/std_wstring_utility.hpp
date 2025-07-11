﻿#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring truncate_std_wstring(std::wstring str, size_t width, bool ellipsis = true);

wchar_t to_uppercase_wchar_t(const wchar_t ch);
wchar_t to_lowercase_wchar_t(const wchar_t ch);

std::wstring to_uppercase_std_wstring(const std::wstring& s);
std::wstring to_lowercase_std_wstring(const std::wstring& s);

std::wstring ltrim_std_wstring(const std::wstring& s);
std::wstring rtrim_std_wstring(const std::wstring& s);
std::wstring trim_std_wstring(const std::wstring& s);

std::wstring unescape_object_std_wstring(const std::wstring& s);
std::wstring escape_object_std_wstring(const std::wstring& s, const std::wstring_view first = L"{", const std::wstring_view last = L"}");

std::vector<std::wstring> tokenize_object_std_wstring(const std::wstring& s, const std::wstring_view delimiter = L",", bool trim = true);

std::wstring replace_std_wstring(const std::wstring& input, const std::wstring& findwhat, const std::wstring& replacewith);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
} 
