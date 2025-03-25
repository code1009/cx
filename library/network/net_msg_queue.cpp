/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "net_msg.hpp"
#include "net_msg_queue.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
net_msg_queue::net_msg_queue() :
	_mutex(),
	_container()
{
}

//===========================================================================
void net_msg_queue::push(net_msg* m)
{
	const std::lock_guard<std::mutex> lock(_mutex);


	_container.push_back(m);
}

net_msg* net_msg_queue::pop(void)
{
	const std::lock_guard<std::mutex> lock(_mutex);


	net_msg* m = nullptr;


	if (!_container.empty())
	{
		m = _container.front();
		_container.pop_front();
	}


	return m;
}

std::size_t net_msg_queue::count(void)
{
	const std::lock_guard<std::mutex> lock(_mutex);

	std::size_t count;


	count = _container.size();

	return count;
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}



