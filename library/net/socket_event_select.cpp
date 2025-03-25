/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

//===========================================================================
#include "../runtime/runtime.hpp"

//===========================================================================
#include "socket_descriptor.hpp"
#include "socket_address.hpp"
#include "socket_event_select.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class socket_event_select::impl
{
public:
	socket_event_select* _socket_event_selector{ nullptr };
	HANDLE _descriptor{ WSA_INVALID_EVENT };
	std::vector<HANDLE> _events;

public:
	explicit impl(socket_event_select* p);
	~impl();

public:
	bool create(void);
	void destroy(void);

public:
	bool select(socket_descriptor_t socket_descriptor, std::uint32_t socket_event_flag);

public:
	std::uint32_t wait(socket_descriptor_t socket_descriptor, std::uint32_t timeout_msec);
	std::uint32_t enum_network_events(socket_descriptor_t socket_descriptor);

public:
	void on_event(std::uint32_t event_index);
	void on_socket_event_accept(void);
	void on_socket_event_connect(void);
	void on_socket_event_write(void);
	void on_socket_event_read(void);
	void on_socket_event_close(void);
};

//===========================================================================
socket_event_select::impl::impl(socket_event_select* p):
	_socket_event_selector{ p }
{
}

socket_event_select::impl::~impl()
{
}


bool socket_event_select::impl::create(void)
{
	//-----------------------------------------------------------------------
	HANDLE event_handle;


	event_handle = WSACreateEvent();
	if (WSA_INVALID_EVENT == event_handle)
	{
		CX_RUNTIME_LOG(cxLError) << L"WSACreateEvent() failed";
		return false;
	}


	//-----------------------------------------------------------------------
	_descriptor = event_handle;


	//-----------------------------------------------------------------------
	_events.clear();
	_events.push_back(_descriptor);

	return true;
}

void socket_event_select::impl::destroy(void)
{
	//-----------------------------------------------------------------------
	_events.clear();


	//-----------------------------------------------------------------------
	if (WSA_INVALID_EVENT == _descriptor)
	{
		return;
	}


	//-----------------------------------------------------------------------
	BOOL rv;


	rv = WSACloseEvent(_descriptor);
	if (FALSE == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"WSACloseEvent() failed";
	}

	_descriptor = WSA_INVALID_EVENT;
}

bool socket_event_select::impl::select(socket_descriptor_t socket_descriptor, std::uint32_t socket_event_flag)
{
	DWORD dwSocketEvent;
	long  lNetworkEvents;

	
	lNetworkEvents = 0;
	if (socket_event_flag & socket_event_flag_accept ){ lNetworkEvents|= FD_ACCEPT; }
	if (socket_event_flag & socket_event_flag_connect){ lNetworkEvents|= FD_CONNECT; }
	if (socket_event_flag & socket_event_flag_write  ){ lNetworkEvents|= FD_WRITE; }
	if (socket_event_flag & socket_event_flag_read   ){ lNetworkEvents|= FD_READ; }
	if (socket_event_flag & socket_event_flag_close  ){ lNetworkEvents|= FD_CLOSE; }


	dwSocketEvent  = WSAEventSelect(socket_descriptor, _descriptor, lNetworkEvents);
	if (SOCKET_ERROR == dwSocketEvent)
	{
		CX_RUNTIME_LOG(cxLError) << L"WSAEventSelect() failed";
		return false;
	}


	return true;
}

std::uint32_t socket_event_select::impl::wait(socket_descriptor_t socket_descriptor, std::uint32_t timeout_msec)
{
	//-----------------------------------------------------------------------
	std::uint32_t result;


	//-----------------------------------------------------------------------
	DWORD         nCount        ;
	CONST HANDLE* lpHandles     ;
	BOOL          bWaitAll      ;
	DWORD         dwMilliseconds;


	nCount = (DWORD)_events.size();
	lpHandles = _events.data();
	bWaitAll = FALSE;
	if (timeout_msec != socket_wait_infinite)
	{
		dwMilliseconds = timeout_msec;
	}
	else
	{
		//dwMilliseconds = WSA_INFINITE;
		dwMilliseconds = INFINITE;
	}


	//-----------------------------------------------------------------------
	DWORD dwObject;


	//dwObject = ::WSAWaitForMultipleEvents(nCount, lpHandles, bWaitAll, dwMilliseconds, FALSE);
	dwObject = WaitForMultipleObjects(nCount, lpHandles, bWaitAll, dwMilliseconds);
	switch (dwObject)
	{
	//WSA_WAIT_FAILED
	case WAIT_FAILED:
		result = socket_event_error;
		break;

	//WSA_WAIT_TIMEOUT
	case WAIT_TIMEOUT:
		result = socket_event_timeout;
		break;

	//WSA_WAIT_EVENT_0
	case WAIT_OBJECT_0:
		result = enum_network_events(socket_descriptor);
		break;

	//case WAIT_OBJECT_0 + 1u:
	//case WAIT_ABANDONED_0:
	//	break;

	default: 
		if ((WAIT_OBJECT_0) < dwObject && dwObject < (WAIT_OBJECT_0 + _events.size()))
		{
			result = socket_event_noerror;
			on_event(dwObject - WAIT_OBJECT_0);
		}
		else
		{
			// Exception
			result = socket_event_error;
		}
		break;
	}


	//-----------------------------------------------------------------------

	return result;
}

std::uint32_t socket_event_select::impl::enum_network_events(socket_descriptor_t socket_descriptor)
{
	//-----------------------------------------------------------------------
	WSANETWORKEVENTS NetworkEvents;
	DWORD            dwNetworkEvents;
	int              iErrorCode;


	dwNetworkEvents = WSAEnumNetworkEvents(socket_descriptor, _descriptor, &NetworkEvents);
	if (SOCKET_ERROR == dwNetworkEvents)
	{
		CX_RUNTIME_LOG(cxLError) << L"WSAEnumNetworkEvents() failed : WSAGetLastError()=0x" << std::hex << WSAGetLastError();
		return socket_event_error;
	}

	
	//-----------------------------------------------------------------------
	std::uint32_t result;


	result = socket_event_noerror;


	if ((NetworkEvents.lNetworkEvents & FD_ACCEPT))
	{
		// OnAccept()
		iErrorCode = NetworkEvents.iErrorCode[FD_ACCEPT_BIT];
		if (0 == iErrorCode)
		{
			result = socket_event_noerror;

			on_socket_event_accept();
		}
		else
		{
			result = socket_event_error;
			CX_RUNTIME_LOG(cxLError) << L"FD_ACCEPT : iErrorCode=" << iErrorCode << L" WSAGetLastError()=" << WSAGetLastError();
		}
	}

	if (NetworkEvents.lNetworkEvents & FD_CONNECT)
	{
		// OnConnect()
		iErrorCode = NetworkEvents.iErrorCode[FD_CONNECT_BIT];
		if (0 == iErrorCode)
		{
			result = socket_event_noerror;

			on_socket_event_connect();
		}
		else
		{
			result = socket_event_error;
			CX_RUNTIME_LOG(cxLError) << L"FD_CONNECT : iErrorCode=" << iErrorCode << L" WSAGetLastError()=" << WSAGetLastError();
		}
	}

	if (NetworkEvents.lNetworkEvents & FD_WRITE)
	{
		// OnSend
		iErrorCode = NetworkEvents.iErrorCode[FD_WRITE_BIT];
		if (0 == iErrorCode)
		{
			result = socket_event_noerror;
			
			on_socket_event_write();
		}
		else
		{
			result = socket_event_error;
			CX_RUNTIME_LOG(cxLError) << L"FD_WRITE : iErrorCode=" << iErrorCode << L" WSAGetLastError()=" << WSAGetLastError();
		}
	}

	if (NetworkEvents.lNetworkEvents & FD_READ)
	{
		// OnReceive
		iErrorCode = NetworkEvents.iErrorCode[FD_READ_BIT];
		if (0 == iErrorCode)
		{
			result = socket_event_noerror;

			on_socket_event_read();
		}
		else
		{
			result = socket_event_error;
			CX_RUNTIME_LOG(cxLError) << L"FD_READ : iErrorCode=" << iErrorCode << L" WSAGetLastError()=" << WSAGetLastError();
		}
	}

	if (NetworkEvents.lNetworkEvents & FD_CLOSE)
	{
		//OnClose
		iErrorCode = NetworkEvents.iErrorCode[FD_CLOSE_BIT];
		if (0 == iErrorCode)
		{
			result = socket_event_noerror;

			on_socket_event_close();
		}
		else
		{
			result = socket_event_error;
			CX_RUNTIME_LOG(cxLError) << L"FD_CLOSE : iErrorCode=" << iErrorCode << L" WSAGetLastError()=" << WSAGetLastError();
		}
	}


	return result;
}

void socket_event_select::impl::on_socket_event_accept(void)
{
	if (_socket_event_selector->on_socket_event_accept)
	{
		_socket_event_selector->on_socket_event_accept();
	}
}

void socket_event_select::impl::on_socket_event_connect(void)
{
	if (_socket_event_selector->on_socket_event_connect)
	{
		_socket_event_selector->on_socket_event_connect();
	}
}

void socket_event_select::impl::on_socket_event_write(void)
{
	if (_socket_event_selector->on_socket_event_write)
	{
		_socket_event_selector->on_socket_event_write();
	}
}

void socket_event_select::impl::on_socket_event_read(void)
{
	if (_socket_event_selector->on_socket_event_read)
	{
		_socket_event_selector->on_socket_event_read();
	}
}

void socket_event_select::impl::on_socket_event_close(void)
{
	if (_socket_event_selector->on_socket_event_close)
	{
		_socket_event_selector->on_socket_event_close();
	}
}

void socket_event_select::impl::on_event(std::uint32_t event_index)
{
	if (_socket_event_selector->on_event)
	{
		_socket_event_selector->on_event(event_index);
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
socket_event_select::socket_event_select()
{
	_pimpl = std::make_unique<impl>(this);
}

socket_event_select::~socket_event_select()
{
}

//===========================================================================
bool socket_event_select::create(void)
{
	return _pimpl->create();
}

void socket_event_select::destroy(void)
{
	_pimpl->destroy();
}

bool socket_event_select::register_event(HANDLE event_handle)
{
	std::size_t count;

	count = _pimpl->_events.size();
	if ((0<count) && (count<64))
	{
		_pimpl->_events.push_back(event_handle);

		return true;
	}

	return false;
}

bool socket_event_select::select(socket_descriptor_t socket_descriptor, std::uint32_t socket_event_flag)
{
	return _pimpl->select(socket_descriptor, socket_event_flag);
}

std::uint32_t socket_event_select::wait(socket_descriptor_t socket_descriptor, std::uint32_t timeout_msec)
{
	return _pimpl->wait(socket_descriptor, timeout_msec);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}


