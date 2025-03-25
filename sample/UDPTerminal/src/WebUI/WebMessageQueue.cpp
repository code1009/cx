/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "WebMessage.hpp"
#include "WebMessageQueue.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebMessageQueue::WebMessageQueue() :
	_mutex(),
	_container()
{
}

//===========================================================================
void WebMessageQueue::push(WebMessage* m)
{
	const std::lock_guard<std::mutex> lock(_mutex);


	_container.push_back(m);
}

WebMessage* WebMessageQueue::pop(void)
{
	const std::lock_guard<std::mutex> lock(_mutex);


	WebMessage* m = nullptr;


	if (!_container.empty())
	{
		m = _container.front();
		_container.pop_front();
	}


	return m;
}

std::size_t WebMessageQueue::count(void)
{
	const std::lock_guard<std::mutex> lock(_mutex);

	std::size_t count;


	count = _container.size();

	return count;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




