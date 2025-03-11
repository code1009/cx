#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcItem
{
public:
	std::wstring _File;
	std::wstring _Filter;

public:
	std::wstring _Type;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcItemFilter
{
public:
	std::wstring _Filter;
	std::wstring _Guid;

public:
	std::wstring _FileExtensions;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcItemData
{
public:
	std::vector<std::shared_ptr<VcItem>> _Items;
	std::map<std::wstring, std::shared_ptr<VcItemFilter>> _Filters;
	std::map<std::wstring, std::wstring> _FileExtensionTypeMap;
	std::map<std::wstring, std::wstring> _FileNameTypeMap;
	std::map<std::wstring, std::wstring> _TypeFileExtensionsMap;

public:
	VcItemData();

public:
	void addItem(std::wstring file, std::wstring filter);
	bool hasFilter(std::wstring filter);
	void makeFilter(std::wstring filterPath);
	std::wstring getType(std::wstring filePath);
	void initializeFileExtensionTypeMap(void);
	void initializeFileNameTypeMap(void);
	void initializeTypeFileExtensionsMap(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool loadVcItemData(VcItemData& doc, std::wstring filePath);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
