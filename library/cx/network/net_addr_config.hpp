#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class net_addr_config
{
public:
	std::string _addr;
	std::string _port;

public:
	net_addr_config();
	~net_addr_config();

public:
	net_addr_config(const net_addr_config& other);
	net_addr_config& operator=(const net_addr_config& other);

public:
	net_addr_config(net_addr_config&& other) noexcept;
	net_addr_config& operator=(net_addr_config&& other) noexcept;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}


