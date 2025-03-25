/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

//===========================================================================
#include "../runtime/runtime.hpp"

//===========================================================================
#include "socket_address.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/****************************************************************************
# socket address (16 bytes)
*****************************************************************************
struct sockaddr {
		u_short sa_family;
		char    sa_data[14];
};
struct sockaddr_in {
		short   sin_family;
		u_short sin_port;
		struct  in_addr sin_addr;
		char    sin_zero[8];
};
+--------+----------------+----------------+--------------------------------+
| offset | sockaddr       | sockaddr_in    |                                |
+--------+----------------+----------------+--------------------------------+
|      0 | family         | family         |                                |
+--------+                +                +--------------------------------+
|      1 |                |                |                                |
+--------+----------------+----------------+--------------------------------+
|      2 | data           | port           |                                |
+--------+                +                +--------------------------------+
|      3 |                |                |                                |
+--------+                +----------------+--------------------------------+
|      4 |                | addr(ipv4)     |                                |
+--------+                +                +--------------------------------+
|      5 |                |                |                                |
+--------+                +                +--------------------------------+
|      6 |                |                |                                |
+--------+                +                +--------------------------------+
|      7 |                |                |                                |
+--------+                +----------------+--------------------------------+
|      8 |                | zero           |                                |
+--------+                +                +--------------------------------+
|      9 |                |                |                                |
+--------+                +                +--------------------------------+
|     10 |                |                |                                |
+--------+                +                +--------------------------------+
|     11 |                |                |                                |
+--------+                +                +--------------------------------+
|     12 |                |                |                                |
+--------+                +                +--------------------------------+
|     13 |                |                |                                |
+--------+                +                +--------------------------------+
|     14 |                |                |                                |
+--------+                +                +--------------------------------+
|     15 |                |                |                                |
+--------+----------------+----------------+--------------------------------+
****************************************************************************/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define get_sockaddr_in() \
	(reinterpret_cast<struct sockaddr_in*>(&_data[0]))


#define get_sockaddr() \
	(reinterpret_cast<struct sockaddr*>(&_data[0]))





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void socket_address_static_assert(void)
{
	static_assert(
		(sizeof(sockaddr) == sizeof(sockaddr_in))
	);

	static_assert(
		(SOCKET_ADDRESS_DATA_MAX_SIZE) == sizeof(sockaddr_in)
	);

	static_assert(
		(SOCKET_ADDRESS_DATA_MAX_SIZE) == sizeof(sockaddr)
	);

	static_assert(
		AF_INET == static_cast<std::uint16_t>(socket_address::family::inet)
	);

	//INADDR_ANY
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
socket_address::socket_address():
	_data{},
	_size{0}
{
	_size = SOCKET_ADDRESS_DATA_MAX_SIZE;
}

socket_address::~socket_address()
{
}

//===========================================================================
socket_address::socket_address (const socket_address& o)
{
	copy_from (o);
}

socket_address& socket_address::operator = (const socket_address& rhs)
{
	copy_from (rhs);
	return *this;
}

//===========================================================================
void socket_address::copy_from(const socket_address& o)
{
	memcpy(&_data, &o._data, SOCKET_ADDRESS_DATA_MAX_SIZE);

	_size = o._size;
}

//===========================================================================
void socket_address::reset(void)
{
	memset(&_data, 0, SOCKET_ADDRESS_DATA_MAX_SIZE);

	_size = SOCKET_ADDRESS_DATA_MAX_SIZE;
}

//===========================================================================
void socket_address::set_inet_v4(
	std::uint32_t addr,
	std::uint16_t port,
	family family_type
)
{
	reset();

	switch (family_type)
	{
	case family::inet:
		get_sockaddr_in()->sin_family = AF_INET;
		break;

	case family::unknown:
	default:
		get_sockaddr_in()->sin_family = AF_UNSPEC;
		break;
	}

	get_sockaddr_in()->sin_port        = htons(port);
	get_sockaddr_in()->sin_addr.s_addr = htonl(addr);


	_size = SOCKET_ADDRESS_DATA_MAX_SIZE;
}

void socket_address::set_inet_v4(
	const std::string_view addr_string,
	std::uint16_t port,
	family family_type
)
{
	std::uint32_t addr;


	// warning C4996: 'inet_addr': Use inet_pton() or InetPton() instead 
	// or 
	// define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
	addr = htonl(inet_addr(addr_string.data()));
	
	set_inet_v4(addr, port, family_type);
}

void socket_address::set_inet_v4(
	const std::string_view addr_string,
	const std::string_view port_string, 
	family family_type
)
{
	std::uint16_t port;


	port = std::atoi(port_string.data());


	set_inet_v4(addr_string, port, family_type);
}

//===========================================================================
std::string socket_address::get_inet_v4_addr(void)
{
	std::string s;
	char text[16];

	
	sprintf_s(text, "%u.%u.%u.%u",
		static_cast<std::uint8_t>(get_data(4)),
		static_cast<std::uint8_t>(get_data(5)),
		static_cast<std::uint8_t>(get_data(6)),
		static_cast<std::uint8_t>(get_data(7))
	);

	s = text;


	return s;
}

std::string socket_address::get_inet_v4_port(void)
{
	std::string s;
	char text[16];


	sprintf_s(text, "%u",
		static_cast<std::uint16_t>((get_data(2) << 8u) | get_data(3))
	);

	s = text;


	return s;
}

void socket_address::dump(void)
{
	CX_RUNTIME_LOG(cxLDebug)
		<< L"address="
		<< static_cast<std::uint8_t >( get_data(4) ) << L"."
		<< static_cast<std::uint8_t >( get_data(5) ) << L"."
		<< static_cast<std::uint8_t >( get_data(6) ) << L"."
		<< static_cast<std::uint8_t >( get_data(7) ) 
		<< L" "
		<< L"port="   << static_cast<std::uint16_t>( (get_data(2)<<8u) | get_data(3) )
		<< L" "
		<< L"family=" << static_cast<std::uint16_t>( (get_data(0)<<8u) | get_data(1) )
		;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}


