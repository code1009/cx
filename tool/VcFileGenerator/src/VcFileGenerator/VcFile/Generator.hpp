#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Generator
{
public:
	Parameter* _Parameter;

public:
	VcTemplateData _VcTemplate;
	VcItemData _VcItemData;

public:
	explicit Generator(Parameter* param);

public:
	bool generate();

public:
	bool loadVcTemplate(void);

public:
	bool saveVcFile(void);
	bool saveVcFile_vcxproj_filters(void);
	bool saveVcFile_vcxproj(void);
	bool saveVcFile_sln(void);

public:
	bool hasVcItemType(std::wstring type);
	bool isVcItem_ClCompile_PrecompiledHeader(std::shared_ptr<VcItem> vcItem);
	bool isVcItem_CustomBuild_Ribbon         (std::shared_ptr<VcItem> vcItem);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
