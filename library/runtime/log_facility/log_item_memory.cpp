/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../log.hpp"

#include "log_item_memory.hpp"







/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 1





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined _DEBUG
#define LOG_ITEM_POOL_DEBUG 1
#else
#define LOG_ITEM_POOL_DEBUG 0
#endif




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class log_item_pool
{
public:
	std::vector<log_item*> _pool;
	std::size_t _grow;

#if 1==LOG_ITEM_POOL_DEBUG
	std::size_t _count;
	std::size_t _max_count;
#endif

public:
	log_item_pool();
	~log_item_pool();

public:
	bool create(std::size_t count);
	void destroy(void);

public:
	void append(std::size_t count);
	void cleanup(void);

public:
	log_item* acquire(void);
	void release(log_item* m);
};

//===========================================================================
log_item_pool::log_item_pool() :
	_pool(),
	_grow(16U)
#if 1==LOG_ITEM_POOL_DEBUG
	,
	_count(0U),
	_max_count(0U)
#endif
{
}

log_item_pool::~log_item_pool()
{
}

bool log_item_pool::create(std::size_t count)
{
	_pool.reserve(count);

	append(count);

	return true;
}

void log_item_pool::destroy(void)
{
	cleanup();
}

void log_item_pool::append(std::size_t count)
{
	log_item* ptr;
	std::size_t i;


	for (i = 0; i < count; ++i)
	{
		ptr = cpp_new log_item();
		if (ptr)
		{
			_pool.push_back(ptr);
		}
	}
}

void log_item_pool::cleanup(void)
{
	log_item* ptr;
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
			std::cerr << "log_item_pool::cleanup(): error!" << std::endl;
		}
	}

	_pool.clear();


#if 1==LOG_ITEM_POOL_DEBUG
	std::cerr << "log_item_pool::cleanup(): count=" << _count << " max_count=" << _max_count << std::endl;
#endif


#if 1==LOG_ITEM_POOL_DEBUG
	char message[256];


	sprintf_s(message, "log_item_pool::cleanup(): count=%zu max_count=%zu \r\n", _count, _max_count);


	OutputDebugStringA("----------------------------------------------------------------------------\r\n");
	OutputDebugStringA(message);
	OutputDebugStringA("----------------------------------------------------------------------------\r\n");
#endif
}

log_item* log_item_pool::acquire(void)
{
	log_item* ptr;


	if (!_pool.empty())
	{
		ptr = _pool.back();

		_pool.pop_back();

#if 1==LOG_ITEM_POOL_DEBUG
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

#if 1==LOG_ITEM_POOL_DEBUG
		_count++;
		if (_count > _max_count)
		{
			_max_count = _count;
		}
#endif

		return ptr;
	}


	std::cerr << "log_item_pool::acquire(): empty!" << std::endl;

	return nullptr;
}

void log_item_pool::release(log_item* ptr)
{
	if (ptr)
	{
		_pool.push_back(ptr);

#if 1==LOG_ITEM_POOL_DEBUG
		_count--;
#endif
	}
	else
	{
		std::cerr << "log_item_pool::release(): nullptr!" << std::endl;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class log_item_concurrency_pool
{
private:
	std::mutex _mutex;
	log_item_pool _pool;

public:
	log_item_concurrency_pool();

public:
	bool create(std::size_t count);
	void destroy(void);

public:
	log_item* acquire(void);
	void release(log_item* ptr);
};

//===========================================================================
log_item_concurrency_pool::log_item_concurrency_pool() :
	_mutex(),
	_pool()
{
}

bool log_item_concurrency_pool::create(std::size_t count)
{
	bool rv;


	_mutex.lock();
	rv = _pool.create(count);
	_mutex.unlock();

	return rv;
}

void log_item_concurrency_pool::destroy(void)
{
	_mutex.lock();
	_pool.destroy();
	_mutex.unlock();
}

log_item* log_item_concurrency_pool::acquire(void)
{
	log_item* ptr;


	_mutex.lock();
	ptr = _pool.acquire();
	_mutex.unlock();

	return ptr;
}

void log_item_concurrency_pool::release(log_item* ptr)
{
	_mutex.lock();
	_pool.release(ptr);
	_mutex.unlock();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static log_item_concurrency_pool* _log_item_concurrency_pool;

//===========================================================================
bool log_item_memory_initialize(std::size_t capacity)
{
	if (nullptr==_log_item_concurrency_pool)
	{
		_log_item_concurrency_pool = cpp_new log_item_concurrency_pool();
		return _log_item_concurrency_pool->create(capacity);
	}

	return false;
}

void log_item_memory_cleanup(void)
{
	if (_log_item_concurrency_pool)
	{
		_log_item_concurrency_pool->destroy();
		cpp_delete _log_item_concurrency_pool;
		_log_item_concurrency_pool = nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
log_item* log_item_alloc(void) noexcept
{
	return _log_item_concurrency_pool->acquire();
}

void log_item_free(log_item* ptr) noexcept
{
	if (ptr)
	{
		_log_item_concurrency_pool->release(ptr);
	}
}
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 0
log_item* log_item_alloc(void) noexcept
{
	log_item* ptr;


	ptr = cpp_new log_item();

	return ptr;
}

void log_item_free(log_item* ptr) noexcept
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



