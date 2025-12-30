#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class OpenFileDialog
{
private:
	std::wstring _File;
	std::wstring _FileTitle;
	const wchar_t* _Filter;
	std::wstring _DefExt;
	std::wstring _InitialDir;
	std::wstring _Title;

public:
	explicit OpenFileDialog(
		std::wstring file = L"",
		std::wstring fileTitle = L"",
		const wchar_t* filter = L"",
		std::wstring defExt = L"",
		std::wstring initialDir = L"",
		std::wstring title = L""
	);

public:
	bool doModal(HWND hWnd, std::wstring& openFilePath);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class SaveFileDialog
{
private:
	std::wstring _File;
	std::wstring _FileTitle;
	const wchar_t* _Filter;
	std::wstring _DefExt;
	std::wstring _InitialDir;
	std::wstring _Title;

public:
	explicit SaveFileDialog(
		std::wstring file = L"",
		std::wstring fileTitle = L"",
		const wchar_t* filter = L"",
		std::wstring defExt = L"",
		std::wstring initialDir = L"",
		std::wstring title = L""
	);

public:
	bool doModal(HWND hWnd, std::wstring& openFilePath);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}



