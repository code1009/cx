﻿#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcFile_vcxproj
{
public:
	class ProjectConfiguration
	{
	public:
		std::wstring _Configuration;
		std::wstring _Platform;

	public:
		std::wstring _Include;
		std::wstring _ConfigurationPlatform;

	public:
		ProjectConfiguration(std::wstring configuration, std::wstring platform) :
			_Configuration(configuration),
			_Platform(platform)
		{
			_Include = _Configuration + L"|" + _Platform;
			_ConfigurationPlatform = _Configuration + L"|" + _Platform;
		}
	};

public:
	Generator* _Generator {nullptr};

public:
	std::wostringstream _oss;

public:
	std::vector<ProjectConfiguration> _ProjectConfigurations;
	std::wstring _ProjectGuid;
	std::wstring _ProjectName;

public:
	explicit VcFile_vcxproj(Generator* generator);

public:
	void write(void);

	void write_xml (void);

	void write_Project (void);

	void write_ItemGroup_ProjectConfigurations (void);

	void write_ItemGroup_Item(void);
	void write_ItemGroup_ItemType(std::wstring type);
	void write_ItemGroup_ItemType_ClCompile_PrecompiledHeader(std::shared_ptr<VcItem> vcItem);
	void write_ItemGroup_ItemType_CustomBuild_Ribbon         (std::shared_ptr<VcItem> vcItem);

	void write_PropertyGroup_Globals(void);
	void write_PropertyGroup_Configuration(void);
	
	void write_PropertyGroup_EnvironmentVariables(void);

	void write_ImportGroup_PropertySheets(void);

	void write_ItemDefinitionGroup(void);

	void write_Import_Project_VCTargetsPath_Microsoft_Cpp_Default_props(void);
	void write_Import_Project_VCTargetsPath_Microsoft_Cpp_props(void);
	void write_ImportGroup_ExtensionSettings(void);
	void write_ImportGroup_Shared(void);
	void write_PropertyGroup_UserMacros(void);
	void write_PropertyGroup_VcPkg(void);
	void write_Import_Project_VCTargetsPath_Microsoft_targets(void);
	void write_ImportGroup_ExtensionTargets(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
