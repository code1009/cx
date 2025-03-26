/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../runtime.hpp"

//===========================================================================
#include "LogItemMemory.hpp"







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
#define LogItemPoolDebug 1
#else
#define LogItemPoolDebug 0
#endif




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class LogItemPool
{
public:
	std::vector<LogItem*> _pool;
	std::size_t _grow;

#if 1==LogItemPoolDebug
	std::size_t _count;
	std::size_t _max_count;
#endif

public:
	LogItemPool();
	~LogItemPool();

public:
	bool create(std::size_t count);
	void destroy(void);

public:
	void append(std::size_t count);
	void cleanup(void);

public:
	LogItem* acquire(void);
	void release(LogItem* m);
};

//===========================================================================
LogItemPool::LogItemPool() :
	_pool(),
	_grow(16U)
#if 1==LogItemPoolDebug
	,
	_count(0U),
	_max_count(0U)
#endif
{
}

LogItemPool::~LogItemPool()
{
}

bool LogItemPool::create(std::size_t count)
{
	_pool.reserve(count);

	append(count);

	return true;
}

void LogItemPool::destroy(void)
{
	cleanup();
}

void LogItemPool::append(std::size_t count)
{
	LogItem* ptr;
	std::size_t i;


	for (i = 0; i < count; ++i)
	{
		ptr = cpp_new LogItem();
		if (ptr)
		{
			_pool.push_back(ptr);
		}
	}
}

void LogItemPool::cleanup(void)
{
	LogItem* ptr;
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
			std::cerr << "LogItemPool::cleanup(): error!" << std::endl;
		}
	}

	_pool.clear();


#if 1==LogItemPoolDebug
	std::cerr << "LogItemPool::cleanup(): count=" << _count << " max_count=" << _max_count << std::endl;
#endif


#if 1==LogItemPoolDebug
	char message[256];


	sprintf_s(message, "LogItemPool::cleanup(): count=%zu max_count=%zu \r\n", _count, _max_count);


	OutputDebugStringA("----------------------------------------------------------------------------\r\n");
	OutputDebugStringA(message);
	OutputDebugStringA("----------------------------------------------------------------------------\r\n");
#endif
}

LogItem* LogItemPool::acquire(void)
{
	LogItem* ptr;


	if (!_pool.empty())
	{
		ptr = _pool.back();

		_pool.pop_back();

#if 1==LogItemPoolDebug
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

#if 1==LogItemPoolDebug
		_count++;
		if (_count > _max_count)
		{
			_max_count = _count;
		}
#endif

		return ptr;
	}


	std::cerr << "LogItemPool::acquire(): empty!" << std::endl;

	return nullptr;
}

void LogItemPool::release(LogItem* ptr)
{
	if (ptr)
	{
		_pool.push_back(ptr);

#if 1==LogItemPoolDebug
		_count--;
#endif
	}
	else
	{
		std::cerr << "LogItemPool::release(): nullptr!" << std::endl;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class LogItemConcurrencyPool
{
private:
	std::mutex  _mutex;
	LogItemPool _pool;

public:
	LogItemConcurrencyPool();

public:
	bool create(std::size_t count);
	void destroy(void);

public:
	LogItem* acquire(void);
	void release(LogItem* ptr);
};

//===========================================================================
LogItemConcurrencyPool::LogItemConcurrencyPool() :
	_mutex(),
	_pool()
{
}

bool LogItemConcurrencyPool::create(std::size_t count)
{
	bool rv;


	_mutex.lock();
	rv = _pool.create(count);
	_mutex.unlock();

	return rv;
}

void LogItemConcurrencyPool::destroy(void)
{
	_mutex.lock();
	_pool.destroy();
	_mutex.unlock();
}

LogItem* LogItemConcurrencyPool::acquire(void)
{
	LogItem* ptr;


	_mutex.lock();
	ptr = _pool.acquire();
	_mutex.unlock();

	return ptr;
}

void LogItemConcurrencyPool::release(LogItem* ptr)
{
	_mutex.lock();
	_pool.release(ptr);
	_mutex.unlock();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static LogItemConcurrencyPool* _LogItemConcurrencyPool;

//===========================================================================
bool LogItemMemory_Initialize(std::size_t capacity)
{
	if (nullptr==_LogItemConcurrencyPool)
	{
		_LogItemConcurrencyPool = cpp_new LogItemConcurrencyPool();
		return _LogItemConcurrencyPool->create(capacity);
	}

	return false;
}

void LogItemMemory_Cleanup(void)
{
	if (_LogItemConcurrencyPool)
	{
		_LogItemConcurrencyPool->destroy();
		cpp_delete _LogItemConcurrencyPool;
		_LogItemConcurrencyPool = nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
LogItem* LogItem_Alloc(void) noexcept
{
	return _LogItemConcurrencyPool->acquire();
}

void LogItem_Free(LogItem* ptr) noexcept
{
	if (ptr)
	{
		_LogItemConcurrencyPool->release(ptr);
	}
}
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 0
LogItem* LogItem_Alloc(void) noexcept
{
	LogItem* ptr;


	ptr = cpp_new LogItem();

	return ptr;
}

void LogItem_Free(LogItem* ptr) noexcept
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



