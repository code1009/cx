/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

//===========================================================================
#include "File.hpp"





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








/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
