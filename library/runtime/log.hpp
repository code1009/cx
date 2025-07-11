/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using log_char = wchar_t;

#define CX_RUNTIME_LOG_TEXT(x) L##x

using log_string = std::wstring;

using log_string_view = std::wstring_view;

using log_stringstream = std::wstringstream;

enum class log_severity
{
	trace   ,
	debug   ,
	info    ,
	warning ,
	error   ,
	critical 
};

using log_source_line = int;

struct log_source
{
	const log_char* file;
	log_source_line line;
	const log_char* func;
};

using log_thread_id = DWORD;

using log_datetime = SYSTEMTIME;

using log_param = std::vector<std::uint8_t>;

//===========================================================================
struct log_item
{
	log_datetime  datetime;
	log_thread_id thread_id;
	log_severity  severity;
	log_source    source;
	log_string    message;
	log_param     param;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void write_log_datetime_YYYYMMDD    (log_stringstream& ss, const log_datetime& datetime);
void write_log_datetime_YYYYMMDD_ddd(log_stringstream& ss, const log_datetime& datetime);
void write_log_datetime_hhmmssms    (log_stringstream& ss, const log_datetime& datetime);
void write_log_datetime             (log_stringstream& ss, const log_datetime& datetime);

void write_log_severity         (log_stringstream& ss, const log_severity severity);
void write_log_severity_short   (log_stringstream& ss, const log_severity severity);

void write_log_thread_id        (log_stringstream& ss, const log_thread_id thread_id);

void write_log_source_file      (log_stringstream& ss, const log_char* const file);
void write_log_source_line      (log_stringstream& ss, const log_source_line line);
void write_log_source_func      (log_stringstream& ss, const log_char* const func);
void write_log_source_file_line (log_stringstream& ss, const log_char* const file, const log_source_line line);
void write_log_source           (log_stringstream& ss, const log_source& source);

//===========================================================================
void write_log(
	log_stringstream&     ss,
	const log_datetime&   datetime,
	const log_thread_id   thread_id,
	const log_severity    severity,
	const log_char* const file,
	const log_source_line line,
	const log_char* const func,
	const log_string_view message
);

void write_log_item(
	log_stringstream& ss,
	const log_item&   item
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class logger
{
public:
	using output_handler = std::function<void(const log_item&)>;

private:
	std::mutex _mutex;

private:
	output_handler _output_handler;

public:
	logger() = default;

public:
	virtual ~logger() = default;

public:
	void set_output_handler(const output_handler handler);

public:
	virtual void output(const log_item& item);

public:
	virtual void log(
		log_severity          severity,
		const log_char* const file,
		log_source_line       line,
		const log_char* const func,
		log_string_view       message,
		log_param&            param
	);
};






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class log 
{
private:
	logger* _logger;

private:
	log_severity     _severity;
	const log_char*  _file    ;
	log_source_line  _line    ;
	const log_char*  _func    ;
	log_stringstream _ss      ;
	log_param        _param   ;

public:
	log(
		logger* const         logger,
		const log_severity    severity,
		const log_char* const file,
		const log_source_line line,
		const log_char* const func,
		const void* const     param_pointer = nullptr,
		const std::size_t     param_size = 0
	);

public:
	~log();

public:
	template<typename T>
	log& operator<<(const T& message) 
	{
		_ss << message;
		return *this;
	}

	log& operator<<(std::wostream& (*manip)(std::wostream&));
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
logger* get_logger(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


