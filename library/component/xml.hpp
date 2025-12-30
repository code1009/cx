#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::xml
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using xml_reader = wil::com_ptr_nothrow<IXmlReader>;
using xml_writer = wil::com_ptr_nothrow<IXmlWriter>;
using stream = wil::com_ptr_nothrow<IStream>;

//===========================================================================
using xml_read_handler = std::function<bool(xml_reader&, void*)>;
using xml_read_handler_map = std::map<std::wstring, xml_read_handler>;

using xml_write_handler = std::function<bool(xml_writer&)>;
using xml_write_handler_map = std::map<std::wstring, xml_write_handler>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class xml_context
{
private:
	std::wstring _xml_root_tag_name{ L"" };

private:
	bool        _xml_error_line{ false };
	std::size_t _xml_error_line_number{ 0 };
	std::size_t _xml_error_line_position{ 0 };

private:
	xml_read_handler _xml_root_tag_read_handler;
	xml_write_handler _xml_root_tag_write_handler;

public:
	explicit xml_context(std::wstring_view root_tag_name);

public:
	const wchar_t* get_xml_root_tag_name(void);

public:
	void reset_xml_error(void);
	void set_xml_error_line(const std::size_t line_number, const std::size_t line_position);
	bool get_xml_error_line(std::size_t& line_number, std::size_t& line_position);

public:
	xml_read_handler get_xml_root_tag_read_handler(void);
	void set_xml_root_tag_read_handler(xml_read_handler handler);
	bool invoke_read_xml_root_tag_handler(xml_reader& reader);

	xml_write_handler get_xml_root_tag_write_handler(void);
	void set_xml_root_tag_write_handler(xml_write_handler handler);
	bool invoke_xml_root_tag_write_handler(xml_writer& writer);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class xml_loader
{
private:
	xml_context _xml_context;

public:
	explicit xml_loader(std::wstring_view root_tag_name);

public:
	xml_context* get_xml_context(void);

public:
	bool load_xml_buffer(const std::vector<std::uint8_t>& xml_buffer);
	bool load_xml_c_buffer(const void* xml_buffer_pointer, const std::size_t xml_buffer_size);

	bool load_xml_string(const std::string& xml_string);
	bool load_xml_c_string(const char* xml_c_string);

	bool load_xml_u8string(const std::u8string& xml_string);
	bool load_xml_c_u8string(const char8_t* xml_c_string);

public:
	bool load_xml_file(const std::wstring_view file_path);

public:
	bool load_xml(stream& input_stream);

public:
	bool read_xml(xml_reader& xml_reader);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class xml_saver
{
private:
	xml_context _xml_context;

public:
	explicit xml_saver(std::wstring root_tag_name);

public:
	xml_context* get_xml_context(void);

public:
	bool get_stream_buffer(stream& input, std::vector<std::uint8_t>& output);
	bool save_xml_buffer(std::vector<std::uint8_t>& xml_buffer, bool bom = false);

	bool save_xml_string(std::string& xml_string, bool bom = false);
	bool save_xml_u8string(std::u8string& xml_string, bool bom = false);

public:
	bool save_xml_file(const std::wstring_view file_path, bool bom = true);

public:
	bool save_xml(stream& output_stream);

public:
	bool write_xml(xml_writer& writer);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool read_xml_child_tag(
	xml_reader& reader,
	std::wstring_view parent_tag_name,
	const xml_read_handler_map& child_read_handler_map,
	void* param = nullptr,
	bool return_if_unknown_child_tag=true
);

//===========================================================================
bool read_xml_attr_int8   (xml_reader& reader, const wchar_t* attr_name, std::int8_t& v);
bool read_xml_attr_uint8  (xml_reader& reader, const wchar_t* attr_name, std::uint8_t& v);
bool read_xml_attr_int16  (xml_reader& reader, const wchar_t* attr_name, std::int16_t& v);
bool read_xml_attr_uint16 (xml_reader& reader, const wchar_t* attr_name, std::uint16_t& v);
bool read_xml_attr_int32  (xml_reader& reader, const wchar_t* attr_name, std::int32_t& v);
bool read_xml_attr_uint32 (xml_reader& reader, const wchar_t* attr_name, std::uint32_t& v);
bool read_xml_attr_int64  (xml_reader& reader, const wchar_t* attr_name, std::int64_t& v);
bool read_xml_attr_uint64 (xml_reader& reader, const wchar_t* attr_name, std::uint64_t& v);
bool read_xml_attr_float  (xml_reader& reader, const wchar_t* attr_name, float& v);
bool read_xml_attr_double (xml_reader& reader, const wchar_t* attr_name, double& v);
bool read_xml_attr_wstring(xml_reader& reader, const wchar_t* attr_name, std::wstring& v);

//===========================================================================
bool write_xml_tabs(xml_writer& writer, std::size_t tabs);
bool write_xml_endl(xml_writer& writer);
bool write_xml_tag_open(xml_writer& writer, const wchar_t* parent_tag_name, std::size_t tab=0);
bool write_xml_tag_close(xml_writer& writer, std::size_t tab=0);

//===========================================================================
bool write_xml_attr_int8   (xml_writer& writer, const wchar_t* attr_name, const std::int8_t v);
bool write_xml_attr_uint8  (xml_writer& writer, const wchar_t* attr_name, const std::uint8_t v);
bool write_xml_attr_int16  (xml_writer& writer, const wchar_t* attr_name, const std::int16_t v);
bool write_xml_attr_uint16 (xml_writer& writer, const wchar_t* attr_name, const std::uint16_t v);
bool write_xml_attr_int32  (xml_writer& writer, const wchar_t* attr_name, const std::int32_t v);
bool write_xml_attr_uint32 (xml_writer& writer, const wchar_t* attr_name, const std::uint32_t v);
bool write_xml_attr_int64  (xml_writer& writer, const wchar_t* attr_name, const std::int64_t v);
bool write_xml_attr_uint64 (xml_writer& writer, const wchar_t* attr_name, const std::uint64_t v);
bool write_xml_attr_float  (xml_writer& writer, const wchar_t* attr_name, const float v);
bool write_xml_attr_double (xml_writer& writer, const wchar_t* attr_name, const double v);
bool write_xml_attr_wstring(xml_writer& writer, const wchar_t* attr_name, const std::wstring& v);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
