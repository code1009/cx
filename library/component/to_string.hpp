/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring ltrim_std_wstring(const std::wstring& s);
std::wstring rtrim_std_wstring(const std::wstring& s);
std::wstring trim_std_wstring(const std::wstring& s);

std::vector<std::wstring> tokenize_std_wstring(const std::wstring& s, std::wstring_view delimiter=L",", bool trim=true);

std::wstring to_std_wstring(const float value);
float to_float(const std::wstring value);

std::wstring to_std_wstring(const double value);
double to_double(const std::wstring value);

std::wstring to_std_wstring(const std::int8_t value);
std::wstring to_std_wstring(const std::int16_t value);
std::wstring to_std_wstring(const std::int32_t value);
std::wstring to_std_wstring(const std::int64_t value);
std::wstring to_std_wstring(const std::uint8_t value);
std::wstring to_std_wstring(const std::uint16_t value);
std::wstring to_std_wstring(const std::uint32_t value);
std::wstring to_std_wstring(const std::uint64_t value);

std::int8_t to_std_int8_t(const std::wstring value);
std::int16_t to_std_int16_t(const std::wstring value);
std::int32_t to_std_int32_t(const std::wstring value);
std::int64_t to_std_int64_t(const std::wstring value);
std::uint8_t to_std_uint8_t(const std::wstring value);
std::uint16_t to_std_uint16_t(const std::wstring value);
std::uint32_t to_std_uint32_t(const std::wstring value);
std::uint64_t to_std_uint64_t(const std::wstring value);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




