#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int32_t  parse_int32_t_c_string             (const char* value_string, int32_t  default_value=0 );
uint32_t parse_uint32_t_c_string            (const char* value_string, uint32_t default_value=0u);
uint32_t parse_uint32_t_hex_c_string        (const char* value_string, uint32_t default_value=0u);
uint32_t parse_uint32_t_prefix_c_string     (const char* value_string, uint32_t default_value=0u);
bool     parse_bool_c_string                (const char* value_string, bool     default_value=false);
uint16_t parse_socket_addr_port_c_string    (const char* socket_addr_port_string);
uint32_t parse_socket_addr_ipv4_c_string    (const char* socket_addr_ipv4_string);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





