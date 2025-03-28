﻿#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool directoryExists(const std::wstring& directoryPath);
std::wstring getRelativePath(const std::wstring& absolutePath, const std::wstring& basePath);
bool isAbsolutePath(const std::wstring& path);
std::vector<std::wstring> getSubdirectories(const std::wstring& directoryPath);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
