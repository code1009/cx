/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <xmllite.h>
#include <wil/resource.h>
#include <wil/com.h>

//===========================================================================
#include <runtime/runtime.hpp>
#include <common/fs_std_wstring.hpp>
#include <common/std_wstring_utility.hpp>
#include <component/xml.hpp>

//===========================================================================
#include "VcFile.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
VcItemData::VcItemData()
{
	initializeTypeFileExtensionsMap();
	initializeFileExtensionTypeMap();
	initializeFileNameTypeMap();
}

//===========================================================================
void VcItemData::addItem(std::wstring file, std::wstring filter)
{
	std::shared_ptr<VcItem> item = std::make_shared<VcItem>();
	item->_File = file;
	item->_Filter = filter;
	item->_Type = getType(file);
	_Items.push_back(item);

	if (!filter.empty())
	{
		if (!hasFilter(filter))
		{
			makeFilter(filter);
		}
	}
}

bool VcItemData::hasFilter(std::wstring filter)
{
	auto found = _Filters.find(filter);
	if (found == _Filters.end())
	{
		return false;
	}

	return true;
}

void VcItemData::makeFilter(std::wstring filterPath)
{
	if (filterPath.empty())
	{
		return;
	}


	std::size_t pos = filterPath.rfind(L"\\");
	if (std::wstring::npos != pos)
	{
		std::wstring parentFilterPath;
		parentFilterPath = filterPath.substr(0, pos);
		if (!hasFilter(parentFilterPath))
		{
			makeFilter(parentFilterPath);
		}
	}


	std::shared_ptr<VcItemFilter> filter;
	filter= std::make_shared<VcItemFilter>();
	filter->_Filter = filterPath;
	filter->_Guid = makeGuid();
	_Filters[filterPath] = filter;
}

std::wstring VcItemData::getType(std::wstring filePath)
{
	std::wstring type;
	type = L"None"; // default


	std::wstring fileName;
	fileName = cx::wfs::get_file_name_of_file_path(filePath);
	if (fileName.empty())
	{
		return type;
	}

	std::map<std::wstring, std::wstring>::iterator foundFileName = _FileNameTypeMap.find(fileName);
	if (foundFileName != _FileNameTypeMap.end())
	{
		type = foundFileName->second;
		return type;
	}


	std::wstring fileExtension;
	fileExtension = cx::wfs::get_file_extension_of_file_path(filePath);
	if (fileExtension.empty())
	{
		return type;
	}

	std::map<std::wstring, std::wstring>::iterator foundFileExtension  = _FileExtensionTypeMap.find(fileExtension);
	if (foundFileExtension != _FileExtensionTypeMap.end())
	{
		type = foundFileExtension->second;
		return type;
	}


	return type;
}

void VcItemData::initializeFileNameTypeMap(void)
{
	_FileNameTypeMap[L"ribbon.xml"] = L"CustomBuild";
}

void VcItemData::initializeFileExtensionTypeMap(void)
{
	_FileExtensionTypeMap.clear();

	for (auto& typeExt : _TypeFileExtensionsMap)
	{
		std::vector<std::wstring> exts;
		exts = cx::tokenize_object_std_wstring(typeExt.second, L";", false);
		for (auto& ext : exts)
		{
			_FileExtensionTypeMap[ext] = typeExt.first;
		}
	}
}

void VcItemData::initializeTypeFileExtensionsMap(void)
{
	_TypeFileExtensionsMap.clear();

	_TypeFileExtensionsMap[L"ClCompile"      ] = L".cpp;.c;.cc;.cxx;";
	_TypeFileExtensionsMap[L"ClInclude"      ] = L".h;.hh;.hpp;.hxx;.inl;.inc";
	_TypeFileExtensionsMap[L"ResourceCompile"] = L".rc";
	_TypeFileExtensionsMap[L"Image"          ] = L".ico;.cur;.bmp;.gif;.jpg;.jpeg;.png;.tiff;.tif";
	_TypeFileExtensionsMap[L"Manifest"       ] = L".manifest";
	_TypeFileExtensionsMap[L"Text"           ] = L".txt;.csv;.md";
	_TypeFileExtensionsMap[L"None"           ] = L".cd";
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcItem_XML_IO
{
protected:
	VcItemData* _Document;
	cx::xml::xml_loader _Loader;

public:
	explicit VcItem_XML_IO(VcItemData* doc) :
		_Document{ doc },
		_Loader{ L"VcItem" }
	{
		_Loader.get_xml_context()->set_xml_root_tag_read_handler(
			std::bind(&VcItem_XML_IO::readTag_Root, this, std::placeholders::_1)
		);
	}

	//-----------------------------------------------------------------------
public:
	bool loadFile(std::wstring filePath)
	{
		return _Loader.load_xml_file(filePath);
	}

	//-----------------------------------------------------------------------
public:
	bool readTag_Root(cx::xml::xml_reader& reader)
	{
		cx::xml::xml_read_handler_map readHandlerMap;


		readHandlerMap[L"Item"] = std::bind(&VcItem_XML_IO::readTag_Item, this, std::placeholders::_1);

		return cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);
	}

public:
	bool readTag_Item(cx::xml::xml_reader& reader)
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
bool loadVcItemData(VcItemData& doc, std::wstring filePath)
{
	VcItem_XML_IO xml_io{ &doc };
	return xml_io.loadFile(filePath);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}

