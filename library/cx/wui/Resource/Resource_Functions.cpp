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
std::wstring loadString(int id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	LPWSTR ptr;
	LPWSTR* pptr;
	int rv;


	ptr = nullptr;
	pptr = &ptr;
	rv = ::LoadStringW(hInstance,
		id,
		(LPWSTR)pptr,
		0
	);


	std::wstring s;


	if (rv)
	{
		if (ptr)
		{
			s = ptr;
		}
	}

	return s;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
LPCWSTR makeIntResource(int id)
{
	return MAKEINTRESOURCEW(id);
}

//===========================================================================
HCURSOR loadCursor(int id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	return loadCursor(makeIntResource(id));
}

HICON loadIcon(int id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	return loadIcon(makeIntResource(id));
}

HBITMAP loadBitmap(int id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	return loadBitmap(makeIntResource(id));
}

HACCEL loadAccelerators(int id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	return loadAccelerators(makeIntResource(id));
}

//===========================================================================
HCURSOR loadCursor(LPCWSTR id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	return ::LoadCursorW(hInstance, id);
}

HICON loadIcon(LPCWSTR id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	return ::LoadIconW(hInstance, id);
}

HBITMAP loadBitmap(LPCWSTR id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	return ::LoadBitmapW(hInstance, id);
}

HACCEL loadAccelerators(LPCWSTR id, HINSTANCE hInstance)
{
	//-----------------------------------------------------------------------
	if (nullptr == hInstance)
	{
		hInstance = getAppModule()->getResourceInstanceHandle();
	}


	//-----------------------------------------------------------------------
	return ::LoadAcceleratorsW(hInstance, id);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


