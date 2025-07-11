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
		std::wstring fileName = L"Config.xml";
		std::wstring filePath;

		filePath = cx::wfs::get_directory_of_current_process() + fileName;

		return _Saver.save_xml_file(filePath);
	}

	bool loadFile(void)
	{
		std::wstring fileName = L"Config.xml";
		std::wstring filePath;

		filePath = cx::wfs::get_directory_of_current_process() + fileName;

		return _Loader.load_xml_file(filePath);
	}

	//-----------------------------------------------------------------------
public:
	bool readTag_Root(cx::xml::xml_reader& reader)
	{
		cx::xml::xml_read_handler_map readHandlerMap;


		readHandlerMap[L"bl"] = std::bind(&Config_XML_IO::readTag_bl, this, std::placeholders::_1);
		readHandlerMap[L"sa"] = std::bind(&Config_XML_IO::readTag_sa, this, std::placeholders::_1);
		readHandlerMap[L"ma"] = std::bind(&Config_XML_IO::readTag_ma, this, std::placeholders::_1);
		readHandlerMap[L"image"] = std::bind(&Config_XML_IO::readTag_image, this, std::placeholders::_1);

		return cx::xml::read_xml_child_tag(
			reader,
			_Loader.get_xml_context()->get_xml_root_tag_name(),
			readHandlerMap
		);
	}

public:
	bool readTag_bl(cx::xml::xml_reader& reader)
	{
		return readTag_FilePath(reader, _Document->_BL_FilePath);
	}
	bool readTag_sa(cx::xml::xml_reader& reader)
	{
		return readTag_FilePath(reader, _Document->_SA_FilePath);
	}
	bool readTag_ma(cx::xml::xml_reader& reader)
	{
		return readTag_FilePath(reader, _Document->_MA_FilePath);
	}
	bool readTag_image(cx::xml::xml_reader& reader)
	{
		return readTag_FilePath(reader, _Document->_Image_FilePath);
	}
	bool readTag_FilePath(cx::xml::xml_reader& reader, std::wstring& filePath)
	{
		bool rv;

		std::wstring v;
		rv = cx::xml::read_xml_attr_wstring(reader, L"FilePath", v);
		if (true != rv)
		{
			return false;
		}
		filePath = v;

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


		rv = writeTag_bl(writer);
		if (true != rv)
		{
			return false;
		}
		rv = writeTag_sa(writer);
		if (true != rv)
		{
			return false;
		}
		rv = writeTag_ma(writer);
		if (true != rv)
		{
			return false;
		}
		rv = writeTag_image(writer);
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
	bool writeTag_bl(cx::xml::xml_writer& writer)
	{
		return writeTag_FilePath(writer, L"bl", _Document->_BL_FilePath);
	}
	bool writeTag_sa(cx::xml::xml_writer& writer)
	{
		return writeTag_FilePath(writer, L"sa", _Document->_SA_FilePath);
	}
	bool writeTag_ma(cx::xml::xml_writer& writer)
	{
		return writeTag_FilePath(writer, L"ma", _Document->_MA_FilePath);
	}
	bool writeTag_image(cx::xml::xml_writer& writer)
	{
		return writeTag_FilePath(writer, L"image", _Document->_Image_FilePath);
	}
	bool writeTag_FilePath(cx::xml::xml_writer& writer, const wchar_t* tagName, const std::wstring& filePath)
	{
		bool rv;


		rv = cx::xml::write_xml_tag_open(writer, tagName, 1);
		if (true != rv)
		{
			return false;
		}


		if (!filePath.empty())
		{
			rv = cx::xml::write_xml_attr_wstring(writer, L"FilePath", filePath);
			if (true != rv)
			{
				return false;
			}
		}


		rv = cx::xml::write_xml_tag_close(writer, 1);
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
		MessageBoxW(NULL, L"Failed to save Config.xml", L"Error", MB_OK);
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
		MessageBoxW(NULL, L"Failed to load Config.xml", L"Error", MB_OK);
		getConfig() = Config();
		saveConfig();
	}
}
