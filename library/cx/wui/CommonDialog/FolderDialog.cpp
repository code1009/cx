/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include <shlobj.h>

//===========================================================================
#include <cx/wui/wui.hpp>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
FolderDialog::FolderDialog(
	std::wstring title
):
	_Title(title)
{
}

//===========================================================================
bool FolderDialog::doModal (HWND hWnd, std::wstring& folder)
{
	BROWSEINFO bi = { 0 };


	bi.lpszTitle = _Title.empty() ? nullptr : _Title.c_str();
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.hwndOwner = hWnd;


	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != nullptr) 
	{
		wchar_t path[MAX_PATH];
		BOOL rv;


		rv = SHGetPathFromIDList(pidl, path);
		CoTaskMemFree(pidl);
		if (TRUE == rv) 
		{
			folder = std::wstring(path);

			return true;
		}
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}





