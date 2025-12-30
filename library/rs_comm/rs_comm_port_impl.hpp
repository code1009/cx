#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace rs_comm_port_internal
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class rs_comm_port_impl
{
public:
	auto_critical_section _mutex;

	rs_comm_port_config _config;
	std::uint8_t _open{ 0 };
	std::vector<std::uint8_t> _rx_buffer;
	std::vector<std::uint8_t> _tx_buffer;

public:
	rs_comm _comm;

	rs_comm_port_connector _connector;
	rs_comm_port_writer    _writer;
	rs_comm_port_reader    _reader;

	HANDLE _exit_event_handle;

public:
	rs_comm_port::event_handler _event_handler_open;
	rs_comm_port::event_handler _event_handler_close;
	rs_comm_port::event_handler _event_handler_recv;

public:
	rs_comm_port_impl();
	~rs_comm_port_impl();

public:
	void set_event_handler_open(rs_comm_port::event_handler handler);
	void set_event_handler_close(rs_comm_port::event_handler handler);
	void set_event_handler_recv(rs_comm_port::event_handler handler);

public:
	bool create(const rs_comm_port_config& config);
	void destroy(void);

public:
	bool is_open(void);
	bool is_rx_empty(void);
	bool is_tx_empty(void);

public:
	std::size_t recv(void* pointer, std::size_t size);
	std::size_t send(const void* pointer, std::size_t size);

public:
	void set_config(const rs_comm_port_config& config);
	void get_config(rs_comm_port_config& config);

	void set_open(const std::uint8_t open);
	void get_open(std::uint8_t& open);

	void        push_rx(std::uint8_t* buffer, std::size_t size);
	std::size_t pop_rx (std::uint8_t* buffer, std::size_t size);

	void push_tx (const std::uint8_t* buffer, std::size_t size);
	void pop_tx  (std::vector<std::uint8_t>& buffer);
	void reset_tx(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}
