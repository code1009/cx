/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <cx/runtime/runtime.hpp>

#include <cx/common/std_wstring_utility.hpp>
#include <cx/common/fs_std_wstring.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::vector<std::wstring> SourceFileList::getFileList(const std::wstring& directoryPath)
{
	std::vector<std::wstring> fileList;
	std::wstring filePath;
	bool rv;


	try 
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
		{
			if (entry.is_regular_file()) 
			{
				filePath = entry.path().wstring();

				rv = shouldIgnoreFile(filePath);
				if (!rv)
				{
					fileList.push_back(filePath);
				}
			}
		}
	}
	catch (const std::filesystem::filesystem_error& ) 
	{
	}

	return fileList;
}

//===========================================================================
static const std::vector<std::wstring> _ignoredExtensions =
{
	L".filters", 
	L".vcxproj", 
	L".sln", 
	L".aps", 
	L".user"
};

static const std::vector<std::wstring> _ignoredKeywords = 
{
	L".vcxproj.filters", 
	L".vcxproj.user",
	L"\\.git\\", 
	L"\\.vs\\", 
	L"\\vcpkg_installed\\", 
	L"\\x64\\Debug\\", 
	L"\\x64\\Release\\",
	L"\\Debug\\", 
	L"\\Release\\"
};

//===========================================================================
bool SourceFileList::shouldIgnoreFile(const std::wstring& filePath)
{
	std::wstring fileExt = cx::wfs::get_file_extension_of_file_path(filePath);
	if (std::find(_ignoredExtensions.begin(), _ignoredExtensions.end(), fileExt) != _ignoredExtensions.end())
	{
		return true;
	}

	for (const auto& keyword : _ignoredKeywords)
	{
		if (filePath.find(keyword) != std::wstring::npos)
		{
			return true;
		}
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
FileCopy::FileCopy(Generator* generator) :
	_Generator(generator)
{
}

//===========================================================================
bool FileCopy::copy(void)
{
	bool rv;


	makeItems();
	rv = copyItems();
	if (!rv)
	{
		return false;
	}

	return true;
}

//===========================================================================
void FileCopy::makeItems(void)
{
	std::wstring sourceDirectory;
	std::wstring targetDirectory;
	sourceDirectory = _Generator->_Parameter->get(L"$(SourceDirectory)");
	targetDirectory = _Generator->_Parameter->get(L"$(VcProjectDirectory)");

	
	SourceFileList sourceFileList;
	std::vector<std::wstring> fileList = sourceFileList.getFileList(sourceDirectory);


	std::wstring sourceFilePath;
	std::wstring targetFilePath;

	std::wstring sourceFileRelativePath;
	std::wstring sourceFileRelativeDirectory;


	for (const auto& file : fileList)
	{
		sourceFileRelativePath = file.substr(sourceDirectory.size());
		sourceFileRelativeDirectory = cx::wfs::get_directory_of_file_path(sourceFileRelativePath);

		sourceFilePath = file;
		targetFilePath = targetDirectory + sourceFileRelativePath;
		auto item = std::make_shared<Item>(sourceFilePath, targetFilePath);
		_Items.push_back(item);
	}
}

//===========================================================================
bool FileCopy::copyItems(void)
{
	//-----------------------------------------------------------------------
	if (_Items.empty())
	{
		return true;
	}


	//-----------------------------------------------------------------------
	std::wstring source;
	std::wstring target;


	CX_RUNTIME_LOG(cxLDebug) << L"count=" << _Items.size();

	for (const auto& item : _Items)
	{
#if 0
		CX_RUNTIME_LOG(cxLDebug) 
			<< L"sourceFilePath=" << item->_Source
			<< L" ";
			<< L"targetFilePath=" << item->_Target
			<< L" ";
#endif

		source.insert(source.end(), item->_Source.begin(),  item->_Source.end());
		target.insert(target.end(), item->_Target.begin(),  item->_Target.end());
		source.push_back(0);
		target.push_back(0);
	}
	source.push_back(0); // FOF_MULTIDESTFILES \0\0
	target.push_back(0); // FOF_MULTIDESTFILES \0\0


	//-----------------------------------------------------------------------
	SHFILEOPSTRUCTW shfo;
	int             rc;


	memset (&shfo, 0, sizeof(shfo));

	shfo.hwnd                  = NULL;
	shfo.wFunc                 = FO_COPY;
	shfo.pFrom                 = source.c_str();
	shfo.pTo                   = target.c_str();
	shfo.fFlags                = FOF_SIMPLEPROGRESS | FOF_MULTIDESTFILES | FOF_NOCONFIRMMKDIR;
	shfo.fAnyOperationsAborted = FALSE;
	shfo.hNameMappings         = NULL;
	shfo.lpszProgressTitle     = L"Copy";

	rc = SHFileOperationW (&shfo);
	if (rc != 0)
	{
		return false;
	}
	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
