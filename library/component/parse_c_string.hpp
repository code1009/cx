#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::int32_t  parse_std_int32_c_string           (const char* value_string, std::int32_t  default_value=0 );
std::uint32_t parse_std_uint32_c_string          (const char* value_string, std::uint32_t default_value=0u);
std::uint32_t parse_std_uint32_hex_c_string      (const char* value_string, std::uint32_t default_value=0u);
std::uint32_t parse_std_uint32_prefix_c_string   (const char* value_string, std::uint32_t default_value=0u);
bool          parse_bool_c_string                (const char* value_string, bool          default_value=false);
std::uint16_t parse_socket_addr_port_c_string    (const char* socket_addr_port_string);
std::uint32_t parse_socket_addr_ipv4_c_string    (const char* socket_addr_ipv4_string);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





