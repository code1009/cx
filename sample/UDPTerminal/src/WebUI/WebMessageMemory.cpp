/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

//===========================================================================
#include "WebMessage.hpp"
#include "WebMessageMemory.hpp"







/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined _DEBUG
#define WebMessagePoolDebug 1
#else
#define WebMessagePoolDebug 0
#endif




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebMessagePool
{
public:
	std::vector<WebMessage*> _pool;
	std::size_t _grow;

#if 1==WebMessagePoolDebug
	std::size_t _count;
	std::size_t _max_count;
#endif

public:
	WebMessagePool();
	~WebMessagePool();

public:
	bool create(std::size_t count);
	void destroy(void);

public:
	void append(std::size_t count);
	void cleanup(void);

public:
	WebMessage* acquire(void);
	void release(WebMessage* m);
};

//===========================================================================
WebMessagePool::WebMessagePool() :
	_pool(),
	_grow(16U)
#if 1==WebMessagePoolDebug
	,
	_count(0U),
	_max_count(0U)
#endif
{
}

WebMessagePool::~WebMessagePool()
{
}

bool WebMessagePool::create(std::size_t count)
{
	_pool.reserve(count);

	append(count);

	return true;
}

void WebMessagePool::destroy(void)
{
	cleanup();
}

void WebMessagePool::append(std::size_t count)
{
	WebMessage* ptr;
	std::size_t i;


	for (i = 0; i < count; ++i)
	{
		ptr = cpp_new WebMessage();
		if (ptr)
		{
			_pool.push_back(ptr);
		}
	}
}

void WebMessagePool::cleanup(void)
{
	WebMessage* ptr;
	std::size_t i;
	std::size_t count;


	count = _pool.size();
	for (i = 0; i < count; ++i)
	{
		ptr = _pool[i];

		if (ptr)
		{
			cpp_delete ptr;
		}
		else
		{
			// 올 수 없는 조건
			CX_RUNTIME_LOG(cxLError) << L"WebMessagePool::cleanup(): error!";
		}
	}

	_pool.clear();


#if 1==WebMessagePoolDebug
	CX_RUNTIME_LOG(cxLDebug)
		<< L"----------------------------------------------------------------------------" << std::endl
		<< L"WebMessagePool::cleanup(): count=" << _count << " max_count=" << _max_count << std::endl
		<< L"----------------------------------------------------------------------------" << std::endl;
#endif
}

WebMessage* WebMessagePool::acquire(void)
{
	WebMessage* ptr;


	if (!_pool.empty())
	{
		ptr = _pool.back();

		_pool.pop_back();

#if 1==WebMessagePoolDebug
		_count++;
		if (_count > _max_count)
		{
			_max_count = _count;
		}
#endif

		return ptr;
	}


	append(_grow);


	if (!_pool.empty())
	{
		ptr = _pool.back();

		_pool.pop_back();

#if 1==WebMessagePoolDebug
		_count++;
		if (_count > _max_count)
		{
			_max_count = _count;
		}
#endif

		return ptr;
	}


	CX_RUNTIME_LOG(cxLError) << L"WebMessagePool::acquire(): empty!";

	return nullptr;
}

void WebMessagePool::release(WebMessage* ptr)
{
	if (ptr)
	{
		_pool.push_back(ptr);

#if 1==WebMessagePoolDebug
		_count--;
#endif
	}
	else
	{
		CX_RUNTIME_LOG(cxLError) << L"WebMessagePool::release(): nullptr!";
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebMessageConcurrencyPool
{
private:
	std::mutex  _mutex;
	WebMessagePool _pool;

public:
	WebMessageConcurrencyPool();

public:
	bool create(std::size_t count);
	void destroy(void);

public:
	WebMessage* acquire(void);
	void release(WebMessage* ptr);
};

//===========================================================================
WebMessageConcurrencyPool::WebMessageConcurrencyPool() :
	_mutex(),
	_pool()
{
}

bool WebMessageConcurrencyPool::create(std::size_t count)
{
	bool rv;


	_mutex.lock();
	rv = _pool.create(count);
	_mutex.unlock();

	return rv;
}

void WebMessageConcurrencyPool::destroy(void)
{
	_mutex.lock();
	_pool.destroy();
	_mutex.unlock();
}

WebMessage* WebMessageConcurrencyPool::acquire(void)
{
	WebMessage* ptr;


	_mutex.lock();
	ptr = _pool.acquire();
	_mutex.unlock();

	return ptr;
}

void WebMessageConcurrencyPool::release(WebMessage* ptr)
{
	_mutex.lock();
	_pool.release(ptr);
	_mutex.unlock();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static WebMessageConcurrencyPool* _WebMessageConcurrencyPool;

//===========================================================================
bool WebMessageMemory_Initialize(std::size_t capacity)
{
	if (nullptr==_WebMessageConcurrencyPool)
	{
		_WebMessageConcurrencyPool = cpp_new WebMessageConcurrencyPool();
		return _WebMessageConcurrencyPool->create(capacity);
	}

	return false;
}

void WebMessageMemory_Cleanup(void)
{
	if (_WebMessageConcurrencyPool)
	{
		_WebMessageConcurrencyPool->destroy();
		cpp_delete _WebMessageConcurrencyPool;
		_WebMessageConcurrencyPool = nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
WebMessage* WebMessage_Alloc(void) noexcept
{
	return _WebMessageConcurrencyPool->acquire();
}

void WebMessage_Free(WebMessage* ptr) noexcept
{
	if (ptr)
	{
		_WebMessageConcurrencyPool->release(ptr);
	}
}
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 0
WebMessage* WebMessage_Alloc(void) noexcept
{
	WebMessage* ptr;


	ptr = cpp_new WebMessage();

	return ptr;
}

void WebMessage_Free(WebMessage* ptr) noexcept
{
	if (ptr)
	{
		cpp_delete ptr;
	}
}
#endif








/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




