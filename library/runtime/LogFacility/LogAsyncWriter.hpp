/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class LogAsyncWriter : public LogWriter
{
private:
	std::condition_variable _EventCv{ };
	std::mutex              _EventCvMutex{};

private:
	std::uint32_t _EventTimeout_msec{ 1000U };

private:
	bool _Shutdown_EventFlag{ false };
	bool _Flush_EventFlag{ false };

private:
	enum class Event
	{
		Unknown = 0,
		Timeout = 1,
		Shutdown = 2,
		Flush = 3
	};

private:
	std::thread _ProcessThread{};

private:
	std::mutex             _mutex{};
	std::vector<LogItem*>  _buffer0{};
	std::vector<LogItem*>  _buffer1{};
	std::vector<LogItem*>* _front{ nullptr };
	std::vector<LogItem*>* _back{ nullptr };

public:
	LogAsyncWriter();

public:
	virtual ~LogAsyncWriter();

public:
	virtual void write(LogItem& item) override;
	virtual void flush(void) override;

private:
	bool create(void);
	void destroy(void);

private:
	void cleanup(void);

private:
	void process(void);

private:
	void onBegin(void);
	void onEnd(void);

private:
	void  eventLoop(void);
	void  eventLoop(const std::uint32_t msec);
	Event waitEvent(void);
	Event waitEvent(const std::uint32_t msec);
	bool  checkEvent(void);
	Event getEvent(void);

private:
	bool onEvent(Event e);
	bool onUnknownEvent(void);
	void onTimeoutEvent(void);
	bool onShutdownEvent(void);
	void onFlushEvent(void);

public:
	void notifyShutdown(void);
	void notifyFlush(void);

private:
	void record(void);
	void recordItems(std::vector<LogItem*>* c);
	void recordItem(LogItem* m);

private:
	void output(LogItem* m);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




