#ifndef INCLUDED__cx__network__socket__hpp
#define INCLUDED__cx__network__socket__hpp





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class socket final
{
public:
	enum class config
	{
		unknown,
		udp,
		tcp
	};

public:
	socket_descriptor_t _descriptor;

public:
	socket ();
	~socket ();

private:
	socket(const socket&) = delete;
	socket& operator= (const socket&) = delete;

private:
	socket(socket&& other) noexcept = delete;
	socket& operator=(socket&& other) noexcept = delete;

public:
	socket_descriptor_t get_descriptor (void) const
	{
		return _descriptor;
	}

	explicit operator socket_descriptor_t (void) const
	{
		return _descriptor;
	}

public:
	bool create (config cfg = config::udp);
	void destroy (void);

public:
	bool shutdown(std::int32_t h);

public:
	bool bind    (socket_address* sa);
	bool listen  (std::int32_t b=5u);
	bool accept  (socket_address* sa, socket_descriptor_t* client);
	bool connect (socket_address* sa);

public:
	bool send (void* pointer, std::int32_t size, std::int32_t* wsize, std::int32_t sflags=0);
	bool recv (void* pointer, std::int32_t size, std::int32_t* rsize, std::int32_t rflags=0);
	
public:
	bool sendto   (socket_address* sa, void* pointer, std::int32_t size, std::int32_t* wsize, std::int32_t sflags=0);
	bool recvfrom (socket_address* sa, void* pointer, std::int32_t size, std::int32_t* rsize, std::int32_t rflags=0);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif



