#ifndef INCLUDED__cx__network__socket_event_select__hpp
#define INCLUDED__cx__network__socket_event_select__hpp





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr std::uint32_t socket_event_flag_accept   = 0x01;
constexpr std::uint32_t socket_event_flag_connect  = 0x02;
constexpr std::uint32_t socket_event_flag_write    = 0x04;
constexpr std::uint32_t socket_event_flag_read     = 0x08;
constexpr std::uint32_t socket_event_flag_close    = 0x10;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr std::uint32_t socket_event_noerror  = 0;
constexpr std::uint32_t socket_event_error    = 1;
constexpr std::uint32_t socket_event_timeout  = 2;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr std::uint32_t socket_wait_infinite = 0xffffffff;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class socket_event_select final
{
private:
	class impl;
	std::unique_ptr<impl> _pimpl;

public:
	using event_handler = std::function<void(std::uint32_t event_index)>;
	using socket_event_handler = std::function<void(void)>;

	event_handler        on_event;
	socket_event_handler on_socket_event_accept;
	socket_event_handler on_socket_event_connect;
	socket_event_handler on_socket_event_read;
	socket_event_handler on_socket_event_write;
	socket_event_handler on_socket_event_close;

public:
	socket_event_select();

public:
	~socket_event_select();

private:
	socket_event_select(const socket_event_select&) = delete;
	socket_event_select& operator=(const socket_event_select&) = delete;

private:
	socket_event_select(socket_event_select&&) = delete;
	socket_event_select& operator=(socket_event_select&&) = delete;

public:
	bool create(void);
	void destroy(void);

public:
	bool register_event(HANDLE event_handle);

public:
	bool select(socket_descriptor_t socket_descriptor, std::uint32_t socket_event_flag = socket_event_flag_read | socket_event_flag_close);

public:
	std::uint32_t wait(socket_descriptor_t socket_descriptor, std::uint32_t timeout_msec = socket_wait_infinite);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif



