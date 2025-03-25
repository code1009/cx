/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../runtime.hpp"

//===========================================================================
#include "LogItemQueue.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
LogItemQueue::LogItemQueue() :
	_mutex(),
	_container()
{
}

//===========================================================================
void LogItemQueue::push(LogItem* m)
{
	const std::lock_guard<std::mutex> lock(_mutex);


	_container.push_back(m);
}

LogItem* LogItemQueue::pop(void)
{
	const std::lock_guard<std::mutex> lock(_mutex);


	LogItem* m = nullptr;


	if (!_container.empty())
	{
		m = _container.front();
		_container.pop_front();
	}


	return m;
}

std::size_t LogItemQueue::count(void)
{
	const std::lock_guard<std::mutex> lock(_mutex);

	std::size_t count;


	count = _container.size();

	return count;
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



