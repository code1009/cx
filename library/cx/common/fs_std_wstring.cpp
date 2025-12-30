/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "fs_std_wstring.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wfs
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring get_directory_of_file_path(std::wstring file_path)
{
	std::wstring result;

	std::wstring::size_type index;


	index = file_path.find_last_of(L"\\/");
	if (std::wstring::npos == index)
	{
		return result;
	}


	result = file_path.substr(0, index + 1);

	return result;
}

std::wstring get_file_of_file_path(std::wstring file_path)
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
std::wstring get_file_name_of_file_path(std::wstring file_path)
{
	std::wstring result;

	std::wstring::size_type index;


	std::wstring file;


	file = get_file_of_file_path(file_path);
	if (file.empty())
	{
		return result;
	}


	index = file.find_last_of(L".");
	if (std::wstring::npos == index)
	{
		return result;
	}


	result = file.substr(0, index);

	return result;
}

std::wstring get_file_extension_of_file_path(std::wstring file_path)
{
	std::wstring result;

	std::wstring::size_type index;


	std::wstring file;


	file = get_file_of_file_path(file_path);
	if (file.empty())
	{
		return result;
	}


	index = file.find_last_of(L".");
	if (std::wstring::npos == index)
	{
		return result;
	}


	result = file.substr(index);

	return result;
}

std::wstring get_file_extension_name_of_file_path(std::wstring file_path)
{
	std::wstring result;

	std::wstring::size_type index;


	std::wstring file;


	file = get_file_of_file_path(file_path);
	if (file.empty())
	{
		return result;
	}


	index = file.find_last_of(L".");
	if (std::wstring::npos == index)
	{
		return result;
	}


	result = file.substr(index + 1);

	return result;
}

//===========================================================================
std::wstring get_directory_name_of_directory(std::wstring directory)
{
	std::wstring result;

	std::wstring::size_type index;


	index = directory.find_last_of(L"\\/");
	if (std::wstring::npos == index)
	{
		return result;
	}


	result = directory.substr(0, index);


	return result;
}

std::wstring get_parent_directory_of_directory(std::wstring directory)
{
	std::wstring result;

	std::wstring::size_type index;


	std::wstring directory_name;


	directory_name = get_directory_name_of_directory(directory);
	if (directory_name.empty())
	{
		return result;
	}


	index = directory_name.find_last_of(L"\\/");
	if (std::wstring::npos == index)
	{
		return result;
	}


	result = directory_name.substr(0, index + 1);


	return result;
}

//===========================================================================
std::wstring get_file_path_of_module(HINSTANCE hinstance)
{
	std::wstring result;


	wchar_t buffer[MAX_PATH];


	if (GetModuleFileNameW(hinstance, buffer, sizeof(buffer) / sizeof(wchar_t)) > 0)
	{
		result = buffer;
	}

	return result;
}

std::wstring get_file_path_of_current_process(void)
{
	return get_file_path_of_module(NULL);
}

std::wstring get_directory_of_current_process(void)
{
	return get_directory_of_file_path(get_file_path_of_current_process());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


