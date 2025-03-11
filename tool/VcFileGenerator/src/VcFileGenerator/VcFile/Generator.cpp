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

	rv = saveVcFile();
	if (!rv)
	{
		return false;
	}

	return true;
}

//===========================================================================
bool Generator::loadVcTemplate(void)
{
	bool rv;


	rv = loadVcTemplateData(_VcTemplate, _Parameter->get(L"$(VcTemplateFilePath)"));
	if (!rv)
	{
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
bool Generator::saveVcFile(void)
{
	bool rv;

	rv = saveVcFile_vcxproj_filters();
	if (!rv)
	{
		return false;
	}
	rv = saveVcFile_vcxproj();
	if (!rv)
	{
		return false;
	}
	rv = saveVcFile_sln();
	if (!rv)
	{
		return false;
	}

	return true;
}

bool Generator::saveVcFile_vcxproj_filters(void)
{
	VcFile_vcxproj_filters _vcFile(this);

	_vcFile.write();
	OutputDebugStringW(_vcFile._oss.str().c_str());

	return true;
}

bool Generator::saveVcFile_vcxproj(void)
{
	VcFile_vcxproj _vcFile(this);

	_vcFile.write();
	OutputDebugStringW(_vcFile._oss.str().c_str());

	return true;
}

bool Generator::saveVcFile_sln(void)
{
	VcFile_sln _vcFile(this);

	_vcFile.write();
	OutputDebugStringW(_vcFile._oss.str().c_str());

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




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
