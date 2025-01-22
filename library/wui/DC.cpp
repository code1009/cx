/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "pch.hpp"

//===========================================================================
#include "wui.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
namespace wui
{





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
HDC DC::getDCHandle(void) const
{
	return _DCHandle;
}

HDC DC::setDCHandle(HDC handle)
{
	HDC old;


	old = _DCHandle;
	_DCHandle = handle;

	return old;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
}


