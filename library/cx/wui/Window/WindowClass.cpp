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
WindowClass::WindowClass(
	LPCWSTR className,
	UINT idMenu,
	UINT idIcon, 
	UINT idSmallIcon, 
	UINT idCursor,
	UINT style,
	HINSTANCE hInstance
)
{
	registerWindowClass(
		className, 
		idMenu, 
		idIcon, 
		idSmallIcon, 
		idCursor, 
		style, 
		hInstance);
}

//===========================================================================
void WindowClass::registerWindowClass(
	LPCWSTR className,
	UINT idMenu, 
	UINT idIcon, 
	UINT idSmallIcon, 
	UINT idCursor, 
	UINT style,
	HINSTANCE hInstance
)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getInstanceHandle();
	}
	_InstanceHandle = hInstance;
	
	
	//-----------------------------------------------------------------------
	_WindowClassName = className;


	//-----------------------------------------------------------------------
#if 0
	memset(&_WindowClassStructure, 0, sizeof(_WindowClassStructure));

	_WindowClassStructure.cbSize        = sizeof(_WindowClassStructure);
	//_WindowClassStructure.style       = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS
	_WindowClassStructure.style         = style;
	_WindowClassStructure.lpfnWndProc   = WindowProc;
	_WindowClassStructure.cbClsExtra    = 0;
	_WindowClassStructure.cbWndExtra    = 0;
	_WindowClassStructure.hInstance     = hInstance;
	_WindowClassStructure.hIcon         = ::LoadIconW(hInstance, MAKEINTRESOURCEW(idIcon));
	_WindowClassStructure.hCursor       = ::LoadCursorW(hInstance, MAKEINTRESOURCEW(idCursor));
	_WindowClassStructure.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	_WindowClassStructure.lpszMenuName  = MAKEINTRESOURCEW(idMenu);
	_WindowClassStructure.lpszClassName = className;
	_WindowClassStructure.hIconSm       = ::LoadIconW(hInstance, MAKEINTRESOURCEW(idSmallIcon));
#endif


	//-----------------------------------------------------------------------
	memset(&_WindowClassStructure, 0, sizeof(_WindowClassStructure));

	_WindowClassStructure.cbSize        = sizeof(_WindowClassStructure);
	//_WindowClassStructure.style       = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS
	_WindowClassStructure.style         = style;
	_WindowClassStructure.lpfnWndProc   = WindowProc;
	_WindowClassStructure.cbClsExtra    = 0;
	_WindowClassStructure.cbWndExtra    = 0;
	_WindowClassStructure.hInstance     = hInstance;
	_WindowClassStructure.hIcon         = (0 == idIcon  ) ? nullptr                       : loadIcon(idIcon);
	_WindowClassStructure.hCursor       = (0 == idCursor) ? ::LoadCursor(NULL, IDC_ARROW) : loadCursor(idCursor);
	_WindowClassStructure.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	_WindowClassStructure.lpszMenuName  = (0 == idMenu  ) ? nullptr                       : makeIntResource(idMenu);
	_WindowClassStructure.lpszClassName = className;
	_WindowClassStructure.hIconSm       = (0 == idIcon  ) ? nullptr                       : loadIcon(idSmallIcon);


	//-----------------------------------------------------------------------
	WNDCLASSEXW wndClass;
	BOOL rv;


	rv = ::GetClassInfoExW(hInstance, className, &wndClass);
	if (FALSE == rv)
	{
		ATOM atom = ::RegisterClassExW(&_WindowClassStructure);
		if (!atom)
		{
			throw std::runtime_error("WindowClass::registerWindowClass(): RegisterClassExW() failed");
		}
	}
}

void WindowClass::unregisterWindowClass(void)
{
	BOOL rv;


	rv = ::UnregisterClassW(_WindowClassName.c_str(), _InstanceHandle);
	if (FALSE == rv)
	{
		throw std::runtime_error("WindowClass::unregisterWindowClass(): UnregisterClassW() failed");
	}
}

//===========================================================================
std::wstring_view WindowClass::getWindowClassName(void)
{
	return _WindowClassName;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}




