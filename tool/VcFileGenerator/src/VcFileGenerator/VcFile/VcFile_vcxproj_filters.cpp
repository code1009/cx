/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <xmllite.h>
#include <wil/resource.h>
#include <wil/com.h>

//===========================================================================
#include <runtime/runtime.hpp>
#include <component/fs_std_wstring.hpp>
#include <component/std_wstring_utility.hpp>
#include <component/xml.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
VcFile_vcxproj_filters::VcFile_vcxproj_filters(Generator* generator):
	_Generator(generator)
{
}

//===========================================================================
void VcFile_vcxproj_filters::write(void)
{
	write_xml();
	write_Project();
}

void VcFile_vcxproj_filters::write_xml(void)
{
	_oss 
		<< L"<?xml"
		<< L" "
		<< L"version="  << dquot(L"1.0"  ) 
		<< L" "
		<< L"encoding=" << dquot(L"utf-8") 
		//<< L" "
		<< L"?>" 
		<< eline();
}

void VcFile_vcxproj_filters::write_Project(void)
{
	_oss 
		<< L"<Project"
		<< L" "
		<< L"ToolsVersion=" << dquot(L"4.0" )
		<< L" "
		<< L"xmlns="        << dquot(L"http://schemas.microsoft.com/developer/msbuild/2003") 
		<< L" "
		<< L">" 
		<< eline();


	write_ItemGroup_Filter();
	write_ItemGroup_ItemType(L"ClCompile"      );
	write_ItemGroup_ItemType(L"ClInclude"      );
	write_ItemGroup_ItemType(L"ResourceCompile");
	write_ItemGroup_ItemType(L"Image"          );
	write_ItemGroup_ItemType(L"Text"           );
	write_ItemGroup_ItemType(L"Manifest"       );
	write_ItemGroup_ItemType(L"CustomBuild"    );
	write_ItemGroup_ItemType(L"None"           );
	write_ItemGroup_ItemType(L"");


	_oss 
		<< L"</Project>"
		<< eline();
}

void VcFile_vcxproj_filters::write_ItemGroup_Filter(void)
{
	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< L"<ItemGroup>"
		<< eline();


	//--------------------------------------------------------------------------
	for (auto& filter: _Generator->_VcItemData._Filters)
	{
		auto element = filter.second;

		_oss 
			<< ispace2(2) 
			<< L"<Filter"
			<< L" "
			<< L"Include=" << dquot(element->_Filter) 
			//<< L" "
			<< ">"
			<< eline();


		_oss 
			<< ispace2(3) 
			<< L"<UniqueIdentifier>"
			<< element->_Guid
			<< L"</UniqueIdentifier>"
			<< eline();

		if (!element->_FileExtensions.empty())
		{
			_oss 
				<< ispace2(3) 
				<< L"<Extensions>"
				<< element->_FileExtensions
				<< L"</Extensions>"
				<< eline();
		}


		_oss 
			<< ispace2(2) 
			<< L"</Filter>"
			<< eline();
	}


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< "</ItemGroup>"
		<< eline();
}

void VcFile_vcxproj_filters::write_ItemGroup_ItemType(std::wstring type)
{
	//--------------------------------------------------------------------------
	if (!_Generator->hasVcItemType(type))
	{
		return;
	}


	//--------------------------------------------------------------------------
	std::wstring typeName;
	if (type.empty())
	{
		typeName = L"None";
	}
	else
	{
		typeName = type;
	}


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< L"<ItemGroup>"
		<< eline();


	//--------------------------------------------------------------------------
	for (auto item : _Generator->_VcItemData._Items)
	{
		if (item->_Type == type)
		{
			if (item->_Filter.empty())
			{
				_oss 
					<< ispace2(2) 
					<< L"<" << typeName 
					<< L" "
					<< L"Include=" << dquot(item->_File) 
					//<< L" "
					<< L"/>"
					<< eline();
			}
			else
			{
				_oss 
					<< ispace2(2) 
					<< L"<" << typeName 
					<< L" "
					<< L"Include=" << dquot(item->_File) 
					//<< L" "
					<< L">"
					<< eline();

				_oss 
					<< ispace2(3) 
					<< L"<Filter>"
					<< item->_Filter
					<< L"</Filter>"
					<< eline();

				_oss 
					<< ispace2(2) 
					<< L"</" << typeName 
					<< L">"
					<< eline();
			}
		}
	}


	//--------------------------------------------------------------------------
	_oss 
		<< ispace2(1) 
		<< L"</ItemGroup>"
		<< eline();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
