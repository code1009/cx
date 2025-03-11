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

	std::vector<std::wstring> fileList = getSourceFileList(sourceDirectory);

	std::shared_ptr<Item> item;

	std::wstring sourceFilePath;
	std::wstring targetFilePath;

	std::wstring sourceFileRelativePath;
	std::wstring sourceFileRelativeDirectory;
	std::wstring sourceFileRelativeDirectoryWithoutTrailingBackslash;

	for (const auto& file : fileList)
	{
		sourceFileRelativePath = file.substr(sourceDirectory.size());
		sourceFileRelativeDirectory = cx::wfs::get_directory_of_file_path(sourceFileRelativePath);
		if (sourceFileRelativeDirectory.rfind(L"\\") == (sourceFileRelativeDirectory.size()-1))
		{
			sourceFileRelativeDirectoryWithoutTrailingBackslash = 
				sourceFileRelativeDirectory.substr(0, sourceFileRelativeDirectory.size() - 1);
		}
		else
		{
			sourceFileRelativeDirectoryWithoutTrailingBackslash = L"";
		}

		sourceFilePath = file;
		targetFilePath = targetDirectory + sourceFileRelativePath;

		item = std::make_shared<Item>(sourceFilePath, targetFilePath);


		_Items.push_back(item);


		CX_RUNTIME_LOG(cxLDebug)
			<< L"<Item"
			<< L" "
			<< L"Filter=" << dquot(sourceFileRelativeDirectoryWithoutTrailingBackslash)
			<< L" "
			<< L"File=" << dquot(sourceFileRelativePath)
			<< L" "
			<< L"/>"
			;
	}
}

std::vector<std::wstring> FileCopy::getSourceFileList(const std::wstring& directoryPath)
{
	std::vector<std::wstring> fileList;
	std::wstring filePath;
	std::wstring fileExt;

	try 
	{
		for (const auto& entry : std::filesystem::recursive_directory_iterator(directoryPath))
		{
			if (entry.is_regular_file()) 
			{
				filePath = entry.path().wstring();


				fileExt = cx::wfs::get_file_extension_of_file_path(filePath);
				if (fileExt == L".sln")
				{
					continue;
				}
				if (fileExt == L".vcxproj")
				{
					continue;
				}
				if (fileExt == L".aps")
				{
					continue;
				}
				if (fileExt == L".filters")
				{
					continue;
				}
				if (fileExt == L".user")
				{
					continue;
				}


				if (std::wstring::npos != filePath.rfind(L".vcxproj.filters"))
				{
					continue;
				}
				if (std::wstring::npos != filePath.rfind(L".vcxproj.user"))
				{
					continue;
				}


				if (std::wstring::npos != filePath.find(L"vcpkg_installed\\"))
				{
					continue;
				}
				if (std::wstring::npos != filePath.find(L"x64\\Debug\\"))
				{
					continue;
				}
				if (std::wstring::npos != filePath.find(L"x64\\Release\\"))
				{
					continue;
				}
				if (std::wstring::npos != filePath.find(L"Win32\\Debug\\"))
				{
					continue;
				}
				if (std::wstring::npos != filePath.find(L"Win32\\Release\\"))
				{
					continue;
				}

				fileList.push_back(filePath);
			}
		}
	}
	catch (const std::filesystem::filesystem_error& ) 
	{
	}

	return fileList;
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

	shfo.hwnd                      = NULL;
	shfo.wFunc                     = FO_COPY;
	shfo.pFrom                     = source.c_str();
	shfo.pTo                       = target.c_str();
	shfo.fFlags                    = FOF_SIMPLEPROGRESS | FOF_MULTIDESTFILES | FOF_NOCONFIRMMKDIR;
	shfo.fAnyOperationsAborted     = FALSE;
	shfo.hNameMappings             = NULL;
	shfo.lpszProgressTitle         = L"Copy";

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
