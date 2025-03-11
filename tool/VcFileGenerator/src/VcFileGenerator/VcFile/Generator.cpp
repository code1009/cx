/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Generator::Generator()
{
}

//===========================================================================
bool Generator::generate(Parameter& param)
{
	bool rv;

	rv = loadTemplate(param);
	if (!rv)
	{
		return false;
	}

	saveVcFile_vcxproj_filters(param);

	return true;
}

bool Generator::loadTemplate(Parameter& param)
{
	bool rv;


	rv = loadVcTemplateData(_VcTemplate, param.get(L"$(VcTemplateFilePath)"));
	if (!rv)
	{
		return false;
	}

	for (auto& itemFile : _VcTemplate._ItemFiles)
	{
		auto vcItemFilePath = param.get(L"$(VcTemplateDirectory)") + itemFile;

		rv = loadVcItemData(_VcItemData, vcItemFilePath);
		if (!rv)
		{
			return false;
		}
	}
	std::sort(_VcItemData._Items.begin(), _VcItemData._Items.end(),
			[](std::shared_ptr<VcItem> a, std::shared_ptr<VcItem> b)
			{
				return a->_Type < b->_Type;
			}
		);

	for (auto& configurationFile : _VcTemplate._ConfigurationFiles)
	{
		auto configurationFilePath = param.get(L"$(VcTemplateDirectory)") + configurationFile.second;

		std::wstring value;
		rv = loadFileString(configurationFilePath, value);
		if (!rv)
		{
			return false;
		}

		param.set(configurationFile.first, value);
	}

	return true;
}

bool Generator::saveVcFile_vcxproj_filters(Parameter& param)
{
	VcFile_vcxproj_filters _vcFile(this);

	_vcFile.write();
	OutputDebugStringW(_vcFile._oss.str().c_str());

	return true;
}

bool Generator::saveVcFile_vcxproj(Parameter& param)
{
	VcFile_vcxproj _vcFile(this);

	_vcFile.write();
	OutputDebugStringW(_vcFile._oss.str().c_str());

	return true;
}

bool Generator::saveVcFile_sln(Parameter& param)
{
	VcFile_sln _vcFile(this);

	_vcFile.write();
	OutputDebugStringW(_vcFile._oss.str().c_str());

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
