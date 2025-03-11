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


	targetDirectory += L"\\";

	std::wstring projectDirectory = targetDirectory + projectName + L"\\";
	std::wstring projectFilePath  = projectDirectory + projectName + L".vcxproj";

	std::wstring projectFiltersFilePath = projectFilePath + L".filters";

	std::wstring solutionDirectory = projectDirectory;
	std::wstring solutionFilePath  = solutionDirectory + projectName + L".sln";


	VcFile::Parameter param;
	param.set(L"$(VcTemplateFilePath)"      , templateFilePath);
	param.set(L"$(VcTargetDirectory)"       , targetDirectory);
	param.set(L"$(VcProjectName)"           , projectName);
	param.set(L"$(VcProjectDirectory)"      , projectDirectory);
	param.set(L"$(VcProjectFilePath)"       , projectFilePath);
	param.set(L"$(VcProjectFiltersFilePath)", projectFiltersFilePath);	
	param.set(L"$(VcSolutionDirectory)"     , solutionDirectory);	
	param.set(L"$(VcSolutionFilePath)"      , solutionFilePath);	


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
