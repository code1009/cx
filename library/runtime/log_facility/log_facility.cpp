/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../log.hpp"

#include "log_facility.hpp"

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
static log_writer* _log_writer = nullptr;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool log_facility_initialize(void)
{
	bool rv;

	rv = log_item_memory_initialize();
	if (!rv)
	{
		return false;
	}

#if 1
	_log_writer = cpp_new log_async_writer();
#else
	_log_writer = cpp_new log_writer();
#endif

	get_logger()->set_output_handler(
		std::bind(&log_writer::write, _log_writer, std::placeholders::_1)
	);

	return true;
}

void log_facility_cleanup(void)
{
	if (_log_writer)
	{
		cpp_delete _log_writer;
		_log_writer = nullptr;
	}

	log_item_memory_cleanup();
}

void log_facility_flush(void)
{
	if (_log_writer)
	{
		_log_writer->flush();
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




