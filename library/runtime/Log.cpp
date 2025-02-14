/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "runtime.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static std::wstring wstring_truncate(std::wstring str, size_t width, bool show_ellipsis = true)
{
	if (str.length() > width)
	{
		if (show_ellipsis)
		{
			return str.substr(0, width - 3U) + L"...";
		}
		else
		{
			return str.substr(0, width);
		}
	}

	return str;
}

static std::wstring get_file_of_file_path(std::wstring file_path)
{
	std::wstring result;

	std::wstring::size_type index;


	index = file_path.find_last_of(L"\\/");
	if (std::wstring::npos == index)
	{
		return result;
	}


	result = file_path.substr(index + 1);

	return result;
}

//===========================================================================
void writeLogDateTime_YYYYMMDD_String(std::wstringstream& ss, const SYSTEMTIME* st)
{
	ss.width(4);
	ss.fill(L'0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st->wYear;

	ss << L"-";

	ss.width(2);
	ss.fill('0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st->wMonth;

	ss << L"-";

	ss.width(2);
	ss.fill('0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st->wDay;

	//ss.width(2);
	//ss.fill('0');
	//ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	//ss << st->wDayOfWeek;
}

void writeLogDateTime_hhmmssms_String(std::wstringstream& ss, const SYSTEMTIME* st)
{
	ss.width(2);
	ss.fill(L'0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st->wHour;

	ss << L":";

	ss.width(2);
	ss.fill('0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st->wMinute;

	ss << L":";

	ss.width(2);
	ss.fill('0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st->wSecond;

	ss << L".";

	ss.width(3);
	ss.fill(L'0');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << st->wMilliseconds;
}

void writeLogDateTimeString(std::wstringstream& ss, const SYSTEMTIME* st)
{
	writeLogDateTime_YYYYMMDD_String(ss, st);

	ss << L" ";

	writeLogDateTime_hhmmssms_String(ss, st);
}

void writeLogLevelString(std::wstringstream& ss, LogLevel level)
{
	switch (level)
	{
	case LogLevel::Verbose    : ss << L"Verbose    "; break;
	case LogLevel::Debug      : ss << L"Debug      "; break;
	case LogLevel::Normal     : ss << L"Normal     "; break;
	case LogLevel::Dump       : ss << L"Dump       "; break;
	case LogLevel::Information: ss << L"Information"; break;
	case LogLevel::Warning    : ss << L"Warning    "; break;
	case LogLevel::Error      : ss << L"Error      "; break;
	case LogLevel::Critical   : ss << L"Critical   "; break;
	default                   : ss << L"Unknown    "; break;
	}
}

void writeLogLevelShortString(std::wstringstream& ss, LogLevel level)
{
	switch (level)
	{
	case LogLevel::Verbose    : ss << L"V"; break;
	case LogLevel::Debug      : ss << L"D"; break;
	case LogLevel::Normal     : ss << L"N"; break;
	case LogLevel::Dump       : ss << L"U"; break;
	case LogLevel::Information: ss << L"I"; break;
	case LogLevel::Warning    : ss << L"W"; break;
	case LogLevel::Error      : ss << L"E"; break;
	case LogLevel::Critical   : ss << L"C"; break;
	default                   : ss << L"?"; break;
	}
}

void writeLogThreadIdString(std::wstringstream& ss, DWORD threadId)
{
	ss << L"0x";
	ss.width(8);
	ss.fill(L'0');
	ss.setf(std::ios::hex | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << threadId;
}

void writeLogSourceFileString(std::wstringstream& ss, const wchar_t* file)
{
	const std::size_t width = 16U;
	ss.width(width);
	ss.fill(L' ');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << wstring_truncate(get_file_of_file_path(file), width, false);
}

void writeLogSourceLineString(std::wstringstream& ss, int line)
{
	ss.width(5);
	ss.fill(L'_');
	ss.setf(std::ios::dec | std::ios::right, std::ios::basefield | std::ios::adjustfield);
	ss << line;
}

void writeLogSourceFileLineString(std::wstringstream& ss, const wchar_t* file, int line)
{
	const std::size_t width = 16U;
	ss.width(width);
	ss.fill(L' ');
	ss.setf(std::ios::dec | std::ios::left, std::ios::basefield | std::ios::adjustfield);

	ss <<
		wstring_truncate(
			wstring_truncate(get_file_of_file_path(file), 10, false)
			+ L":"
			+ std::to_wstring(line),
			width,
			false
		);
}

void writeLogSourceFuncString(std::wstringstream& ss, const wchar_t* func)
{
	const std::size_t width = 12U;
	ss.width(width);
	ss.fill(L' ');
	ss.setf(std::ios::dec | std::ios::left, std::ios::basefield | std::ios::adjustfield);
	ss << wstring_truncate(func, width, false);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void writeLogString(
	std::wstringstream& ss,
	SYSTEMTIME*         dateTime,
	DWORD               threadId,
	LogLevel            level,
	const wchar_t*      file,
	int                 line,
	const wchar_t*      func,
	std::wstring_view   message
)
{
	ss << L"[";
#if 0
	writeLogDateTimeString(ss, dateTime);
#else
	writeLogDateTime_hhmmssms_String(ss, dateTime);
#endif
	ss << L"]";


	ss << L"[";
	writeLogThreadIdString(ss, threadId);
	ss << L"]";


	ss << L"[";
	writeLogLevelShortString(ss, level);
	ss << L"]";


#if 0
	ss << L"[";
	writeLogSourceFileString(ss, file);
	ss << L":";
	writeLogSourceLineString(ss, line);
	ss << L"]";
#else
	ss << L"[";
	writeLogSourceFileLineString(ss, file, line);
	ss << L"]";
#endif


#if 1
	ss << L"[";
	writeLogSourceFuncString(ss, func);
	ss << L"]";
#endif


	ss << L" ";
	ss << message;
	ss << std::endl;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void writeLogString(
	std::wstringstream& ss,
	LogInfomation& info
)
{
	writeLogString(
		ss,
		&info.dateTime,
		info.threadId,
		info.level,
		info.file,
		info.line,
		info.func,
		info.message->str()
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void Logger::setOutputHandler(OutputHandler handler)
{
	_OutputHandler = handler;
}

void Logger::output(std::wstringstream& output, LogInfomation& info)
{
	if (_OutputHandler)
	{
		_OutputHandler(output, info);
	}
	else
	{
		OutputDebugStringW(output.str().c_str());
	}
}

void Logger::log(
	LogLevel            level,
	const wchar_t*      file,
	int                 line,
	const wchar_t*      func,
	std::wstringstream* message,
	void*               param
)
{
	//-----------------------------------------------------------------------
	LogInfomation info;
	

	GetLocalTime(&info.dateTime);	
	info.threadId = GetCurrentThreadId();

	info.level   = level;
	info.file    = file;
	info.line    = line;
	info.func    = func;
	info.message = message;
	info.param   = param;


	//-----------------------------------------------------------------------
	std::wstringstream ss;


	writeLogString(ss, info);


	//-----------------------------------------------------------------------
	output(ss, info);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Log::Log(
	Logger*        logger, 
	LogLevel       level, 
	const wchar_t* file,
	int            line,
	const wchar_t* func,
	void*          param
) :
	_logger(logger), 
	_level (level), 
	_file  (file), 
	_line  (line),
	_func  (func),
	_param (param)
{
}

//===========================================================================
Log::~Log()
{
	_logger->log(_level, _file, _line, _func, &_ss, _param);
}

//===========================================================================
Log& Log::operator<<(std::wostream& (*manip)(std::wostream&))
{
	if (manip == static_cast<std::wostream & (*)(std::wostream&)>(std::endl))
	{
		_logger->log(_level, _file, _line, _func, &_ss);
		_ss.str(L"");
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
Logger* getLogger(void)
{
	static Logger logger;


	return &logger;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




