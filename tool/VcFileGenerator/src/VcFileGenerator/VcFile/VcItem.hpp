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
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcItemData
{
public:
	std::vector<std::shared_ptr<VcItem>> _Items;
	std::map<std::wstring, std::shared_ptr<VcItemFilter>> _Filters;
	std::map<std::wstring, std::wstring> _FileExtensionsTypeMap;
	std::map<std::wstring, std::wstring> _TypeFileExtensionsMap;

public:
	VcItemData();

public:
	void addItem(std::wstring file, std::wstring filter);
	void makeFilters(void);
	std::wstring getType(std::wstring filePath);
	void initializeFileExtensionsTypeMap(void);
	void initializeTypeFileExtensionsMap(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
