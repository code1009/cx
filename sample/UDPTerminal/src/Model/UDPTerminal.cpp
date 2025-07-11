/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <common/charset_system.hpp>
#include <common/to_std_wstring.hpp>
#include <runtime/runtime.hpp>
#include <network/network.hpp>
#include <wui/wui.hpp>

//===========================================================================
#include "../WebUI/WebUI.hpp"

#include "UDPTerminal.hpp"
#include "Model.hpp"
#include "Utility.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class udp_unicast
{
public:
	UDPTerminal* _UDPTerminal{ nullptr };

public:
	cx::network::net_addr_config _addr_config_local{};
	cx::network::net_addr_config _addr_config_remote{};

public:
	cx::network::net_msg_event_queue _tx_queue{};
	cx::network::net_msg_event_queue _rx_queue{};

private:
	cx::network::socket              _socket{};
	cx::network::socket_address      _socket_address_local{};
	cx::network::socket_address      _socket_address_remote{};
	cx::network::socket_event_select _socket_event_select{};

private:
	std::thread _thread{};
	
private:
	std::atomic_bool _run       { false };
	std::atomic_bool _event_loop{ false };

private:
	DWORD _rerun_sleep_time{ 3000 };

private:
	HANDLE _event_handle_destroy{ nullptr };

private:
	std::promise<void> _start_barrier        {};
	std::future <void> _start_barrier_future {};

public:
	udp_unicast() = default;
	~udp_unicast()= default;

public:
	bool create(void);
	void destroy(void);

private:
	void cleanup(void);
	void cleanup_tx_queue(void);
	void cleanup_rx_queue(void);

private:
	void process(void);

private:
	void run(void);

private:
	bool setup(void);
	void event_loop(void);

private:
	void on_event(std::uint32_t event_index);
	void on_event_destroy(void);
	void on_event_send(void);
	void on_socket_event_read(void);
	void on_socket_event_close(void);

private:
	void do_send(void);
	void do_recv(void);
	void on_recv(cx::network::socket_address& socket_remote_address, std::uint8_t* packet_pointer, std::size_t packet_size);

public:
	std::size_t recv(void* packet_pointer, std::size_t packet_size);
	void        send(void* packet_pointer, std::size_t packet_size);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool udp_unicast::create(void)
{
	//-----------------------------------------------------------------------
	_event_handle_destroy = CreateEvent(NULL, TRUE, FALSE, NULL);


	//-----------------------------------------------------------------------
	_run = true;
	_event_loop = true;


	//-----------------------------------------------------------------------
	_start_barrier_future = _start_barrier.get_future();


	//-----------------------------------------------------------------------
	_thread = std::thread(&udp_unicast::process, this);
	if (!_thread.joinable())
	{
		return false;
	}


	//-----------------------------------------------------------------------
	//_start_barrier_future.wait();
	_start_barrier_future.get();

	return true;
}

void udp_unicast::destroy(void)
{
	_run = false;
	_event_loop = false;


	if (_event_handle_destroy)
	{
		SetEvent(_event_handle_destroy);
	}


	if (_thread.joinable())
	{
		_thread.join();
	}


	if (_event_handle_destroy)
	{
		CloseHandle(_event_handle_destroy);
		_event_handle_destroy = nullptr;
	}


	cleanup();
}

void udp_unicast::cleanup(void)
{
	cleanup_tx_queue();
	cleanup_rx_queue();
}

void udp_unicast::cleanup_tx_queue(void)
{
	std::size_t i;
	std::size_t count;

	cx::network::net_msg* m;


	count = _tx_queue.count();
	CX_RUNTIME_LOG(cxLInfo) << L"cleanup(): _tx_queue.count=" << count;

	for (i = 0U; i < count; i++)
	{
		m = _tx_queue.pop();
		if (m)
		{
			cx::network::net_msg_free(m);
		}
	}
}

void udp_unicast::cleanup_rx_queue(void)
{
	std::size_t i;
	std::size_t count;

	cx::network::net_msg* m;


	count = _rx_queue.count();
	CX_RUNTIME_LOG(cxLInfo) << L"cleanup(): _rx_queue.count=" << count;


	for (i = 0U; i < count; i++)
	{
		m = _rx_queue.pop();
		if (m)
		{
			cx::network::net_msg_free(m);
		}
	}
}

//===========================================================================
void udp_unicast::process(void)
{
	//-----------------------------------------------------------------------
	_start_barrier.set_value(); // signal!!!


	//-----------------------------------------------------------------------
	while (_run)
	{
		run();
		if (_run)
		{
			Sleep(_rerun_sleep_time);
		}
	}
}

void udp_unicast::run(void)
{
	//-----------------------------------------------------------------------
	_UDPTerminal->postWebMessage(L"연결...");


	//-----------------------------------------------------------------------
	bool rv;


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo) << L"begin";


	//-----------------------------------------------------------------------
	cleanup_tx_queue();


	//-----------------------------------------------------------------------
	rv = setup();
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"setup() failed";
		return;
	}


	//-----------------------------------------------------------------------
	event_loop();


	//-----------------------------------------------------------------------
	_socket_event_select.destroy();
	_socket.destroy();


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo) << L"end";
}

bool udp_unicast::setup(void)
{
	//-----------------------------------------------------------------------
	bool rv;


	//-----------------------------------------------------------------------
	rv = _socket.create();
	if(false==rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.create() failed";
		return false;
	}


	//-----------------------------------------------------------------------
	_socket_address_local.set_inet_v4(_addr_config_local._addr, _addr_config_local._port);
	_socket_address_remote.set_inet_v4(_addr_config_remote._addr, _addr_config_remote._port);


	//-----------------------------------------------------------------------
	rv = _socket.bind(&_socket_address_local);
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.bind() failed";
		_socket.destroy();
		return false;
	}


	//-----------------------------------------------------------------------
	rv = _socket_event_select.create();
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket_event_select.create() failed";
		_socket.destroy();
		return false;
	}


	//-----------------------------------------------------------------------
	_socket_event_select.on_event = std::bind(&udp_unicast::on_event, this, std::placeholders::_1);
	_socket_event_select.on_socket_event_read = std::bind(&udp_unicast::on_socket_event_read, this);
	_socket_event_select.on_socket_event_close = std::bind(&udp_unicast::on_socket_event_close, this);


	//-----------------------------------------------------------------------
	_socket_event_select.register_event(_event_handle_destroy);
	_socket_event_select.register_event(_tx_queue._event);


	//-----------------------------------------------------------------------
	rv = _socket_event_select.select(_socket.get_descriptor());
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket_event_select.select() failed";
		_socket_event_select.destroy();
		_socket.destroy();
		return false;
	}


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo) << L"ok";


	return true;
}

void udp_unicast::event_loop(void)
{
	//-----------------------------------------------------------------------
	_UDPTerminal->postWebMessage(L"연결완료");


	//-----------------------------------------------------------------------
	std::uint32_t rv;


	CX_RUNTIME_LOG(cxLInfo) << L"begin";


	while (_event_loop)
	{
		rv = _socket_event_select.wait(_socket.get_descriptor());
		if (rv == cx::network::socket_event_error)
		{
			CX_RUNTIME_LOG(cxLError) << L"_socket_event_select.wait(): socket_event_error";

			break;
		}
	}


	CX_RUNTIME_LOG(cxLInfo) << L"end";


	//-----------------------------------------------------------------------
	_UDPTerminal->postWebMessage(L"연결해제");
}

void udp_unicast::on_event(std::uint32_t event_index)
{
	switch (event_index)
	{
	case 1:
		on_event_destroy();
		break;

	case 2:
		on_event_send();
		break;

	default:
		CX_RUNTIME_LOG(cxLError) << L"?";
		break;
	}
}

void udp_unicast::on_event_destroy(void)
{
	//-----------------------------------------------------------------------
	_event_loop = false;
	_run = false;


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo) << L"on_event_destroy(): " << cx::mbcs_to_wcs(_addr_config_local._port);
}

void udp_unicast::on_event_send(void)
{
	std::size_t i;
	std::size_t count;


	count = _tx_queue.count();
	for (i = 0; i < count; i++)
	{
		do_send();
	}
}

void udp_unicast::on_socket_event_read(void)
{
	do_recv();
}

void udp_unicast::on_socket_event_close(void)
{
	CX_RUNTIME_LOG(cxLInfo) << L"on_socket_event_close(): " << cx::mbcs_to_wcs(_addr_config_local._port);

	_event_loop = false;
}

//===========================================================================
void udp_unicast::do_send(void)
{
	//-----------------------------------------------------------------------
	cx::network::net_msg* m;


	//-----------------------------------------------------------------------
	m = _tx_queue.pop();
	if (m == nullptr)
	{
		return;
	}


	//-----------------------------------------------------------------------
	bool rv;
	std::int32_t wsize;
	std::int32_t size;
	void* pointer;


	pointer = static_cast<void*>(m->_data.data());
	size = static_cast<std::int32_t>(m->_data.size());
	rv = _socket.sendto(&_socket_address_remote, pointer, size, &wsize);
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.sendto() failed: " << cx::mbcs_to_wcs(_addr_config_local._port);
		CX_RUNTIME_LOG(cxLError) << L"Socket Error: " << cx::network::get_socket_error();
	}
	else
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.sendto(): " << wsize << L"Byte(s)";
	}


	//-----------------------------------------------------------------------
	if (true == rv)
	{
		std::wstring message;
		message = L"송신: ";
		message += L" (";
		message += std::format(L"{:04}",m->_data.size());
		message += L") ";
		message += byteArrayToHexString(m->_data);
		_UDPTerminal->postWebMessage(message);
	}
	else
	{
		std::wstring message;
		message = L"송신실패:";
		message += L" (";
		message += std::format(L"{:04}",m->_data.size());
		message += L") ";
		message += byteArrayToHexString(m->_data);
		_UDPTerminal->postWebMessage(message);
	}


	//-----------------------------------------------------------------------
	cx::network::net_msg_free(m);
}

void udp_unicast::do_recv(void)
{
	bool rv;
	std::int32_t rsize;
	std::int32_t size;
	void* pointer;

	std::uint8_t buffer[1500];

	cx::network::socket_address _socket_address_remote;


	pointer = static_cast<void*>(buffer);
	size = sizeof(buffer);
	rv = _socket.recvfrom(&_socket_address_remote, pointer, size, &rsize);
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.recvfrom() failed: " << cx::mbcs_to_wcs(_addr_config_local._port);
		CX_RUNTIME_LOG(cxLError) << L"Socket Error: " << cx::network::get_socket_error();
	}
	else
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.recvfrom(): " << rsize << L"Byte(s)";
		on_recv(_socket_address_remote, buffer, rsize);
	}


	//-----------------------------------------------------------------------
	if (true == rv)
	{
		std::vector<std::uint8_t> data(buffer, buffer + rsize);
		std::wstring message;
		message = L"수신: ";
		message += L" (";
		message += std::format(L"{:04}",data.size());
		message += L") ";
		message += byteArrayToHexString(data);
		_UDPTerminal->postWebMessage(message);
	}
	else
	{
		std::wstring message;
		message = L"수신실패: ";
		_UDPTerminal->postWebMessage(message);
	}
}

void udp_unicast::on_recv(cx::network::socket_address& socket_remote_address, std::uint8_t* packet_pointer, std::size_t packet_size)
{
#if 0
	cx::network::net_msg* m;


	m = cx::network::net_msg_alloc();
	if (nullptr == m)
	{
		CX_RUNTIME_LOG(cxLError) << L"cx::network::net_msg_alloc() failed";
		return;
	}


	std::uint8_t* pointer;
	std::size_t size;


	pointer = static_cast<std::uint8_t*>(packet_pointer);
	size = packet_size;
	m->_data.insert(m->_data.begin(), pointer, pointer + size);


	_rx_queue.push(m);
#endif
}

//===========================================================================
void udp_unicast::send(void* packet_pointer, std::size_t packet_size)
{
	cx::network::net_msg* m;


	m = cx::network::net_msg_alloc();
	if (nullptr == m)
	{
		CX_RUNTIME_LOG(cxLError) << L"cx::network::net_msg_alloc() failed";
		return;
	}


	std::uint8_t* pointer;
	std::size_t size;


	pointer = static_cast<std::uint8_t*>(packet_pointer);
	size = packet_size;
	m->_data.insert(m->_data.begin(), pointer, pointer + size);


	_tx_queue.push(m);
}

std::size_t udp_unicast::recv(void* packet_pointer, std::size_t packet_size)
{
	//-----------------------------------------------------------------------
	std::size_t copy_size = 0;


	//-----------------------------------------------------------------------
	cx::network::net_msg* m;


	//-----------------------------------------------------------------------
	m = _rx_queue.pop();
	if (m == nullptr)
	{
		return copy_size;
	}


	//-----------------------------------------------------------------------
	if (m->_data.size() <= packet_size)
	{
		copy_size = m->_data.size();
	}
	else
	{
		copy_size = packet_size;
	}
	memcpy(packet_pointer, m->_data.data(), copy_size);


	//-----------------------------------------------------------------------
	cx::network::net_msg_free(m);


	return copy_size;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
UDPTerminal::UDPTerminal(Model* model, std::wstring laddress, std::wstring lport, std::wstring raddress, std::wstring rport) :
	_Model{ model },
	_LocalAddress{ laddress },
	_LocalPort{ lport },
	_RemoteAddress{ raddress },
	_RemotePort{ rport }
{
	_udp_unicast = std::make_unique<udp_unicast>();
	_udp_unicast->_UDPTerminal = this;
	_udp_unicast->_addr_config_local._addr = cx::wcs_to_mbcs(_LocalAddress);
	_udp_unicast->_addr_config_local._port = cx::wcs_to_mbcs(_LocalPort);
	_udp_unicast->_addr_config_remote._addr = cx::wcs_to_mbcs(_RemoteAddress);
	_udp_unicast->_addr_config_remote._port = cx::wcs_to_mbcs(_RemotePort);
	
	_udp_unicast->create();

	CX_RUNTIME_LOG(cxLError) << L"UDPTerminal()";
}

UDPTerminal::~UDPTerminal()
{
	_udp_unicast->destroy();
	_udp_unicast.reset();

	CX_RUNTIME_LOG(cxLError) << L"~UDPTerminal()";
}

void UDPTerminal::send(const std::vector<std::uint8_t>& data)
{
	if (_udp_unicast)
	{
		_udp_unicast->send((void*)data.data(), data.size());
	}
}

void UDPTerminal::postWebMessage(std::wstring_view message, std::wstring command)
{
	std::wstringstream ss;

	SYSTEMTIME st;
	GetLocalTime(&st);


	ss.width(2);
	ss.fill(L'0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st.wHour;

	ss << L":";

	ss.width(2);
	ss.fill('0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st.wMinute;

	ss << L":";

	ss.width(2);
	ss.fill('0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st.wSecond;

	ss << L".";

	ss.width(3);
	ss.fill(L'0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st.wMilliseconds;

	ss << L" ";
	ss << message;


	_Model->_WebUIManager->getMessageService()->writeWebMessage(command, ss.str());
}

