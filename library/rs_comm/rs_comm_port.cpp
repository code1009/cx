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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
rs_comm_port_config::rs_comm_port_config() :
	_open_interval(3000),

	_device_name(L"COM1:"  ),
	_baudrate   (9600U     ),
	_byte_size  (8U        ),
	_parity     (NOPARITY  ),
	_stop_bits  (ONESTOPBIT)
{

}

//===========================================================================
rs_comm_port_config::rs_comm_port_config(const rs_comm_port_config& other) :
	_open_interval(other._open_interval),

	_device_name(other._device_name),
	_baudrate   (other._baudrate   ),
	_byte_size  (other._byte_size  ),
	_parity     (other._parity     ),
	_stop_bits  (other._stop_bits  )
{

}

rs_comm_port_config& rs_comm_port_config::operator=(const rs_comm_port_config& other)
{
	if (this != &other)
	{
		_open_interval = other._open_interval;

		_device_name = other._device_name;
		_baudrate    = other._baudrate   ;
		_byte_size   = other._byte_size  ;
		_parity      = other._parity     ;
		_stop_bits   = other._stop_bits  ;
	}

	return *this;
}

//===========================================================================
rs_comm_port_config::rs_comm_port_config(rs_comm_port_config&& other) noexcept :
	_open_interval(std::move(other._open_interval)),

	_device_name(std::move(other._device_name)),
	_baudrate   (std::move(other._baudrate   )),
	_byte_size  (std::move(other._byte_size  )),
	_parity     (std::move(other._parity     )),
	_stop_bits  (std::move(other._stop_bits  ))
{

}

rs_comm_port_config& rs_comm_port_config::operator=(rs_comm_port_config&& other) noexcept
{
	if (this != &other)
	{
		_open_interval = std::move(other._open_interval);

		_device_name = std::move(other._device_name);
		_baudrate    = std::move(other._baudrate   );
		_byte_size   = std::move(other._byte_size  );
		_parity      = std::move(other._parity     );
		_stop_bits   = std::move(other._stop_bits  );
	}

	return *this;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
rs_comm_port::rs_comm_port() :
	_pimp(nullptr)
{
	_pimp = new rs_comm_port_internal::rs_comm_port_impl();
}

rs_comm_port::~rs_comm_port()
{
	destroy();

	if (_pimp)
	{
		delete _pimp;

		_pimp = nullptr;
	}
}

void rs_comm_port::set_event_handler_open(event_handler handler)
{
	if (_pimp)
	{
		return _pimp->set_event_handler_open(handler);
	}
}

void rs_comm_port::set_event_handler_close(event_handler handler)
{
	if (_pimp)
	{
		return _pimp->set_event_handler_close(handler);
	}
}

void rs_comm_port::set_event_handler_recv(event_handler handler)
{
	if (_pimp)
	{
		return _pimp->set_event_handler_recv(handler);
	}
}

bool rs_comm_port::create(const rs_comm_port_config& config)
{
	if (_pimp)
	{
		return _pimp->create(config);
	}

	return false;
}

void rs_comm_port::destroy(void)
{
	if (_pimp)
	{
		_pimp->destroy();
	}
}

bool rs_comm_port::is_open(void)
{
	if (_pimp)
	{
		return _pimp->is_open();
	}

	return false;
}

bool rs_comm_port::is_rx_empty(void)
{
	if (_pimp)
	{
		return _pimp->is_rx_empty();
	}

	return false;
}

bool rs_comm_port::is_tx_empty(void)
{
	if (_pimp)
	{
		return _pimp->is_tx_empty();
	}

	return false;
}

std::size_t rs_comm_port::recv(void* pointer, std::size_t size)
{
	if (_pimp)
	{
		return _pimp->recv(pointer, size);
	}

	return 0;
}

std::size_t rs_comm_port::send(const void* pointer, std::size_t size)
{
	if (_pimp)
	{
		return _pimp->send(pointer, size);
	}

	return 0;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
