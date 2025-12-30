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
class rs_comm_port_connector
{
public:
	enum class state
	{
		unknown,
		ready,
		idle,
		io,
		io_ov
	};

	enum class event
	{
		unknown,
		exit,
		ready,
		idle,
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
	HANDLE _io_open_event_handle; // connector --> reader / writer
	HANDLE _io_close_event_handle; // connector --> reader / writer
	HANDLE _io_error_event_handle; // connector <-- reader / writer / connector

	state _state;
	std::uint8_t _io_open{ 0 };
	rs_comm_port_config _config;

public:
	DWORD _comm_event;
	OVERLAPPED _io_ov;

public:
	rs_comm_port_connector();
	~rs_comm_port_connector();

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
	event wait_event_state_idle(void);
	event wait_event_state_ready(void);
	event wait_event_state_io(void);
	event wait_event_state_io_ov(void);

	void on_event_ready(void);
	void on_event_idle(void);
	void on_event_io_open(void);
	void on_event_io(void);
	void on_event_io_ov(void);
	void on_event_io_close(void);
	void on_event_io_exit(void);

public:
	bool wait_io_ready(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}
