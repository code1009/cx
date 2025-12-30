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
rs_comm_port_impl::rs_comm_port_impl() :
	_mutex(),
	_config(),
	_open(false),
	_rx_buffer(),
	_tx_buffer(),
	_comm(),
	_connector(),
	_writer(),
	_reader(),
	_exit_event_handle(nullptr),
	_event_handler_open(),
	_event_handler_close(),
	_event_handler_recv()
{

}

rs_comm_port_impl::~rs_comm_port_impl()
{

}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void rs_comm_port_impl::set_event_handler_open(rs_comm_port::event_handler handler)
{
	_mutex.lock();
	_event_handler_open = handler;
	_mutex.unlock();
}

void rs_comm_port_impl::set_event_handler_close(rs_comm_port::event_handler handler)
{
	_mutex.lock();
	_event_handler_close = handler;
	_mutex.unlock();
}

void rs_comm_port_impl::set_event_handler_recv(rs_comm_port::event_handler handler)
{
	_mutex.lock();
	_event_handler_recv = handler;
	_mutex.unlock();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
bool rs_comm_port_impl::create(const rs_comm_port_config& config)
{
	_exit_event_handle = CreateEvent(0, TRUE, FALSE, 0);
	if (nullptr == _exit_event_handle)
	{
		destroy();
		return false;
	}


	set_config(config);


	_open = false;

	_connector._core = this;
	_writer._core = this;
	_reader._core = this;


	bool rv;


	rv = _connector.create();
	if (rv == false)
	{
		destroy();
		return false;
	}

	rv = _writer.create();
	if (rv == false)
	{
		destroy();
		return false;
	}

	rv = _reader.create();
	if (rv == false)
	{
		destroy();
		return false;
	}


	return true;
}

void rs_comm_port_impl::destroy(void)
{
	if (_exit_event_handle)
	{
		SetEvent(_exit_event_handle);
	}


	_connector.destroy();
	_writer.destroy();
	_reader.destroy();


	if (_exit_event_handle)
	{
		CloseHandle(_exit_event_handle);
		_exit_event_handle = nullptr;
	}


	_tx_buffer.clear();
	_rx_buffer.clear();
}

bool rs_comm_port_impl::is_open(void)
{
	bool result;


	_mutex.lock();


	result = _open == 1 ? true : false;


	_mutex.unlock();


	return result;
}

bool rs_comm_port_impl::is_rx_empty(void)
{
	bool result;


	_mutex.lock();


	result = _rx_buffer.empty();


	_mutex.unlock();


	return result;
}

bool rs_comm_port_impl::is_tx_empty(void)
{
	bool result;


	_mutex.lock();


	result = _tx_buffer.empty();


	_mutex.unlock();


	return result;
}

std::size_t rs_comm_port_impl::recv(void* pointer, std::size_t size)
{
	std::uint8_t* buffer;


	buffer = reinterpret_cast<std::uint8_t*>(pointer);


	return pop_rx(buffer, size);
}

std::size_t rs_comm_port_impl::send(const void* pointer, std::size_t size)
{
	if (0U == size)
	{
		return 0U;
	}


	if (!_writer._io_event_handle)
	{
		return 0U;
	}


	const std::uint8_t* buffer;


	buffer = reinterpret_cast<const std::uint8_t*>(pointer);

	push_tx(buffer, size);
	SetEvent(_writer._io_event_handle);


	return size;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void rs_comm_port_impl::set_config(const rs_comm_port_config& config)
{
	_mutex.lock();


	_config = config;


	_mutex.unlock();
}

void rs_comm_port_impl::get_config(rs_comm_port_config& config)
{
	_mutex.lock();


	config = _config;


	_mutex.unlock();
}

void rs_comm_port_impl::set_open(const std::uint8_t open)
{
	bool changed = false;


	_mutex.lock();


	if (_open != open)
	{
		changed = true;
	}

	_open = open;

	if (changed)
	{
		if (1==_open)
		{
			if (_event_handler_open)
			{
				_event_handler_open();
			}
		}
		else
		{
			if (_event_handler_close)
			{
				_event_handler_close();
			}
		}
	}


	_mutex.unlock();
}

void rs_comm_port_impl::get_open(std::uint8_t& open)
{
	_mutex.lock();


	open = _open;


	_mutex.unlock();
}

void rs_comm_port_impl::push_rx(std::uint8_t* buffer, std::size_t size)
{
	std::size_t buffer_max_size = 1024U * 1024U;
	std::size_t buffer_size;
	std::size_t buffer_input_size;
	std::size_t buffer_input_max_size;
	std::size_t buffer_current_size;
	std::size_t buffer_input_copy_size;


	_mutex.lock();


	buffer_size = _rx_buffer.size();
	buffer_input_size = size;
	buffer_current_size = buffer_size + buffer_input_size;
	buffer_input_max_size = (buffer_current_size < buffer_max_size) ? buffer_current_size : buffer_max_size;
	buffer_input_copy_size = buffer_input_max_size - buffer_size;


	if (buffer_input_copy_size > 0U)
	{
		_rx_buffer.insert(_rx_buffer.end(), buffer, buffer + buffer_input_copy_size);

		if (_event_handler_recv)
		{
			_event_handler_recv();
		}
	}


	if (size > buffer_input_copy_size)
	{
		CX_RUNTIME_LOG(cxLWarning)
			<< size << L" / " << buffer_input_copy_size << L" : lost " << size - buffer_input_copy_size << L"bytes";
	}


	_mutex.unlock();
}

std::size_t rs_comm_port_impl::pop_rx(std::uint8_t* buffer, std::size_t size)
{
	std::size_t  buffer_size;
	std::size_t  buffer_output_copy_size;


	_mutex.lock();


	buffer_size = _rx_buffer.size();
	buffer_output_copy_size = (buffer_size < size) ? buffer_size : size;

	if (buffer_output_copy_size > 0U)
	{
		memcpy(buffer, &*_rx_buffer.begin(), buffer_output_copy_size);
		_rx_buffer.erase(_rx_buffer.begin(), _rx_buffer.begin() + buffer_output_copy_size);
	}


	_mutex.unlock();


	return buffer_output_copy_size;
}

void rs_comm_port_impl::push_tx(const std::uint8_t* buffer, std::size_t size)
{
	_mutex.lock();


	_tx_buffer.insert(_tx_buffer.end(),  buffer, buffer + size);


	_mutex.unlock();
}

void rs_comm_port_impl::pop_tx(std::vector<std::uint8_t>& buffer)
{
	_mutex.lock();


	buffer = _tx_buffer;
	_tx_buffer.clear();


	_mutex.unlock();
}

void rs_comm_port_impl::reset_tx(void)
{
	_mutex.lock();


	_tx_buffer.clear();


	_mutex.unlock();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}




