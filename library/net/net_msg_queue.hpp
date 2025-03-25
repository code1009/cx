#ifndef INCLUDED__cx__network__net_msg_queue__hpp
#define INCLUDED__cx__network__net_msg_queue__hpp





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class net_msg_queue final
{
public:
	std::mutex           _mutex;
	std::deque<net_msg*> _container;

public:
	net_msg_queue();

private:
	net_msg_queue(const net_msg_queue& other) = delete;
	net_msg_queue& operator=(const net_msg_queue& other) = delete;

private:
	net_msg_queue(net_msg_queue&& other) noexcept = delete;
	net_msg_queue& operator=(net_msg_queue&& other) noexcept = delete;

public:
	void push(net_msg* m);
	net_msg* pop(void);

public:
	std::size_t count(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif


