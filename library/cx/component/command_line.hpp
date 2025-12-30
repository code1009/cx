#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
using command_line_exit_handler = std::function<void(std::uint32_t)>;
using command_line_output_handler = std::function<void(std::wstring const&)>;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class command_line_executor
{
private:
	class impl;

private:
	std::unique_ptr<impl> _impl{ nullptr };

public:
	explicit command_line_executor(
		std::uint32_t code_page,
		command_line_exit_handler exit_handler,
		command_line_output_handler output_handler,
		std::wstring const& file_path
	);

	/*
	explicit command_line_executor(
		pipe_output output,
		std::wstring const& file_path,
		std::wstring const& arguments,
		std::wstring const& work_directory,
		std::map<std::wstring, std::wstring> environment
	);
	*/

public:
	~command_line_executor();

public:
	void kill(std::uint32_t exit_code = 0xffffffff);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}
