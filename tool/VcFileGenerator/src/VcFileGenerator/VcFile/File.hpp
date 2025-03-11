#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool saveFile(std::wstring filePath, std::vector<std::uint8_t>& fileData);
bool loadFile(std::wstring filePath, std::vector<std::uint8_t>& fileData);

bool loadFileString(std::wstring filePath, std::wstring& s);
bool saveFileString(std::wstring filePath, std::wstring& s);




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
