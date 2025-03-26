/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../runtime.hpp"
#include "LogItemMemory.hpp"
#include "LogItemQueue.hpp"
#include "LogWriter.hpp"
#include "LogAsyncWriter.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
LogAsyncWriter::LogAsyncWriter() :
	_EventCv{},
	_EventCvMutex{},
	_EventTimeout_msec{1000U},
	_EventFlag_Shutdown{false},
	_EventFlag_Flush{false},
	_ProcessThread{},
	_mutex{},
	_buffer0{},
	_buffer1{},
	_front{nullptr},
	_back{nullptr}
{
	create();
}

LogAsyncWriter::~LogAsyncWriter()
{
	destroy();
}

//===========================================================================
void LogAsyncWriter::write(LogItem& item)
{
	_mutex.lock();


	if (_front)
	{
		LogItem* m = LogItem_Alloc();
		if(m)
		{
			*m = item;
			_front->push_back(m);
		}
	}


	_mutex.unlock();
}

void LogAsyncWriter::flush(void)
{
	notifyFlush();
}

//===========================================================================
bool LogAsyncWriter::create(void)
{
	_buffer0.reserve(1024);
	_buffer1.reserve(1024);

	_front = &_buffer0;
	_back = &_buffer1;


	_ProcessThread = std::thread(&LogAsyncWriter::process, this);
	if (!_ProcessThread.joinable())
	{
		return false;
	}

	return true;
}

void LogAsyncWriter::destroy(void)
{
	notifyShutdown();

	if (_ProcessThread.joinable())
	{
		_ProcessThread.join();
	}

	cleanup();
}

//===========================================================================
void LogAsyncWriter::cleanup(void)
{
	if (_back)
	{
		recordItems(_back);

		_back = nullptr;
	}

	if (_front)
	{
		recordItems(_front);

		_front = nullptr;
	}
}

//===========================================================================
void LogAsyncWriter::process(void)
{
	onBegin();

	if (0U == _EventTimeout_msec)
	{
		eventLoop();
	}
	else
	{
		eventLoop(_EventTimeout_msec);
	}

	onEnd();
}

//===========================================================================
void LogAsyncWriter::onBegin(void)
{
	SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);
}

void LogAsyncWriter::onEnd(void)
{
}

void LogAsyncWriter::eventLoop(void)
{
	Event e;
	bool loop_flag;


	do
	{
		e = waitEvent();

		loop_flag = onEvent(e);
	} while (loop_flag);
}

void LogAsyncWriter::eventLoop(const std::uint32_t msec)
{
	Event e;
	bool loop_flag;

	do
	{
		e = waitEvent(msec);

		loop_flag = onEvent(e);
	} while (loop_flag);
}

bool LogAsyncWriter::onEvent(Event e)
{
	bool loop_flag;


	loop_flag = true;
	switch (e)
	{
	case Event::Timeout:
		onEvent_Timeout();
		break;

	case Event::Shutdown:
		loop_flag = onEvent_Shutdown();
		break;

	case Event::Flush:
		onEvent_Flush();
		break;

	case Event::Unknown:
	default:
		loop_flag = onEvent_Unknown();
		break;
	}

	return loop_flag;
}

//===========================================================================
bool LogAsyncWriter::checkEvent(void)
{
	if (_EventFlag_Shutdown)
	{
		return true;
	}

	if (_EventFlag_Flush)
	{
		return true;
	}

	return false;
}

LogAsyncWriter::Event LogAsyncWriter::getEvent(void)
{
	if (_EventFlag_Shutdown)
	{
		_EventFlag_Shutdown = false;

		return Event::Shutdown;
	}

	if (_EventFlag_Flush)
	{
		_EventFlag_Flush = false;

		return Event::Flush;
	}

	return Event::Unknown;
}

LogAsyncWriter::Event LogAsyncWriter::waitEvent(void)
{
	std::unique_lock<std::mutex> lock(_EventCvMutex);


	_EventCv.wait(lock, [this]() { return checkEvent(); });


	return getEvent();
}

LogAsyncWriter::Event LogAsyncWriter::waitEvent(const std::uint32_t msec)
{
	std::unique_lock<std::mutex> lock(_EventCvMutex);

	auto rel_time = std::chrono::milliseconds(msec);
	bool rv;


	rv = _EventCv.wait_for(lock, rel_time, [this]() { return checkEvent(); });

	if (false == rv)
	{
		return Event::Timeout;
	}


	return getEvent();
}

bool LogAsyncWriter::onEvent_Unknown(void)
{
	return false;
}

void LogAsyncWriter::onEvent_Timeout(void)
{
	record();
}

bool LogAsyncWriter::onEvent_Shutdown(void)
{
	return false;
}

void LogAsyncWriter::onEvent_Flush(void)
{
	record();
}

void LogAsyncWriter::notifyShutdown(void)
{
	{
		std::unique_lock<std::mutex> lock(_EventCvMutex);

		_EventFlag_Shutdown = true;
	}
	_EventCv.notify_one();
}

void LogAsyncWriter::notifyFlush(void)
{
	{
		std::unique_lock<std::mutex> lock(_EventCvMutex);

		_EventFlag_Flush = true;
	}
	_EventCv.notify_one();
}

void LogAsyncWriter::record(void)
{
	std::vector<LogItem*>* container;


	_mutex.lock();

	container = _back;
	_back = _front;
	_front = container;

	_mutex.unlock();


	if (_back)
	{
		recordItems(_back);
	}
}

void LogAsyncWriter::recordItems(std::vector<LogItem*>* container)
{
	std::vector<LogItem*>::iterator iter;
	LogItem* m;


	for (iter = container->begin(); iter != container->end(); iter++)
	{
		m = *iter;

		recordItem(m);
	}


	container->clear();
}

void LogAsyncWriter::recordItem(LogItem* m)
{
	output(m);

	LogItem_Free(m);
}

//===========================================================================
void LogAsyncWriter::output(LogItem* m)
{
	std::wstringstream ss;
	writeLogString(ss, *m);
	OutputDebugStringW(ss.str().c_str());
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
