/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../runtime/runtime.hpp"

//===========================================================================
#include "net_msg.hpp"
#include "net_msg_memory.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined _DEBUG
#define net_msg_pool_debug 1
#else
#define net_msg_pool_debug 0
#endif




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class net_msg_pool
{
public:
	std::vector<net_msg*> _pool;
	std::size_t _grow;

#if 1==net_msg_pool_debug
	std::size_t _count;
	std::size_t _max_count;
#endif

public:
	net_msg_pool();
	~net_msg_pool();

public:
	bool create(std::size_t count);
	void destroy(void);

public:
	void append(std::size_t count);
	void cleanup(void);

public:
	net_msg* acquire(void);
	void release(net_msg* m);
};

//===========================================================================
net_msg_pool::net_msg_pool() :
	_pool(),
	_grow(16U)
#if 1==net_msg_pool_debug
	,
	_count(0U),
	_max_count(0U)
#endif
{
}

net_msg_pool::~net_msg_pool()
{
}

bool net_msg_pool::create(std::size_t count)
{
	_pool.reserve(count);

	append(count);

	return true;
}

void net_msg_pool::destroy(void)
{
	cleanup();
}

void net_msg_pool::append(std::size_t count)
{
	net_msg* ptr;
	std::size_t i;


	for (i = 0; i < count; ++i)
	{
		ptr = cpp_new net_msg();
		if (ptr)
		{
			_pool.push_back(ptr);
		}
	}
}

void net_msg_pool::cleanup(void)
{
	net_msg* ptr;
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
			// 올수 없는 조건
			CX_RUNTIME_LOG(cxLError) << L"net_msg_pool::cleanup(): error!";
		}
	}

	_pool.clear();


#if 1==net_msg_pool_debug
	CX_RUNTIME_LOG(cxLDebug)
		<< L"----------------------------------------------------------------------------" << std::endl
		<< L"net_msg_pool::cleanup(): count=" << _count << " max_count=" << _max_count << std::endl
		<< L"----------------------------------------------------------------------------" << std::endl;
#endif
}

net_msg* net_msg_pool::acquire(void)
{
	net_msg* ptr;


	if (!_pool.empty())
	{
		ptr = _pool.back();

		_pool.pop_back();

#if 1==net_msg_pool_debug
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

#if 1==net_msg_pool_debug
		_count++;
		if (_count > _max_count)
		{
			_max_count = _count;
		}
#endif

		return ptr;
	}


	CX_RUNTIME_LOG(cxLError) << L"net_msg_pool::acquire(): empty!";

	return nullptr;
}

void net_msg_pool::release(net_msg* ptr)
{
	if (ptr)
	{
		_pool.push_back(ptr);

#if 1==net_msg_pool_debug
		_count--;
#endif
	}
	else
	{
		CX_RUNTIME_LOG(cxLError) << L"net_msg_pool::release(): nullptr!";
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class net_msg_concurrency_pool_mutex
{
private:
	std::mutex _mutex;

public:
	net_msg_concurrency_pool_mutex();

public:
	void lock(void);
	void unlock(void);
};

//===========================================================================
net_msg_concurrency_pool_mutex::net_msg_concurrency_pool_mutex() :
	_mutex()
{
}

void net_msg_concurrency_pool_mutex::lock(void)
{
	_mutex.lock();
}

void net_msg_concurrency_pool_mutex::unlock(void)
{
	_mutex.unlock();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class net_msg_concurrency_pool
{
private:
	net_msg_concurrency_pool_mutex _mutex;
	net_msg_pool _pool;

public:
	net_msg_concurrency_pool();

public:
	bool create(std::size_t count);
	void destroy(void);

public:
	net_msg* acquire(void);
	void release(net_msg* ptr);
};

//===========================================================================
net_msg_concurrency_pool::net_msg_concurrency_pool() :
	_mutex(),
	_pool()
{
}

bool net_msg_concurrency_pool::create(std::size_t count)
{
	bool rv;


	_mutex.lock();
	rv = _pool.create(count);
	_mutex.unlock();

	return rv;
}

void net_msg_concurrency_pool::destroy(void)
{
	_mutex.lock();
	_pool.destroy();
	_mutex.unlock();
}

net_msg* net_msg_concurrency_pool::acquire(void)
{
	net_msg* ptr;


	_mutex.lock();
	ptr = _pool.acquire();
	_mutex.unlock();

	return ptr;
}

void net_msg_concurrency_pool::release(net_msg* ptr)
{
	_mutex.lock();
	_pool.release(ptr);
	_mutex.unlock();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static net_msg_concurrency_pool* _net_msg_concurrency_pool = nullptr;

//===========================================================================
bool net_msg_memory_initialize(std::size_t capacity)
{
	if (nullptr == _net_msg_concurrency_pool)
	{
		_net_msg_concurrency_pool = cpp_new net_msg_concurrency_pool();
		return _net_msg_concurrency_pool->create(capacity);
	}

	return false;
}

void net_msg_memory_cleanup(void)
{
	if (_net_msg_concurrency_pool)
	{
		_net_msg_concurrency_pool->destroy();
		cpp_delete _net_msg_concurrency_pool;
		_net_msg_concurrency_pool = nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
net_msg* net_msg_alloc(void) noexcept
{
	return _net_msg_concurrency_pool->acquire();
}

void net_msg_free(net_msg* ptr) noexcept
{
	if (ptr)
	{
		ptr->reset();

		_net_msg_concurrency_pool->release(ptr);
	}
}
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 0
net_msg* net_msg_alloc(void) noexcept
{
	net_msg* ptr;


	ptr = cpp_new net_msg();

	return ptr;
}

void net_msg_free(net_msg* ptr) noexcept
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
}



