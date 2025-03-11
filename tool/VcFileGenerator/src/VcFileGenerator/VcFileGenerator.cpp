/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>
#include <component/fs_std_wstring.hpp>

//===========================================================================
#include "VcFileGenerator.hpp"
#include "Config.hpp"

//===========================================================================
#include "VcFile\VcFile.hpp"





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


	std::wstring templateDirectory = cx::wfs::get_directory_of_file_path(templateFilePath);

	targetDirectory += L"\\";

	std::wstring projectDirectory = targetDirectory + projectName + L"\\";
	std::wstring projectFilePath  = projectDirectory + projectName + L".vcxproj";
	std::wstring projectGuid      = VcFile::makeGuid();

	std::wstring projectFiltersFilePath = projectFilePath + L".filters";

	std::wstring solutionDirectory = projectDirectory;
	std::wstring solutionFilePath  = solutionDirectory + projectName + L".sln";


	VcFile::Parameter param;
	param.set(L"$(VcTemplateDirectory)"     , templateDirectory);
	param.set(L"$(VcTemplateFilePath)"      , templateFilePath);

	param.set(L"$(VcTargetDirectory)"       , targetDirectory);

	param.set(L"$(VcProjectName)"           , projectName);
	param.set(L"$(VcProjectDirectory)"      , projectDirectory);
	param.set(L"$(VcProjectFilePath)"       , projectFilePath);
	param.set(L"$(VcProjectGuid)"           , projectGuid);
	
	param.set(L"$(VcProjectFiltersFilePath)", projectFiltersFilePath);	

	param.set(L"$(VcSolutionDirectory)"     , solutionDirectory);	
	param.set(L"$(VcSolutionFilePath)"      , solutionFilePath);	


	VcFile::Generator generator(&param);
	rv = generator.generate();
	if (rv == false)
	{
		return false;
	}


	getConfig()._TemplateFilePath = templateFilePath;
	saveConfig();
	return true;
}
