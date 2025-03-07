#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Config
{
public:
	std::wstring _BL_FilePath   { L"" };
	std::wstring _SA_FilePath   { L"" };
	std::wstring _MA_FilePath   { L"" };
	std::wstring _Image_FilePath{ L"" };

public:
	Config() = default;

public:
	virtual ~Config() = default;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Config& getConfig(void);

//===========================================================================
void saveConfig(void);
void loadConfig(void);
