#ifndef INCLUDED__cx__network__net_msg_event_queue
#define INCLUDED__cx__network__net_msg_event_queue





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
namespace network
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class net_msg_event_queue
{
public:
	std::mutex           _mutex;
	std::deque<net_msg*> _container;
	HANDLE _event;

public:
	net_msg_event_queue();
	~net_msg_event_queue();

private:
	net_msg_event_queue(const net_msg_event_queue& other) = delete;
	net_msg_event_queue& operator=(const net_msg_event_queue& other) = delete;

private:
	net_msg_event_queue(net_msg_event_queue&& other) noexcept = delete;
	net_msg_event_queue& operator=(net_msg_event_queue&& other) noexcept = delete;

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


