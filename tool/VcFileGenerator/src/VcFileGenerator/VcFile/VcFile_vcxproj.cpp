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
VcFile_vcxproj::VcFile_vcxproj(Generator* generator):
	_Generator(generator)
{
}

//===========================================================================
void VcFile_vcxproj::write(void)
{
	_ProjectGuid = _Generator->_Parameter->get(L"$(VcProjectGuid)");
	_ProjectName = _Generator->_Parameter->get(L"$(VcProjectName)");

	ProjectConfiguration projectConfiguration_Debug_Win32(L"Debug", L"Win32");
	ProjectConfiguration projectConfiguration_Release_Win32(L"Release", L"Win32");
	ProjectConfiguration projectConfiguration_Debug_x64(L"Debug", L"x64");
	ProjectConfiguration projectConfiguration_Release_x64(L"Release", L"x64");
	_ProjectConfigurations.push_back(projectConfiguration_Debug_Win32);
	_ProjectConfigurations.push_back(projectConfiguration_Release_Win32);
	_ProjectConfigurations.push_back(projectConfiguration_Debug_x64);
	_ProjectConfigurations.push_back(projectConfiguration_Release_x64);

	write_xml();
	write_Project();
}

//===========================================================================
void VcFile_vcxproj::write_xml(void)
{
	_oss 
		<< L"<?xml"
		<< L" "
		<< L"version="  << dquot(L"1.0"  ) 
		<< L" "
		<< L"encoding=" << dquot(L"utf-8") 
		//<< L" "
		<< L"?>" 
		<< eline();
}

void VcFile_vcxproj::write_Project (void)
{
	//--------------------------------------------------------------------------
	_oss 
		<< L"<Project"
		<< L" "
		<< L"DefaultTargets=" << dquot(L"Build" )                                              
		<< L" "
		<< L"xmlns="          << dquot(L"http://schemas.microsoft.com/developer/msbuild/2003") 
		//<< L" "
		<< L">" 
		<< eline();


	//--------------------------------------------------------------------------
	write_ItemGroup_ProjectConfigurations();	
	write_ItemGroup_Item();
	write_PropertyGroup_Globals();
	
	write_Import_Project_VCTargetsPath_Microsoft_Cpp_Default_props();
	
	write_PropertyGroup_Configuration();
	
	write_Import_Project_VCTargetsPath_Microsoft_Cpp_props();
	write_ImportGroup_ExtensionSettings();
	write_ImportGroup_Shared();

	write_ImportGroup_PropertySheets();

	//write_PropertyGroup_UserMacros();
	write_PropertyGroup_EnvironmentVariables();
	write_PropertyGroup_VcPkg();

	write_ItemDefinitionGroup();

	write_Import_Project_VCTargetsPath_Microsoft_targets();
	write_ImportGroup_ExtensionTargets();


	//--------------------------------------------------------------------------
	_oss 
		<< L"</Project>"
		<< eline();
}

//===========================================================================
void VcFile_vcxproj::write_ItemGroup_ProjectConfigurations(void)
{
	/*
	<ItemGroup Label="ProjectConfigurations">
		<ProjectConfiguration Include="Debug|Win32">
			<Configuration>Debug</Configuration>
			<Platform>Win32</Platform>
		</ProjectConfiguration>
		<ProjectConfiguration Include="Release|Win32">
			<Configuration>Release</Configuration>
			<Platform>Win32</Platform>
		</ProjectConfiguration>
		<ProjectConfiguration Include="Debug|x64">
			<Configuration>Debug</Configuration>
			<Platform>x64</Platform>
		</ProjectConfiguration>
		<ProjectConfiguration Include="Release|x64">
			<Configuration>Release</Configuration>
			<Platform>x64</Platform>
		</ProjectConfiguration>
	</ItemGroup>
	*/
	_oss 
		<< ispace2(1) 
		<< L"<ItemGroup"
		<< L" "
		<< L"Label=" << dquot(L"ProjectConfigurations") 
		//<< L" "
		<< L">" 
		<< eline();


	for (auto e : _ProjectConfigurations)
	{
		_oss 
			<< ispace2(2) 
			<< L"<ProjectConfiguration"
			<< L" "
			<< L"Include=" << dquot(e._Include) 
			//<< L" "
			<< ">" 
			<< eline();

		_oss << ispace2(3) << L"<Configuration>" << e._Configuration << L"</Configuration>" << eline();
		_oss << ispace2(3) << L"<Platform>"      << e._Platform      << L"</Platform>"      << eline();

		_oss << ispace2(2) << L"</ProjectConfiguration>"<< eline();
	}


	_oss 
		<< ispace2(1) 
		<< L"</ItemGroup>"  
		<< eline();
}

//===========================================================================
void VcFile_vcxproj::write_ItemGroup_Item(void)
{
	write_ItemGroup_ItemType(L"ClCompile"      );
	write_ItemGroup_ItemType(L"ClInclude"      );
	write_ItemGroup_ItemType(L"ResourceCompile");
	write_ItemGroup_ItemType(L"Image"          );
	write_ItemGroup_ItemType(L"Text"           );
	write_ItemGroup_ItemType(L"Manifest"       );
	write_ItemGroup_ItemType(L"CustomBuild"    );
	write_ItemGroup_ItemType(L"None"           );
	write_ItemGroup_ItemType(L""               );
}

void VcFile_vcxproj::write_ItemGroup_ItemType(std::wstring type)
{
	//--------------------------------------------------------------------------
	if (!_Generator->hasVcItemType(type))
	{
		return;
	}


	//--------------------------------------------------------------------------
	std::wstring typeName;
	if (type.empty())
	{
		typeName = L"None";
	}
	else
	{
		typeName = type;
	}


	//--------------------------------------------------------------------------
	_oss << ispace2(1) << L"<ItemGroup>" << eline();


	//--------------------------------------------------------------------------
	for (auto item : _Generator->_VcItemData._Items)
	{
		if (item->_Type == type)
		{
			if (_Generator->isVcItem_ClCompile_PrecompiledHeader(item))
			{
				write_ItemGroup_ItemType_ClCompile_PrecompiledHeader(item);
			}
			else if (_Generator->isVcItem_CustomBuild_Ribbon(item))
			{
				write_ItemGroup_ItemType_CustomBuild_Ribbon(item);
			}
			else
			{
				_oss
					<< ispace2(2)
					<< L"<" << typeName
					<< L" "
					<< L"Include=" << dquot(item->_File)
					//<< L" "
					<< L"/>"
					<< eline();
			}
		}
	}


	//--------------------------------------------------------------------------
	_oss << ispace2(1) << L"</ItemGroup>" << eline();
}

void VcFile_vcxproj::write_ItemGroup_ItemType_ClCompile_PrecompiledHeader(std::shared_ptr<VcItem> vcItem)
{
	//--------------------------------------------------------------------------
	std::wstring typeName;

	typeName = L"ClCompile";
	_oss << ispace2(2)
		<< L"<" << typeName
		<< L" "
		<< L"Include=" << dquot(vcItem->_File)
		//<< L" "
		<< L">" 
		<< eline();


	//--------------------------------------------------------------------------
	std::wstring Condition_Variable;
	std::wstring Condition_Value;
	std::wstring Condition;
	std::wstring value;


	value = L"Create";


	//--------------------------------------------------------------------------
	/*
	<PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">Create</PrecompiledHeader>
	<PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|x64'">Create</PrecompiledHeader>
	...
	*/
	for (auto e : _ProjectConfigurations)
	{
		Condition_Variable = squot(L"$(Configuration)|$(Platform)");
		Condition_Value = squot(e._ConfigurationPlatform);
		Condition = Condition_Variable + L"==" + Condition_Value;

		_oss 
			<< ispace2(3) 
			<< L"<PrecompiledHeader" 
			<< L" "
			<< L"Condition=" << dquot(Condition)
			//<< L" "
			<< L">" 
			<< value
			//<< L" "
			<< L"</PrecompiledHeader>"
			<< eline();
	}


	//--------------------------------------------------------------------------
	_oss << ispace2(2) << "</ClCompile>" << eline();

}

void VcFile_vcxproj::write_ItemGroup_ItemType_CustomBuild_Ribbon(std::shared_ptr<VcItem> vcItem)
{
	//--------------------------------------------------------------------------
	/*
	<CustomBuild Include="..\src\Ribbon.xml">
		<Command Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'"  >uicc.exe ..\src\Ribbon.xml ..\src\Ribbon.bml /header:..\src\RibbonUI.h /res:..\src\RibbonUI.rc</Command>
		<Outputs Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'"  >Ribbon.bml;RibbonUI.rc;RibbonUI.h</Outputs>
		<Command Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">uicc.exe ..\src\Ribbon.xml ..\src\Ribbon.bml /header:..\src\RibbonUI.h /res:..\src\RibbonUI.rc</Command>
		<Outputs Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">Ribbon.bml;RibbonUI.rc;RibbonUI.h</Outputs>
	</CustomBuild>
	*/
	/*
	1. Ribbon.xml Ribbon.bml RibbonUI.h RibbonUI.rc

	2. Resouce.rc­
	   //////////////////////////////////////////////////////
	   // Ribbon
	   #include "RibbonUI.rc"

	3. RibbonUI.rc
	   ...
	   #include ".\RibbonUI.h"
	   ...
	   APPLICATION_RIBBON    UIFILE    "..\\src\\Ribbon.bml"
	*/


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(2) 
		<< L"<CustomBuild" 
		<< L" "
		<< L"Include=" << dquot(vcItem->_File)
		//<< L" "
		<< L">" 
		<< eline();


	//--------------------------------------------------------------------------
	std::wstring Condition_Variable;
	std::wstring Condition_Value;
	std::wstring Condition;
	std::wstring Command;
	std::wstring Outputs;


	Command = L"uicc.exe ..\\src\\Ribbon.xml ..\\src\\Ribbon.bml /header:..\\src\\RibbonUI.h /res:..\\src\\RibbonUI.rc";
	Outputs = L"Ribbon.bml;RibbonUI.rc;RibbonUI.h";


	//--------------------------------------------------------------------------
	for (auto e : _ProjectConfigurations)
	{
		Condition_Variable = squot(L"$(Configuration)|$(Platform)");
		Condition_Value = squot(e._ConfigurationPlatform);
		Condition = Condition_Variable + L"==" + Condition_Value;

		_oss << ispace2(3) 
			<< L"<Command" 
			<< L" "
			<< L"Condition=" << dquot(Condition)
			//<< L" "
			<< L">" 
			<< Command
			<< L"</Command>"
			<< eline();

		_oss << ispace2(3) 
			<< L"<Outputs" 
			<< L" "
			<< L"Condition=" << dquot(Condition)
			//<< L" "
			<< L">" 
			<< Outputs
			<< L"</Outputs>"
			<< eline();
	}


	//--------------------------------------------------------------------------
	_oss << ispace2(2) << "</CustomBuild>" << eline();
}

//===========================================================================
void VcFile_vcxproj::write_PropertyGroup_Globals(void)
{
	/*
	<PropertyGroup Label="Globals">
		<VCProjectVersion>17.0</VCProjectVersion>
		<Keyword>Win32Proj</Keyword>
		<ProjectGuid>{b916c1dd-b567-42d1-b448-a51d8fa0589c}</ProjectGuid>
		<RootNamespace>Application</RootNamespace>
		<WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
	</PropertyGroup>
	*/

	_oss 
		<< ispace2(1) 
		<< L"<PropertyGroup"
		<< L" "
		<< L"Label=" << dquot(L"Globals") 
		//<< L" "
		<< L">" 
		<< eline();

	_oss << ispace2(2) << L"<VCProjectVersion>"             << L"17.0"      << L"</VCProjectVersion>"             << eline();
	_oss << ispace2(2) << L"<Keyword>"                      << L"Win32Proj" << L"</Keyword>"                      << eline();
	_oss << ispace2(2) << L"<ProjectGuid>"                  << _ProjectGuid << L"</ProjectGuid>"                  << eline();
	_oss << ispace2(2) << L"<RootNamespace>"                << _ProjectName << L"</RootNamespace>"                << eline();
	_oss << ispace2(2) << L"<WindowsTargetPlatformVersion>" << L"10.0"      << L"</WindowsTargetPlatformVersion>" << eline();

	_oss 
		<< ispace2(1) 
		<< L"</PropertyGroup>"  
		<< eline();
}

//===========================================================================
void VcFile_vcxproj::write_PropertyGroup_Configuration(void)
{
	/*
	<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
		<ConfigurationType>Application</ConfigurationType>
		<UseDebugLibraries>true</UseDebugLibraries>
		<PlatformToolset>v143</PlatformToolset>
		<CharacterSet>Unicode</CharacterSet>
	</PropertyGroup>
	...
	*/

	std::wstring Condition_Variable;
	std::wstring Condition_Value;
	std::wstring Condition;

	bool isDebug;

	std::wstring ConfigurationType       ;
	std::wstring UseDebugLibraries       ;
	std::wstring PlatformToolset         ;
	std::wstring WholeProgramOptimization;
	std::wstring CharacterSet            ;


	ConfigurationType        = L"Application";
	UseDebugLibraries        = L"true"       ; // "true" or "false";
	PlatformToolset          = L"v143"       ; // "v120" or "v120_xp" or "v143"
	WholeProgramOptimization = L"true"       ;
	CharacterSet             = L"Unicode"    ; // "MultiByte" or "Unicode";

	for (auto e : _ProjectConfigurations)
	{
		Condition_Variable = squot(L"$(Configuration)|$(Platform)");
		Condition_Value = squot(e._ConfigurationPlatform);
		Condition = Condition_Variable + L"==" + Condition_Value;


		_oss 
			<< ispace2(1) 
			<< L"<PropertyGroup"
			<< L" "
			<< L"Condition=" << dquot(Condition)
			<< L" "
			<< L"Label=" << dquot(L"Configuration") 
			//<< L" "
			<< L">" 
			<< eline();


		isDebug = (e._Configuration == L"Debug") ? true : false;
		if (isDebug)
		{
			UseDebugLibraries = L"true";
		}
		else
		{
			UseDebugLibraries = L"false";
		}

		_oss << ispace2(2) << L"<" << L"ConfigurationType"        << L">" << ConfigurationType        << L"</" << L"ConfigurationType"        << L">" << eline();
		_oss << ispace2(2) << L"<" << L"UseDebugLibraries"        << L">" << UseDebugLibraries        << L"</" << L"UseDebugLibraries"        << L">" << eline();
		_oss << ispace2(2) << L"<" << L"PlatformToolset"          << L">" << PlatformToolset          << L"</" << L"PlatformToolset"          << L">" << eline();
		
		if (!isDebug)
		{
		_oss << ispace2(2) << L"<" << L"WholeProgramOptimization" << L">" << WholeProgramOptimization << L"</" << L"WholeProgramOptimization" << L">" << eline();
		}

		_oss << ispace2(2) << L"<" << L"CharacterSet"             << L">" << CharacterSet             << L"</" << L"CharacterSet"             << L">" << eline();


		_oss 
			<< ispace2(1) 
			<< L"</PropertyGroup>"  
			<< eline();
	}
}

//===========================================================================
void VcFile_vcxproj::write_ImportGroup_PropertySheets(void)
{
	/*
	<ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
		<Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
	</ImportGroup>
	<ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
		<Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
	</ImportGroup>
	<ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
		<Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
	</ImportGroup>
	<ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
		<Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
	</ImportGroup>
	*/

	std::wstring Condition_Variable;
	std::wstring Condition_Value;
	std::wstring Condition;

	std::wstring Condition_LocalAppDataPlatform = L"exists(\'$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\')";


	for (auto e : _ProjectConfigurations)
	{
		Condition_Variable = squot(L"$(Configuration)|$(Platform)");
		Condition_Value = squot(e._ConfigurationPlatform);
		Condition = Condition_Variable + L"==" + Condition_Value;

		_oss 
			<< ispace2(1) 
			<< L"<ImportGroup"
			<< L" "
			<< L"Label=" << dquot(L"PropertySheets") 
			<< L" "
			<< L"Condition=" << dquot(Condition)
			//<< L" "
			<< L">" 
			<< eline();


		_oss 
			<< ispace2(2) 
			<< "<Import"
			<< L" "
			<< "Project="   << dquot(L"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props") 
			<< L" "
			<< "Condition=" << dquot(Condition_LocalAppDataPlatform) 
			<< L" "
			<< "Label="     << dquot(L"LocalAppDataPlatform") 
			<< "/>" 
			<< eline();


		_oss 
			<< ispace2(1) 
			<< L"</ImportGroup>"  
			<< eline();
	}
}

//===========================================================================
void VcFile_vcxproj::write_ItemDefinitionGroup(void)
{
	/*
	<ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
		<ClCompile>
			...
		</ClCompile>
		<Link>
			...
		</Link>
	</ItemDefinitionGroup>
	...
	*/
	std::wstring Condition_Variable;
	std::wstring Condition_Value;
	std::wstring Condition;

	std::wstring ConfigurationParamName;
	std::wstring Configuration;


	for (auto e : _ProjectConfigurations)
	{
		Condition_Variable = squot(L"$(Configuration)|$(Platform)");
		Condition_Value = squot(e._ConfigurationPlatform);
		Condition = Condition_Variable + L"==" + Condition_Value;

		
		ConfigurationParamName = L"$(VcConfiguration." + e._ConfigurationPlatform + L")";
		Configuration = _Generator->_Parameter->get(ConfigurationParamName);


		_oss 
			<< ispace2(1) 
			<< L"<ItemDefinitionGroup"
			<< L" "
			<< L"Condition=" << dquot(Condition)
			//<< L" "
			<< L">" 
			<< eline();

		_oss
			<< Configuration;

		_oss 
			<< ispace2(1) 
			<< L"</ItemDefinitionGroup>"  
			<< eline();
	}
}

//===========================================================================
void VcFile_vcxproj::write_PropertyGroup_EnvironmentVariables(void)
{
	/*
	<PropertyGroup>
		<cxDirectory>$(SolutionDir)..\cx</cxDirectory>
	</PropertyGroup>
	*/

	
	if (_Generator->_VcTemplate._Settings.cxDirectory.empty() &&
		_Generator->_VcTemplate._EnvironmentVariables.empty() )
	{
		return;
	}


	//--------------------------------------------------------------------------
	_oss
		<< ispace2(1)
		<< L"<PropertyGroup"
		<< L" "
		<< L"Label=" << dquot(L"UserMacros") 
		//<< L" "
		<< L">"
		<< eline();

	
	//--------------------------------------------------------------------------
	std::wstring name;
	std::wstring value;


	//--------------------------------------------------------------------------
	std::wstring cxDirectory = _Generator->_VcTemplate._Settings.cxDirectory;
	std::wstring cxRelativeDirectory;
	if (!cxDirectory.empty())
	{
		if (isAbsolutePath(cxDirectory))
		{
			cxRelativeDirectory = getRelativePath(cxDirectory, _Generator->_Parameter->get(L"$(VcProjectDirectory)"));
			cxRelativeDirectory += L"\\";
			value = cxRelativeDirectory;
		}
		else
		{
			value = cxDirectory;
		}


		name = L"cxDirectory";
		_oss
			<< ispace2(2)
			<< L"<" << name << L">"
			<< value
			<< L"</" << name << L">"
			<< eline();
	}


	//--------------------------------------------------------------------------
	for(auto& variable : _Generator->_VcTemplate._EnvironmentVariables)
	{
		name = variable.first;
		value = variable.second;

		_oss
			<< ispace2(2)
			<< L"<" << name << L">"
			<< value
			<< L"</" << name << L">"
			<< eline();
	}


	//--------------------------------------------------------------------------
	_oss
		<< ispace2(1)
		<< L"</PropertyGroup>"
		<< eline();
}

void VcFile_vcxproj::write_PropertyGroup_VcPkg(void)
{
	/*
	<PropertyGroup Label="Vcpkg">
		<VcpkgEnableManifest>true</VcpkgEnableManifest>
	</PropertyGroup>
	<PropertyGroup Label="Vcpkg" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">
		<VcpkgTriplet>x64-windows</VcpkgTriplet>
	</PropertyGroup>
	<PropertyGroup Label="Vcpkg" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">
		<VcpkgTriplet>x64-windows</VcpkgTriplet>
	</PropertyGroup>
	*/

	_oss 
		<< ispace2(1) 
		<< L"<PropertyGroup"
		<< L" "
		<< L"Label=" << dquot(L"Vcpkg") 
		//<< L" "
		<< L">"
		<< eline();

	_oss << ispace2(2) << L"<" << L"VcpkgEnableManifest" << L">" << "True" << L"</" << L"VcpkgEnableManifest" << L">" << eline();

	_oss 
		<< ispace2(1) 
		<< L"</PropertyGroup>"
		<< eline();

	std::wstring Condition_Variable;
	std::wstring Condition_Value;
	std::wstring Condition;

	std::wstring ConfigurationParamName;
	std::wstring Configuration;


	for (auto e : _ProjectConfigurations)
	{
		Condition_Variable = squot(L"$(Configuration)|$(Platform)");
		Condition_Value = squot(e._ConfigurationPlatform);
		Condition = Condition_Variable + L"==" + Condition_Value;


		_oss 
			<< ispace2(1) 
			<< L"<PropertyGroup"
			<< L" "
			<< L"Label=" << dquot(L"Vcpkg") 
			<< L" "
			<< L"Condition=" << dquot(Condition)
			//<< L" "
			<< L">"
			<< eline();

		_oss << ispace2(2) << L"<" << L"VcpkgTriplet" << L">" << "x64-windows" << L"</" << L"VcpkgTriplet" << L">" << eline();

		_oss 
			<< ispace2(1) 
			<< L"</PropertyGroup>"
			<< eline();
	}
}

//===========================================================================
void VcFile_vcxproj::write_Import_Project_VCTargetsPath_Microsoft_Cpp_Default_props(void)
{
	/*
	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
	*/
	_oss 
		<< ispace2(1) 
		<< L"<Import"
		<< L" "
		<< L"Project=" << dquot(L"$(VCTargetsPath)\\Microsoft.Cpp.Default.props") 
		<< L" "
		<< L"/>"
		<< eline();
}

void VcFile_vcxproj::write_Import_Project_VCTargetsPath_Microsoft_Cpp_props(void)
{
	/*
	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
	*/
	_oss 
		<< ispace2(1) 
		<< L"<Import"
		<< L" "
		<< L"Project=" << dquot(L"$(VCTargetsPath)\\Microsoft.Cpp.props") 
		<< L" "
		<< L"/>"
		<< eline();
}

void VcFile_vcxproj::write_ImportGroup_ExtensionSettings(void)
{
	/*
	<ImportGroup Label="ExtensionSettings">
	</ImportGroup>
	*/
	_oss 
		<< ispace2(1) 
		<< L"<ImportGroup"
		<< L" "
		<< L"Label=" << dquot(L"ExtensionSettings") 
		//<< L" "
		<< L">"
		<< eline();
	_oss 
		<< ispace2(1) 
		<< L"</ImportGroup>"
		<< eline();
}

void VcFile_vcxproj::write_ImportGroup_Shared(void)
{
	/*
	<ImportGroup Label="Shared">
	</ImportGroup>
	*/
	_oss 
		<< ispace2(1) 
		<< L"<ImportGroup"
		<< L" "
		<< L"Label=" << dquot(L"Shared") 
		//<< L" "
		<< L">"
		<< eline();
	_oss 
		<< ispace2(1) 
		<< L"</ImportGroup>"
		<< eline();
}

void VcFile_vcxproj::write_PropertyGroup_UserMacros(void)
{
	/*
	<PropertyGroup Label="UserMacros" />
	*/
	_oss 
		<< ispace2(1) 
		<< L"<PropertyGroup"
		<< L" "
		<< L"Label=" << dquot(L"UserMacros") 
		<< L" "
		<< L"/>"
		<< eline();
}

void VcFile_vcxproj::write_Import_Project_VCTargetsPath_Microsoft_targets(void)
{
	/*
	<Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
	*/
	_oss 
		<< ispace2(1) 
		<< L"<Import"
		<< L" "
		<< L"Project=" << dquot(L"$(VCTargetsPath)\\Microsoft.Cpp.targets") 
		<< L" "
		<< L"/>"
		<< eline();
}

void VcFile_vcxproj::write_ImportGroup_ExtensionTargets(void)
{
	/*
	<ImportGroup Label="ExtensionTargets">
	</ImportGroup>
	*/
	_oss 
		<< ispace2(1) 
		<< L"<ImportGroup"
		<< L" "
		<< L"Label=" << dquot(L"ExtensionTargets") 
		//<< L" "
		<< L">"
		<< eline();
	_oss 
		<< ispace2(1) 
		<< L"</ImportGroup>"
		<< eline();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
