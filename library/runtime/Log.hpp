/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::runtime
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
enum class LogLevel 
{
	Verbose,
	Debug,
	Normal,
	Dump,
	Information,
	Warning,
	Error,
	Critical
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void writeLogDateTime_YYYYMMDD_String(std::wstringstream& ss, const SYSTEMTIME* st);
void writeLogDateTime_hhmmssms_String(std::wstringstream& ss, const SYSTEMTIME* st);
void writeLogDateTimeString(std::wstringstream& ss, const SYSTEMTIME* st);

void writeLogLevelString(std::wstringstream& ss, LogLevel level);

void writeLogThreadIdString(std::wstringstream& ss, DWORD threadId);

void writeLogLevelShortString(std::wstringstream& ss, LogLevel level);

void writeLogSourceFileString(std::wstringstream& ss, const wchar_t* file);
void writeLogSourceLineString(std::wstringstream& ss, int line);
void writeLogSourceFuncString(std::wstringstream& ss, const wchar_t* func);
void writeLogSourceFileLineString(std::wstringstream& ss, const wchar_t* file, int line);

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
);




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _LogItem
{
	SYSTEMTIME                dateTime;
	DWORD                     threadId;
	LogLevel                  level;
	const wchar_t*            file;
	int                       line;
	const wchar_t*            func;
	std::wstring              message;
	std::vector<std::uint8_t> param;
} LogItem;

//===========================================================================
void writeLogString(
	std::wstringstream& ss,
	LogItem&            item
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Logger
{
public:
	using OutputHandler = std::function<void(LogItem&)>;

private:
	std::mutex _Mutex;

private:
	OutputHandler _OutputHandler;

public:
	Logger() = default;

public:
	virtual ~Logger() = default;

public:
	void setOutputHandler(OutputHandler handler);

public:
	virtual void output(LogItem& item);

public:
	virtual void log(
		LogLevel                   level,
		const wchar_t*             file,
		int                        line,
		const wchar_t*             func,
		std::wstring_view          message,
		std::vector<std::uint8_t>& param
	);
};






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Log 
{
private:
	Logger* _logger;

private:
	LogLevel                  _level;
	const wchar_t*            _file ;
	int                       _line ;
	const wchar_t*            _func ;
	std::wstringstream        _ss   ;
	std::vector<std::uint8_t> _param;

public:
	Log(
		Logger*        logger, 
		LogLevel       level,
		const wchar_t* file,
		int            line,
		const wchar_t* func,
		void*          paramPointer = nullptr,
		std::size_t    paramSize = 0
	);

public:
	~Log();

public:
	template<typename T>
	Log& operator<<(const T& message) 
	{
		_ss << message;
		return *this;
	}

	Log& operator<<(std::wostream& (*manip)(std::wostream&));
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
Logger* getLogger(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


