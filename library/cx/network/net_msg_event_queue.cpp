/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "net_msg.hpp"
#include "net_msg_event_queue.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
net_msg_event_queue::net_msg_event_queue() :
	_mutex(),
	_container()
{
	_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

net_msg_event_queue::~net_msg_event_queue()
{
	CloseHandle(_event);
}

//===========================================================================
void net_msg_event_queue::push(net_msg* m)
{
	{
		const std::lock_guard<std::mutex> lock(_mutex);


		_container.push_back(m);
	}

	SetEvent(_event);
}

net_msg* net_msg_event_queue::pop(void)
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

std::size_t net_msg_event_queue::count(void)
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



