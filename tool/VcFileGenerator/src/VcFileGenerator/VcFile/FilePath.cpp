/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

#include <common/std_wstring_utility.hpp>
#include <common/fs_std_wstring.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool directoryExists(const std::wstring& directoryPath) 
{
	return std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath);
}

std::wstring getRelativePath(const std::wstring& absolutePath, const std::wstring& basePath) 
{
	std::filesystem::path absPath(absolutePath);
	std::filesystem::path base(basePath);

	absPath = std::filesystem::absolute(absPath);
	base = std::filesystem::absolute(base);

	std::filesystem::path relativePath = std::filesystem::relative(absPath, base);
	return relativePath.wstring();
}

bool isAbsolutePath(const std::wstring& path)
{
	/*
	if (path.empty())
	{
		if (path[0] == L'$' || path[0] == L'.')
		{
			return false;
		}
		return true;
	}
	return false;
	*/

	return std::filesystem::path(path).is_absolute();
}

std::vector<std::wstring> getSubdirectories(const std::wstring& directoryPath)
{
	std::vector<std::wstring> subdirectories;

	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
		{
			if (entry.is_directory())
			{
				subdirectories.push_back(entry.path().wstring());
			}
		}
	}
	catch (const std::filesystem::filesystem_error&)
	{
	}

	return subdirectories;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
