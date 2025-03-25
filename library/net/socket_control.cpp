/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
//#define NOMINMAX
//#define _CRT_SECURE_NO_WARNINGS
//#define _WINSOCKAPI_
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <mstcpip.h>

//===========================================================================
#include "../runtime/runtime.hpp"

//===========================================================================
#include "socket_descriptor.hpp"
#include "socket_control.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//
// multicast - sender
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_multicast_ttl(socket_descriptor_t descriptor, std::uint8_t multicast_ttl)
{
	int result;

	DWORD value;


	value = static_cast<DWORD>( multicast_ttl );

	result = ::setsockopt(descriptor, IPPROTO_IP, IP_MULTICAST_TTL, reinterpret_cast<const char*>(&value), sizeof(value));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}

	return true;
}

bool set_socket_option_multicast_loop(socket_descriptor_t descriptor, bool multicast_loop)
{
	int result;

	DWORD value;


	value = (true==multicast_loop) ? 1 : 0;

	result = ::setsockopt(descriptor, IPPROTO_IP, IP_MULTICAST_LOOP, reinterpret_cast<const char*>(&value), sizeof(value));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}

	return true;
}

static bool set_socket_option_multicast_if(socket_descriptor_t descriptor, struct in_addr& address)
{
	int result;


	result = ::setsockopt(descriptor, IPPROTO_IP, IP_MULTICAST_IF, reinterpret_cast<const char*>(&address), sizeof(address));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}

	return true;
}

bool set_socket_option_multicast_if(socket_descriptor_t descriptor, const char* _local)
{
	struct in_addr _interface_address;


	memset(&_interface_address, 0, sizeof(_interface_address));
	_interface_address.s_addr = inet_addr(_local);

	return set_socket_option_multicast_if(descriptor, _interface_address);
}





/////////////////////////////////////////////////////////////////////////////
//
// multicast - receiver
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static bool set_socket_option_add_membership (socket_descriptor_t descriptor, struct ip_mreq& address)
{
	int result;


	result = ::setsockopt(descriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<const char*>(&address), sizeof(address));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}

	return true;
}

bool set_socket_option_add_membership(socket_descriptor_t descriptor, const char* _local, const char* _group)
{
	struct ip_mreq _interface;


	memset(&_interface, 0, sizeof(_interface));

	_interface.imr_interface.s_addr = inet_addr(_local);
	_interface.imr_multiaddr.s_addr = inet_addr(_group);

	return set_socket_option_add_membership(descriptor, _interface);
}

static bool set_socket_option_drop_membership (socket_descriptor_t descriptor, struct ip_mreq& address)
{
	int result;


	result = ::setsockopt(descriptor, IPPROTO_IP, IP_DROP_MEMBERSHIP, reinterpret_cast<const char*>(&address), sizeof(address));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}

	return true;
}

bool set_socket_option_drop_membership(socket_descriptor_t descriptor, const char* _local, const char* _group)
{
	struct ip_mreq _interface;


	memset(&_interface, 0, sizeof(_interface));

	_interface.imr_interface.s_addr = inet_addr(_local);
	_interface.imr_multiaddr.s_addr = inet_addr(_group);

	return set_socket_option_drop_membership(descriptor, _interface);
}





/////////////////////////////////////////////////////////////////////////////
//
// broadcast
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_broadcast(socket_descriptor_t descriptor, bool enable_broadcast)
{
	int result;

	BOOL value;


	value = (true==enable_broadcast) ? 1 : 0;

	result = ::setsockopt(descriptor, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&value), sizeof(value));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
// tcp
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_keepalive(socket_descriptor_t descriptor, bool keepalive)
{
	int result;

	struct tcp_keepalive
	{
		u_long  onoff;
		u_long  keepalivetime;
		u_long  keepaliveinterval;
	};

	struct tcp_keepalive option;
	DWORD                dwBytesReturned;


	option.onoff = (true == keepalive) ? 1 : 0;

	// 양 socket간에 packet교환이 아래 간격시간 보다 작은경우
	// keep-alive-probe packet을 전송한다.
	option.keepalivetime = 3000;

	// keep-alive probe packet에 대하여 상대방이 응답이 없으면 아래 시간 
	// 간격으로 keep-alive probe packet을 반복적으로(ms-tcp 는 10회까지)
	// 보내고, 이후에 연결이 종료되고 application단에 종료가 감지된다.
	option.keepaliveinterval = 1000;

	result = ::WSAIoctl(descriptor, SIO_KEEPALIVE_VALS, &option, sizeof(option), 0, 0, &dwBytesReturned, NULL, NULL);
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}

	return true;
}

bool set_socket_option_reuseaddr (socket_descriptor_t descriptor, bool reuseaddr)
{
	// Specify the SO_REUSEADDR option  to bind a stream socket to a local   
	// port that may be still bound to another stream socket that may be  
	// hanging around with a "zombie" protocol control block whose context 
	// is not yet freed from previous sessions. 
	
	int result;

	BOOL value;


	value  = (true==reuseaddr) ? 1 : 0;

	result = ::setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&value), sizeof(value));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if ( 0 !=  result)
	{
		return false;
	}

	return true;
}

//===========================================================================
bool set_socket_option_tcp_nodelay (socket_descriptor_t descriptor, bool tcp_nodelay)
{
	// Specify the TCP_NODELAY option for protocols such as X Window System  
	// Protocol that require immediate delivery of many small messages.  
	// By default VxWorks uses congestion avoidance algorithm  
	// for virtual  terminal  protocols and  bulk  data  transfer   
	// protocols. When the TCP_NODELAY option is turned on and there are  
	// segments to be sent out, TCP  bypasses  the  congestion 
	// avoidance  algorithm  and sends the segments out if there  
	// is enough space in the send window. 


	int result;
	
	DWORD value;


	value  = (true==tcp_nodelay) ? 1 : 0;

	result = ::setsockopt(descriptor, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&value), sizeof(value));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
// linger
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_linger (socket_descriptor_t descriptor, bool onoff, std::uint16_t timeout)
{
	//-----------------------------------------------------------------------
	// If SO_LINGER is set with a nonzero time-out interval on a blocking 
	// socket, the closesocket call blocks on a blocking socket until the 
	// remaining data has been sent or until the time-out expires. 
	// This is called a graceful disconnect. 
	// If the time-out expires before all data has been sent, 
	// the Windows Sockets implementation terminates the connection before 
	// closesocket returns.
	// If SO_LINGER is enabled with a nonzero time-out: 
	// - with a blocking socket, it blocks until all data sent or time-out expires. 
	// - with a nonblocking socket, it returns immediately indicating failure
	//-----------------------------------------------------------------------
	// blcoking-socket에서 SO_LINGER가 0이 아닌 timeout interval 설정되어 있다면,
	// closesocket()을 호출했을 때 남아 있는 data는 timeout 될때까지 data는
	// 보내지게 된다.
	// 이를 칭하여 '"graceful-disconnect"라고 한다. 
	// 만일 timeout이 되기 전에 모든 data가 보냈다면 winddows socket에서는 
	// closesocket()이 return 되기도 전에 연결을 종료하도록 구현되어 있다.
	// 만약, SO_LINGER의 timeout 값이 0이 아닌 값이 설정되어 있다면,
	// - blcoking-socket에서는 정해진 시간내에서 다 보낼때까지 blcok된다.
	// - none-blcoking-socket에서는 바로 지정된 실패값이 리턴된다.
	//-----------------------------------------------------------------------

	// Specify the SO_LINGER option to perform a  "graceful" close. 
	// A graceful close occurs when a connection is shutdown, TCP  will   
	// try  to  make sure that all the unacknowledged data in transmission  
	// channel are acknowledged and the peer is shutdown properly by going  
	// through an elaborate set of state transitions. 

 
	int result;
	
	struct linger so_linger;

	
	so_linger.l_onoff  = (true==onoff) ? 1 : 0;
	so_linger.l_linger = timeout; // 0: Use default amount of time to shutdown
	
	result = ::setsockopt(descriptor, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&so_linger), sizeof(so_linger));
	// no-error: 0 
	// error:    SOCKET_ERROR(-1)
	if (0 != result)
	{
		return false;
	}
	
	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
// none-blocking
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_io_control_none_blocking_mode (socket_descriptor_t descriptor, bool enable)
{
	int result;

	u_long mode;
	

	mode = (true==enable) ? 1u : 0u;

	result = ::ioctlsocket(descriptor, FIONBIO, &mode);
	if (0 != result) // NO_ERROR
	{
		return false;
	}
	
	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
// none-blocking
//
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool set_socket_option_tx_timeout(socket_descriptor_t descriptor, std::uint32_t msec)
{
	int result;

	
	DWORD timeout;


	timeout = msec;


	result = setsockopt(descriptor, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
	if (SOCKET_ERROR == result)
	{
		return false;
	}

	return true;
}

bool set_socket_option_rx_timeout(socket_descriptor_t descriptor, std::uint32_t msec)
{
	int result;


	DWORD timeout;


	timeout = msec;


	result = setsockopt(descriptor, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
	if (SOCKET_ERROR == result)
	{
		return false;
	}

	return true;
}



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}


