/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cinttypes>

//===========================================================================
#include "parse_c_string.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::int32_t parse_std_int32_c_string(const char* value_string, std::int32_t default_value)
{
	//-----------------------------------------------------------------------
	if (nullptr==value_string)
	{
		return default_value;
	}
	
	
	//-----------------------------------------------------------------------
	bool validate = true;


	//-----------------------------------------------------------------------
	std::uint32_t string_length;


	// -2,147,483,648 ~ +2,147,483,647 = max length 11(with sign)
	string_length = static_cast<std::uint32_t>( strlen(value_string) );
	if ( 0u==string_length) { validate = false; }
	if (11u< string_length) { validate = false; }
		

	//-----------------------------------------------------------------------
	std::uint32_t i;
	std::uint32_t count;

	char ch;


	count = string_length;
	for (i=0u; i<count;i++)
	{
		if (false==validate)
		{
			break;
		}

		ch = *(value_string+i);

		switch (ch)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '+':
		case '-':
			break;
		default:
			validate = false; 
			break;
		}
		
		if (0u!=i)
		{
			if (ch=='+') { validate = false; }
			if (ch=='-') { validate = false; }
		}
	}


	//-----------------------------------------------------------------------
	if (string_length==11u)
	{
		ch = *value_string;

		if (!((ch == '+') || (ch == '-')))
		{
			validate = false;
		}
	}


	//-----------------------------------------------------------------------
	std::int32_t value = default_value;


	if (true==validate)
	{
		if (-1==sscanf_s(value_string, "%d", &value))
		{
			value = default_value;
		}
	}

	return value;
}

//===========================================================================
std::uint32_t parse_std_uint32_c_string(const char* value_string, std::uint32_t default_value)
{
	//-----------------------------------------------------------------------
	if (nullptr==value_string)
	{
		return default_value;
	}
	
	
	//-----------------------------------------------------------------------
	bool validate = true;


	//-----------------------------------------------------------------------
	std::uint32_t string_length;


	// 0 ~ 4,294,967,295 = max length 10
	string_length = static_cast<std::uint32_t>( strlen(value_string) );
	if ( 0u==string_length) { validate = false; }
	if (10u< string_length) { validate = false; }
		

	//-----------------------------------------------------------------------
	std::uint32_t i;
	std::uint32_t count;

	char ch;


	count = string_length;
	for (i=0u; i<count;i++)
	{
		if (false==validate)
		{
			break;
		}

		ch = *(value_string+i);

		switch (ch)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		default:
			validate = false; 
			break;
		}
	}

	//-----------------------------------------------------------------------
	std::uint32_t value = default_value;


	if (true==validate)
	{
		/* EOF == -1 */
		if (-1==sscanf_s(value_string, "%u", &value))
		{
			value = default_value;
		}
	}

	return value;
}

std::uint32_t parse_std_uint32_hex_c_string(const char* value_string, std::uint32_t default_value)
{
	//-----------------------------------------------------------------------
	if (nullptr==value_string)
	{
		return default_value;
	}
	
	
	//-----------------------------------------------------------------------
	bool validate = true;


	//-----------------------------------------------------------------------
	std::uint32_t string_length;


	// 0 ~ FFFFFFFF = max length 8
	string_length = static_cast<std::uint32_t>( strlen(value_string) );
	if (0u==string_length) { validate = false; }
	if (8u< string_length) { validate = false; }
		

	//-----------------------------------------------------------------------
	std::uint32_t i;
	std::uint32_t count;

	char ch;


	count = string_length;
	for (i=0u; i<count;i++)
	{
		if (false==validate)
		{
			break;
		}

		ch = *(value_string+i);

		switch (ch)
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
			break;
		default:
			validate = false; 
			break;
		}
	}


	//-----------------------------------------------------------------------
	std::uint32_t value = default_value;


	if (true==validate)
	{
		if (-1==sscanf_s(value_string, "%x", &value))
		{
			value = default_value;
		}
	}

	return value;
}

std::uint32_t parse_std_uint32_prefix_c_string(const char* value_string, std::uint32_t default_value)
{
	//-----------------------------------------------------------------------
	if (nullptr==value_string)
	{
		return default_value;
	}
	
	
	//-----------------------------------------------------------------------
	std::uint32_t string_length;


	string_length = static_cast<std::uint32_t>( strlen(value_string) );
	if (0u==string_length)
	{
		return default_value;
	}


	//-----------------------------------------------------------------------
	if (1u<string_length)
	{
		if ( 0==strncmp (value_string, "#", 1u) )
		{
			return parse_std_uint32_hex_c_string(value_string+1, default_value);
		}
	}

	if (2u<string_length)
	{
		if ( 0==strncmp (value_string, "0x", 2u) )
		{
			return parse_std_uint32_hex_c_string(value_string+2, default_value);
		}
		if ( 0==strncmp (value_string, "0X", 2u) )
		{
			return parse_std_uint32_hex_c_string(value_string+2, default_value);
		}
	}


	return parse_std_uint32_c_string(value_string, default_value);
}

//===========================================================================
bool parse_bool_c_string(const char* value_string, bool default_value)
{
	//-----------------------------------------------------------------------
	if (nullptr==value_string)
	{
		return default_value;
	}

	
	//-----------------------------------------------------------------------
	std::uint32_t string_length;


	string_length = static_cast<std::uint32_t>( strlen(value_string) );
	if (0u==string_length)
	{
		return default_value;
	}
	
	
	//-----------------------------------------------------------------------
	bool value = default_value;


	if (5u==string_length)
	{
		if      (0==strcmp("false", value_string)) { value = false; }
		else if (0==strcmp("FALSE", value_string)) { value = false; }
		else if (0==strcmp("False", value_string)) { value = false; }
	}
	if (4u==string_length)
	{
		if      (0==strcmp("true", value_string)) { value = true; }
		else if (0==strcmp("TRUE", value_string)) { value = true; }
		else if (0==strcmp("True", value_string)) { value = true; }
	}

	return value;
}

//===========================================================================
std::uint16_t parse_socket_addr_port_c_string(const char* socket_addr_port_string)
{
	int           number;
	std::uint16_t port;


	number = parse_std_uint32_c_string(socket_addr_port_string, 0u);
	port   = static_cast<std::uint16_t>(number);

	return port;
}

std::uint32_t parse_socket_addr_ipv4_c_string(const char* socket_addr_ipv4_string)
{
	// ip adddress: "000.000.000.000" 16 bytes

	std::uint32_t string_length;
	std::uint32_t i;
	std::uint32_t count;
	char ch;
	std::uint32_t offset;
	bool validate;

	std::uint32_t address_class_index;
	std::uint8_t  address_class[4];
	char          address_class_string[4][32];
	std::uint32_t address;


	offset   = 0u;
	validate = true;

	address = 0u;
	address_class_index = 0u;
	memset (address_class, 0, sizeof(address_class));
	memset (address_class_string, 0, sizeof(address_class_string));
	

	if (nullptr!= socket_addr_ipv4_string)
	{
		string_length = static_cast<std::uint32_t>( strlen (socket_addr_ipv4_string) );

		if ( (7 <= string_length) && (string_length  <= 16) )
		{
			count = string_length;
			for (i=0u; i<count;i++)
			{
				if (false==validate)
				{
					break;
				}

				ch = *(socket_addr_ipv4_string +i);

				switch (ch)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					address_class_string[address_class_index][offset] = ch;
					offset++;
					if (3u<offset)
					{
						validate = false; 
					}
					break;

				case '.':
					offset = 0u;
					address_class_index++;
					if ( 4 <= address_class_index )
					{
						validate = false; 
					}
					break;

				default:
					validate = false; 
					break;
				}
			}

			if ( validate )
			{
				count = 4;
				for (i=0u; i<count;i++)
				{
					address_class[i] = atoi( address_class_string[i] );
				}
			}
		}
	}
	
	address = 
		( (address_class[0]&0xFFu) << 24u ) +
		( (address_class[1]&0xFFu) << 16u ) +
		( (address_class[2]&0xFFu) <<  8u ) +
		( (address_class[3]&0xFFu)        ) ;


	return address;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



