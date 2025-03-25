#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::vector<std::uint8_t> hexStringToByteArray(const std::wstring& hexString);
std::wstring byteArrayToHexString(const std::vector<std::uint8_t>& byteArray);
