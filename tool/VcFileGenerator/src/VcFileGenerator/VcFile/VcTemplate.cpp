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
#include "Guid.hpp"
#include "VcItem.hpp"
#include "VcTemplate.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace VcFile
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void VcTemplateData::setSettings(const VcTemplateData::Settings& settings)
{
	_Settings = settings;
}

void VcTemplateData::addItemFile(const std::wstring& file)
{
	_ItemFiles.push_back(file);
}

void VcTemplateData::addConfigurationFile(const std::wstring& name, const std::wstring& file)
{
	_ConfigurationFiles[name] = file;
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcTemplate_XML_IO
{
public:
	VcTemplateData* _Document;
	cx::xml::xml_loader _Loader;

public:
	explicit VcTemplate_XML_IO(VcTemplateData* doc) :
		_Document{ doc },
		_Loader{ L"VcTemplate" }
	{
		_Loader.get_xml_context()->set_read_xml_root_tag_handler(
			std::bind(&VcTemplate_XML_IO::readTag_Root, this, std::placeholders::_1)
		);
	}

	//-----------------------------------------------------------------------
public:
	bool loadFile(void)
	{
		std::wstring fileName = L"VcTemplate.xml";
		std::wstring filePath;

		filePath = cx::wfs::get_directory_of_current_process() + fileName;

		return _Loader.load_xml_file(filePath);
	}

	//-----------------------------------------------------------------------
public:
	bool readTag_Root(cx::xml::xml_reader_t& reader)
	{
		cx::xml::xml_read_handler_map_t readHandlerMap;


		readHandlerMap[L"Settings"] = std::bind(&VcTemplate_XML_IO::readTag_Settings, this, std::placeholders::_1);
		readHandlerMap[L"ItemFiles"] = std::bind(&VcTemplate_XML_IO::readTag_ItemFiles, this, std::placeholders::_1);
		readHandlerMap[L"ConfigurationFiles"] = std::bind(&VcTemplate_XML_IO::readTag_ConfigurationFiles, this, std::placeholders::_1);

		return cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);
	}

public:
	bool readTag_Settings(cx::xml::xml_reader_t& reader)
	{
		VcTemplateData::Settings settings;


		bool rv;
		std::wstring v;

		rv = cx::xml::read_xml_attr_wstring(reader, L"SourceDirectory", v);
		if (true != rv)
		{
			return false;
		}
		settings.SourceDirectory = v;


		rv = cx::xml::read_xml_attr_wstring(reader, L"ClCompile_PrecompiledHeader", v);
		if (true != rv)
		{
			//return false;
		}
		settings.ClCompile_PrecompiledHeader = v;


		_Document->setSettings(settings);

		return true;
	}

public:
	bool readTag_ItemFiles(cx::xml::xml_reader_t& reader)
	{
		cx::xml::xml_read_handler_map_t readHandlerMap;


		readHandlerMap[L"Item"] = std::bind(&VcTemplate_XML_IO::readTag_ItemFiles_Item, this, std::placeholders::_1);

		return cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);
	}
	bool readTag_ItemFiles_Item(cx::xml::xml_reader_t& reader)
	{
		std::wstring file;


		bool rv;
		std::wstring v;

		rv = cx::xml::read_xml_attr_wstring(reader, L"File", file);
		if (true != rv)
		{
			return false;
		}

		_Document->addItemFile(file);

		return true;
	}

public:
	bool readTag_ConfigurationFiles(cx::xml::xml_reader_t& reader)
	{
		cx::xml::xml_read_handler_map_t readHandlerMap;


		readHandlerMap[L"Item"] = std::bind(&VcTemplate_XML_IO::readTag_ConfigurationFiles_Item, this, std::placeholders::_1);

		return cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);
	}
	bool readTag_ConfigurationFiles_Item(cx::xml::xml_reader_t& reader)
	{
		std::wstring name;
		std::wstring file;


		bool rv;
		std::wstring v;

		rv = cx::xml::read_xml_attr_wstring(reader, L"Name", name);
		if (true != rv)
		{
			return false;
		}

		rv = cx::xml::read_xml_attr_wstring(reader, L"File", file);
		if (true != rv)
		{
			return false;
		}

		_Document->addConfigurationFile(name, file);

		return true;
	}
};




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
