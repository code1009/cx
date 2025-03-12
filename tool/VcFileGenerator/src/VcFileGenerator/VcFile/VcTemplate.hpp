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
		std::wstring cxDirectory;
		std::wstring SourceDirectory;
		std::wstring ClCompile_PrecompiledHeader;
	};

public:
	Settings _Settings;
	std::vector<std::wstring> _ItemFiles;
	std::map<std::wstring, std::wstring> _ConfigurationFiles;
	std::map<std::wstring, std::wstring> _EnvironmentVariables;

public:
	void setSettings(const Settings& settings);
	void addItemFile(const std::wstring& file);
	void addConfigurationFile(const std::wstring& name, const std::wstring& file);
	void addEnvironmentVariable(const std::wstring& name, const std::wstring& value);
};

 



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool loadVcTemplateData(VcTemplateData& doc, std::wstring filePath);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
