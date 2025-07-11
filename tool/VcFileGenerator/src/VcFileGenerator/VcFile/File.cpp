/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>
#include <common/fs_std_wstring.hpp>
#include <common/charset_system.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool saveFile(std::wstring filePath, std::vector<std::uint8_t>& fileData)
{
	std::ofstream file(filePath, std::ios::binary);
	if (!file)
	{
		return false;
	}

	if (!file.write(reinterpret_cast<const char*>(fileData.data()), fileData.size()))
	{
		return false;
	}

	return true;
}

bool loadFile(std::wstring filePath, std::vector<std::uint8_t>& fileData)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file)
	{
		return false;
	}

	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	fileData.clear();
	fileData.resize(static_cast<size_t>(size));


	if (!file.read(reinterpret_cast<char*>(fileData.data()), size))
	{
		return false;
	}

	return true;
}

bool loadFileString(std::wstring filePath, std::wstring& s)
{
	bool rv;

	std::vector<std::uint8_t> fileData;
	rv = loadFile(filePath, fileData);
	if (!rv)
	{
		return false;
	}


	if (fileData.size() > 3)
	{
		if (fileData[0] == 0xEF && fileData[1] == 0xBB && fileData[2] == 0xBF)
		{
			std::string utf8String;
			
			utf8String = std::string(reinterpret_cast<char*>(fileData.data() + 3), fileData.size() - 3);
			s = cx::utf8_to_wcs(utf8String);
		}
		else
		{
			std::string mbcsString;

			mbcsString = std::string(reinterpret_cast<char*>(fileData.data()), fileData.size());
			s = cx::mbcs_to_wcs(mbcsString);
		}
	}
	else
	{
		std::string mbcsString;

		mbcsString = std::string(reinterpret_cast<char*>(fileData.data()), fileData.size());
		s = cx::mbcs_to_wcs(mbcsString);
	}

	return true;
}

bool saveFileString(std::wstring filePath, std::wstring& s)
{
	bool rv;

	std::string utf8String;
	std::vector<std::uint8_t> fileData;
	
	utf8String = cx::wcs_to_utf8(s);
	fileData.resize(utf8String.size() + 3);
	fileData[0] = 0xEF;
	fileData[1] = 0xBB;
	fileData[2] = 0xBF;

	std::copy(utf8String.begin(), utf8String.end(), fileData.begin() + 3);
	rv = saveFile(filePath, fileData);
	if (!rv)
	{
		return false;
	}
	return true;
}







/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
