/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <runtime/runtime.hpp>

#include <component/charset_system.hpp>

#include <network/socket_descriptor.hpp>
#include <network/socket_address.hpp>
#include <network/socket.hpp>
#include <network/socket_error.hpp>

#include <network/socket_event_select.hpp>

#include <network/net_msg.hpp>
#include <network/net_msg_memory.hpp>
#include <network/net_msg_event_queue.hpp>

#include <network/net_addr_config.hpp>

#include <network/wsa.hpp>

//===========================================================================
#include "UDPTerminal.hpp"
#include "Model.hpp"




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class udp_unicast
{
public:
	cx::network::net_addr_config _local_addr_config{};
	cx::network::net_addr_config _remote_addr_config{};

public:
	cx::network::net_msg_event_queue _tx_queue{};
	cx::network::net_msg_event_queue _rx_queue{};

private:
	cx::network::socket _socket{};
	cx::network::socket_address _socket_local_address{};
	cx::network::socket_address _socket_remote_address{};
	cx::network::socket_event_select _socket_event_select{};

private:
	std::thread _thread{};
	
	bool _run{ false };
	bool _socket_event_loop{ false };
	DWORD _rerun_sleep_time{ 3000 };

	HANDLE _destroy_event_handle{ nullptr };

	std::promise<void> _start_barrier;
	std::future<void> _start_barrier_future;

public:
	udp_unicast();
	~udp_unicast();

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
	bool setup_socket(void);
	void socket_event_loop(void);

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
udp_unicast::udp_unicast()
{
}

udp_unicast::~udp_unicast()
{
}

//===========================================================================
bool udp_unicast::create(void)
{
	//-----------------------------------------------------------------------
	_destroy_event_handle = CreateEvent(NULL, TRUE, FALSE, NULL);


	//-----------------------------------------------------------------------
	_run = true;
	_socket_event_loop = true;


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
	if (_destroy_event_handle)
	{
		SetEvent(_destroy_event_handle);
	}


	if (_thread.joinable())
	{
		_thread.join();
	}


	if (_destroy_event_handle)
	{
		CloseHandle(_destroy_event_handle);
		_destroy_event_handle = nullptr;
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
		if (!_run)
		{
			Sleep(_rerun_sleep_time);
		}
	}
}

void udp_unicast::run(void)
{
	//-----------------------------------------------------------------------
	bool rv;


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo) << L"begin";


	//-----------------------------------------------------------------------
	cleanup_tx_queue();


	//-----------------------------------------------------------------------
	rv = setup_socket();
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"socket_setup() failed";
		return;
	}


	//-----------------------------------------------------------------------
	socket_event_loop();


	//-----------------------------------------------------------------------
	_socket_event_select.destroy();
	_socket.destroy();


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo) << L"end";
}

bool udp_unicast::setup_socket(void)
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
	_socket_local_address.set_inet_v4(_local_addr_config._addr, _local_addr_config._port);
	_socket_remote_address.set_inet_v4(_remote_addr_config._addr, _remote_addr_config._port);


	//-----------------------------------------------------------------------
	rv = _socket.bind(&_socket_local_address);
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
	_socket_event_select.register_event(_destroy_event_handle);
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

void udp_unicast::socket_event_loop(void)
{
	std::uint32_t rv;


	CX_RUNTIME_LOG(cxLInfo) << L"begin";


	while (_socket_event_loop)
	{
		rv = _socket_event_select.wait(_socket.get_descriptor());
		if (rv == cx::network::socket_event_error)
		{
			CX_RUNTIME_LOG(cxLError) << L"_socket_event_select.wait(): socket_event_error";

			break;
		}
	}


	CX_RUNTIME_LOG(cxLInfo) << L"end";
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
	_socket_event_loop = false;
	_run = false;


	//-----------------------------------------------------------------------
	CX_RUNTIME_LOG(cxLInfo) << L"on_event_destroy(): " << cx::mbcs_to_wcs(_local_addr_config._port);
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
	CX_RUNTIME_LOG(cxLInfo) << L"on_socket_event_close(): " << cx::mbcs_to_wcs(_local_addr_config._port);

	_socket_event_loop = false;
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
	rv = _socket.sendto(&_socket_remote_address, pointer, size, &wsize);
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.sendto() failed: " << cx::mbcs_to_wcs(_local_addr_config._port);
		CX_RUNTIME_LOG(cxLError) << L"Socket Error: " << cx::network::get_socket_error();
	}
	else
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.sendto(): " << wsize << L"Byte(s)";
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

	std::uint8_t buffer[1024];

	cx::network::socket_address _socket_remote_address;


	pointer = static_cast<void*>(buffer);
	size = sizeof(buffer);
	rv = _socket.recvfrom(&_socket_remote_address, pointer, size, &rsize);
	if (false == rv)
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.recvfrom() failed: " << cx::mbcs_to_wcs(_local_addr_config._port);
		CX_RUNTIME_LOG(cxLError) << L"Socket Error: " << cx::network::get_socket_error();
	}
	else
	{
		CX_RUNTIME_LOG(cxLError) << L"_socket.recvfrom(): " << rsize << L"Byte(s)";
		on_recv(_socket_remote_address, buffer, rsize);
	}
}

void udp_unicast::on_recv(cx::network::socket_address& socket_remote_address, std::uint8_t* packet_pointer, std::size_t packet_size)
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


	_rx_queue.push(m);
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
UDPTerminal::UDPTerminal(Model* model, std::wstring address, std::wstring port) :
	_Model{ model },
	_Address{ address },
	_Port{ port }
{
}

bool UDPTerminal::initialize(void)
{

	return true;
}

void UDPTerminal::terminate(void)
{
}

