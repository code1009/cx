/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class log_async_writer : public log_writer
{
private:
	std::condition_variable _event_cv      {};
	std::mutex              _event_cv_mutex{};

private:
	std::uint32_t _event_timeout_msec{ 1000U };

private:
	bool _event_flag_shutdown{ false };
	bool _event_flag_flush   { false };

private:
	enum class event
	{
		unknown  = 0,
		timeout  = 1,
		shutdown = 2,
		flush    = 3
	};

private:
	std::thread _thread{};

private:
	std::mutex              _mutex{};
	std::vector<log_item*>  _buffer0{};
	std::vector<log_item*>  _buffer1{};
	std::vector<log_item*>* _front{ nullptr };
	std::vector<log_item*>* _back { nullptr };

public:
	log_async_writer();

public:
	virtual ~log_async_writer();

public:
	virtual void write(const log_item& item) override;
	virtual void flush(void) override;

private:
	bool create(void);
	void destroy(void);

private:
	void cleanup(void);

private:
	void process(void);

private:
	void on_begin(void);
	void on_end(void);

private:
	void  event_loop(void);
	void  event_loop(const std::uint32_t msec);
	event wait_event(void);
	event wait_event(const std::uint32_t msec);
	bool  check_event(void);
	event get_event(void);

private:
	bool on_event(const event e);
	bool on_event_unknown(void);
	void on_event_timeout(void);
	bool on_event_shutdown(void);
	void on_event_flush(void);

public:
	void notify_shutdown(void);
	void notify_flush(void);

private:
	void record(void);
	void record_items(std::vector<log_item*>* c);
	void record_item(log_item* m);

private:
	void output(log_item* m);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




