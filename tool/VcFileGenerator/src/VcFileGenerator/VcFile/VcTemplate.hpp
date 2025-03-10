#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcTemplateData
{
public:
	struct Settings
	{
		std::wstring SourceDirectory;
		std::wstring ClCompile_PrecompiledHeader;
	};

public:
	Settings _Settings;
	std::vector<std::wstring> _VcItemFiles;
	std::map<std::wstring, std::wstring> _VcConfigurationFiles;

public:
	void setSettings(const Settings& settings);
	void addVcItemFile(const std::wstring& file);
	void addVcConfigurationFile(const std::wstring& name, const std::wstring& file);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
