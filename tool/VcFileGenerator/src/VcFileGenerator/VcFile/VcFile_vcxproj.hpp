#pragma once





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

	void write_ItemGroup_Label_ProjectConfigurations (void);

	void write_ItemGroup_Item(void);
	void write_ItemGroup_ItemType(std::wstring type);
	void write_ItemGroup_ItemType_ClCompile_PrecompiledHeader(std::shared_ptr<VcItem> vcItem);
	void write_ItemGroup_ItemType_CustomBuild_Ribbon         (std::shared_ptr<VcItem> vcItem);

	void write_PropertyGroup_Label_Globals(void);
	void write_PropertyGroup_Label_Configuration(void);
	
	void wirte_ImportGroup_Label_PropertySheets(void);

	void wirte_ItemDefinitionGroup(void);

	void wirte_Import_Project_VCTargetsPath_Microsoft_Cpp_Default_props(void);
	void wirte_Import_Project_VCTargetsPath_Microsoft_Cpp_props(void);
	void wirte_ImportGroup_Lable_ExtensionSettings(void);
	void wirte_ImportGroup_Lable_Shared(void);
	void wirte_PropertyGroup_Label_UserMacros(void);
	void wirte_Import_Project_VCTargetsPath_Microsoft_targets(void);
	void wirte_ImportGroup_Lable_ExtensionTargets(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
