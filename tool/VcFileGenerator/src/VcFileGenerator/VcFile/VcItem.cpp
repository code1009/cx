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
#include "VcItem.hpp"
#include "Guid.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
VcItemData::VcItemData()
{
	initializeTypeFileExtensionsMap();
	initializeFileExtensionsTypeMap();
}

//===========================================================================
void VcItemData::addItem(std::wstring file, std::wstring filter)
{
	std::shared_ptr<VcItem> item = std::make_shared<VcItem>();
	item->_File = file;
	item->_Filter = filter;
	item->_Type = getType(file);


	if (!filter.empty())
	{
		auto found = _Filters.find(filter);
		if (found == _Filters.end())
		{
			auto vcItemFilter = std::make_shared<VcItemFilter>();

			vcItemFilter->_Filter = filter;
			vcItemFilter->_Guid = makeGuid();

			_Filters[filter] = vcItemFilter;
		}
	}
}

void VcItemData::makeFilters(void)
{
	_Filters.clear();

	for (auto& item : _Items)
	{
		_Filters[item->_Filter] = nullptr;
	}

	std::shared_ptr<VcItemFilter> filter;
	for (auto& filter: _Filters)
	{
		filter.second = std::make_shared<VcItemFilter>();

		filter.second->_Filter = filter.first;
		filter.second->_Guid = makeGuid();
	}
}

std::wstring VcItemData::getType(std::wstring filePath)
{
	std::wstring type;
	type = L"None"; // default


	std::wstring fileExtension;
	fileExtension = cx::wfs::get_file_extension_of_file_path(filePath);
	if (fileExtension.empty())
	{
		return type;
	}

	auto it = _FileExtensionsTypeMap.find(fileExtension);
	if (it != _FileExtensionsTypeMap.end())
	{
		type = it->second;
	}


	return type;
}

void VcItemData::initializeFileExtensionsTypeMap(void)
{
	_FileExtensionsTypeMap.clear();

	for (auto& typeExt : _TypeFileExtensionsMap)
	{
		std::vector<std::wstring> exts;
		exts = cx::tokenizeObject_std_wstring(typeExt.second, L";", false);
		for (auto& ext : exts)
		{
			_FileExtensionsTypeMap[ext] = typeExt.first;
		}
	}
}

void VcItemData::initializeTypeFileExtensionsMap(void)
{
	_TypeFileExtensionsMap.clear();

	_TypeFileExtensionsMap[L"ClCompile"] = L".cpp;.c;.cc;.cxx;";
	_TypeFileExtensionsMap[L"ClInclude"] = L".h;.hh;.hpp;.hxx;.inl;.inc";
	_TypeFileExtensionsMap[L"ResourceCompile"] = L".rc";
	_TypeFileExtensionsMap[L"Image"] = L".ico;.cur;.bmp;.gif;.jpg;.jpeg;.png;.tiff;.tif";
	_TypeFileExtensionsMap[L"Manifest"] = L".manifest";
	_TypeFileExtensionsMap[L"None"] = L".cd";
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcItem_XML_IO
{
public:
	VcItemData* _Document;
	cx::xml::xml_loader _Loader;

public:
	explicit VcItem_XML_IO(VcItemData* doc) :
		_Document{ doc },
		_Loader{ L"VcItem" }
	{
		_Loader.get_xml_context()->set_read_xml_root_tag_handler(
			std::bind(&VcItem_XML_IO::readTag_Root, this, std::placeholders::_1)
		);
	}

	//-----------------------------------------------------------------------
public:
	bool loadFile(void)
	{
		std::wstring fileName = L"VcItem.xml";
		std::wstring filePath;

		filePath = cx::wfs::get_directory_of_current_process() + fileName;

		return _Loader.load_xml_file(filePath);
	}

	//-----------------------------------------------------------------------
public:
	bool readTag_Root(cx::xml::xml_reader_t& reader)
	{
		cx::xml::xml_read_handler_map_t readHandlerMap;


		readHandlerMap[L"Item"] = std::bind(&VcItem_XML_IO::readTag_Item, this, std::placeholders::_1);

		return cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);
	}

public:
	bool readTag_Item(cx::xml::xml_reader_t& reader)
	{
		std::wstring file;
		std::wstring filter;


		bool rv;
		std::wstring v;

		rv = cx::xml::read_xml_attr_wstring(reader, L"File", file);
		if (true != rv)
		{
			return false;
		}

		rv = cx::xml::read_xml_attr_wstring(reader, L"Filter", filter);
		if (true != rv)
		{
			return false;
		}

		_Document->addItem(file, filter);

		return true;
	}
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
