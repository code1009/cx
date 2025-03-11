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
	VcTemplateData _VcTemplate;
	VcItemData _VcItemData;

public:
	Generator();

public:
	bool generate(Parameter& param);

public:
	bool loadTemplate(Parameter& param);
	bool saveVcFile_vcxproj_filters(Parameter& param);
	bool saveVcFile_vcxproj(Parameter& param);
	bool saveVcFile_sln(Parameter& param);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
