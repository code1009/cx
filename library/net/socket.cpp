/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

//===========================================================================
#include "../runtime/runtime.hpp"

//===========================================================================
#include "socket_descriptor.hpp"
#include "socket_address.hpp"
#include "socket.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
socket::socket() :
	_descriptor{ INVALID_SOCKET_DESCRIPTOR }
{
}

socket::~socket()
{
	destroy();
}

//===========================================================================
bool socket::create(config cfg)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR==_descriptor);
	

	//-----------------------------------------------------------------------
	socket_descriptor_t descriptor;
	int domain   ;
	int type     ; 
	int protocol ;


	switch (cfg)
	{
	case config::udp:
		domain   = static_cast<int>(AF_INET);
		type     = static_cast<int>(SOCK_DGRAM);
		protocol = static_cast<int>(IPPROTO_UDP);
		break;

	case config::tcp:
		domain   = static_cast<int>(AF_INET);
		type     = static_cast<int>(SOCK_STREAM);
		protocol = static_cast<int>(IPPROTO_TCP);
		break;

	default:
		return false;
		//break;
	}


	//-----------------------------------------------------------------------
	descriptor = ::socket(domain, type, protocol);


	//-----------------------------------------------------------------------
	// error:    INVALID_SOCKET(-1)
	// no-error: >=0
	if ( descriptor == -1 )
	{
		CX_RUNTIME_LOG(cxLError) << L"fail to socket()=" << descriptor;
		return false;
	}


	//-----------------------------------------------------------------------
	_descriptor = descriptor;

	return true;
}

void socket::destroy(void)
{
	//-----------------------------------------------------------------------
	int result ;
	

	if (INVALID_SOCKET_DESCRIPTOR!=_descriptor)
	{
		result = ::closesocket(_descriptor);
	}
	else
	{
		result = 0;
	}


	//-----------------------------------------------------------------------
	// error:    SOCKET_ERROR(-1)
	// no-error: 0
	if ( result != 0 )
	{
		CX_RUNTIME_LOG(cxLError) << L"fail to close()=" << result;
	}


	//-----------------------------------------------------------------------
	_descriptor = INVALID_SOCKET_DESCRIPTOR;
}

//===========================================================================
bool socket::shutdown(std::int32_t h)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);


	//-----------------------------------------------------------------------
	int result ;
	int how    ;


	how    = static_cast<int>(h);
	result = ::shutdown(_descriptor, how);


	//-----------------------------------------------------------------------
	// error:    SOCKET_ERROR(-1)
	// no-error: 0
	if ( result != 0 )
	{
		CX_RUNTIME_LOG(cxLError) << L"fail to shutdown()=" << result;
		return false;
	}
		
	return true;
}

bool socket::bind(socket_address* sa)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);

	CX_RUNTIME_ASSERT(nullptr != sa);


	//-----------------------------------------------------------------------
	int              result  ;
	struct sockaddr* name    ;
	int              namelen ;


	name    = reinterpret_cast<struct sockaddr*>(sa->get_data_ptr());
	namelen = static_cast<int>(sa->get_size());
	result  = ::bind(_descriptor, name, namelen);


	//-----------------------------------------------------------------------
	bool status;


	// error:    SOCKET_ERROR(-1)
	// no-error: 0	
	if ( result != 0)
	{
		status = false;
	}
	else
	{
		status = true;
	}


	//-----------------------------------------------------------------------
	if (false==status)
	{
		CX_RUNTIME_LOG(cxLError)
			<< L"fail to bind()=" << result
			<< L" : "
			<< L"address="
			<< static_cast<std::uint8_t >( sa->get_data(4) ) << L"."
			<< static_cast<std::uint8_t >( sa->get_data(5) ) << L"."
			<< static_cast<std::uint8_t >( sa->get_data(6) ) << L"."
			<< static_cast<std::uint8_t >( sa->get_data(7) ) 
			<< L" "
			<< L"port="   << static_cast<std::uint16_t>( (sa->get_data(2)<<8u) | sa->get_data(3) )
			<< L" "
			<< L"family=" << static_cast<std::uint16_t>( (sa->get_data(0)<<8u) | sa->get_data(1) )
			;
	}


	return status;
}

bool socket::listen(std::int32_t b)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);


	//-----------------------------------------------------------------------
	int result  ;
	int backlog ;


	backlog = static_cast<int>(b);
	result  = ::listen(_descriptor, backlog);


	//-----------------------------------------------------------------------
	// error:    SOCKET_ERROR(-1)
	// no-error: 0	
	if ( result != 0)
	{  
		CX_RUNTIME_LOG(cxLError) << L"fail to listen()=" << result;
		return false;
	}  


	return true;
}

bool socket::accept(socket_address* sa, socket_descriptor_t* client)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);

	CX_RUNTIME_ASSERT( nullptr!=sa     );
	CX_RUNTIME_ASSERT( nullptr!=client );


	//-----------------------------------------------------------------------
	socket_descriptor_t descriptor;
	struct sockaddr* addr;
	int* addrlen;

	int sa_size;


	//sa_size = static_cast<int>(sa->get_size());
	sa_size = SOCKET_ADDRESS_DATA_MAX_SIZE;

	addr       = reinterpret_cast<struct sockaddr*>(sa->get_data_ptr());
	addrlen    = reinterpret_cast<int*>(&sa_size);
	descriptor = ::accept(_descriptor, addr, addrlen);


	//-----------------------------------------------------------------------
	// error:    SOCKET_ERROR(-1)
	// no-error: a descriptor for the new socket	
	if (descriptor==-1)
	{
		CX_RUNTIME_LOG(cxLError) << L"fail to accept()=" << descriptor;
		return false;
	}


	//-----------------------------------------------------------------------
	sa->set_size(static_cast<std::size_t>(sa_size));


	*client = descriptor;


	return true;
}

bool socket::connect(socket_address* sa)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);
	
	CX_RUNTIME_ASSERT( nullptr!=sa );


	//-----------------------------------------------------------------------
	int              result  ;
	struct sockaddr* name    ;
	int              namelen ;


	name    = reinterpret_cast<struct sockaddr*>(sa->get_data_ptr());
	namelen = static_cast<int>(sa->get_size());
	result  = ::connect(_descriptor, name, namelen);


	//-----------------------------------------------------------------------
	// error:    SOCKET_ERROR(-1)
	// no-error: 0
	if (result!=0)
	{
		CX_RUNTIME_LOG(cxLError) << L"fail to ::connect()=" << result;
		return false;
	}

	return true;
}

//===========================================================================
bool socket::send(void* pointer, std::int32_t size, std::int32_t* wsize, std::int32_t sflags)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);
	
	CX_RUNTIME_ASSERT( nullptr!=pointer );
	CX_RUNTIME_ASSERT( 0u     !=size    );
	
	
	//-----------------------------------------------------------------------
	int   result ;
	char* buf    ;
	int   len    ;
	int   flags  ;


	buf    = reinterpret_cast<char*>( pointer );
	len    = static_cast     <int  >( size    );
	flags  = static_cast     <int  >( sflags  );
	result = ::send(_descriptor, buf, len, flags);


	//-----------------------------------------------------------------------
	// no-error: the total number of bytes sent
	//           (can be less than the number indicated by len)           
	// error:    SOCKET_ERROR(-1)
	if (-1==result)
	{
		if (nullptr!=wsize)
		{
			*wsize=0u; 
		}
		return false;
	}


	//-----------------------------------------------------------------------
	if (nullptr!=wsize)
	{
		*wsize = static_cast<std::int32_t>(result);
	}


	return true;
}

bool socket::recv(void* pointer, std::int32_t size, std::int32_t* rsize, std::int32_t rflags)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);
	
	CX_RUNTIME_ASSERT( nullptr!=pointer );
	CX_RUNTIME_ASSERT( 0u     !=size    );
	

	//-----------------------------------------------------------------------
	int   result ;
	char* buf    ;
	int   len    ;
	int   flags  ;


	buf    = reinterpret_cast<char*>( pointer );
	len    = static_cast     <int  >( size    );
	flags  = static_cast     <int  >( rflags  );
	result = ::recv(_descriptor, buf, len, flags);


	//-----------------------------------------------------------------------
	// no-error: the number of bytes received
	//           If the connection has been gracefully closed, the return value is zero
	// error:    SOCKET_ERROR(-1)
	if (-1==result)
	{
		if (nullptr!=rsize)
		{
			*rsize=0u; 
		}
		return false;
	}


	//-----------------------------------------------------------------------
	if (nullptr!=rsize)
	{
		*rsize = static_cast<std::int32_t>(result);
	}


	return true;
}

//===========================================================================
bool socket::sendto(socket_address* sa, void* pointer, std::int32_t size, std::int32_t* wsize, std::int32_t sflags)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);
	
	CX_RUNTIME_ASSERT( nullptr!=sa      );
	CX_RUNTIME_ASSERT( nullptr!=pointer );
	CX_RUNTIME_ASSERT( 0u     !=size    );
	

	//-----------------------------------------------------------------------
	int              result ;
	char*            buf    ;
	int              len    ;
	int              flags  ;
	struct sockaddr* to     ;
	int              tolen  ;


	buf    = reinterpret_cast<char*           >( pointer );
	len    = static_cast     <int             >( size    );
	flags  = static_cast     <int             >( sflags  );
	to     = reinterpret_cast<struct sockaddr*>( sa->get_data_ptr() );
	tolen  = static_cast     <int             >( sa->get_size()     );
	result = ::sendto(_descriptor, buf, len, flags, to, tolen);


	//-----------------------------------------------------------------------
	// no-error: the total number of bytes sent
	//           (can be less than the number indicated by len)
	// error:    SOCKET_ERROR(-1)
	if (-1==result)
	{
		if (nullptr!=wsize)
		{
			*wsize=0u; 
		}
		return false;
	}


	//-----------------------------------------------------------------------
	if (nullptr!=wsize)
	{
		*wsize = static_cast<std::int32_t>(result);
	}


	return true;
}

bool socket::recvfrom (socket_address* sa, void* pointer, std::int32_t size, std::int32_t* rsize, std::int32_t rflags)
{
	//-----------------------------------------------------------------------
	CX_RUNTIME_ASSERT(INVALID_SOCKET_DESCRIPTOR!=_descriptor);
	
	CX_RUNTIME_ASSERT( nullptr!=sa      );
	CX_RUNTIME_ASSERT( nullptr!=pointer );
	CX_RUNTIME_ASSERT( 0u     !=size    );
	

	//-----------------------------------------------------------------------
	int              result  ;
	char*            buf     ;
	int              len     ;
	int              flags   ;
	struct sockaddr* from    ;
	int*             fromlen ;

	int sa_size;


	//sa_size = static_cast<int>(sa->get_size());
	sa_size = SOCKET_ADDRESS_DATA_MAX_SIZE;

	buf      = reinterpret_cast<char*           >( pointer );
	len      = static_cast     <int             >( size    ); 
	flags    = static_cast     <int             >( rflags  );
	from     = reinterpret_cast<struct sockaddr*>( sa->get_data_ptr());
	fromlen  = reinterpret_cast<int*            >(&sa_size           );
	result   = ::recvfrom(_descriptor, buf, len, flags, from, fromlen);


	//-----------------------------------------------------------------------
	// no-error: the number of bytes received
	//           If the connection has been gracefully closed, the return value is zero
	// error:    SOCKET_ERROR(-1)
	if (-1==result)
	{
		if (nullptr!=rsize)
		{
			*rsize=0u;
		}

		return false;
	}


	//-----------------------------------------------------------------------
	sa->set_size(sa_size);


	//-----------------------------------------------------------------------
	if (nullptr!=rsize)
	{
		*rsize = static_cast<std::int32_t>(result);
	}


	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}


