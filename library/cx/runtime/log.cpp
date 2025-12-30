/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "log.hpp"

#include "../common/std_wstring_utility.hpp"
#include "../common/fs_std_wstring.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
const std::size_t LOG_STRING_NAME_WIDTH = 24U;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static log_string get_function_name(log_string func)
{
	log_string::size_type pos = func.rfind(CX_RUNTIME_LOG_TEXT("::"));
	log_string name;
	if (pos != log_string::npos)
	{
		name = func.substr(pos + 2U);
	}
	else
	{
		name = func;
	}
	name += CX_RUNTIME_LOG_TEXT("()");
	return name;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void write_log_datetime_YYYYMMDD(log_stringstream& ss, const log_datetime& datetime)
{
	ss.width(4U);
	ss.fill(CX_RUNTIME_LOG_TEXT('0'));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << datetime.wYear;

	ss << CX_RUNTIME_LOG_TEXT("-");

	ss.width(2U);
	ss.fill(CX_RUNTIME_LOG_TEXT('0'));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << datetime.wMonth;

	ss << CX_RUNTIME_LOG_TEXT("-");

	ss.width(2U);
	ss.fill(CX_RUNTIME_LOG_TEXT('0'));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << datetime.wDay;
}

void write_log_datetime_YYYYMMDD_ddd(log_stringstream& ss, const log_datetime& datetime)
{
	write_log_datetime_YYYYMMDD(ss, datetime);

	ss << CX_RUNTIME_LOG_TEXT("(");
	switch (datetime.wDayOfWeek)
	{
	case 0: ss << CX_RUNTIME_LOG_TEXT("Sun"); break;
	case 1: ss << CX_RUNTIME_LOG_TEXT("Mon"); break;
	case 2: ss << CX_RUNTIME_LOG_TEXT("Tue"); break;
	case 3: ss << CX_RUNTIME_LOG_TEXT("Wed"); break;
	case 4: ss << CX_RUNTIME_LOG_TEXT("Thu"); break;
	case 5: ss << CX_RUNTIME_LOG_TEXT("Fri"); break;
	case 6: ss << CX_RUNTIME_LOG_TEXT("Sat"); break;
	default:
		ss << CX_RUNTIME_LOG_TEXT("???");
		break;
	}
	ss << CX_RUNTIME_LOG_TEXT(")");
}

void write_log_datetime_hhmmssms(log_stringstream& ss, const log_datetime& datetime)
{
	ss.width(2U);
	ss.fill(CX_RUNTIME_LOG_TEXT('0'));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << datetime.wHour;

	ss << CX_RUNTIME_LOG_TEXT(":");

	ss.width(2U);
	ss.fill(CX_RUNTIME_LOG_TEXT('0'));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << datetime.wMinute;

	ss << CX_RUNTIME_LOG_TEXT(":");

	ss.width(2U);
	ss.fill(CX_RUNTIME_LOG_TEXT('0'));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << datetime.wSecond;

	ss << CX_RUNTIME_LOG_TEXT(".");

	ss.width(3U);
	ss.fill(CX_RUNTIME_LOG_TEXT('0'));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << datetime.wMilliseconds;
}

void write_log_datetime(log_stringstream& ss, const log_datetime& datetime)
{
#if 0
	write_log_datetime_YYYYMMDD(ss, datetime);
	ss << CX_RUNTIME_LOG_TEXT(" ");
#endif
	write_log_datetime_hhmmssms(ss, datetime);
}

//===========================================================================
void write_log_severity(log_stringstream& ss, const log_severity severity)
{
	switch (severity)
	{
	case log_severity::trace   : ss << CX_RUNTIME_LOG_TEXT("trace   "); break;
	case log_severity::debug   : ss << CX_RUNTIME_LOG_TEXT("debug   "); break;
	case log_severity::info    : ss << CX_RUNTIME_LOG_TEXT("info    "); break;
	case log_severity::warning : ss << CX_RUNTIME_LOG_TEXT("warning "); break;
	case log_severity::error   : ss << CX_RUNTIME_LOG_TEXT("error   "); break;
	case log_severity::critical: ss << CX_RUNTIME_LOG_TEXT("critical"); break;
	default                    : ss << CX_RUNTIME_LOG_TEXT("Unknown "); break;
	}
}

void write_log_severity_short(log_stringstream& ss, const log_severity severity)
{
	switch (severity)
	{
	case log_severity::trace   : ss << CX_RUNTIME_LOG_TEXT("T"); break;
	case log_severity::debug   : ss << CX_RUNTIME_LOG_TEXT("D"); break;
	case log_severity::info    : ss << CX_RUNTIME_LOG_TEXT("I"); break;
	case log_severity::warning : ss << CX_RUNTIME_LOG_TEXT("W"); break;
	case log_severity::error   : ss << CX_RUNTIME_LOG_TEXT("E"); break;
	case log_severity::critical: ss << CX_RUNTIME_LOG_TEXT("C"); break;
	default                    : ss << CX_RUNTIME_LOG_TEXT("?"); break;
	}
}

//===========================================================================
void write_log_thread_id(log_stringstream& ss, const log_thread_id thread_id)
{
	ss << CX_RUNTIME_LOG_TEXT("0x");
	ss.width(8U);
	ss.fill(CX_RUNTIME_LOG_TEXT('0'));
	ss.setf(std::ios::hex | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << std::uppercase;
	ss << thread_id;
}

//===========================================================================
void write_log_source_file(log_stringstream& ss, const log_char* const file)
{
	const std::size_t width = LOG_STRING_NAME_WIDTH;
	ss.width(width);
	ss.fill(CX_RUNTIME_LOG_TEXT(' '));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << truncate_std_wstring(wfs::get_file_of_file_path(file), width, false);
}

void write_log_source_line(log_stringstream& ss, const log_source_line line)
{
	const std::size_t width = 4U;
	ss.width(width);
	ss.fill(CX_RUNTIME_LOG_TEXT(' '));
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << line;
}

void write_log_source_file_line(log_stringstream& ss, const log_char* const file, const log_source_line line)
{
	const std::size_t width = LOG_STRING_NAME_WIDTH + 1U + 4U;
	ss.width(width);
	ss.fill(CX_RUNTIME_LOG_TEXT(' '));
	ss.setf(std::ios::dec | std::ios::left, std::ios::basefield | std::ios::adjustfield);

	ss <<
		truncate_std_wstring(
			truncate_std_wstring(wfs::get_file_of_file_path(file), 20, false)
			+ CX_RUNTIME_LOG_TEXT(":")
			+ std::format(CX_RUNTIME_LOG_TEXT("{}"), line),
			width,
			false
		);
}

void write_log_source_func(log_stringstream& ss, const log_char* func)
{
	const std::size_t width = LOG_STRING_NAME_WIDTH;
	ss.width(width);
	ss.fill(CX_RUNTIME_LOG_TEXT(' '));
	ss.setf(std::ios::dec | std::ios::left, std::ios::basefield | std::ios::adjustfield);
	ss << truncate_std_wstring(get_function_name(func), width, false);
}

void write_log_source(log_stringstream& ss, const log_source& source)
{
#if 0
	write_log_source_file_line(ss, source.file, source.line);
	ss << L" ";
	write_log_source_func(ss, source.func);
#endif

	const std::size_t width = LOG_STRING_NAME_WIDTH + 1U + 4U + 3U + LOG_STRING_NAME_WIDTH + 3U;
	ss.width(width);
	ss.fill(CX_RUNTIME_LOG_TEXT(' '));
	ss.setf(std::ios::dec | std::ios::left, std::ios::basefield | std::ios::adjustfield);

	ss <<
		truncate_std_wstring(
			truncate_std_wstring(wfs::get_file_of_file_path(source.file), 20, false)
			+ CX_RUNTIME_LOG_TEXT(":")
			+ std::format(CX_RUNTIME_LOG_TEXT("{}"), source.line)
			+ CX_RUNTIME_LOG_TEXT(" > ")
			+ get_function_name(source.func),
			width,
			false
		);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void write_log(
	log_stringstream& ss,
	const log_datetime&   datetime,
	const log_thread_id   thread_id,
	const log_severity    severity,
	const log_char* const file,
	const log_source_line line,
	const log_char* const func,
	const log_string_view message
)
{
	log_item item
	{
		datetime,
		thread_id,
		severity,
		{file,line,func},
		log_string(message),
		log_param{}
	};
	write_log_item(ss, item);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void write_log_item(
	log_stringstream& ss,
	const log_item& item
)
{
	write_log_datetime(ss, item.datetime);
	ss << CX_RUNTIME_LOG_TEXT(" (");
	write_log_thread_id(ss, item.thread_id);
	ss << CX_RUNTIME_LOG_TEXT(") [");
	write_log_severity_short(ss, item.severity);
	ss << CX_RUNTIME_LOG_TEXT("] ");
	write_log_source(ss, item.source);
	ss << CX_RUNTIME_LOG_TEXT(": ");
	ss << item.message;
	ss << std::endl;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void logger::set_output_handler(output_handler handler)
{
	const std::lock_guard<std::mutex> lock(_mutex);
	_output_handler = handler;
}

void logger::output(const log_item& item)
{
	const std::lock_guard<std::mutex> lock(_mutex);
	if (_output_handler)
	{
		_output_handler(item);
	}
	else
	{
		log_stringstream ss;
		write_log_item(ss, item);
		OutputDebugStringW(ss.str().c_str());
	}
}

void logger::log(
	log_severity          severity,
	const log_char* const file,
	log_source_line       line,
	const log_char* const func,
	log_string_view       message,
	log_param&            param
)
{
	log_item item;
	GetLocalTime(&item.datetime);	
	item.thread_id   = GetCurrentThreadId();
	item.severity    = severity;
	item.source.file = file;
	item.source.line = line;
	item.source.func = func;
	item.message     = message;
	item.param       = param;

	output(item);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
log::log(
	logger* const         logger,
	const log_severity    severity,
	const log_char* const file,
	const log_source_line line,
	const log_char* const func,
	const void* const     param_pointer,
	const std::size_t     param_size
) :
	_logger   (logger), 
	_severity (severity), 
	_file     (file), 
	_line     (line),
	_func     (func)
{
	if (param_pointer && param_size)
	{
		_param.resize(param_size);
		std::memcpy(_param.data(), param_pointer, param_size);
	}
}

log::~log()
{
	_logger->log(_severity, _file, _line, _func, _ss.str(), _param);
}

log& log::operator<<(std::wostream& (*manip)(std::wostream&))
{
	if (manip == static_cast<std::wostream & (*)(std::wostream&)>(std::endl))
	{
		std::vector<std::uint8_t> empty_param;
		_logger->log(_severity, _file, _line, _func, _ss.str(), empty_param);
		_ss.str(CX_RUNTIME_LOG_TEXT(""));
		_ss.clear();
	}
	else
	{
		_ss << manip;
	}
	
	return *this;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
logger* get_logger(void)
{
	static logger instance;
	return &instance;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




