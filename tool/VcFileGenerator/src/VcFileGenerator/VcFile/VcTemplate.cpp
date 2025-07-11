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

void VcTemplateData::addEnvironmentVariable(const std::wstring& name, const std::wstring& value)
{
	_EnvironmentVariables[name] = value;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class VcTemplate_XML_IO
{
protected:
	VcTemplateData* _Document;
	cx::xml::xml_loader _Loader;

public:
	explicit VcTemplate_XML_IO(VcTemplateData* doc) :
		_Document{ doc },
		_Loader{ L"VcTemplate" }
	{
		_Loader.get_xml_context()->set_xml_root_tag_read_handler(
			std::bind(&VcTemplate_XML_IO::readTag_Root, this, std::placeholders::_1, std::placeholders::_2)
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
	bool readTag_Root(cx::xml::xml_reader& reader, void* /*param*/)
	{
		bool rv;

		cx::xml::xml_read_handler_map readHandlerMap;
		readHandlerMap[L"Settings"] = std::bind(&VcTemplate_XML_IO::readTag_Settings, this, std::placeholders::_1, std::placeholders::_2);
		readHandlerMap[L"ItemFiles"] = std::bind(&VcTemplate_XML_IO::readTag_ItemFiles, this, std::placeholders::_1, std::placeholders::_2);
		readHandlerMap[L"ConfigurationFiles"] = std::bind(&VcTemplate_XML_IO::readTag_ConfigurationFiles, this, std::placeholders::_1, std::placeholders::_2);
		readHandlerMap[L"EnvironmentVariables"] = std::bind(&VcTemplate_XML_IO::readTag_EnvironmentVariables, this, std::placeholders::_1, std::placeholders::_2);
		
		rv = cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);

		return rv;
	}

public:
	bool readTag_Settings(cx::xml::xml_reader& reader, void* /*param*/)
	{
		VcTemplateData::Settings settings;


		bool rv;
		std::wstring v;

		v.clear();
		rv = cx::xml::read_xml_attr_wstring(reader, L"SourceDirectory", v);
		if (true != rv)
		{
			return false;
		}
		settings.SourceDirectory = v;


		v.clear();
		rv = cx::xml::read_xml_attr_wstring(reader, L"ClCompile_PrecompiledHeader", v);
		if (true != rv)
		{
			//return false;
		}
		settings.ClCompile_PrecompiledHeader = v;


		v.clear();
		rv = cx::xml::read_xml_attr_wstring(reader, L"cxDirectory", v);
		if (true != rv)
		{
			//return false;
		}
		settings.cxDirectory = v;


		_Document->setSettings(settings);

		return true;
	}

public:
	bool readTag_ItemFiles(cx::xml::xml_reader& reader, void* /*param*/)
	{
		bool rv;

		cx::xml::xml_read_handler_map readHandlerMap;
		readHandlerMap[L"Item"] = std::bind(&VcTemplate_XML_IO::readTag_ItemFiles_Item, this, std::placeholders::_1, std::placeholders::_2);
		rv = cx::xml::read_xml_child_tag(
			reader,
			L"ItemFiles",
			readHandlerMap
		);

		return rv;
	}
	bool readTag_ItemFiles_Item(cx::xml::xml_reader& reader, void* /*param*/)
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
	bool readTag_ConfigurationFiles(cx::xml::xml_reader& reader, void* /*param*/)
	{
		bool rv;

		cx::xml::xml_read_handler_map readHandlerMap;
		readHandlerMap[L"Item"] = std::bind(&VcTemplate_XML_IO::readTag_ConfigurationFiles_Item, this, std::placeholders::_1, std::placeholders::_2);
		rv = cx::xml::read_xml_child_tag(
			reader,
			L"ConfigurationFiles",
			readHandlerMap
		);

		return rv;
	}
	bool readTag_ConfigurationFiles_Item(cx::xml::xml_reader& reader, void* /*param*/)
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

public:
	bool readTag_EnvironmentVariables(cx::xml::xml_reader& reader, void* /*param*/)
	{
		bool rv;

		cx::xml::xml_read_handler_map readHandlerMap;
		readHandlerMap[L"Item"] = std::bind(&VcTemplate_XML_IO::readTag_EnvironmentVariables_Item, this, std::placeholders::_1, std::placeholders::_2);
		rv = cx::xml::read_xml_child_tag(
			reader,
			L"EnvironmentVariables",
			readHandlerMap
		);

		return rv;
	}
	bool readTag_EnvironmentVariables_Item(cx::xml::xml_reader& reader, void* /*param*/)
	{
		std::wstring name;
		std::wstring value;


		bool rv;
		std::wstring v;

		rv = cx::xml::read_xml_attr_wstring(reader, L"Name", name);
		if (true != rv)
		{
			return false;
		}

		rv = cx::xml::read_xml_attr_wstring(reader, L"Value", value);
		if (true != rv)
		{
			return false;
		}

		_Document->addEnvironmentVariable(name, value);
		return true;
	}
};

bool loadVcTemplateData(VcTemplateData& doc, std::wstring filePath)
{
	VcTemplate_XML_IO xml_io{ &doc };
	return xml_io.loadFile(filePath);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
