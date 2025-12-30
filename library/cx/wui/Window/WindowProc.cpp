/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "../wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
LRESULT __stdcall WindowProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (WM_NCCREATE == umsg)
	{
		auto userData = reinterpret_cast<CREATESTRUCTW*>(lparam)->lpCreateParams;


		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userData));

		(reinterpret_cast<Window*>(userData))->setWindowHandle(hwnd);
	}


	auto windowPtr = reinterpret_cast<Window*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	if (windowPtr)
	{
		auto lResult = windowPtr->onMessage(hwnd, umsg, wparam, lparam);


		if (WM_NCDESTROY == umsg)
		{
			::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
		}


		return lResult;
	}


	return ::DefWindowProcW(hwnd, umsg, wparam, lparam);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
INT_PTR __stdcall DialogProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (WM_INITDIALOG == umsg)
	{
		auto userData = reinterpret_cast<Window*>(lparam);


		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userData));

		(reinterpret_cast<Window*>(userData))->setWindowHandle(hwnd);
	}


	auto windowPtr = reinterpret_cast<Window*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	if (windowPtr)
	{
		auto lResult = windowPtr->onMessage(hwnd, umsg, wparam, lparam);


		if (WM_NCDESTROY == umsg)
		{
			::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
		}

		return static_cast<INT_PTR>(lResult);
	}


	return FALSE;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




