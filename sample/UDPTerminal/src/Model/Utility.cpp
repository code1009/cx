/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

//===========================================================================
#include "Utility.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::vector<std::uint8_t> hexStringToByteArray(const std::wstring& hexString) 
{
	std::vector<std::uint8_t> byteArray;
	std::wistringstream hexStream(hexString);
	std::wstring byteString;

	while (hexStream >> byteString) 
	{
		std::uint8_t byte = static_cast<std::uint8_t>(std::stoi(byteString, nullptr, 16));
		byteArray.push_back(byte);
	}

	return byteArray;
}

std::wstring byteArrayToHexString(const std::vector<std::uint8_t>& byteArray)
{
	std::wostringstream hexStream;
	for (const auto& byte : byteArray) 
	{
		if (&byte != &byteArray[0]) 
		{
			hexStream << L" ";
		}
		hexStream << std::setw(2) << std::setfill(L'0') << std::hex << static_cast<int>(byte);
	}
	return hexStream.str();
}

