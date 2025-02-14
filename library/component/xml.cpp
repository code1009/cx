/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <string>
#include <vector>
#include <map>

#include <functional>

//===========================================================================
#include <Windows.h>
#include <shlwapi.h>
#include <xmllite.h>

//===========================================================================
#include <wil/resource.h>
#include <wil/com.h>

//===========================================================================
#include "xml.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma comment(lib, "xmllite.lib")
#pragma comment(lib, "shlwapi.lib")





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::xml
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
xml_context::xml_context(std::wstring xml_root_tag_name) :
	_xml_root_tag_name{ xml_root_tag_name }
{
}

//===========================================================================
const wchar_t* xml_context::get_xml_root_tag_name(void)
{
	return _xml_root_tag_name.c_str();
}

//===========================================================================
void xml_context::reset_xml_error(void)
{
	_xml_error_line = false;
	_xml_error_line_number = 0;
	_xml_error_line_position = 0;
}

void xml_context::set_xml_error_line(std::size_t line_number, std::size_t line_position)
{
	_xml_error_line = true;
	_xml_error_line_number = line_number;
	_xml_error_line_position = line_position;
}

bool xml_context::get_xml_error_line(std::size_t& line_number, std::size_t& line_position)
{
	line_number = _xml_error_line_number;
	line_position = _xml_error_line_position;

	return _xml_error_line;
}

//===========================================================================
xml_read_handler_t xml_context::get_read_xml_root_tag_handler(void)
{
	return _read_xml_root_tag_handler;
}

void xml_context::set_read_xml_root_tag_handler(xml_read_handler_t xml_read_handler)
{
	_read_xml_root_tag_handler = xml_read_handler;
}

bool xml_context::invoke_read_xml_root_tag_handler(xml_reader_t& xml_reader)
{
	if (_read_xml_root_tag_handler)
	{
		return _read_xml_root_tag_handler(xml_reader);
	}

	return false;
}

//===========================================================================
xml_write_handler_t xml_context::get_write_xml_root_tag_handler(void)
{
	return _write_xml_root_tag_handler;
}

void xml_context::set_write_xml_root_tag_handler(xml_write_handler_t xml_write_handler)
{
	_write_xml_root_tag_handler = xml_write_handler;
}

bool xml_context::invoke_write_xml_root_tag_handler(xml_writer_t& xml_writer)
{
	if (_write_xml_root_tag_handler)
	{
		return _write_xml_root_tag_handler(xml_writer);
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
xml_loader::xml_loader(std::wstring xml_root_tag_name) :
	_xml_context{ xml_root_tag_name }
{
}

//===========================================================================
xml_context* xml_loader::get_xml_context(void)
{
	return &_xml_context;
}

//===========================================================================
bool xml_loader::load_xml_buffer(std::vector<std::uint8_t>& xml_buffer)
{
	return load_xml_c_buffer(xml_buffer.data(), xml_buffer.size());
}

bool xml_loader::load_xml_c_buffer(const void* xml_buffer_pointer, std::size_t xml_buffer_size)
{
	get_xml_context()->reset_xml_error();


	bool rv;


	stream_t stream;


	const BYTE* pInit = reinterpret_cast<const BYTE*>(xml_buffer_pointer);
	UINT cbInit = static_cast<UINT>(xml_buffer_size);
	stream = SHCreateMemStream(pInit, cbInit);
	if (nullptr == stream)
	{
		return false;
	}


	rv = load_xml(stream);
	if (true != rv)
	{
		return false;
	}

	return true;
}

bool xml_loader::load_xml_string(const std::string& xml_string)
{
	return load_xml_c_buffer(xml_string.c_str(), xml_string.size() * sizeof(char));
}

bool xml_loader::load_xml_c_string(const char* xml_c_string)
{
	return load_xml_c_buffer(xml_c_string, std::char_traits<char>::length(xml_c_string));
}

bool xml_loader::load_xml_u8string(const std::u8string& xml_string)
{
	return load_xml_c_buffer(xml_string.c_str(), xml_string.size() * sizeof(char8_t));
}

bool xml_loader::load_xml_c_u8string(const char8_t* xml_c_string)
{
	return load_xml_c_buffer(xml_c_string, std::char_traits<char8_t>::length(xml_c_string));
}

//===========================================================================
bool xml_loader::load_xml_file(const std::wstring& file_path)
{
	get_xml_context()->reset_xml_error();


	bool rv;
	HRESULT hr;


	stream_t stream;


	hr = SHCreateStreamOnFileEx(
		file_path.c_str(),
		STGM_READ,
		FILE_ATTRIBUTE_NORMAL,
		FALSE,
		nullptr,
		&stream
	);
	if (FAILED(hr))
	{
		return false;
	}


	rv = load_xml(stream);
	if (true != rv)
	{
		return false;
	}

	return true;
}

//===========================================================================
bool xml_loader::load_xml(stream_t& stream)
{
	bool rv;
	HRESULT hr;


	xml_reader_t xml_reader;


	hr = CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<void**>(&xml_reader), nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	hr = xml_reader->SetInput(stream.get());
	if (FAILED(hr))
	{
		return false;
	}


	rv = read_xml(xml_reader);
	if (true != rv)
	{
		UINT line_number;
		UINT line_position;
		hr = xml_reader->GetLineNumber(&line_number);
		if (FAILED(hr))
		{
			line_number = 0;
		}
		hr = xml_reader->GetLinePosition(&line_position);
		if (FAILED(hr))
		{
			line_position = 0;
		}
		get_xml_context()->set_xml_error_line(line_number, line_position);

		return false;
	}

	return true;
}

//===========================================================================
bool xml_loader::read_xml(xml_reader_t& xml_reader)
{
	bool rv;
	HRESULT hr;


	XmlNodeType nodeType;
	const wchar_t* localName;


	hr = S_OK;
	while (S_OK == hr)
	{
		hr = xml_reader->Read(&nodeType);
		if (FAILED(hr))
		{
			return false;
		}
		if (nodeType == XmlNodeType_Element)
		{
			hr = xml_reader->GetLocalName(&localName, nullptr);
			if (FAILED(hr))
			{
				return false;
			}

			if (0 == wcscmp(localName, get_xml_context()->get_xml_root_tag_name()))
			{
				rv = get_xml_context()->invoke_read_xml_root_tag_handler(xml_reader);
				if (true != rv)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
xml_saver::xml_saver(std::wstring xml_root_tag_name) :
	_xml_context{ xml_root_tag_name }
{
}

//===========================================================================
xml_context* xml_saver::get_xml_context(void)
{
	return &_xml_context;
}

//===========================================================================
bool xml_saver::get_stream_buffer(stream_t& stream, std::vector<std::uint8_t>& output)
{
	HRESULT hr;


	STATSTG statstg;
	hr = stream->Stat(&statstg, STATFLAG_NONAME);
	if (FAILED(hr))
	{
		return false;
	}


	LARGE_INTEGER move;
	move.QuadPart = 0;
	hr = stream->Seek(move, STREAM_SEEK_SET, nullptr);
	if (FAILED(hr))
	{
		return false;
	}


	ULONG streamSize = static_cast<ULONG>(statstg.cbSize.QuadPart);
	std::vector<std::uint8_t> buffer(streamSize);
	ULONG bytesRead;
	hr = stream->Read(buffer.data(), streamSize, &bytesRead);
	if (FAILED(hr))
	{
		return false;
	}
	if (bytesRead != streamSize)
	{
		return false;
	}


	output = buffer;

	return true;
}

bool xml_saver::save_xml_buffer(std::vector<std::uint8_t>& xml_buffer, bool bom)
{
	bool rv;
	HRESULT hr;


	stream_t stream;


	const BYTE* pInit = reinterpret_cast<const BYTE*>(nullptr);
	UINT cbInit = static_cast<UINT>(0);
	stream = SHCreateMemStream(pInit, cbInit);
	if (nullptr == stream)
	{
		return false;
	}


	if (bom)
	{
		const BYTE utf8_bom[] = { 0xEF, 0xBB, 0xBF };
		ULONG written;
		hr = stream->Write(utf8_bom, sizeof(utf8_bom), &written);
		if (FAILED(hr))
		{
			return false;
		}
		if (written != sizeof(utf8_bom))
		{
			return false;
		}
	}


	rv = save_xml(stream);
	if (true != rv)
	{
		return false;
	}


	rv = get_stream_buffer(stream, xml_buffer);
	if (true != rv)
	{
		return false;
	}

	return true;
}

//===========================================================================
bool xml_saver::save_xml_u8string(std::u8string& xml_string, bool bom)
{
	bool rv;


	std::vector<std::uint8_t> buffer;


	rv = save_xml_buffer(buffer, bom);
	if (true != rv)
	{
		return false;
	}


	std::u8string result(
		reinterpret_cast<char8_t*>(buffer.data()),
		buffer.size() / sizeof(char8_t)
	);
	xml_string = result;

	return true;
}

bool xml_saver::save_xml_string(std::string& xml_string, bool bom)
{
	bool rv;


	std::vector<std::uint8_t> buffer;


	rv = save_xml_buffer(buffer, bom);
	if (true != rv)
	{
		return false;
	}


	std::string result(
		reinterpret_cast<char*>(buffer.data()),
		buffer.size() / sizeof(char)
	);
	xml_string = result;

	return true;
}

//===========================================================================
bool xml_saver::save_xml_file(const std::wstring& file_path, bool bom)
{
	bool rv;
	HRESULT hr;


	stream_t stream;


	hr = SHCreateStreamOnFileEx(
		file_path.c_str(),
		STGM_CREATE | STGM_WRITE,
		FILE_ATTRIBUTE_NORMAL,
		TRUE,
		nullptr,
		&stream
	);
	if (FAILED(hr))
	{
		return false;
	}


	if (bom)
	{
		const BYTE utf8_bom[] = { 0xEF, 0xBB, 0xBF };
		ULONG written;
		hr = stream->Write(utf8_bom, sizeof(utf8_bom), &written);
		if (FAILED(hr))
		{
			return false;
		}
		if (sizeof(utf8_bom) != written)
		{
			return false;
		}
	}


	rv = save_xml(stream);
	if (true != rv)
	{
		return false;
	}

	return true;
}

//===========================================================================
bool xml_saver::save_xml(stream_t& stream)
{
	bool rv;
	HRESULT hr;


	xml_writer_t xml_writer;


	hr = CreateXmlWriter(__uuidof(IXmlWriter), reinterpret_cast<void**>(&xml_writer), nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	hr = xml_writer->SetOutput(stream.get());
	if (FAILED(hr))
	{
		return false;
	}


	rv = write_xml(xml_writer);
	if (true != rv)
	{
		return false;
	}


	hr = xml_writer->Flush();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool xml_saver::write_xml(xml_writer_t& xml_writer)
{
	bool rv;
	HRESULT hr;


	hr = xml_writer->WriteStartDocument(XmlStandalone_Omit);
	if (FAILED(hr))
	{
		return false;
	}


	hr = xml_writer->WriteWhitespace(L"\n");
	if (FAILED(hr))
	{
		return false;
	}
	rv = get_xml_context()->invoke_write_xml_root_tag_handler(xml_writer);
	if (true != rv)
	{
		return false;
	}


	hr = xml_writer->WriteEndDocument();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool read_xml_child_tag(
	xml_reader_t& xml_reader, 
	std::wstring xml_parent_tag_name, 
	xml_read_handler_map_t xml_read_handler_map, 
	bool return_if_xml_unknown_child_tag)
{
	bool rv;
	HRESULT hr;


	XmlNodeType nodeType;
	const wchar_t* localName;


	hr = S_OK;
	while (S_OK == hr)
	{
		hr = xml_reader->Read(&nodeType);
		if (FAILED(hr))
		{
			return false;
		}
		if (nodeType == XmlNodeType_EndElement)
		{
			hr = xml_reader->GetLocalName(&localName, nullptr);
			if (FAILED(hr))
			{
				return false;
			}


			if (0 == wcscmp(localName, xml_parent_tag_name.c_str()))
			{
				break;
			}
		}
		else if (nodeType == XmlNodeType_Element)
		{
			hr = xml_reader->GetLocalName(&localName, nullptr);
			if (FAILED(hr))
			{
				return false;
			}


			std::wstring key = localName;
			auto it = xml_read_handler_map.find(key);
			if (it != xml_read_handler_map.end())
			{
				rv = it->second(xml_reader);
				if (true != rv)
				{
					return false;
				}
			}
			else
			{
				if (true == return_if_xml_unknown_child_tag)
				{
					return false;
				}
			}
		}
	}

	return true;
}

//===========================================================================
bool read_xml_attr_int8(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::int8_t& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<std::int8_t>(_wtoi(value));
		return true;
	}

	return true;
}

bool read_xml_attr_uint8(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::uint8_t& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<std::uint8_t>(_wtoi(value));
		return true;
	}

	return true;
}

bool read_xml_attr_int16(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::int16_t& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<std::int16_t>(_wtoi(value));
		return true;
	}

	return true;
}

bool read_xml_attr_uint16(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::uint16_t& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<std::uint16_t>(_wtoi(value));
		return true;
	}

	return true;
}

bool read_xml_attr_int32(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::int32_t& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<std::int32_t>(_wtoi(value));
		return true;
	}

	return true;
}

bool read_xml_attr_uint32(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::uint32_t& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<std::uint32_t>(_wtoi(value));
		return true;
	}

	return true;
}

bool read_xml_attr_int64(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::int64_t& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<std::int64_t>(_wtoll(value));
		return true;
	}

	return true;
}

bool read_xml_attr_uint64(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::uint64_t& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<std::uint64_t>(_wtoll(value));
		return true;
	}

	return true;
}

bool read_xml_attr_float(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, float& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<float>(_wtof(value));
		return true;
	}

	return true;
}

bool read_xml_attr_double(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, double& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = static_cast<double>(_wtof(value));
		return true;
	}

	return true;
}

bool read_xml_attr_wstring(xml_reader_t& xml_reader, const wchar_t* xml_attr_name, std::wstring& v)
{
	HRESULT hr;


	const wchar_t* value;
	UINT valueSize;


	hr = xml_reader->MoveToAttributeByName(xml_attr_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	if (S_FALSE == hr)
	{
		return true;
	}

	hr = xml_reader->GetValue(&value, &valueSize);
	if (FAILED(hr))
	{
		return false;
	}

	if (valueSize > 0)
	{
		v = value;
		return true;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool write_xml_parent_tag_start(xml_writer_t& xml_writer, const wchar_t* xml_parent_tag_name, std::size_t tab)
{
	HRESULT hr;


	if (tab > 0)
	{
		std::wstring tab_string(tab, L'\t');


		hr = xml_writer->WriteWhitespace(tab_string.c_str());
		if (FAILED(hr))
		{
			return false;
		}
	}
	hr = xml_writer->WriteStartElement(nullptr, xml_parent_tag_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	hr = xml_writer->WriteWhitespace(L"\n");
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool write_xml_parent_tag_end(xml_writer_t& xml_writer, std::size_t tab)
{
	HRESULT hr;


	if (tab > 0)
	{
		std::wstring tab_string(tab, L'\t');


		hr = xml_writer->WriteWhitespace(tab_string.c_str());
		if (FAILED(hr))
		{
			return false;
		}
	}
	hr = xml_writer->WriteEndElement();
	if (FAILED(hr))
	{
		return false;
	}
	hr = xml_writer->WriteWhitespace(L"\n");
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool write_xml_single_tag_start(xml_writer_t& xml_writer, const wchar_t* xml_single_tag_name, std::size_t tab)
{
	HRESULT hr;


	if (tab>0)
	{
		std::wstring tab_string(tab, L'\t');


		hr = xml_writer->WriteWhitespace(tab_string.c_str());
		if (FAILED(hr))
		{
			return false;
		}
	}


	hr = xml_writer->WriteStartElement(nullptr, xml_single_tag_name, nullptr);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

bool write_xml_single_tag_end(xml_writer_t& xml_writer)
{
	HRESULT hr;


	hr = xml_writer->WriteEndElement();
	if (FAILED(hr))
	{
		return false;
	}

	hr = xml_writer->WriteWhitespace(L"\n");
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool write_xml_attr_int8(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::int8_t v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_uint8(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::uint8_t v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_int16(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::int16_t v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_uint16(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::uint16_t v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_int32(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::int32_t v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_uint32(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::uint32_t v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_int64(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::int64_t v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_uint64(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::uint64_t v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_float(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const float v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_double(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const double v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, std::to_wstring(v).c_str());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool write_xml_attr_wstring(xml_writer_t& xml_writer, const wchar_t* xml_attr_name, const std::wstring& v)
{
	HRESULT hr;


	hr = xml_writer->WriteAttributeString(nullptr, xml_attr_name, nullptr, v.c_str());
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
