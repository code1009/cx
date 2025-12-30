#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace rs_comm_port_internal
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class rs_comm_port_impl;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class rs_comm_port_reader
{
public:
	enum class state
	{
		unknown,
		ready,
		io,
		io_ov
	};

	enum class event
	{
		unknown,
		exit,
		ready,
		io_open,
		io,
		io_ov,
		io_close,
		io_exit
	};

public:
	rs_comm_port_impl* _core;

public:
	HANDLE _thread_handle;

	HANDLE _ready_event_handle;
	HANDLE _io_event_handle;
	HANDLE _io_ov_event_handle;

	state _state;

public:
	std::vector<std::uint8_t> _buffer;
	OVERLAPPED _io_ov;
	LPVOID     _io_buffer;
	DWORD      _io_size;
	DWORD      _io_transferred;

public:
	rs_comm_port_reader();
	~rs_comm_port_reader();

public:
	bool create(void);
	void destroy(void);

public:
	void set_state(state s);

public:
	void thread_loop(void);

	event wait_event(void);
	bool on_event(event e);

public:
	event wait_event_state_ready(void);
	event wait_event_state_io(void);
	event wait_event_state_io_ov(void);

	void on_event_ready(void);
	void on_event_io_open(void);
	void on_event_io(void);
	void on_event_io_ov(void);
	void on_event_io_close(void);
	void on_event_io_exit(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}




