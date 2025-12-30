/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

#if 0

//===========================================================================
#include <cinttypes>
#include <string>

#include <vector>

#include <functional>

//===========================================================================
#include <Windows.h>

#endif

//===========================================================================
#include "auto_critical_section.hpp"

//===========================================================================
#include "rs_comm_port.hpp"

#include "rs_comm.hpp"

#include "rs_comm_port_connector.hpp"
#include "rs_comm_port_reader.hpp"
#include "rs_comm_port_writer.hpp"

#include "rs_comm_port_impl.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace rs_comm_port_internal
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
DWORD WINAPI _rs_comm_port_reader_thread_proc(LPVOID param)
{
	rs_comm_port_reader* instance = reinterpret_cast<rs_comm_port_reader*>(param);


	CX_RUNTIME_LOG(cxLTrace) << L"begin";
	instance->thread_loop();
	CX_RUNTIME_LOG(cxLTrace) << L"end";

	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
rs_comm_port_reader::rs_comm_port_reader() :
	_core(nullptr),

	_thread_handle(nullptr),

	_ready_event_handle(nullptr),
	_io_event_handle(nullptr),
	_io_ov_event_handle(nullptr),

	_state(state::unknown),

	_buffer(),
	_io_ov(),
	_io_buffer(nullptr),
	_io_size(0U),
	_io_transferred(0U)
{

}

rs_comm_port_reader::~rs_comm_port_reader()
{

}

bool rs_comm_port_reader::create(void)
{
	_buffer.resize(8192U);


	if (nullptr == _core)
	{
		return false;
	}

	_ready_event_handle = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (nullptr == _ready_event_handle)
	{
		return false;
	}

	_io_event_handle = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (nullptr == _io_event_handle)
	{
		return false;
	}

	_io_ov_event_handle = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (nullptr == _io_ov_event_handle)
	{
		return false;
	}


	DWORD thread_id;


	_thread_handle = CreateThread(0, 0, _rs_comm_port_reader_thread_proc, this, 0, &thread_id);
	if (nullptr == _io_event_handle)
	{
		return false;
	}


	return true;
}

void rs_comm_port_reader::destroy(void)
{
	if (_thread_handle)
	{
		DWORD dwObject;


		dwObject = WaitForSingleObject(_thread_handle, INFINITE);
	}


	if (_thread_handle)
	{
		CloseHandle(_thread_handle);
		_thread_handle = nullptr;
	}


	if (_io_ov_event_handle)
	{
		CloseHandle(_io_ov_event_handle);
		_io_ov_event_handle = nullptr;
	}


	if (_io_event_handle)
	{
		CloseHandle(_io_event_handle);
		_io_event_handle = nullptr;
	}


	if (_ready_event_handle)
	{
		CloseHandle(_ready_event_handle);
		_ready_event_handle = nullptr;
	}


	_buffer.clear();


	_core = nullptr;
}

void rs_comm_port_reader::set_state(state s)
{
	_state = s;
}

void rs_comm_port_reader::thread_loop(void)
{
	bool loop;
	event e;


	_io_transferred = 0U;
	_io_size = static_cast<DWORD>(_buffer.size());
	_io_buffer = &*_buffer.begin();


	set_state(state::ready);
	do
	{
		e = wait_event();

		loop = on_event(e);

	} while (loop);
}

rs_comm_port_reader::event rs_comm_port_reader::wait_event(void)
{
	event e;


	switch (_state)
	{
	case state::ready:
		SetEvent(_ready_event_handle);
		e = wait_event_state_ready();
		ResetEvent(_ready_event_handle);
		break;

	case state::io:
		e = wait_event_state_io();
		break;

	case state::io_ov:
		e = wait_event_state_io_ov();
		break;

	default:
		e = event::exit;
		break;
	}


	return e;
}

rs_comm_port_reader::event rs_comm_port_reader::wait_event_state_ready(void)
{
	event e;


	HANDLE handles[2];
	DWORD count;
	DWORD timeout;

	DWORD object;


	handles[0] = _core->_exit_event_handle;
	handles[1] = _core->_connector._io_open_event_handle;
	count = 2U;
	timeout = INFINITE;


	object = WaitForMultipleObjects(count, handles, FALSE, timeout);
	switch (object)
	{
	case WAIT_OBJECT_0:
		e = event::exit;
		break;

	case WAIT_OBJECT_0 + 1U:
		e = event::io_open;
		break;

	case WAIT_TIMEOUT:
		e = event::exit;
		break;

	default:
		e = event::exit;
		break;
	}


	return e;
}

rs_comm_port_reader::event rs_comm_port_reader::wait_event_state_io(void)
{
	event e;


	HANDLE handles[3];
	DWORD count;
	DWORD timeout;

	DWORD object;


	handles[0] = _core->_exit_event_handle;
	handles[1] = _core->_connector._io_close_event_handle;
	handles[2] = _io_event_handle;
	count = 3U;
	timeout = INFINITE;


	object = WaitForMultipleObjects(count, handles, FALSE, timeout);
	switch (object)
	{
	case WAIT_OBJECT_0:
		e = event::exit;
		break;

	case WAIT_OBJECT_0 + 1U:
		e = event::ready;
		break;

	case WAIT_OBJECT_0 + 2U:
		e = event::io;
		break;

	case WAIT_TIMEOUT:
		e = event::exit;
		break;

	default:
		e = event::exit;
		break;
	}


	return e;
}

rs_comm_port_reader::event rs_comm_port_reader::wait_event_state_io_ov(void)
{
	event e;


	HANDLE handles[3];
	DWORD count;
	DWORD timeout;

	DWORD object;


	handles[0] = _core->_exit_event_handle;
	handles[1] = _core->_connector._io_close_event_handle;
	handles[2] = _io_ov_event_handle;
	count = 3U;
	timeout = INFINITE;


	object = WaitForMultipleObjects(count, handles, FALSE, timeout);
	switch (object)
	{
	case WAIT_OBJECT_0:
		e = event::io_exit;
		break;

	case WAIT_OBJECT_0 + 1U:
		e = event::io_close;
		break;

	case WAIT_OBJECT_0 + 2U:
		e = event::io_ov;
		break;

	case WAIT_TIMEOUT:
		e = event::exit;
		break;

	default:
		e = event::exit;
		break;
	}


	return e;
}

bool rs_comm_port_reader::on_event(event e)
{
	bool result;


	result = true;

	switch (e)
	{
	case event::exit:
		result = false;
		break;

	case event::ready:
		on_event_ready();
		break;

	case event::io_open:
		on_event_io_open();
		break;

	case event::io:
		on_event_io();
		break;

	case event::io_ov:
		on_event_io_ov();
		break;

	case event::io_close:
		on_event_io_close();
		break;

	case event::io_exit:
		on_event_io_exit();
		result = false;
		break;

	default:
		break;
	}


	return result;
}

void rs_comm_port_reader::on_event_ready(void)
{
	set_state(state::ready);
}

void rs_comm_port_reader::on_event_io_open(void)
{
	SetEvent(_io_event_handle);


	set_state(state::io);
}

void rs_comm_port_reader::on_event_io(void)
{
	_io_transferred = 0U;
	_io_size = static_cast<DWORD>(_buffer.size());
	if (0U == _io_size)
	{
		return;
	}
	_io_buffer = &*_buffer.begin();


	BOOL rv;
	DWORD error;


	ResetEvent(_io_ov_event_handle);


	memset(&_io_ov, 0, sizeof(_io_ov));
	_io_ov.hEvent = _io_ov_event_handle;

	rv = _core->_comm.read(_io_buffer, _io_size, &_io_transferred, &_io_ov);

	if (TRUE == rv)
	{
		//CX_RUNTIME_LOG(cxLTrace) << L"TRUE";

		set_state(state::io_ov);
		return;
	}


	error = GetLastError();
	switch (error)
	{
	case NO_ERROR:
		//CX_RUNTIME_LOG(cxLTrace) << L"NO_ERROR";

	case ERROR_IO_PENDING:
		set_state(state::io_ov);
		return;

	default:
		break;
	}


	rv = CancelIoEx(_core->_comm._file_handle, &_io_ov);
	if (FALSE == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"CancelIoEx() failed";
	}


	SetEvent(_core->_connector._io_error_event_handle);


	set_state(state::ready);
}

void rs_comm_port_reader::on_event_io_ov(void)
{
	BOOL rv;


	rv = GetOverlappedResult(_core->_comm._file_handle, &_io_ov, &_io_transferred, FALSE);
	if (FALSE == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"GetOverlappedResult() failed";

		SetEvent(_io_event_handle);


		set_state(state::io);


		return;
	}


	//CX_RUNTIME_LOG(cxLError) << L"GetOverlappedResult() : " <<  _io_transferred;

	
	if (0U<_io_transferred)
	{
		//CX_RUNTIME_LOG(cxLError) << L"GetOverlappedResult() : " <<  _io_transferred;

		_core->push_rx(reinterpret_cast<std::uint8_t*>(_io_buffer), _io_transferred);
	}


	SetEvent(_io_event_handle);


	set_state(state::io);
}

void rs_comm_port_reader::on_event_io_close(void)
{
	BOOL rv;


	rv = CancelIoEx(_core->_comm._file_handle, &_io_ov);
	if (FALSE == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"CancelIoEx() failed";
	}


	set_state(state::ready);
}

void rs_comm_port_reader::on_event_io_exit(void)
{
	BOOL rv;


	rv = CancelIoEx(_core->_comm._file_handle, &_io_ov);
	if (FALSE == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"CancelIoEx() failed";
	}


	set_state(state::ready);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}
