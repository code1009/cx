/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../log.hpp"

#include "log_item_memory.hpp"
#include "log_item_queue.hpp"
#include "log_writer.hpp"

#include "log_async_writer.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
log_async_writer::log_async_writer() :
	_event_cv{},
	_event_cv_mutex{},
	_event_timeout_msec{1000U},
	_event_flag_shutdown{false},
	_event_flag_flush{false},
	_thread{},
	_mutex{},
	_buffer0{},
	_buffer1{},
	_front{nullptr},
	_back{nullptr}
{
	create();
}

log_async_writer::~log_async_writer()
{
	destroy();
}

//===========================================================================
void log_async_writer::write(const log_item& item)
{
	_mutex.lock();


	if (_front)
	{
		log_item* m = log_item_alloc();
		if(m)
		{
			*m = item;
			_front->push_back(m);
		}
	}


	_mutex.unlock();
}

void log_async_writer::flush(void)
{
	notify_flush();
}

//===========================================================================
bool log_async_writer::create(void)
{
	_buffer0.reserve(1024);
	_buffer1.reserve(1024);

	_front = &_buffer0;
	_back = &_buffer1;


	_thread = std::thread(&log_async_writer::process, this);
	if (!_thread.joinable())
	{
		return false;
	}

	return true;
}

void log_async_writer::destroy(void)
{
	notify_shutdown();

	if (_thread.joinable())
	{
		_thread.join();
	}

	cleanup();
}

//===========================================================================
void log_async_writer::cleanup(void)
{
	if (_back)
	{
		record_items(_back);

		_back = nullptr;
	}

	if (_front)
	{
		record_items(_front);

		_front = nullptr;
	}
}

//===========================================================================
void log_async_writer::process(void)
{
	on_begin();

	if (0U == _event_timeout_msec)
	{
		event_loop();
	}
	else
	{
		event_loop(_event_timeout_msec);
	}

	on_end();
}

//===========================================================================
void log_async_writer::on_begin(void)
{
	SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);
}

void log_async_writer::on_end(void)
{
}

void log_async_writer::event_loop(void)
{
	event e;
	bool loop_flag;


	do
	{
		e = wait_event();
		loop_flag = on_event(e);
	} while (loop_flag);
}

void log_async_writer::event_loop(const std::uint32_t msec)
{
	event e;
	bool loop_flag;

	do
	{
		e = wait_event(msec);
		loop_flag = on_event(e);
	} while (loop_flag);
}

bool log_async_writer::on_event(event e)
{
	bool loop_flag;


	loop_flag = true;
	switch (e)
	{
	case event::timeout:
		on_event_timeout();
		break;

	case event::shutdown:
		loop_flag = on_event_shutdown();
		break;

	case event::flush:
		on_event_flush();
		break;

	case event::unknown:
	default:
		loop_flag = on_event_unknown();
		break;
	}

	return loop_flag;
}

//===========================================================================
bool log_async_writer::check_event(void)
{
	if (_event_flag_shutdown)
	{
		return true;
	}

	if (_event_flag_flush)
	{
		return true;
	}

	return false;
}

log_async_writer::event log_async_writer::get_event(void)
{
	if (_event_flag_shutdown)
	{
		_event_flag_shutdown = false;
		return event::shutdown;
	}

	if (_event_flag_flush)
	{
		_event_flag_flush = false;
		return event::flush;
	}

	return event::unknown;
}

log_async_writer::event log_async_writer::wait_event(void)
{
	std::unique_lock<std::mutex> lock(_event_cv_mutex);


	_event_cv.wait(lock, [this]() { return check_event(); });


	return get_event();
}

log_async_writer::event log_async_writer::wait_event(const std::uint32_t msec)
{
	std::unique_lock<std::mutex> lock(_event_cv_mutex);


	auto rel_time = std::chrono::milliseconds(msec);
	bool rv;


	rv = _event_cv.wait_for(lock, rel_time, [this]() { return check_event(); });
	if (false == rv)
	{
		return event::timeout;
	}


	return get_event();
}

bool log_async_writer::on_event_unknown(void)
{
	return false;
}

void log_async_writer::on_event_timeout(void)
{
	record();
}

bool log_async_writer::on_event_shutdown(void)
{
	return false;
}

void log_async_writer::on_event_flush(void)
{
	record();
}

void log_async_writer::notify_shutdown(void)
{
	{
		std::unique_lock<std::mutex> lock(_event_cv_mutex);

		_event_flag_shutdown = true;
	}
	_event_cv.notify_one();
}

void log_async_writer::notify_flush(void)
{
	{
		std::unique_lock<std::mutex> lock(_event_cv_mutex);

		_event_flag_flush = true;
	}
	_event_cv.notify_one();
}

void log_async_writer::record(void)
{
	std::vector<log_item*>* container;


	_mutex.lock();

	container = _back;
	_back = _front;
	_front = container;

	_mutex.unlock();


	if (_back)
	{
		record_items(_back);
	}
}

void log_async_writer::record_items(std::vector<log_item*>* container)
{
	std::vector<log_item*>::iterator iter;
	log_item* m;


	for (iter = container->begin(); iter != container->end(); iter++)
	{
		m = *iter;

		record_item(m);
	}


	container->clear();
}

void log_async_writer::record_item(log_item* m)
{
	output(m);

	log_item_free(m);
}

//===========================================================================
void log_async_writer::output(log_item* m)
{
	log_stringstream ss;
	write_log_item(ss, *m);
	OutputDebugStringW(ss.str().c_str());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
