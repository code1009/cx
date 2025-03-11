#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcFile_sln
{
	class Project
	{
	public:
		std::wstring _Guid;
		VcFile_vcxproj* _vcxproj;
		std::wstring _FilePath;

	public:
		explicit Project(std::wstring guid, VcFile_vcxproj* vcxproj, std::wstring filePath);
	};

public:
	Generator* _Generator {nullptr};

public:
	std::wostringstream _oss;
	
public:
	std::vector<std::shared_ptr<Project>> _Projects;
	
	std::wstring _SolutionGuid;
	std::wstring _SolutionName;

public:
	explicit VcFile_sln(Generator* generator);

public:
	void addProject(std::wstring guid, VcFile_vcxproj* vcxproj, std::wstring filePath);

public:
	void write(void);

public:
	void write_head (void);
	void write_Project(void);
	void write_Global(void);
	void write_GlobalSection_SolutionConfigurationPlatforms(void);
	void write_GlobalSection_ProjectConfigurationPlatforms(void);
	void write_GlobalSection_SolutionProperties (void);
	void write_GlobalSection_NestedProjects (void);
	void write_GlobalSection_ExtensibilityGlobals (void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
