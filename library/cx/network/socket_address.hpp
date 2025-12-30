#ifndef INCLUDED__cx__network__socket_address__hpp
#define INCLUDED__cx__network__socket_address__hpp





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
constexpr std::size_t SOCKET_ADDRESS_DATA_MAX_SIZE = 16;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class socket_address final
{
public:
	enum class family : std::uint16_t
	{
		unknown,
		inet = 2
	};

private:
	std::uint8_t _data[SOCKET_ADDRESS_DATA_MAX_SIZE];
	std::size_t _size;

public:
	socket_address();
	~socket_address();

public:
	socket_address(const socket_address& other);
	socket_address& operator=(const socket_address& other);

public:
	void copy_from(const socket_address& other);

public:
	void reset(void);

	void set_inet_v4(
		std::uint32_t addr,
		std::uint16_t port,
		family family_type = family::inet
		);

	void set_inet_v4(
		const std::string_view addr_string,
		std::uint16_t port,
		family family_type = family::inet
	);

	void set_inet_v4(
		const std::string_view addr_string,
		const std::string_view port_string,
		family family_type = family::inet
	);

	std::string get_inet_v4_addr(void);
	std::string get_inet_v4_port(void);

public:
	void         set_size     (std::size_t size) { _size = size; }
	std::size_t  get_size     (void)             { return _size; }
	void*        get_data_ptr (void)             { return reinterpret_cast<void*>(&_data[0]); }
	std::uint8_t get_data     (std::size_t n)    { return _data[n]; }

public:
	void dump(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif

