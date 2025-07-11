#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wfs
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
std::wstring get_directory_of_file_path(std::wstring file_path);
std::wstring get_file_of_file_path(std::wstring file_path);

std::wstring get_file_name_of_file_path(std::wstring file_path);
std::wstring get_file_extension_of_file_path(std::wstring file_path);
std::wstring get_file_extension_name_of_file_path(std::wstring file_path);

std::wstring get_directory_name_of_directory(std::wstring directory);
std::wstring get_parent_directory_of_directory(std::wstring directory);

std::wstring get_file_path_of_module(HINSTANCE hinstance);
std::wstring get_file_path_of_current_process(void);
std::wstring get_directory_of_current_process(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




