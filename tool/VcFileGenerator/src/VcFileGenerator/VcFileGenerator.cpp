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
static void dumpVcFileItems(std::wstring sourceDirectory)
{
	CX_RUNTIME_LOG(cxLDebug) << sourceDirectory;


	VcFile::SourceFileList sourceFileList;
	std::vector<std::wstring> fileList = sourceFileList.getFileList(sourceDirectory);


	std::wstring sourceFilePath;
	std::wstring targetFilePath;


	std::wstring sourceFileRelativePath;
	std::wstring sourceFileRelativeDirectory;
	std::wstring sourceFileRelativeDirectoryWithoutTrailingBackslash;
	for (const auto& file : fileList)
	{
		sourceFileRelativePath = file.substr(sourceDirectory.size());
		sourceFileRelativeDirectory = cx::wfs::get_directory_of_file_path(sourceFileRelativePath);
		if (sourceFileRelativeDirectory.rfind(L"\\") == (sourceFileRelativeDirectory.size() - 1))
		{
			sourceFileRelativeDirectoryWithoutTrailingBackslash = 
				sourceFileRelativeDirectory.substr(0, sourceFileRelativeDirectory.size() - 1);
		}
		else
		{
			sourceFileRelativeDirectoryWithoutTrailingBackslash = L"";
		}

		CX_RUNTIME_LOG(cxLDebug)
			<< L"<Item"
			<< L" "
			<< L"Filter=" << VcFile::dquot(sourceFileRelativeDirectoryWithoutTrailingBackslash)
			<< L" "
			<< L"File=" << VcFile::dquot(L".\\" + sourceFileRelativePath)
			<< L" "
			<< L"/>"
			;
	}

	CX_RUNTIME_LOG(cxLDebug) << std::endl;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool VcFileGenerator::initialize(void)
{
	auto Subdirectories = VcFile::getSubdirectories(LR"__(D:\prj_my\cx\template\)__");

	for (auto& directory : Subdirectories)
	{
		dumpVcFileItems(directory);
	}

	loadConfig();
	return true;
}

void VcFileGenerator::terminate(void)
{
}

bool VcFileGenerator::generate(std::wstring templateFilePath, std::wstring targetDirectory, std::wstring solutionName, std::wstring projectName)
{
	bool rv;


	//-----------------------------------------------------------------------
	std::wstring templateDirectory = cx::wfs::get_directory_of_file_path(templateFilePath);
	std::wstring templateFileName  = cx::wfs::get_file_name_of_file_path(templateFilePath);
	std::wstring templateFile      = cx::wfs::get_file_of_file_path(templateFilePath);

	targetDirectory += L"\\";


	std::wstring solutionDirectory = targetDirectory;
	std::wstring solutionFilePath  = solutionDirectory + solutionName + L".sln";
	std::wstring solutionFileName  = cx::wfs::get_file_name_of_file_path(solutionFilePath);
	std::wstring solutionFile      = cx::wfs::get_file_of_file_path(solutionFilePath);
	std::wstring solutionGuid      = VcFile::makeGuid();

	std::wstring projectDirectory = solutionDirectory + projectName + L"\\";
	std::wstring projectFilePath  = projectDirectory + projectName + L".vcxproj";
	std::wstring projectFileName  = cx::wfs::get_file_name_of_file_path(projectFilePath);
	std::wstring projectFile      = cx::wfs::get_file_of_file_path(projectFilePath);
	std::wstring projectGuid      = VcFile::makeGuid();

	std::wstring projectFiltersFilePath = projectFilePath + L".filters";


	//-----------------------------------------------------------------------
	VcFile::Parameter param;
	param.set(L"$(TargetDirectory)"         , targetDirectory);

	param.set(L"$(VcTemplateDirectory)"     , templateDirectory);
	param.set(L"$(VcTemplateFilePath)"      , templateFilePath);
	param.set(L"$(VcTemplateFileName)"      , templateFileName);
	param.set(L"$(VcTemplateFile)"          , templateFile);

	param.set(L"$(VcSolutionName)"          , solutionName);	
	param.set(L"$(VcSolutionDirectory)"     , solutionDirectory);	
	param.set(L"$(VcSolutionFilePath)"      , solutionFilePath);	
	param.set(L"$(VcSolutionFileName)"      , solutionFileName);	
	param.set(L"$(VcSolutionFile)"          , solutionFile);	
	param.set(L"$(VcSolutionGuid)"          , solutionGuid);

	param.set(L"$(VcProjectName)"           , projectName);
	param.set(L"$(VcProjectDirectory)"      , projectDirectory);
	param.set(L"$(VcProjectFilePath)"       , projectFilePath);
	param.set(L"$(VcProjectFileName)"       , projectFileName);
	param.set(L"$(VcProjectFile)"           , projectFile);
	param.set(L"$(VcProjectGuid)"           , projectGuid);
	
	param.set(L"$(VcProjectFiltersFilePath)", projectFiltersFilePath);


	//-----------------------------------------------------------------------
	VcFile::Generator generator(&param);
	rv = generator.generate();
	if (rv == false)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	getConfig()._TemplateFilePath = templateFilePath;
	getConfig()._SolutionName = solutionName;
	getConfig()._ProjectName = projectName;
	saveConfig();


	//-----------------------------------------------------------------------

	return true;
}
