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
	std::vector<std::wstring> _ItemFiles;
	std::map<std::wstring, std::wstring> _ConfigurationFiles;

public:
	void setSettings(const Settings& settings);
	void addItemFile(const std::wstring& file);
	void addConfigurationFile(const std::wstring& name, const std::wstring& file);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool loadVcTemplateData(VcTemplateData& doc, std::wstring filePath);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
