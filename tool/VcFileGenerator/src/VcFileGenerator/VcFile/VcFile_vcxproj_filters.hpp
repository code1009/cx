#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Generator;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcFile_vcxproj_filters
{
public:
	Generator* _Generator {nullptr};

public:
	std::wostringstream _oss;

public:
	explicit VcFile_vcxproj_filters(Generator* generator);

public:
	void write(void);

	void write_xml(void);
	void write_Project(void);
	void write_ItemGroup_Filter(void);
	void write_ItemGroup_None(void);
	void write_ItemGroup_Type(std::wstring type);

public:
	bool hasType(std::wstring type);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
