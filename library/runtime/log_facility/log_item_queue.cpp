/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../log.hpp"

#include "log_item_queue.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
log_item_queue::log_item_queue() :
	_mutex(),
	_container()
{
}

//===========================================================================
void log_item_queue::push(log_item* m)
{
	const std::lock_guard<std::mutex> lock(_mutex);


	_container.push_back(m);
}

log_item* log_item_queue::pop(void)
{
	const std::lock_guard<std::mutex> lock(_mutex);


	log_item* m = nullptr;


	if (!_container.empty())
	{
		m = _container.front();
		_container.pop_front();
	}


	return m;
}

std::size_t log_item_queue::count(void)
{
	const std::lock_guard<std::mutex> lock(_mutex);

	std::size_t count;


	count = _container.size();

	return count;
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



