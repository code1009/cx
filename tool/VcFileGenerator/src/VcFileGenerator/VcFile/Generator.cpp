/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>
#include <component/fs_std_wstring.hpp>
#include <component/std_wstring_utility.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static bool directoryExists(const std::wstring& directoryPath) 
{
	return std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Generator::Generator(Parameter* param) :
	_Parameter(param)
{
}

//===========================================================================
bool Generator::generate(void)
{
	bool rv;

	rv = loadVcTemplate();
	if (!rv)
	{
		return false;
	}

	rv = copyTemplateFiles();
	if (!rv)
	{
		return false;
	}

	rv = generateVcFile();
	if (!rv)
	{
		return false;
	}

	return true;
}

//===========================================================================
static std::wstring getRelativePath(const std::wstring& absolutePath, const std::wstring& basePath) 
{
	std::filesystem::path absPath(absolutePath);
	std::filesystem::path base(basePath);

	absPath = std::filesystem::absolute(absPath);
	base = std::filesystem::absolute(base);

	std::filesystem::path relativePath = std::filesystem::relative(absPath, base);
	return relativePath.wstring();
}

bool Generator::loadVcTemplate(void)
{
	bool rv;


	rv = loadVcTemplateData(_VcTemplate, _Parameter->get(L"$(VcTemplateFilePath)"));
	if (!rv)
	{
		return false;
	}
	std::wstring sourceDirectory;
	sourceDirectory = _Parameter->get(L"$(VcTemplateDirectory)");
	sourceDirectory += _VcTemplate._Settings.SourceDirectory;
	_Parameter->set(L"$(SourceDirectory)", sourceDirectory);


	if (!directoryExists(sourceDirectory))
	{
		MessageBox(nullptr, L"템플릿 디렉토리가 존재하지 않습니다.", L"에러", MB_OK);
		return false;
	}
	std::wstring targetDirectory;
	targetDirectory = _Parameter->get(L"$(TargetDirectory)");
	if (!directoryExists(targetDirectory))
	{
		MessageBox(nullptr, L"대상 디렉토리가 존재하지 않습니다.", L"에러", MB_OK);
		return false;
	}


	for (auto& itemFile : _VcTemplate._ItemFiles)
	{
		auto vcItemFilePath = _Parameter->get(L"$(VcTemplateDirectory)") + itemFile;

		rv = loadVcItemData(_VcItemData, vcItemFilePath);
		if (!rv)
		{
			return false;
		}
	}
	std::sort(_VcItemData._Items.begin(), _VcItemData._Items.end(),
			[](std::shared_ptr<VcItem> a, std::shared_ptr<VcItem> b)
			{
				if (a->_Type < b->_Type)
				{
					return true;
				}
				if (a->_Type > b->_Type)
				{
					return false;
				}

				if (a->_File < b->_File)
				{
					return true;
				}

				return false;
			}
		);
	for (auto& item : _VcItemData._Items)
	{
		if (item->_File.empty())
		{
			return false;
		}
		if (item->_File.size()>1)
		{
			if (item->_File[0] != '.')
			{
				std::wstring sourceFilePath;
				sourceFilePath = getRelativePath(item->_File, _Parameter->get(L"$(VcProjectDirectory)"));
				item->_File = sourceFilePath;
			}
		}
	}

	for (auto& configurationFile : _VcTemplate._ConfigurationFiles)
	{
		auto configurationFilePath = _Parameter->get(L"$(VcTemplateDirectory)") + configurationFile.second;

		std::wstring value;
		rv = loadFileString(configurationFilePath, value);
		if (!rv)
		{
			return false;
		}

		_Parameter->set(configurationFile.first, value);
	}

	return true;
}

//===========================================================================
bool Generator::generateVcFile(void)
{
	//-----------------------------------------------------------------------
	bool rv;
	std::wstring filePath;
	std::wstring fileString;


	//-----------------------------------------------------------------------
	VcFile_vcxproj_filters _vcFile_vcxproj_filters(this);

	_vcFile_vcxproj_filters.write();
	//OutputDebugStringW(_vcFile_vcxproj_filters._oss.str().c_str());

	filePath = _Parameter->get(L"$(VcProjectFiltersFilePath)");
	fileString = _vcFile_vcxproj_filters._oss.str();
	rv = saveFileString(filePath, fileString);
	if (!rv)
	{
		return false;
	}

	//-----------------------------------------------------------------------
	VcFile_vcxproj _vcFile_vcxproj(this);
	_vcFile_vcxproj.write();
	//OutputDebugStringW(_vcFile_vcxproj._oss.str().c_str());

	filePath = _Parameter->get(L"$(VcProjectFilePath)");
	fileString = _vcFile_vcxproj._oss.str();
	rv = saveFileString(filePath, fileString);
	if (!rv)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	std::wstring projectFilePath;
	projectFilePath =
		_Parameter->get(L"$(VcProjectName)") +
		L"\\" +
		_Parameter->get(L"$(VcProjectFile)");

	std::wstring solutionFolderGuid = makeGuid();
	VcFile_sln _vcFile_sln(this);
	_vcFile_sln.addProject(solutionFolderGuid, &_vcFile_vcxproj, projectFilePath);
	_vcFile_sln.write();
	//OutputDebugStringW(_vcFile_sln._oss.str().c_str());

	filePath = _Parameter->get(L"$(VcSolutionFilePath)");
	fileString = _vcFile_sln._oss.str();
	rv = saveFileString(filePath, fileString);
	if (!rv)
	{
		return false;
	}


	return true;
}

//===========================================================================
bool Generator::hasVcItemType(std::wstring type)
{
	for (auto item : _VcItemData._Items)
	{
		if (item->_Type == type)
		{
			return true;
		}
	}

	return false;
}

bool Generator::isVcItem_ClCompile_PrecompiledHeader(std::shared_ptr<VcItem> vcItem)
{
	if (_VcTemplate._Settings.ClCompile_PrecompiledHeader == vcItem->_File)
	{
		return true;
	}

	return false;
}

bool Generator::isVcItem_CustomBuild_Ribbon(std::shared_ptr<VcItem> vcItem)
{
	std::wstring fileName;
	fileName = cx::wfs::get_file_name_of_file_path(vcItem->_File);


	if (fileName == L"ribbon.xml")
	{
		return true;
	}

	return false;
}

//===========================================================================
bool Generator::copyTemplateFiles(void)
{
	FileCopy fileCopy(this);
	return fileCopy.copy();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
