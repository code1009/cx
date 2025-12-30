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
DWORD WINAPI _rs_comm_port_connector_thread_proc(LPVOID param)
{
	rs_comm_port_connector* instance = reinterpret_cast<rs_comm_port_connector*>(param);


	CX_RUNTIME_LOG(cxLTrace) << L"begin";
	instance->thread_loop();
	CX_RUNTIME_LOG(cxLTrace) << L"end";

	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
rs_comm_port_connector::rs_comm_port_connector() :
	_core(nullptr),

	_thread_handle(nullptr),

	_ready_event_handle(nullptr),
	_io_event_handle(nullptr),
	_io_ov_event_handle(nullptr),
	_io_open_event_handle(nullptr),
	_io_close_event_handle(nullptr),
	_io_error_event_handle(nullptr),

	_state(state::unknown),
	_io_open(0U),
	_config(),

	_comm_event(0U),
	_io_ov()
{

}

rs_comm_port_connector::~rs_comm_port_connector()
{

}

bool rs_comm_port_connector::create(void)
{
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

	_io_open_event_handle = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (nullptr == _io_open_event_handle)
	{
		return false;
	}

	_io_close_event_handle = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (nullptr == _io_close_event_handle)
	{
		return false;
	}

	_io_error_event_handle = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (nullptr == _io_error_event_handle)
	{
		return false;
	}


	DWORD thread_id;


	_thread_handle = CreateThread(0, 0, _rs_comm_port_connector_thread_proc, this, 0, &thread_id);
	if (nullptr == _io_event_handle)
	{
		return false;
	}


	return true;
}

void rs_comm_port_connector::destroy(void)
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


	if (_io_open_event_handle)
	{
		CloseHandle(_io_open_event_handle);
		_io_open_event_handle = nullptr;
	}


	if (_io_close_event_handle)
	{
		CloseHandle(_io_close_event_handle);
		_io_close_event_handle = nullptr;
	}


	if (_io_error_event_handle)
	{
		CloseHandle(_io_error_event_handle);
		_io_error_event_handle = nullptr;
	}


	if (_ready_event_handle)
	{
		CloseHandle(_ready_event_handle);
		_ready_event_handle = nullptr;
	}


	_core = nullptr;
}

void rs_comm_port_connector::set_state(state s)
{
	_state = s;
}

void rs_comm_port_connector::thread_loop(void)
{
	bool loop;
	event e;


	_core->set_open(0);
	set_state(state::ready);
	do
	{
		e = wait_event();

		loop = on_event(e);

	} while (loop);
}

rs_comm_port_connector::event rs_comm_port_connector::wait_event(void)
{
	event e;


	switch (_state)
	{
	case state::ready:
		SetEvent(_ready_event_handle);
		e = wait_event_state_ready();
		ResetEvent(_ready_event_handle);
		break;

	case state::idle:
		e = wait_event_state_idle();
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

rs_comm_port_connector::event rs_comm_port_connector::wait_event_state_idle(void)
{
	event e;


	HANDLE handles[1];
	DWORD count;
	DWORD timeout;

	DWORD object;


	handles[0] = _core->_exit_event_handle;
	count = 1U;
	timeout = INFINITE;


	object = WaitForMultipleObjects(count, handles, FALSE, timeout);
	switch (object)
	{
	case WAIT_OBJECT_0:
		e = event::exit;
		break;

	case WAIT_TIMEOUT:
		e = event::idle;
		break;

	default:
		e = event::exit;
		break;
	}


	return e;
}

rs_comm_port_connector::event rs_comm_port_connector::wait_event_state_ready(void)
{
	_core->get_config(_config);


	event e;


	HANDLE handles[2];
	DWORD count;
	DWORD timeout;

	DWORD object;


	handles[0] = _core->_exit_event_handle;
	handles[1] = _io_error_event_handle;
	count = 2U;
	timeout = _config._open_interval;


	object = WaitForMultipleObjects(count, handles, FALSE, timeout);
	switch (object)
	{
	case WAIT_OBJECT_0:
		e = event::exit;
		break;

	case WAIT_OBJECT_0 + 1U:
		e = event::ready;
		break;

	case WAIT_TIMEOUT:
		e = event::io_open;
		break;

	default:
		e = event::exit;
		break;
	}


	return e;
}

rs_comm_port_connector::event rs_comm_port_connector::wait_event_state_io(void)
{
	event e;


	HANDLE handles[3];
	DWORD count;
	DWORD timeout;

	DWORD object;


	handles[0] = _core->_exit_event_handle;
	handles[1] = _io_error_event_handle;
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

rs_comm_port_connector::event rs_comm_port_connector::wait_event_state_io_ov(void)
{
	event e;


	HANDLE handles[3];
	DWORD count;
	DWORD timeout;

	DWORD object;


	handles[0] = _core->_exit_event_handle;
	handles[1] = _io_error_event_handle;
	handles[2] = _io_ov_event_handle;
	count = 3U;
	timeout = INFINITE;


	object = WaitForMultipleObjects(count, handles, FALSE, INFINITE);
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

bool rs_comm_port_connector::on_event(event e)
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

	case event::idle:
		on_event_idle();
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

void rs_comm_port_connector::on_event_ready(void)
{
	SetEvent(_io_close_event_handle);


	_core->_comm.close();
	CX_RUNTIME_LOG(cxLTrace) << L"close";


	ResetEvent(_io_error_event_handle);


	set_state(state::ready);


	_core->set_open(2);
}

void rs_comm_port_connector::on_event_idle(void)
{
	CX_RUNTIME_LOG(cxLTrace) << L"idle";
}

bool rs_comm_port_connector::wait_io_ready(void)
{
	HANDLE handles[2];
	DWORD  count;
	DWORD  object;
	DWORD  timeout;


	handles[0] = _core->_reader._ready_event_handle;
	handles[1] = _core->_writer._ready_event_handle;
	count = 2U;
	timeout = INFINITE;

	object = WaitForMultipleObjects(count, handles, TRUE, timeout);
	if (WAIT_OBJECT_0 != object)
	{
		return false;
	}

	return true;
}

void rs_comm_port_connector::on_event_io_open(void)
{
	if (false == wait_io_ready())
	{
		return;
	}


	_core->get_config(_config);


	BOOL rv;

#if 0
	LPCSTR lpPortName = "COM1:";
	DWORD  dwBaudRate = 921600U;
//	DWORD  dwBaudRate = 9600U;
	BYTE   byByteSize = 8U;
	BYTE   byParity   = NOPARITY;
	BYTE   byStopBits = ONESTOPBIT;
#endif

#if 1
	LPCWSTR lpPortName ;
	DWORD   dwBaudRate ;
	BYTE    byByteSize ;
	BYTE    byParity   ;
	BYTE    byStopBits ;
#endif

	lpPortName = _config._device_name.c_str();
	dwBaudRate = _config._baudrate   ;
	byByteSize = _config._byte_size  ;
	byParity   = _config._parity     ;
	byStopBits = _config._stop_bits  ;


	DWORD  dwReadTimeout = 200U;
	DWORD  dwTxQueueSize = 4096U;
	DWORD  dwRxQueueSize = 4096U;


	rv= _core->_comm.open(lpPortName, FILE_FLAG_OVERLAPPED);
	if (TRUE==rv)
	{
		CX_RUNTIME_LOG(cxLTrace) << L"open";


		_core->_comm.set_dcb(dwBaudRate, byByteSize, byParity, byStopBits);
		_core->_comm.set_mask(EV_RXCHAR);
		_core->_comm.set_timeouts(MAXDWORD, MAXDWORD, dwReadTimeout, 0, 0);
		_core->_comm.setup(dwTxQueueSize, dwRxQueueSize);

		_io_open = 1;


		SetEvent(_io_event_handle);
		SetEvent(_io_open_event_handle);


		set_state(state::io);
	}
	else
	{
		CX_RUNTIME_LOG(cxLError) << L"open failed";


		_io_open = 2;


		if (0 == _config._open_interval)
		{
			set_state(state::idle);
		}
		else
		{
			set_state(state::ready);
		}
	}


	_core->set_open(_io_open);
}

void rs_comm_port_connector::on_event_io(void)
{
	_comm_event = 0U;


	BOOL rv;
	DWORD error;


	ResetEvent(_io_ov_event_handle);


	memset(&_io_ov, 0, sizeof(_io_ov));
	_io_ov.hEvent = _io_ov_event_handle;

	rv = _core->_comm.wait_event(&_comm_event, &_io_ov);

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
	CX_RUNTIME_LOG(cxLError) << L"Error : " << error;


	rv = CancelIoEx(_core->_comm._file_handle, &_io_ov);
	if (FALSE == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"CancelIoEx() failed";
	}


	SetEvent(_io_error_event_handle);


	//set_state(state::ready);
}

void rs_comm_port_connector::on_event_io_ov(void)
{
	BOOL rv;



	DWORD  _io_transferred;


	rv = GetOverlappedResult(_core->_comm._file_handle, &_io_ov, &_io_transferred, FALSE);
	if (FALSE == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"GetOverlappedResult() failed";

		//SetEvent(_io_event_handle);
		SetEvent(_io_error_event_handle);

		set_state(state::io);

		return;
	}


	//CX_RUNTIME_LOG(cxLTrace) << L"GetOverlappedResult() : " << _io_transferred; // 항상 4


	SetEvent(_io_event_handle);


	set_state(state::io);


	/*
	DWORD dwMask;

	if (FALSE==_core->_comm.GetCommMask(&dwMask))
	{
		CX_RUNTIME_LOG(cxLError) << L"GetCommMask() failed";
	}
	*/

	/*
	#define EV_RXCHAR           0x0001  // Any Character received
	#define EV_RXFLAG           0x0002  // Received certain character
	#define EV_TXEMPTY          0x0004  // Transmitt Queue Empty
	#define EV_CTS              0x0008  // CTS changed state
	#define EV_DSR              0x0010  // DSR changed state
	#define EV_RLSD             0x0020  // RLSD changed state
	#define EV_BREAK            0x0040  // BREAK received
	#define EV_ERR              0x0080  // Line status error occurred
	#define EV_RING             0x0100  // Ring signal detected
	#define EV_PERR             0x0200  // Printer error occured
	#define EV_RX80FULL         0x0400  // Receive buffer is 80 percent full
	#define EV_EVENT1           0x0800  // Provider specific event 1
	#define EV_EVENT2           0x1000  // Provider specific event 2
	*/

	#if 0
	CX_RUNTIME_LOG(cxLTrace) << L"comm_event: " << _comm_event;
	if (_comm_event & EV_RXCHAR  ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_RXCHAR  "; }
	if (_comm_event & EV_RXFLAG  ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_RXFLAG  "; }
	if (_comm_event & EV_TXEMPTY ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_TXEMPTY "; }
	if (_comm_event & EV_CTS     ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_CTS     "; }
	if (_comm_event & EV_DSR     ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_DSR     "; }
	if (_comm_event & EV_RLSD    ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_RLSD    "; }
	if (_comm_event & EV_BREAK   ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_BREAK   "; }
	if (_comm_event & EV_ERR     ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_ERR     "; }
	if (_comm_event & EV_RING    ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_RING    "; }
	if (_comm_event & EV_PERR    ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_PERR    "; }
	if (_comm_event & EV_RX80FULL){ CX_RUNTIME_LOG(cxLTrace) << L"EV_RX80FULL"; }
	if (_comm_event & EV_EVENT1  ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_EVENT1  "; }
	if (_comm_event & EV_EVENT2  ){ CX_RUNTIME_LOG(cxLTrace) << L"EV_EVENT2  "; }
	#endif

//	if (_comm_event & EV_RXCHAR  ){ SetEvent (_core->_reader._io_event_handle); }

/*
	if (_comm_event & EV_RXFLAG  ){}
	if (_comm_event & EV_TXEMPTY ){}
	if (_comm_event & EV_CTS     ){}
	if (_comm_event & EV_DSR     ){}
	if (_comm_event & EV_RLSD    ){}
	if (_comm_event & EV_BREAK   ){}
	if (_comm_event & EV_ERR     ){}
	if (_comm_event & EV_RING    ){}
	if (_comm_event & EV_PERR    ){}
	if (_comm_event & EV_RX80FULL){}
	if (_comm_event & EV_EVENT1  ){}
	if (_comm_event & EV_EVENT2  ){}
*/
}

void rs_comm_port_connector::on_event_io_close(void)
{
	BOOL rv;


	rv = CancelIoEx(_core->_comm._file_handle, &_io_ov);
	if (FALSE == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"CancelIoEx() failed";
	}


	set_state(state::ready);
}

void rs_comm_port_connector::on_event_io_exit(void)
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
