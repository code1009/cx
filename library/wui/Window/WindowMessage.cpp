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
WindowMessage::WindowMessage(
	HWND      hwnd,
	UINT      umsg,
	WPARAM    wparam,
	LPARAM    lparam
) :
	hWnd   { hwnd   },
	uMsg   { umsg   },
	wParam { wparam },
	lParam { lparam },
	lResult{ 0      }
{
}

//===========================================================================
WindowMessage::WindowMessage(const WindowMessage& other):
	hWnd   {other.hWnd   }, 
	uMsg   {other.uMsg   }, 
	wParam {other.wParam }, 
	lParam {other.lParam }, 
	lResult{other.lResult}
{
}

WindowMessage& WindowMessage::operator=(const WindowMessage& other)
{
	if (this != &other)
	{
		hWnd    = other.hWnd;
		uMsg    = other.uMsg;
		wParam  = other.wParam;
		lParam  = other.lParam;
		lResult = other.lResult;
	}

	return *this;
}

WindowMessage::WindowMessage(WindowMessage&& other) noexcept : 
	hWnd   {std::move(other.hWnd   )},
	uMsg   {std::move(other.uMsg   )}, 
	wParam {std::move(other.wParam )}, 
	lParam {std::move(other.lParam )}, 
	lResult{std::move(other.lResult)}
{
	other.hWnd = nullptr;
	other.uMsg = 0;
	other.wParam = 0;
	other.lParam = 0;
	other.lResult = 0;
}

WindowMessage& WindowMessage::operator=(WindowMessage&& other) noexcept
{
	if (this != &other)
	{
		hWnd    = std::move(other.hWnd   );
		uMsg    = std::move(other.uMsg   );
		wParam  = std::move(other.wParam );
		lParam  = std::move(other.lParam );
		lResult = std::move(other.lResult);

		other.hWnd    = nullptr;
		other.uMsg    = 0;
		other.wParam  = 0;
		other.lParam  = 0;
		other.lResult = 0;
	}


	return *this;
}

//===========================================================================
void WindowMessage::setResult(LRESULT rv)
{
	lResult = rv;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




