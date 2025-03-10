/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>

//===========================================================================
#include "VcFileGenerator.hpp"
#include "Config.hpp"

//===========================================================================
#include "VcFile\VcItem.hpp"
#include "VcFile\Parameter.hpp"
#include "VcFile\Generator.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool VcFileGenerator::initialize(void)
{
	loadConfig();
	return true;
}

void VcFileGenerator::terminate(void)
{
}

bool VcFileGenerator::generate(std::wstring templateFilePath, std::wstring targetDirectory, std::wstring projectName)
{
	bool rv;


	VcFile::Parameter param;
	param.set(L"$(TemplateFilePath)", templateFilePath);
	param.set(L"$(TargetDirectory)", targetDirectory);
	param.set(L"$(ProjectName)", projectName);	


	VcFile::Generator generator;
	rv = generator.generate(param);
	if (rv == false)
	{
		return false;
	}


	getConfig()._TemplateFilePath = templateFilePath;
	saveConfig();
	return true;
}
