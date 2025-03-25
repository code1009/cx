#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace app
{




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WebMessageQueue final
{
public:
	std::mutex           _mutex;
	std::deque<WebMessage*> _container;

public:
	WebMessageQueue();

private:
	WebMessageQueue(const WebMessageQueue& other) = delete;
	WebMessageQueue& operator=(const WebMessageQueue& other) = delete;

private:
	WebMessageQueue(WebMessageQueue&& other) noexcept = delete;
	WebMessageQueue& operator=(WebMessageQueue&& other) noexcept = delete;

public:
	void push(WebMessage* m);
	WebMessage* pop(void);

public:
	std::size_t count(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}









