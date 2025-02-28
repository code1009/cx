#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowMessage
{
public:
	HWND    hWnd  ;
	UINT    uMsg  ;
	WPARAM  wParam;
	LPARAM  lParam;

public:
	LRESULT lResult;

public:
	WindowMessage(
		HWND   hwnd,
		UINT   umsg,
		WPARAM wparam,
		LPARAM lparam
	);

public:
	virtual ~WindowMessage() = default;

public:
	WindowMessage(const WindowMessage& other);
	WindowMessage& operator=(const WindowMessage& other);

	WindowMessage(WindowMessage&& other) noexcept;
	WindowMessage& operator=(WindowMessage&& other) noexcept;

public:
	void setResult(LRESULT rv);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




