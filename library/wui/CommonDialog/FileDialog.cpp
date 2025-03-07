/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <commdlg.h>

//===========================================================================
#include <wui/wui.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
OpenFileDialog::OpenFileDialog(
	std::wstring   file      , // std::wstring   file      ;
	std::wstring   fileTitle , // std::wstring   fileTitle ;
	const wchar_t* filter    , // wchar_t        filter[]= { L"bin File(*.bin)\0*.bin\0All Files(*.*)\0*.*\0" };
	std::wstring   defExt    , // std::wstring   defExt    { L".bin" };
	std::wstring   initialDir, // std::wstring   initialDir{ L"C:\\" };
	std::wstring   title       // std::wstring   title     { L"File Open" };
):
	_File(file),
	_FileTitle(fileTitle),
	_Filter(filter),
	_DefExt(defExt),
	_InitialDir(initialDir),
	_Title(title)
{
}

//===========================================================================
bool OpenFileDialog::doModal (HWND hWnd, std::wstring& openFilePath)
{
	//-----------------------------------------------------------------------
	wchar_t _ofn_open_szFile      [MAX_PATH];
	wchar_t _ofn_open_szFileTitle [MAX_PATH];

	wchar_t _ofn_open_szDefExt    [MAX_PATH];
	wchar_t _ofn_open_szInitialDir[MAX_PATH];
	wchar_t _ofn_open_szTitle     [MAX_PATH];


	//-----------------------------------------------------------------------
	ZeroMemory(_ofn_open_szFile      , sizeof(_ofn_open_szFile      ));
	ZeroMemory(_ofn_open_szFileTitle , sizeof(_ofn_open_szFileTitle ));

	ZeroMemory(_ofn_open_szDefExt    , sizeof(_ofn_open_szDefExt    ));
	ZeroMemory(_ofn_open_szInitialDir, sizeof(_ofn_open_szInitialDir));
	ZeroMemory(_ofn_open_szTitle     , sizeof(_ofn_open_szTitle     ));


	//-----------------------------------------------------------------------	
	if (!_File      .empty()) { lstrcpyW(_ofn_open_szFile      , _File      .c_str()); };
	if (!_FileTitle .empty()) { lstrcpyW(_ofn_open_szFileTitle , _FileTitle .c_str()); };

	if (!_DefExt    .empty()) { lstrcpyW(_ofn_open_szDefExt    , _DefExt    .c_str()); };
	if (!_InitialDir.empty()) { lstrcpyW(_ofn_open_szInitialDir, _InitialDir.c_str()); };
	if (!_Title     .empty()) { lstrcpyW(_ofn_open_szTitle     , _Title     .c_str()); };


	//-----------------------------------------------------------------------
	OPENFILENAMEW ofn;


	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize     = sizeof(ofn); 	
	ofn.hwndOwner       = hWnd; 
	
	ofn.lpstrFile       = _ofn_open_szFile; 
	ofn.nMaxFile        = sizeof(_ofn_open_szFile) / sizeof(_ofn_open_szFile[0]); 
	
	if (0!=_ofn_open_szFileTitle[0])
	{
		ofn.lpstrFileTitle = &_ofn_open_szFileTitle[0];
		ofn.nMaxFileTitle  = sizeof(_ofn_open_szFileTitle) / sizeof(_ofn_open_szFileTitle[0]); 
	}
	else
	{
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle  = 0;
	}

	ofn.Flags           = 0 
		| OFN_FILEMUSTEXIST 
		| OFN_PATHMUSTEXIST 
		| OFN_EXPLORER 
		| OFN_ENABLESIZING 
		| OFN_DONTADDTORECENT 
		| OFN_EXTENSIONDIFFERENT 
		//| OFN_ENABLEHOOK
		;

	if (_Filter)
	{
		ofn.lpstrFilter = _Filter;
		ofn.nFilterIndex = 0;
	}
	else
	{
		ofn.lpstrFilter = nullptr; 
		ofn.nFilterIndex = 0;
	}

	if (0!=_ofn_open_szDefExt[0])
	{
		ofn.lpstrDefExt = &_ofn_open_szDefExt[0]; 
	}
	else
	{
		ofn.lpstrDefExt = nullptr; 
	}

	if (0!=_ofn_open_szInitialDir[0])
	{
		ofn.lpstrInitialDir = &_ofn_open_szInitialDir[0]; 
	}
	else
	{
		ofn.lpstrInitialDir = nullptr; 
	}

	if (0!=_ofn_open_szTitle[0])
	{
		ofn.lpstrTitle = &_ofn_open_szTitle[0];
	}
	else
	{
		ofn.lpstrTitle = nullptr;
	}

	//ofn.FlagsEx  = OFN_EX_NOPLACESBAR;
	//ofn.lpfnHook = &_ofn_hook_proc;


	//-----------------------------------------------------------------------
	if (FALSE==GetOpenFileNameW(&ofn))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	openFilePath = _ofn_open_szFile;

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
SaveFileDialog::SaveFileDialog(
	std::wstring   file      , // std::wstring   file      ;
	std::wstring   fileTitle , // std::wstring   fileTitle ;
	const wchar_t* filter    , // wchar_t        filter[]= { L"bin File(*.bin)\0*.bin\0All Files(*.*)\0*.*\0" };
	std::wstring   defExt    , // std::wstring   defExt    { L".bin" };
	std::wstring   initialDir, // std::wstring   initialDir{ L"C:\\" };
	std::wstring   title       // std::wstring   title     { L"File Save" };
):
	_File(file),
	_FileTitle(fileTitle),
	_Filter(filter),
	_DefExt(defExt),
	_InitialDir(initialDir),
	_Title(title)
{
}

//===========================================================================
bool SaveFileDialog::doModal(HWND hWnd, std::wstring& saveFilePath)
{
	//-----------------------------------------------------------------------
	wchar_t _ofn_save_szFile      [MAX_PATH];
	wchar_t _ofn_save_szFileTitle [MAX_PATH];

	//wchar_t _ofn_save_szFilter    [MAX_PATH]; 
	wchar_t _ofn_save_szDefExt    [MAX_PATH];   
	wchar_t _ofn_save_szInitialDir[MAX_PATH];   
	wchar_t _ofn_save_szTitle     [MAX_PATH];   


	//-----------------------------------------------------------------------
	ZeroMemory(_ofn_save_szFile      , sizeof(_ofn_save_szFile      ));
	ZeroMemory(_ofn_save_szFileTitle , sizeof(_ofn_save_szFileTitle ));

	ZeroMemory(_ofn_save_szDefExt    , sizeof(_ofn_save_szDefExt    ));
	ZeroMemory(_ofn_save_szInitialDir, sizeof(_ofn_save_szInitialDir));
	ZeroMemory(_ofn_save_szTitle     , sizeof(_ofn_save_szTitle     ));


	//-----------------------------------------------------------------------
	if (!_File      .empty()) { lstrcpyW(_ofn_save_szFile      , _File      .c_str()); };
	if (!_FileTitle .empty()) { lstrcpyW(_ofn_save_szFileTitle , _FileTitle .c_str()); };

	if (!_DefExt    .empty()) { lstrcpyW(_ofn_save_szDefExt    , _DefExt    .c_str()); };
	if (!_InitialDir.empty()) { lstrcpyW(_ofn_save_szInitialDir, _InitialDir.c_str()); };
	if (!_Title     .empty()) { lstrcpyW(_ofn_save_szTitle     , _Title     .c_str()); };


	//-----------------------------------------------------------------------
	OPENFILENAMEW ofn;


	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize     = sizeof(ofn); 	
	ofn.hwndOwner       = hWnd; 

	ofn.lpstrFile       = _ofn_save_szFile; 
	ofn.nMaxFile        = sizeof(_ofn_save_szFile) / sizeof(_ofn_save_szFile[0]); 

	if (0!=_ofn_save_szFileTitle[0])
	{
		ofn.lpstrFileTitle = &_ofn_save_szFileTitle[0];
		ofn.nMaxFileTitle  = sizeof(_ofn_save_szFileTitle) / sizeof(_ofn_save_szFileTitle[0]); 
	}
	else
	{
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle  = 0;
	}

	ofn.Flags           = 0
		| OFN_OVERWRITEPROMPT 
		| OFN_PATHMUSTEXIST 
		| OFN_EXPLORER 
		| OFN_ENABLESIZING 
		| OFN_DONTADDTORECENT 
		| OFN_EXTENSIONDIFFERENT 
		| OFN_NOREADONLYRETURN 
		//| OFN_ENABLEHOOK
		;

	if (_Filter)
	{
		ofn.lpstrFilter = _Filter;
		ofn.nFilterIndex = 0;
	}
	else
	{
		ofn.lpstrFilter = nullptr; 
		ofn.nFilterIndex = 0;
	}

	if (0!=_ofn_save_szDefExt[0])
	{
		ofn.lpstrDefExt = &_ofn_save_szDefExt[0]; 
	}
	else
	{
		ofn.lpstrDefExt = nullptr; 
	}

	if (0!=_ofn_save_szInitialDir[0])
	{
		ofn.lpstrInitialDir = &_ofn_save_szInitialDir[0]; 
	}
	else
	{
		ofn.lpstrInitialDir = nullptr; 
	}

	if (0!=_ofn_save_szTitle[0])
	{
		ofn.lpstrTitle = &_ofn_save_szTitle[0];
	}
	else
	{
		ofn.lpstrTitle = nullptr;
	}

	//ofn.FlagsEx  = OFN_EX_NOPLACESBAR;
	//ofn.lpfnHook = &ofn_hook_proc;


	//-----------------------------------------------------------------------
	if (FALSE==GetSaveFileNameW(&ofn))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	saveFilePath = _ofn_save_szFile;

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





