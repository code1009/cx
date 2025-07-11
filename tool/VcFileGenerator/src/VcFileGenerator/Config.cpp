/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <xmllite.h>
#include <wil/resource.h>
#include <wil/com.h>

//===========================================================================
#include <wui/wui.hpp>
#include <runtime/runtime.hpp>
#include <common/fs_std_wstring.hpp>
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
		_Loader.get_xml_context()->set_xml_root_tag_read_handler(
			std::bind(&Config_XML_IO::readTag_Root, this, std::placeholders::_1)
		);

		_Saver.get_xml_context()->set_xml_root_tag_write_handler(
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
	bool readTag_Root(cx::xml::xml_reader& reader)
	{
		cx::xml::xml_read_handler_map readHandlerMap;


		readHandlerMap[L"Template"] = std::bind(&Config_XML_IO::readTag_Template, this, std::placeholders::_1);
		readHandlerMap[L"Project"] = std::bind(&Config_XML_IO::readTag_Project, this, std::placeholders::_1);
		readHandlerMap[L"Solution"] = std::bind(&Config_XML_IO::readTag_Solution, this, std::placeholders::_1);

		return cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);
	}

public:
	bool readTag_Template(cx::xml::xml_reader& reader)
	{
		bool rv;

		std::wstring v;
		rv = cx::xml::read_xml_attr_wstring(reader, L"FilePath", _Document->_TemplateFilePath);
		if (true != rv)
		{
			return false;
		}

		return true;
	}

	bool readTag_Project(cx::xml::xml_reader& reader)
	{
		bool rv;

		std::wstring v;
		rv = cx::xml::read_xml_attr_wstring(reader, L"Name", _Document->_ProjectName);
		if (true != rv)
		{
			return false;
		}

		return true;
	}

	bool readTag_Solution(cx::xml::xml_reader& reader)
	{
		bool rv;

		std::wstring v;
		rv = cx::xml::read_xml_attr_wstring(reader, L"Name", _Document->_SolutionName);
		if (true != rv)
		{
			return false;
		}

		return true;
	}

	//-----------------------------------------------------------------------
public:
	bool writeTag_Root(cx::xml::xml_writer& writer)
	{
		bool rv;


		rv = cx::xml::write_xml_tag_open(writer, _Saver.get_xml_context()->get_xml_root_tag_name());
		if (true != rv)
		{
			return false;
		}


		rv = writeTag_Template(writer);
		if (true != rv)
		{
			return false;
		}
		rv = writeTag_Solution(writer);
		if (true != rv)
		{
			return false;
		}
		rv = writeTag_Project(writer);
		if (true != rv)
		{
			return false;
		}


		rv = cx::xml::write_xml_tag_close(writer);
		if (true != rv)
		{
			return false;
		}

		return true;
	}

public:
	bool writeTag_Template(cx::xml::xml_writer& writer)
	{
		bool rv;


		rv = cx::xml::write_xml_tag_open(writer, L"Template", 1);
		if (true != rv)
		{
			return false;
		}


		if (!_Document->_TemplateFilePath.empty())
		{
			rv = cx::xml::write_xml_attr_wstring(writer, L"FilePath", _Document->_TemplateFilePath);
			if (true != rv)
			{
				return false;
			}
		}


		// single xml tag로 설정시에는 write_xml_tag_close()의 tab의 개수를 0으로 설정합니다.
		rv = cx::xml::write_xml_tag_close(writer, 0);
		if (true != rv)
		{
			return false;
		}

		return true;
	}

	bool writeTag_Project(cx::xml::xml_writer& writer)
	{
		bool rv;


		rv = cx::xml::write_xml_tag_open(writer, L"Project", 1);
		if (true != rv)
		{
			return false;
		}


		if (!_Document->_ProjectName.empty())
		{
			rv = cx::xml::write_xml_attr_wstring(writer, L"Name", _Document->_ProjectName);
			if (true != rv)
			{
				return false;
			}
		}


		// single xml tag로 설정시에는 write_xml_tag_close()의 tab의 개수를 0으로 설정합니다.
		rv = cx::xml::write_xml_tag_close(writer, 0);
		if (true != rv)
		{
			return false;
		}

		return true;
	}

	bool writeTag_Solution(cx::xml::xml_writer& writer)
	{
		bool rv;


		rv = cx::xml::write_xml_tag_open(writer, L"Solution", 1);
		if (true != rv)
		{
			return false;
		}


		if (!_Document->_SolutionName.empty())
		{
			rv = cx::xml::write_xml_attr_wstring(writer, L"Name", _Document->_SolutionName);
			if (true != rv)
			{
				return false;
			}
		}


		// single xml tag로 설정시에는 write_xml_tag_close()의 tab의 개수를 0으로 설정합니다.
		rv = cx::xml::write_xml_tag_close(writer, 0);
		if (true != rv)
		{
			return false;
		}

		return true;
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static Config* _config = nullptr;

//===========================================================================
Config& getConfig(void)
{
	//static Config instance;
	//return instance;
	if (nullptr == _config)
	{
		_config = cpp_new Config();
	}

	return *_config;
}

void deleteConfig(void)
{
	if (nullptr != _config)
	{
		cpp_delete _config;
		_config = nullptr;
	}
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
