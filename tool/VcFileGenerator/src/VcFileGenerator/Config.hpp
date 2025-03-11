#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Config
{
public:
	std::wstring _TemplateFilePath { L"" };
	std::wstring _SolutionName { L"" };
	std::wstring _ProjectName  { L"" };

public:
	Config() = default;

public:
	virtual ~Config() = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Config& getConfig(void);
void deleteConfig(void);

//===========================================================================
void saveConfig(void);
void loadConfig(void);
