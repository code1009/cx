/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

#include <component/std_wstring_utility.hpp>
#include <component/fs_std_wstring.hpp>

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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
