/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "net_addr_config.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
net_addr_config::net_addr_config() :
	_addr{  },
	_port{  }
{
}

net_addr_config::~net_addr_config()
{
}

//===========================================================================
net_addr_config::net_addr_config(const net_addr_config& other) :
	_addr(other._addr),
	_port(other._port)
{
}

net_addr_config& net_addr_config::operator=(const net_addr_config& other)
{
	if (this != &other)
	{
		_addr = other._addr;
		_port = other._port;
	}

	return *this;
}

//===========================================================================
net_addr_config::net_addr_config(net_addr_config&& other) noexcept :
	_addr(std::move(other._addr)),
	_port(std::move(other._port))
{
}

net_addr_config& net_addr_config::operator=(net_addr_config&& other) noexcept
{
	if (this != &other)
	{
		_addr = std::move(other._addr);
		_port = std::move(other._port);
	}

	return *this;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}