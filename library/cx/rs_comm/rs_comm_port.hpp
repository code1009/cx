/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#ifndef INCLUDED__cx_rs_comm_port
#define INCLUDED__cx_rs_comm_port





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*
The term RS232 stands for "Recommended Standard 232" and 
it is a type of serial communication used for transmission of data normally 
in medium distances.
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class rs_comm_port_config
{
public:
	std::uint32_t _open_interval;

public:
	std::wstring  _device_name;
	std::uint32_t _baudrate   ;
	std::uint32_t _byte_size  ;
	std::uint32_t _parity     ;
	std::uint32_t _stop_bits  ;

public:
	rs_comm_port_config();

public:
	rs_comm_port_config(const rs_comm_port_config& other);
	rs_comm_port_config& operator=(const rs_comm_port_config& other);

public:
	rs_comm_port_config(rs_comm_port_config&& other) noexcept;
	rs_comm_port_config& operator=(rs_comm_port_config&& other) noexcept;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace rs_comm_port_internal
{
class rs_comm_port_impl;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class rs_comm_port
{
private:
	rs_comm_port_internal::rs_comm_port_impl* _pimp;

public:
	rs_comm_port();
	~rs_comm_port();

public:
	using event_handler = std::function<void(void)>;
	void set_event_handler_open (event_handler handler);
	void set_event_handler_close(event_handler handler);
	void set_event_handler_recv (event_handler handler);

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
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





#endif




