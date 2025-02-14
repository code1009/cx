/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#pragma once





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace cx::wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class WindowClass
{
private:
	HINSTANCE    _InstanceHandle      { nullptr };
	std::wstring _WindowClassName     { };
	WNDCLASSEXW  _WindowClassStructure{ };

public:
	WindowClass(void) = default;

	explicit WindowClass(
		LPCWSTR   className,
		UINT      idMenu    = 0,
		UINT      idIcon    = 0, UINT idSmallIcon = 0, UINT idCursor = 32512/*IDC_ARROW*/,
		UINT      style     = CS_HREDRAW | CS_VREDRAW,
		HINSTANCE hInstance = nullptr
	);

public:
	virtual ~WindowClass() = default;

public:
	WindowClass(const WindowClass&) = delete;
	WindowClass& operator=(const WindowClass&) = delete;

	WindowClass(WindowClass&&) = delete;
	WindowClass& operator=(WindowClass&&) = delete;

public:
	void registerWindowClass(
		LPCWSTR   className,
		UINT      idMenu    = 0,
		UINT      idIcon    = 0, UINT idSmallIcon = 0, UINT idCursor = 32512/*IDC_ARROW*/,
		UINT      style     = CS_HREDRAW | CS_VREDRAW,
		HINSTANCE hInstance = nullptr
		);
	void unregisterWindowClass(void);

public:
	std::wstring_view getWindowClassName(void);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




