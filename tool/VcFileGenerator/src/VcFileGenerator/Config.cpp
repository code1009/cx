﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <xmllite.h>
#include <wil/resource.h>
#include <wil/com.h>

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>
#include <component/fs_std_wstring.hpp>
#include <component/xml.hpp>

//===========================================================================
#include "Config.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Config_XML_IO
{
public:
	Config*             _Document;
	cx::xml::xml_loader _Loader;
	cx::xml::xml_saver  _Saver;

public:
	explicit Config_XML_IO(Config* doc) :
		_Document{ doc },
		_Loader  { L"Config" },
		_Saver   { L"Config" }
	{
		_Loader.get_xml_context()->set_read_xml_root_tag_handler(
			std::bind(&Config_XML_IO::readTag_Root, this, std::placeholders::_1)
		);

		_Saver.get_xml_context()->set_write_xml_root_tag_handler(
			std::bind(&Config_XML_IO::writeTag_Root, this, std::placeholders::_1)
		);
	}

	//-----------------------------------------------------------------------
public:
	bool saveFile(void)
	{
		std::wstring fileName = L"VcFileGenerator.xml";
		std::wstring filePath;

		filePath = cx::wfs::get_directory_of_current_process() + fileName;

		return _Saver.save_xml_file(filePath);
	}

	bool loadFile(void)
	{
		std::wstring fileName = L"VcFileGenerator.xml";
		std::wstring filePath;

		filePath = cx::wfs::get_directory_of_current_process() + fileName;

		return _Loader.load_xml_file(filePath);
	}

	//-----------------------------------------------------------------------
public:
	bool readTag_Root(cx::xml::xml_reader_t& reader)
	{
		cx::xml::xml_read_handler_map_t readHandlerMap;


		readHandlerMap[L"Source"] = std::bind(&Config_XML_IO::readTag_Source, this, std::placeholders::_1);

		return cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);
	}

public:
	bool readTag_Source(cx::xml::xml_reader_t& reader)
	{
		bool rv;

		std::wstring v;
		rv = cx::xml::read_xml_attr_wstring(reader, L"TemplateFilePath", _Document->_TemplateFilePath);
		if (true != rv)
		{
			return false;
		}

		return true;
	}

	//-----------------------------------------------------------------------
public:
	bool writeTag_Root(cx::xml::xml_writer_t& writer)
	{
		bool rv;


		rv = cx::xml::write_xml_parent_tag_start(writer, _Saver.get_xml_context()->get_xml_root_tag_name());
		if (true != rv)
		{
			return false;
		}


		rv = writeTag_Source(writer);
		if (true != rv)
		{
			return false;
		}


		rv = cx::xml::write_xml_parent_tag_end(writer);
		if (true != rv)
		{
			return false;
		}

		return true;
	}

public:
	bool writeTag_Source(cx::xml::xml_writer_t& writer)
	{
		bool rv;


		rv = cx::xml::write_xml_single_tag_start(writer, L"Source", 1);
		if (true != rv)
		{
			return false;
		}


		if (!_Document->_TemplateFilePath.empty())
		{
			rv = cx::xml::write_xml_attr_wstring(writer, L"TemplateFilePath", _Document->_TemplateFilePath);
			if (true != rv)
			{
				return false;
			}
		}


		rv = cx::xml::write_xml_single_tag_end(writer);
		if (true != rv)
		{
			return false;
		}

		return true;
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Config& getConfig(void)
{
	static Config instance;


	return instance;
}

//===========================================================================
void saveConfig(void)
{
	bool rv;


	Config& config = getConfig();


	Config_XML_IO xml_io(&config);
	rv = xml_io.saveFile();
	if (true != rv)
	{
		MessageBoxW(NULL, L"Failed to save VcFileGenerator.xml", L"Error", MB_OK);
	}
}

void loadConfig(void)
{
	bool rv;


	Config& config = getConfig();


	Config_XML_IO xml_io(&config);
	rv = xml_io.loadFile();
	if (true != rv)
	{
		MessageBoxW(NULL, L"Failed to load VcFileGenerator.xml", L"Error", MB_OK);
		getConfig() = Config();
		saveConfig();
	}
}
