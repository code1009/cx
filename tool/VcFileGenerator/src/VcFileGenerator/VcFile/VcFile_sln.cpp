/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <xmllite.h>
#include <wil/resource.h>
#include <wil/com.h>

//===========================================================================
#include <runtime/runtime.hpp>
#include <component/fs_std_wstring.hpp>
#include <component/std_wstring_utility.hpp>
#include <component/xml.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
VcFile_sln::Project::Project(std::wstring guid, VcFile_vcxproj* vcxproj, std::wstring filePath) :
	_Guid(guid),
	_vcxproj(vcxproj),
	_FilePath(filePath)
{
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
VcFile_sln::VcFile_sln(Generator* generator):
	_Generator(generator)
{
}

//===========================================================================
void VcFile_sln::addProject(std::wstring guid, VcFile_vcxproj* vcxproj, std::wstring filePath)
{
	_Projects.push_back(std::make_shared<Project>(guid, vcxproj, filePath));
}

//===========================================================================
void VcFile_sln::write(void)
{
	_SolutionGuid = _Generator->_Parameter->get(L"$(VcSolutionGuid)");
	_SolutionName = _Generator->_Parameter->get(L"$(VcSolutionName)");
	
	write_head();
	write_Project();
	write_Global();
}

void VcFile_sln::write_head(void)
{
	_oss << eline();

	_oss << L"Microsoft Visual Studio Solution File, Format Version " << L"12.00" << eline();
	_oss << L"# Visual Studio "                                       << L"17" << eline();
	_oss << L"VisualStudioVersion = "                                 << L"17.10.35013.160" << eline();
	_oss << L"MinimumVisualStudioVersion = "                          << L"10.0.40219.1" << eline();
}

void VcFile_sln::write_Project(void)
{
	/*
	Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "Application", "template\Application\Application.vcxproj", "{92090ECB-9102-45AF-94A6-9631B6B104A3}"
	EndProject
	*/

	for(auto& project : _Projects)
	{
		std::wstring solutionItemGuid;
		std::wstring projectName;
		std::wstring projectFile;
		std::wstring projectGuid;


		solutionItemGuid = project->_Guid;

		projectName = project->_vcxproj->_ProjectName;
		projectFile = project->_FilePath;
		projectGuid = project->_vcxproj->_ProjectGuid;

		_oss 
			<< "Project" 
			<< parentheses(dquot(solutionItemGuid)) << " = " 
			<< dquot(projectName) << ", " 
			<< dquot(projectFile) << ", " 
			<< dquot(projectGuid)  
			<< eline();

		_oss << "EndProject" << eline();
	}
}

void VcFile_sln::write_Global(void)
{
	_oss 
		<< "Global" 
		<< eline();


	write_GlobalSection_SolutionConfigurationPlatforms();
	write_GlobalSection_ProjectConfigurationPlatforms();
	write_GlobalSection_SolutionProperties();
	write_GlobalSection_ExtensibilityGlobals();


	_oss << "EndGlobal" << eline();
}

void VcFile_sln::write_GlobalSection_SolutionConfigurationPlatforms(void)
{
	/*
	GlobalSection(SolutionConfigurationPlatforms) = preSolution
		Debug|x64 = Debug|x64
		Debug|x86 = Debug|x86
		Release|x64 = Release|x64
		Release|x86 = Release|x86
	EndGlobalSection
	*/

	_oss << itab(1u) << L"GlobalSection(SolutionConfigurationPlatforms) = preSolution" << eline();

	_oss << itab(2u) << L"Debug|x64 = Debug|x64" << eline();
	_oss << itab(2u) << L"Debug|x86 = Debug|x86" << eline();
	_oss << itab(2u) << L"Release|x64 = Release|x64" << eline();
	_oss << itab(2u) << L"Release|x86 = Release|x86" << eline();

	_oss << itab(1u) << L"EndGlobalSection" << eline();
}

void VcFile_sln::write_GlobalSection_ProjectConfigurationPlatforms(void)
{
	/*
	GlobalSection(ProjectConfigurationPlatforms) = postSolution
		{73F5AA99-51DC-4C96-B04C-BD641BC27CAD}.Debug|x64.ActiveCfg = Debug|x64
		{73F5AA99-51DC-4C96-B04C-BD641BC27CAD}.Debug|x64.Build.0 = Debug|x64
		{73F5AA99-51DC-4C96-B04C-BD641BC27CAD}.Debug|x86.ActiveCfg = Debug|Win32
		{73F5AA99-51DC-4C96-B04C-BD641BC27CAD}.Debug|x86.Build.0 = Debug|Win32
		{73F5AA99-51DC-4C96-B04C-BD641BC27CAD}.Release|x64.ActiveCfg = Release|x64
		{73F5AA99-51DC-4C96-B04C-BD641BC27CAD}.Release|x64.Build.0 = Release|x64
		{73F5AA99-51DC-4C96-B04C-BD641BC27CAD}.Release|x86.ActiveCfg = Release|Win32
		{73F5AA99-51DC-4C96-B04C-BD641BC27CAD}.Release|x86.Build.0 = Release|Win32
	EndGlobalSection
	*/

	_oss << itab(1u) << L"GlobalSection(ProjectConfigurationPlatforms) = postSolution" << eline();

	for(auto& project : _Projects)
	{
		for(auto projectConfigurations : project->_vcxproj->_ProjectConfigurations)
		{
			std::wstring _ConfigurationTarget;

			if (projectConfigurations._Platform == L"x64")
			{
				_ConfigurationTarget = projectConfigurations._Configuration + L"|x64";
			}
			else if (projectConfigurations._Platform == L"Win32")
			{
				_ConfigurationTarget = projectConfigurations._Configuration + L"|x86";
			}
			else
			{
				_ConfigurationTarget = projectConfigurations._Configuration + L"|" + projectConfigurations._Platform;
			}

			_oss << itab(2u) << project->_vcxproj->_ProjectGuid << "." << _ConfigurationTarget << ".ActiveCfg = " << projectConfigurations._ConfigurationPlatform << eline();
			_oss << itab(2u) << project->_vcxproj->_ProjectGuid << "." << _ConfigurationTarget << ".Build.0 = "   << projectConfigurations._ConfigurationPlatform << eline();
		}
	}

	_oss << itab(1u) << L"EndGlobalSection" << eline();
}

void VcFile_sln::write_GlobalSection_SolutionProperties(void)
{
	/*
	GlobalSection(SolutionProperties) = preSolution
		HideSolutionNode = FALSE
	EndGlobalSection
	*/

	_oss << itab(1u) << L"GlobalSection(SolutionProperties) = preSolution" << eline();
	
	_oss << itab(2u) << L"HideSolutionNode = FALSE" << eline();
	
	_oss << itab(1u) << L"EndGlobalSection" << eline();
}

void VcFile_sln::write_GlobalSection_NestedProjects(void)
{
	/*
	GlobalSection(NestedProjects) = preSolution
		{A59C4142-D346-40AF-91E4-9CFC2F540069} = {C2826898-6919-49D6-9840-D436441D42E9}
		{58861549-E03E-4DF1-A103-B65DBDDC8BB7} = {50EFC057-4A08-4362-8DAB-9B2001376C18}
		{2E28F682-4E72-4C6E-9FE1-7296F3E7D0EF} = {50EFC057-4A08-4362-8DAB-9B2001376C18}
		{90A3A1A3-5647-4606-974A-E9E53080BBAC} = {060EEB5B-2577-46C9-AA87-4D9B6B7E623D}
		{E2975FFD-2F13-4133-AB09-89DBED289258} = {060EEB5B-2577-46C9-AA87-4D9B6B7E623D}
		{92090ECB-9102-45AF-94A6-9631B6B104A3} = {060EEB5B-2577-46C9-AA87-4D9B6B7E623D}
		{B916C1DD-B567-42D1-B448-A51D8FA0589C} = {50EFC057-4A08-4362-8DAB-9B2001376C18}
		{6E7C18C8-C702-418B-88BC-1A60D3C3A19C} = {C2826898-6919-49D6-9840-D436441D42E9}
	EndGlobalSection
	*/

	std::wstring guid1;
	std::wstring guid2;

	_oss << itab(1u) << "GlobalSection(NestedProjects) = preSolution" << eline();

	_oss << itab(2u) << guid1 << L" = " << guid2 << eline();

	_oss << itab(1u) << "EndGlobalSection" << eline();
}

void VcFile_sln::write_GlobalSection_ExtensibilityGlobals(void)
{
	/*
	GlobalSection(ExtensibilityGlobals) = postSolution
		SolutionGuid = {CC7C65DC-29E2-40ED-8440-0F008964DA46}
	EndGlobalSection
	*/

	_oss << itab(1u) << "GlobalSection(ExtensibilityGlobals) = postSolution" << eline();

	_oss << itab(2u) << L"SolutionGuid = " << _SolutionGuid << eline();

	_oss << itab(1u) << "EndGlobalSection" << eline();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
