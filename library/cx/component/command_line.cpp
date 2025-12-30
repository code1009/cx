/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "command_line.hpp"

#include "../runtime/runtime.hpp"
#include "../common/charset_system.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{
	
	



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class command_line_pipe
{
public:
	HANDLE _hread{ nullptr };
	HANDLE _hwrite{ nullptr };
	SECURITY_ATTRIBUTES _sa = { 0 };

public:
	command_line_pipe();
	~command_line_pipe();

private:
	void create(void);
	void destroy(void);
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
command_line_pipe::command_line_pipe()
{
	create();
}

command_line_pipe::~command_line_pipe()
{
	destroy();
}

void command_line_pipe::create(void)
{
	memset(&_sa, 0, sizeof(_sa));
	_sa.nLength = sizeof(_sa);
	_sa.bInheritHandle = TRUE;
	_sa.lpSecurityDescriptor = nullptr;

	BOOL rv;
	rv = CreatePipe(&_hread, &_hwrite, &_sa, 0);
	if (FALSE == rv)
	{
		destroy();
		throw std::runtime_error("Failed to CreatePipe()");
	}
}

void command_line_pipe::destroy(void)
{
	if (_hread != nullptr)
	{
		CloseHandle(_hread);
		_hread = nullptr;
	}
	if (_hwrite != nullptr)
	{
		CloseHandle(_hwrite);
		_hwrite = nullptr;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class command_line_process
{
private:
	std::uint32_t _code_page;
	command_line_exit_handler _exit_handler;
	command_line_output_handler _output_handler;

private:
	std::wstring _environment;
	std::wstring _current_directory;
	std::wstring _arguments;
	std::wstring _file_path;
	std::wstring _command_line;

	std::thread _thread;

	command_line_pipe _rpipe;
	command_line_pipe _wpipe;

	STARTUPINFOW _si = { 0 };
	PROCESS_INFORMATION _pi = { 0 };

public:
	explicit command_line_process(
		std::uint32_t code_page,
		command_line_exit_handler exit_handler,
		command_line_output_handler output_handler,
		std::wstring const& file_path
	);

public:
	~command_line_process();

private:
	std::wstring make_command_line(void);

private:
	void create(void);
	void destroy(void);

	void thread_entry(void);

private:
	void read_output(void);

public:
	void write_input(const std::wstring& s);

	bool wait(std::uint32_t timeout = INFINITE);
	bool kill(std::uint32_t exit_code = 0xffffffff);

public:
	std::uint32_t get_exit_code(void);

public:
	std::string to_pipe_string(std::wstring s);
	std::wstring from_pipe_string(std::string s);
};

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
command_line_process::command_line_process(
	std::uint32_t code_page,
	command_line_exit_handler exit_handler,
	command_line_output_handler output_handler,
	std::wstring const& file_path
) :
	_code_page(code_page),
	_exit_handler(exit_handler),
	_output_handler(output_handler),
	_file_path(file_path)
{
	_command_line = make_command_line();
	create();
}

command_line_process::~command_line_process()
{
	destroy();
}

//===========================================================================
std::wstring command_line_process::make_command_line(void)
{
	std::wstring file_path_and_arguments;
	file_path_and_arguments = _file_path;
	if (!_arguments.empty())
	{
		file_path_and_arguments += L" ";
		file_path_and_arguments += _arguments;
	}
#if 0
	file_path_and_arguments = LR"(dir d:\*.*)";
//	file_path_and_arguments = LR"(D:\prj_my\RSDataEditor\RSDataEditor\x64\Debug\RSDataExternalTool.exe)";
#endif


	std::wstring command_line;
#if 0
	command_line = file_path_and_arguments;
#else
	std::wstring cmd_file_path;
	WCHAR szCmdFilePath[MAX_PATH] = { 0 };
	DWORD dwReturn;
	dwReturn = GetEnvironmentVariableW(L"ComSpec", szCmdFilePath, _countof(szCmdFilePath));
	if (0 == dwReturn)
	{
		cmd_file_path = L"cmd.exe";
	}
	else
	{
		cmd_file_path = szCmdFilePath;
	}


	command_line = cmd_file_path;
	command_line += LR"( )";
	command_line += LR"(/D )";
	//command_line += LR"(/U )";
	command_line += LR"(/C ")";
	command_line += file_path_and_arguments;
	command_line += LR"(" )";
#endif

	return command_line;
}

//===========================================================================
void command_line_process::create(void)
{
	CX_RUNTIME_LOG(cxLDebug) << L"Launch: " << _command_line << std::endl;


	LPCWSTR lpCurrentDirectory = nullptr;
	if (!_current_directory.empty())
	{
		lpCurrentDirectory = _current_directory.c_str();
	}
	else
	{
		lpCurrentDirectory = nullptr;
	}


	LPVOID lpEnvironment = nullptr;



	memset(&_pi, 0, sizeof(_pi));
	memset(&_si, 0, sizeof(_si));
	_si.cb = sizeof(_si);
	_si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	_si.hStdOutput = _rpipe._hwrite;
	_si.hStdError = _rpipe._hwrite;
	_si.hStdInput = _wpipe._hread;
	_si.wShowWindow = SW_HIDE;


	BOOL rv;
	rv = CreateProcessW(
		nullptr,
		const_cast<LPWSTR>(_command_line.c_str()),
		nullptr,
		nullptr,
		TRUE,
		0,
		lpEnvironment,
		lpCurrentDirectory,
		&_si,
		&_pi
	);
	if (FALSE == rv)
	{
		destroy();
		throw std::runtime_error("Failed to CreateProcessW()");
	}


	_thread = std::thread(&command_line_process::thread_entry, this);
}

void command_line_process::destroy(void)
{
	if (_pi.hProcess)
	{
		DWORD object;
		object = WaitForSingleObject(_pi.hProcess, 1000);
		switch (object)
		{
		case WAIT_OBJECT_0:
		case WAIT_ABANDONED:
			break;

		case WAIT_TIMEOUT:
			if (FALSE == TerminateProcess(_pi.hProcess, 0xffffffff))
			{
				CX_RUNTIME_LOG(cxLError) << L"Failed to TerminateProcess()" << std::endl;
			}
			break;

		case WAIT_FAILED:
		default:
			break;
		}
	}


	if (_thread.joinable())
	{
		_thread.join();
	}


	if (_pi.hThread != nullptr)
	{
		CloseHandle(_pi.hThread);
		_pi.hThread = nullptr;
	}
	if (_pi.hProcess != nullptr)
	{
		CloseHandle(_pi.hProcess);
		_pi.hProcess = nullptr;
	}
}

void command_line_process::thread_entry(void)
{
	bool loop = true;
	do
	{
		DWORD object;
		object = WaitForSingleObject(_pi.hProcess, 100);
		switch (object)
		{
		case WAIT_OBJECT_0:
			read_output();
			CX_RUNTIME_LOG(cxLDebug) << L"WAIT_OBJECT_0" << std::endl;
			loop = false;
			break;

		case WAIT_ABANDONED:
			read_output();
			CX_RUNTIME_LOG(cxLDebug) << L"WAIT_ABANDONED" << std::endl;
			loop = false;
			break;

		case WAIT_TIMEOUT:
			read_output();
			break;

		case WAIT_FAILED:
			CX_RUNTIME_LOG(cxLError) << L"WAIT_FAILED" << std::endl;
			loop = false;
			break;

		default:
			CX_RUNTIME_LOG(cxLDebug) << L"?" << object << std::endl;
			loop = false;
			break;
		}
	} while (loop);


	if (_exit_handler)
	{
		std::uint32_t exit_code = get_exit_code();
	
		CX_RUNTIME_LOG(cxLDebug) << L"Exit code: " << exit_code << std::endl;

		_exit_handler(exit_code);
	}


	CX_RUNTIME_LOG(cxLDebug) << L"Process has exited." << std::endl;
}

std::string command_line_process::to_pipe_string(std::wstring s)
{
	//return wcs_to_mbcs(s, CP_THREAD_ACP);
	//return wcs_to_mbcs(s, CP_UTF8);
	return wcs_to_mbcs(s, _code_page);	
}

std::wstring command_line_process::from_pipe_string(std::string s)
{
	//return mbcs_to_wcs(s, CP_THREAD_ACP);
	//return mbcs_to_wcs(s, CP_UTF8);
	return mbcs_to_wcs(s, _code_page);
}

void command_line_process::read_output(void)
{
	DWORD TotalBytesAvail;
	if (PeekNamedPipe(_rpipe._hread, nullptr, 0, nullptr, &TotalBytesAvail, nullptr))
	{
		if (TotalBytesAvail > 0)
		{
			std::vector<char> buffer(TotalBytesAvail);
			DWORD NumberOfBytesRead;
			if (ReadFile(_rpipe._hread, buffer.data(), static_cast<DWORD>(buffer.size()), &NumberOfBytesRead, nullptr))
			{
				if (NumberOfBytesRead > 0)
				{
					if (_output_handler)
					{
						std::wstring output = from_pipe_string(std::string(buffer.data(), NumberOfBytesRead));
						//CX_RUNTIME_LOG(cxLDebug) << L"Output: " << output << std::endl;

						_output_handler(output);
					}
				}
				else
				{
					CX_RUNTIME_LOG(cxLError) << L"Failed to ReadFile() - NumberOfBytesRead is 0." << std::endl;
				}
			}
		}
	}
}

void command_line_process::write_input(const std::wstring& input)
{
	CX_RUNTIME_LOG(cxLDebug) << L"Input: " << input << std::endl;


	std::wstring input_command;
	input_command = input + L"\n";


	std::string command = to_pipe_string(input_command);
	if (_wpipe._hwrite != nullptr)
	{
		DWORD NumberOfBytesWritten;
		WriteFile(_wpipe._hwrite, command.c_str(), static_cast<DWORD>(command.size()), &NumberOfBytesWritten, nullptr);
	}
}

bool command_line_process::wait(std::uint32_t timeout)
{
	if (nullptr == _pi.hProcess)
	{
		throw std::runtime_error("process handle is null!");
	}


	bool result = false;


	DWORD object;
	object = WaitForSingleObject(_pi.hProcess, timeout);
	switch (object)
	{
	case WAIT_OBJECT_0:
		result = true;
		break;

	case WAIT_ABANDONED:
		result = true;
		break;

	case WAIT_TIMEOUT:
		result = false;
		break;

	case WAIT_FAILED:
		throw std::runtime_error("Failed to WaitForSingleObject()");
		break;

	default:
		throw std::runtime_error("Failed to WaitForSingleObject()");
		break;
	}

	return result;
}

bool command_line_process::kill(std::uint32_t exit_code)
{
	if (nullptr == _pi.hProcess)
	{
		throw std::runtime_error("process handle is null!");
	}


	bool result = false;


	DWORD object;
	object = WaitForSingleObject(_pi.hProcess, 0);
	switch (object)
	{
	case WAIT_OBJECT_0:
		result = true;
		break;

	case WAIT_ABANDONED:
		result = true;
		break;

	case WAIT_TIMEOUT:
		if (FALSE == TerminateProcess(_pi.hProcess, exit_code))
		{
			result = false;
		}
		else
		{
			result = true;
		}
		break;

	case WAIT_FAILED:
		throw std::runtime_error("Failed to WaitForSingleObject()");
		break;

	default:
		throw std::runtime_error("Failed to WaitForSingleObject()");
		break;
	}

	return result;
}

std::uint32_t command_line_process::get_exit_code(void)
{
	if (nullptr == _pi.hProcess)
	{
		throw std::runtime_error("process handle is null!");
	}


	DWORD exit_code = 0;
	if (GetExitCodeProcess(_pi.hProcess, &exit_code) == FALSE)
	{
		throw std::runtime_error("Failed to GetExitCodeProcess()");
	}

	return exit_code;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class command_line_executor::impl
{
private:
	command_line_process _process;

public:
	explicit impl(
		std::uint32_t code_page,
		command_line_exit_handler exit_handler,
		command_line_output_handler output_handler,
		std::wstring const& file_path
	) :
		_process(code_page, exit_handler, output_handler, file_path)
	{
	}

public:
	~impl()
	{
		_process.wait();
	}

public:
	void kill(std::uint32_t exit_code = 0xffffffff)
	{
		_process.kill(exit_code);
	}
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
command_line_executor::command_line_executor(
	std::uint32_t code_page,
	command_line_exit_handler exit_handler,
	command_line_output_handler output_handler,
	std::wstring const& file_path
)
{
	_impl = std::make_unique<command_line_executor::impl>(code_page, exit_handler, output_handler, file_path);
}

command_line_executor::~command_line_executor()
{
	_impl.reset();
}

void command_line_executor::kill(std::uint32_t exit_code)
{
	if (_impl)
	{
		_impl->kill(exit_code);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
